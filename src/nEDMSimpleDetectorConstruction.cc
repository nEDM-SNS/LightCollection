////////////////////////////////////////////////////////
// nEDMSimpleDetectorConstruction.cc                        //
// Coarse nEDM Geometry                               //
////////////////////////////////////////////////////////

#include "nEDMSimpleDetectorConstruction.hh"

#include "G4Material.hh"
#include "nEDMMaterials.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh"

#include "nEDMSensitiveDetector.hh"

#include "nEDMSimplePhotDetConstructor.hh"

nEDMSimpleDetectorConstruction::nEDMSimpleDetectorConstruction()
{
    fStepLimit = NULL;
    
    fCheckOverlaps = false;
    fEmbeddedFibers = false;
    fFiberReflector = false;
    fNumberOfFibers = 98;
    fFiberSpacing = 0.103*cm;
    
}

nEDMSimpleDetectorConstruction::~nEDMSimpleDetectorConstruction()
{
    delete fStepLimit;
}

G4VPhysicalVolume* nEDMSimpleDetectorConstruction::Construct()
{
    
    fMaterials = nEDMMaterials::GetInstance();
    
    // World volume
    G4double world_x = 15.0*m;
    G4double world_y = 15.0*m;
    G4double world_z = 15.0*m;
    
    G4String worldName = "World";
    G4Box* solidHall = new G4Box(worldName, world_x, world_y, world_z);
    
    fLogicHall = new G4LogicalVolume(solidHall, fMaterials->GetMaterial("G4_Galactic"), worldName);
    
    
    fPhysHall = new G4PVPlacement(0,                     // rotation
                                  G4ThreeVector(0,0,0),
                                  fLogicHall,             // logical volume
                                  worldName,                  // name
                                  0,                     // mother volume
                                  false,                 // no boolean operations
                                  0,fCheckOverlaps);     // not a copy
    
    //
    G4VisAttributes* HallAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    HallAtt->SetVisibility(true);
    HallAtt->SetForceWireframe(true);
    fLogicHall->SetVisAttributes(HallAtt);
    
    
    // ************** Choose Volume to Construct Here *****************
    ConstructTestStand();
    
    // Always return the physical World
    return fPhysHall;
    
}

void nEDMSimpleDetectorConstruction::ConstructTestStand()
{
    
    // Liquid Helium Volume
    
    G4String LHeName = "LHE";
    G4double fLHELength = 1.9*m;
    G4double fLHERadius = 1.*m;
    
    G4LogicalVolume* fLogicLHE = new G4LogicalVolume(new G4Tubs(LHeName,0.,fLHERadius,fLHELength/2.,0.,360*deg),fMaterials->GetMaterial("G4_AIR"),LHeName);
    
    G4VPhysicalVolume* fPhysLHe = new G4PVPlacement(0,                          // rotation
                                                    G4ThreeVector(0,0,0),  // position
                                                    fLogicLHE,        // logical volume
                                                    LHeName,         // name
                                                    fLogicHall,            // mother volume
                                                    false,                 // no boolean operations
                                                    0,                     // not a copy
                                                    fCheckOverlaps);
    
    
    G4VisAttributes* LHeAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
    LHeAtt->SetVisibility(true);
    LHeAtt->SetForceWireframe(true);
    fLogicLHE->SetVisAttributes(LHeAtt);
    
    
    // Cell Side Solid
    
    G4double cellWidth= 2*5.08*cm;
    G4double cellThickness = 2*0.635*cm;
    G4double cellLength= 2*20.64*cm;
    
    G4Box* cellSide_solid = new G4Box("CellSide",
                                      cellWidth/2.,
                                      cellThickness/2.,
                                      cellLength/2.);
    
    
    //////////////////////////
    // Cell Side 1
    //////////////////////////
    G4String side1Name = "CellSide1";
    G4LogicalVolume* logicCellSide1 = new G4LogicalVolume(cellSide_solid,fMaterials->GetMaterial("PMMA"),side1Name);
    
    G4VPhysicalVolume* physCellSide1 = new G4PVPlacement(0,                     // rotation
                                                         G4ThreeVector(0,0,0),  // position
                                                         logicCellSide1,        // logical volume
                                                         side1Name,                  // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);     // not a copy
    
    if (fNumberOfFibers>0) {
        
        G4String fiberName = "WLSFiber";
        
        // Parameters
        G4double fiberRmin = 0.*cm;
        G4double fiberRmax = 0.100*cm/2;
        
        G4double fiberLength    = 2*20.64*cm;
        G4double fiberSphi = 0.00*deg;
        G4double fiberEphi = 360.*deg;
        
        G4double fClad1_rmin = 0.*cm;
        G4double fClad1_rmax = fiberRmax - 0.003*cm;
        
        G4double fClad1_z    = fiberLength;
        G4double fClad1_sphi = fiberSphi;
        G4double fClad1_ephi = fiberEphi;
        
        G4double fFiber_rmin = 0.00*cm;
        G4double fFiber_rmax = fClad1_rmax - 0.003*cm;
        G4double fFiber_z    = fiberLength;
        G4double fFiber_sphi = fClad1_sphi;
        G4double fFiber_ephi = fClad1_ephi;
        
        G4double fOuterSurfaceRoughness = 0.9;
        
        G4double fMirrorRmax  = fFiber_rmax;
        G4double fMirrorRmin  = 0.*cm;
        G4double fMirrorThick = 1.*mm;
        G4double fMirrorSPhi  = fFiber_sphi;
        G4double fMirrorEPhi  = fFiber_ephi;
        
        G4double fMirrorPosZ  = -1*(fFiber_z - fMirrorThick)/2;
        
        // Cladding (outer layer)
        //
        G4String OuterCladdingName = "WLSFiberOuterCladding";
        
        G4Tubs* fiberTube =
        new G4Tubs(OuterCladdingName,fiberRmin,fiberRmax,fiberLength/2,fiberSphi,
                   fiberEphi);
        
        G4LogicalVolume* fiberLog =
        new G4LogicalVolume(fiberTube,fMaterials->GetMaterial("FPethylene"),
                            OuterCladdingName,0,0,0);
        
        //Set Vis attributes and return logical volume
        //
        G4VisAttributes* FiberVis=new G4VisAttributes(G4Color(0.0,1.0,0.0));
        FiberVis->SetVisibility(true);
        fiberLog->SetVisAttributes(FiberVis);
        
        // Cladding (first layer)
        //
        
        G4String InnerCladdingName = "WLSFiberInnerCladding";
        
        G4Tubs* clad1_tube =
        new G4Tubs(InnerCladdingName,fClad1_rmin,fClad1_rmax,fClad1_z/2,fClad1_sphi,
                   fClad1_ephi);
        
        G4LogicalVolume* clad1_log =
        
        new G4LogicalVolume(clad1_tube,fMaterials->GetMaterial("Pethylene"),
                            InnerCladdingName,0,0,0);
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),clad1_log,
                          InnerCladdingName,fiberLog,false,0,fCheckOverlaps);
        
        
        
        // Fiber Core
        //
        
        G4String CoreName = "WLSFiberCore";
        
        G4Tubs* core_tube =
        new G4Tubs(CoreName,fFiber_rmin,fFiber_rmax,fFiber_z/2,fFiber_sphi,fFiber_ephi);
        
        G4LogicalVolume* core_log =
        new G4LogicalVolume(core_tube,fMaterials->GetMaterial("WLSPMMA"),
                            CoreName);
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),core_log,
                          CoreName, clad1_log,false,0,fCheckOverlaps);
        
        
        if (fFiberReflector) {
            // Fiber Reflector
            G4Tubs* solidMirror = new G4Tubs("Mirror",
                                             fMirrorRmin,
                                             fMirrorRmax,
                                             fMirrorThick,
                                             fMirrorSPhi,
                                             fMirrorEPhi);
            
            
            G4LogicalVolume* logicMirror = new G4LogicalVolume(solidMirror,G4Material::GetMaterial("PMMA"),"Mirror");
            
            // Photon Energies for which mirror properties will be given
            const G4int kEnergies = 3;
            G4double the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 16*eV};
            
            // Optical Surface for mirror
            G4OpticalSurface* mirror_surface_ =
            new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                                 dielectric_dielectric);
            
            // Reflectivity of mirror for each photon energy
            G4double mirror_REFL[kEnergies] = {0.998, 0.998, 0.998};
            
            //Table of Surface Properties for Mirror
            G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
            mirrorSurfaceProperty->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
            mirror_surface_->SetMaterialPropertiesTable(mirrorSurfaceProperty);
            
            // Place Mirror
            new G4PVPlacement(0,                                 //no rotation
                              G4ThreeVector(0.,0.,fMirrorPosZ),   //position
                              logicMirror,            //its logical volume
                              "Mirror",               //its name
                              //Clad2_log,            //its mother  volume
                              core_log,               //its mother  volume
                              false,                  //no boolean operation
                              0,                    //copy number
                              fCheckOverlaps);
            
            // Create Skin Surface to link logical surface and optical surface
            new G4LogicalSkinSurface("MirrorSurface",logicMirror,mirror_surface_);
            
            // Set Visualization Properties of the Mirror
            G4VisAttributes* MirrorVis=new G4VisAttributes(G4Color(0.0,0.0,1.0));
            MirrorVis->SetVisibility(true);
            logicMirror->SetVisAttributes(MirrorVis);
            
        }
        
        G4VPhysicalVolume* physFiber[1000] = {0};
        G4VPhysicalVolume* physDetector1[1000] = {0};
        G4VPhysicalVolume* physDetector2[1000] = {0};
        
        G4double FibXPos;
        G4double CellThickness = cellThickness;
        G4double FibThickness = fiberRmax;
        
        G4double FibYPos = -1*(CellThickness/2+FibThickness/2+.005);
        
        // Fiber Detectors
        
        //
        G4String fibDetName = "fibDet";
        G4double fibDetWidth = 0.100*cm;
        G4double fibDetThickness = .1*mm;
        G4Box* fibDetSolid = new G4Box(fibDetName,
                                       fibDetWidth/2.,
                                       fibDetWidth/2.,
                                       fibDetThickness/2.);
        
        G4LogicalVolume* fibDetLog = new G4LogicalVolume(fibDetSolid,fMaterials->GetMaterial("PMMA"),fibDetName);
        
        
        G4VisAttributes* fibDetVis=new G4VisAttributes(G4Color(1.0,0.0,0.0));
        fibDetVis->SetVisibility(true);
        fibDetVis->SetForceWireframe(true);
        fibDetLog->SetVisAttributes(fibDetVis);
        
        
        // Back Face on Fiber Detectors to block stray photons
        G4String fibDetBackFaceName = fibDetName + "/BackFace";
        
        G4Box* fibDetBackFaceSolid = new G4Box(fibDetBackFaceName,
                                               fibDetWidth/2.,
                                               fibDetWidth/2.,
                                               FibThickness*.99/2);
        
        G4LogicalVolume* fibDetBackFaceLog = new G4LogicalVolume(fibDetBackFaceSolid,fMaterials->GetMaterial("PMMA"),fibDetBackFaceName);
        
        G4ThreeVector fibDetBackFacePos = G4ThreeVector(0.,0.,.05*fibDetThickness);
        
        new G4PVPlacement(0,fibDetBackFacePos,fibDetBackFaceLog,fibDetBackFaceName,                                                            fibDetLog,false,0,fCheckOverlaps);
        
        G4VisAttributes* faceVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
        faceVis->SetVisibility(true);
        fibDetBackFaceLog->SetVisAttributes(faceVis);
        
        
        // Place Physical Fibers and Detectors
        
        G4double fibDetZPos = fiberLength/2.+fibDetThickness/2.;
   
        G4RotationMatrix* det2Rot = new G4RotationMatrix();
        det2Rot->rotateY(180*deg);

        
        for(G4int i=0;i<fNumberOfFibers;i++){
            FibXPos=-(fFiberSpacing)*(fNumberOfFibers-1)*0.5 + i*fFiberSpacing;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),fiberLog,fiberName+std::to_string(i+1),fLogicLHE,false,0,fCheckOverlaps);
            
            
            // Place +Z detectors
            physDetector1[i] = new G4PVPlacement(0, G4ThreeVector(FibXPos,FibYPos,fibDetZPos),fibDetLog,fibDetName+"1_"+std::to_string(i+1),fLogicLHE,false,0,fCheckOverlaps);
            
            
            // Place -Z detectors
            physDetector2[i] = new G4PVPlacement(det2Rot,G4ThreeVector(FibXPos,FibYPos,-fibDetZPos),fibDetLog,fibDetName+"2_"+std::to_string(i+1),fLogicLHE,false,0,fCheckOverlaps);
            
            if (fEmbeddedFibers) {
                
            }
            else {
                
            }
        }
        
        if (fOuterSurfaceRoughness < 1.){
            // Boundary Surface Properties
            
            G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
            
            G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
            
            G4VPhysicalVolume* outerVol;
            if (fEmbeddedFibers) {outerVol = physCellSide1;}
            else {outerVol = fPhysLHe; }
            
            for(G4int i=0;i<fNumberOfFibers;i++){
                
                fiberOuterRoughSurface = new G4LogicalBorderSurface("fiberOuterRoughSurface",
                                                                    outerVol,
                                                                    physFiber[i],
                                                                    fiberOuterRoughOpSurface);
                
                
                fiberOuterRoughOpSurface->SetModel(glisur);
                fiberOuterRoughOpSurface->SetFinish(ground);
                fiberOuterRoughOpSurface->SetType(dielectric_dielectric);
                fiberOuterRoughOpSurface->SetPolish(fOuterSurfaceRoughness);
                
            }
            
        }
        
    }
    
    
    // Std Light Guide Detectors
    
    G4String stdDetName = "PhotDet";
    //
    //    // Long Strip Detector
    //    G4double detXWidth =
    G4double detXWidth = 0.100*cm;
    G4double detYWidth = 0.100*cm;
    G4double detThickness = .1*mm;
    //    G4Box* photDet_Solid = new G4Box(name,
    //                                     fXWidth/2.,
    //                                     fYWidth/2.,
    //                                     fThickness/2.);
    //
    //    G4LogicalVolume* photonDet_log = new G4LogicalVolume(photDet_Solid,materials->GetMaterial("PMMA"),name);
    //
    //    G4VisAttributes* DetVis=new G4VisAttributes(G4Color(1.0,0.0,0.0));
    //    DetVis->SetVisibility(true);
    //    DetVis->SetForceWireframe(true);
    //    photonDet_log->SetVisAttributes(DetVis);
    //
    //
    //    // Back Face to block stray photons
    //    G4String backFaceName = name + "/BackFace";
    //
    //    G4Box* backFace_solid = new G4Box(backFaceName,
    //                                      fXWidth/2.,
    //                                      fYWidth/2.,
    //                                      fThickness*.99/2);
    //
    //    G4LogicalVolume* backFace_log = new G4LogicalVolume(backFace_solid,materials->GetMaterial("PMMA"),backFaceName);
    //
    //    G4ThreeVector backFace_pos = G4ThreeVector(0.,0.,.05*fThickness);
    //
    //    G4VPhysicalVolume* backFace_phys = new G4PVPlacement(0,backFace_pos,
    //                                                         backFace_log,
    //                                                         backFaceName,
    //                                                         photonDet_log,false,0,fCheckOverlaps);
    //
    //    G4VisAttributes* faceVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    //    faceVis->SetVisibility(true);
    //    backFace_log->SetVisAttributes(faceVis);
    

    //////////////////////////
    // Cell Side 2
    //////////////////////////
    
    G4String side2Name = "CellSide2";
    
    G4LogicalVolume* logicCellSide2 = new G4LogicalVolume(cellSide_solid,fMaterials->GetMaterial("PMMA"),side2Name);
    
    G4ThreeVector cell2pos = G4ThreeVector(cellWidth*0.6,cellWidth*0.6,0);
    G4RotationMatrix* cell2rotm = new G4RotationMatrix();
    cell2rotm->rotateZ(-90.*deg);
    
    G4VPhysicalVolume* physCellSide2 = new G4PVPlacement(cell2rotm,                     // rotation
                                                         cell2pos,  // position
                                                         logicCellSide2,        // logical volume
                                                         side2Name,   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    
    
    
    //    detector1.SetYWidth(cellSide2.GetWidth());
    //    detector1.SetXWidth(cellSide2.GetThickness());
    //    detector2.SetYWidth(cellSide2.GetWidth());
    //    detector2.SetXWidth(cellSide2.GetThickness());
    
    //    G4LogicalVolume* logicStdDet1 = detector1.GetPiece();
    //    G4LogicalVolume* logicStdDet2 = detector2.GetPiece();
    //
    //    detZPos = cellSide2.GetLength()/2.+detector1.GetThickness()/2.;
    //
    //
    //    G4VPhysicalVolume* physStdDet2_1 = new G4PVPlacement(0,                     // rotation
    //                                                         cell2pos+G4ThreeVector(0.,0.,detZPos),  // position
    //                                                         logicStdDet1,        // logical volume
    //                                                         detector1.name,   // name
    //                                                         fLogicLHE,             // mother volume
    //                                                         false,                 // no boolean operations
    //                                                         0,fCheckOverlaps);                    // not a copy
    //
    //
    //
    //    G4VPhysicalVolume* physStdDet2_2 = new G4PVPlacement(det2Rot,                     // rotation
    //                                                         cell2pos+G4ThreeVector(0.,0.,-1*detZPos),  // position
    //                                                         logicStdDet2,        // logical volume
    //                                                         detector2.name,   // name
    //                                                         fLogicLHE,             // mother volume
    //                                                         false,                 // no boolean operations
    //                                                         0,fCheckOverlaps);                    // not a copy
    //
    
    
    //////////////////////////
    // Cell Side 3
    //////////////////////////
    
    G4String side3Name = "CellSide3";
    
    G4LogicalVolume* logicCellSide3 = new G4LogicalVolume(cellSide_solid,fMaterials->GetMaterial("PMMA"),side2Name);
    
    G4ThreeVector cell3pos = G4ThreeVector(-1*cellWidth*0.6,cellWidth*0.6,0);
    G4RotationMatrix* cell3rotm = new G4RotationMatrix();
    cell3rotm->rotateZ(90.*deg);
    
    G4VPhysicalVolume* physCellSide3 = new G4PVPlacement(cell3rotm,                     // rotation
                                                         cell3pos,  // position
                                                         logicCellSide3,        // logical volume
                                                         side3Name,   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    
    
    
    //    G4VPhysicalVolume* physStdDet3_1 = new G4PVPlacement(0,                     // rotation
    //                                                         cell3pos+G4ThreeVector(0.,0.,detZPos),  // position
    //                                                         logicStdDet1,        // logical volume
    //                                                         detector1.name,   // name
    //                                                         fLogicLHE,             // mother volume
    //                                                         false,                 // no boolean operations
    //                                                         0,fCheckOverlaps);                    // not a copy
    //
    //
    //
    //    G4VPhysicalVolume* physStdDet3_2 = new G4PVPlacement(det2Rot,                     // rotation
    //                                                         cell3pos+G4ThreeVector(0.,0.,-1*detZPos),  // position
    //                                                         logicStdDet2,        // logical volume
    //                                                         detector2.name,   // name
    //                                                         fLogicLHE,             // mother volume
    //                                                         false,                 // no boolean operations
    //                                                         0,fCheckOverlaps);                    // not a copy
    //
    //
    
    // Square Tube Reflector Around Volume
    
    // Reflector Wrapping
    G4Box* OuterReflector = new G4Box("OuterReflector",8.*cm,8.*cm,2*20.64*cm);
    G4Box* InnerReflector = new G4Box("InnerReflector",7.*cm,7.*cm,2*20.64*cm);
    G4SubtractionSolid* SolidReflector = new G4SubtractionSolid("Reflector",OuterReflector,InnerReflector);
    
    G4LogicalVolume* Reflector_Log = new G4LogicalVolume(SolidReflector, G4Material::GetMaterial("PMMA"), "Reflector");
    
    
    // Photon Energies for which mirror properties will be given
    const G4int kEnergies = 3;
    G4double the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 16*eV};
    
    // Optical Surface for mirror
    G4OpticalSurface* mirror_surface_ =
    new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                         dielectric_dielectric);
    
    // Reflectivity of mirror for each photon energy
    G4double mirror_REFL[kEnergies] = {0.998, 0.998, 0.998};
    
    //Table of Surface Properties for Mirror
    G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
    mirrorSurfaceProperty->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
    mirror_surface_->SetMaterialPropertiesTable(mirrorSurfaceProperty);
    
    new G4LogicalSkinSurface("Reflector_surface", Reflector_Log, mirror_surface_);
    
    
    G4VisAttributes* ReflectVis=new G4VisAttributes(G4Color(1.0,1.0,1.0));
    ReflectVis->SetVisibility(true);
    ReflectVis->SetForceWireframe(true);
    Reflector_Log->SetVisAttributes(ReflectVis);
    
    new G4PVPlacement(0,                            //no rotation
                      G4ThreeVector(0.,5.*cm,0.),              //at (0,0,0)
                      Reflector_Log,                     //its logical volume
                      "SquareTubeReflector",            //its name
                      fLogicLHE,                //its mother  volume
                      false,                        //no boolean operation
                      0,                        //copy number
                      fCheckOverlaps);                    // Check Overlaps
    
    
}

void nEDMSimpleDetectorConstruction::SetMaxStep(G4double maxStep)
{
    if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void nEDMSimpleDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
    fCheckOverlaps = checkOverlaps;
}
