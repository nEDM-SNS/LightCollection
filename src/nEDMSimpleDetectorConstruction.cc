////////////////////////////////////////////////////////
// nEDMSimpleDetectorConstruction.cc                        //
// Coarse nEDM Geometry                               //
////////////////////////////////////////////////////////

#include "nEDMSimpleDetectorConstruction.hh"

#include <sstream>

#include "G4Material.hh"
#include "nEDMMaterials.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
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

nEDMSimpleDetectorConstruction::nEDMSimpleDetectorConstruction()
{
    // G4 Specific Flags
    fStepLimit = NULL;
    fCheckOverlaps = false;
    
    // nEDM Geometry Flags
    fEmbeddedFibers = true;
    fFiberReflector = false;
    fSqureTubeReflector = false;
    fFullTentReflector = true;
    
    // Cell Size
    fCellWidth= 2*5.08*cm;
    fCellThickness = 2*0.635*cm;
    fCellLength= 2*20.64*cm;

    // Fiber Params
    fNumberOfFibers = 98;
    //fNumberOfFibers = 0;
    fFiberSpacing = 0.103*cm;
    fFiberOuterSurfaceRoughness = 0.9;
    
    // TPB Params
    fTPB_Thickness = .1*mm;
    fTPB_outerThickness = 5*nm;
    
}

nEDMSimpleDetectorConstruction::~nEDMSimpleDetectorConstruction()
{
    delete fStepLimit;
}

G4VPhysicalVolume* nEDMSimpleDetectorConstruction::Construct()
{
    
    fMaterials = nEDMMaterials::GetInstance();
    
    // World volume
    G4double world_x = 1.0*m;
    G4double world_y = 1.0*m;
    G4double world_z = 1.0*m;
    
    G4String worldName = "World";
    G4Box* solidHall = new G4Box(worldName, world_x/2., world_y/2., world_z/2.);
    
    fLogicHall = new G4LogicalVolume(solidHall, fMaterials->GetMaterial("SuperfluidHelium"), worldName);
    
    
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
    ConstructSinglePlate();
    if (fSqureTubeReflector) {ConstructSquareTubeReflector();}
    else if (fFullTentReflector) {ConstructFullTentReflector();}
    
    // Always return the physical World
    return fPhysHall;
    
}

void nEDMSimpleDetectorConstruction::ConstructSinglePlate(){
    
    //////////////////////////
    // Cell Side 1
    //////////////////////////
    G4String side1Name = "CellSide1";
    
    // Cell Side Solid, Logical, and TPB interface
    
    G4Box* cellSide1_solid = new G4Box(side1Name,
                                       fCellWidth/2.,
                                       fCellThickness/2.,
                                       fCellLength/2.);
    
    G4LogicalVolume* logicCellSide1 = new G4LogicalVolume(cellSide1_solid,fMaterials->GetMaterial("PMMA"),side1Name);
    
    G4String TPBInterface1Name = side1Name + "/TPBInterface";
    
    // Create TPB Interface Layer
    G4Box* TPBInterface1_solid = new G4Box(TPBInterface1Name,
                                           fCellWidth/2.,
                                           fTPB_Thickness/2.,
                                           fCellLength/2.);
    
    G4LogicalVolume* TPBInterface1_log = new G4LogicalVolume(TPBInterface1_solid,fMaterials->GetMaterial("TPB_inner"),TPBInterface1Name);
    
    G4ThreeVector TPBInterface1_pos = G4ThreeVector(0.,(fCellThickness-fTPB_Thickness)/2.,0.);
    
    new G4PVPlacement(0,TPBInterface1_pos,
                      TPBInterface1_log,
                      TPBInterface1Name,
                      logicCellSide1,false,0,fCheckOverlaps);
    
    G4VisAttributes* tpb1Vis=new G4VisAttributes(G4Color(1.0,0.0,1.0));
    tpb1Vis->SetVisibility(true);
    TPBInterface1_log->SetVisAttributes(tpb1Vis);
    
    // Create outer TPB layer with low index of refraction
    
    G4String TPBInterfaceOuter1Name = side1Name + "/TPB_outer";
    
    G4Box* TPBInterface_outer1_solid = new G4Box(TPBInterfaceOuter1Name,
                                                 fCellWidth/2,
                                                 fTPB_outerThickness/2,
                                                 fCellLength/2);
    
    
    G4LogicalVolume* TPBInterface_outer1_log = new G4LogicalVolume(TPBInterface_outer1_solid,fMaterials->GetMaterial("TPB_outer"),TPBInterfaceOuter1Name);
    
    G4ThreeVector TPBInterface_outer1_pos = G4ThreeVector(0.,(fTPB_Thickness-fTPB_outerThickness)/2.,0.);
    
    new G4PVPlacement(0,TPBInterface_outer1_pos,
                      TPBInterface_outer1_log,
                      TPBInterfaceOuter1Name,
                      TPBInterface1_log,false,0,fCheckOverlaps);
    
    G4VisAttributes* tpbOuter1Vis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    tpbOuter1Vis->SetVisibility(true);
    TPBInterface_outer1_log->SetVisAttributes(tpbOuter1Vis);
    
    
    // Place CellSide1
    G4VPhysicalVolume* physCellSide1 =
        new G4PVPlacement(0,                     // rotation
                          G4ThreeVector(0,0,0),  // position
                          logicCellSide1,        // logical volume
                          side1Name,                  // name
                          fLogicHall,             // mother volume
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
        
        G4double FibXPos, FibYPos;
        G4double FibThickness = 2.*fiberRmax;
        
        
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
                                               fibDetThickness/4);
        
        G4LogicalVolume* fibDetBackFaceLog = new G4LogicalVolume(fibDetBackFaceSolid,fMaterials->GetMaterial("PMMA"),fibDetBackFaceName);
        
        G4ThreeVector fibDetBackFacePos = G4ThreeVector(0.,0.,fibDetThickness/4);
        
        new G4PVPlacement(0,fibDetBackFacePos,fibDetBackFaceLog,fibDetBackFaceName,fibDetLog,false,0,fCheckOverlaps);
        
        G4VisAttributes* fibDetBackFaceVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
        fibDetBackFaceVis->SetVisibility(true);
        fibDetBackFaceLog->SetVisAttributes(fibDetBackFaceVis);
        
        
        // Place Physical Fibers and Detectors
        
        G4double fibDetZPos = fiberLength/2.+fibDetThickness/2.;
        
        G4RotationMatrix* det2Rot = new G4RotationMatrix();
        det2Rot->rotateY(180*deg);
        
        G4LogicalVolume* fiberParentLog;
        if (fEmbeddedFibers) {
            fiberParentLog = logicCellSide1;
            FibYPos = -1*(fCellThickness/2.-FibThickness/2.-0.001*cm);
        }
        else{
            fiberParentLog = fLogicHall;
            FibYPos = -1*(fCellThickness/2+FibThickness/2+.005*cm);
        }
        
        // Loop over number of Fibers
        
        for(G4int i=0;i<fNumberOfFibers;i++){
            FibXPos=-(fFiberSpacing)*(fNumberOfFibers-1)*0.5 + i*fFiberSpacing;
            
            std::stringstream i_plusOne;
            i_plusOne << i+1;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),fiberLog,fiberName+i_plusOne.str(),fiberParentLog,false,0,fCheckOverlaps);
            
            
            // Place +Z detectors
            physDetector1[i] = new G4PVPlacement(0, G4ThreeVector(FibXPos,FibYPos,fibDetZPos),fibDetLog,fibDetName+"1_"+i_plusOne.str(),fLogicHall,false,0,fCheckOverlaps);
            
            
            // Place -Z detectors
            physDetector2[i] = new G4PVPlacement(det2Rot,G4ThreeVector(FibXPos,FibYPos,-fibDetZPos),fibDetLog,fibDetName+"2_"+i_plusOne.str(),fLogicHall,false,0,fCheckOverlaps);
            
            
        }
        
        if (fFiberOuterSurfaceRoughness < 1.){
            // Boundary Surface Properties
            
            G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
            
            G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
            
            G4VPhysicalVolume* outerVol;
            if (fEmbeddedFibers) {outerVol = physCellSide1;}
            else {outerVol = fPhysHall; }
            
            for(G4int i=0;i<fNumberOfFibers;i++){
                
                fiberOuterRoughSurface = new G4LogicalBorderSurface("fiberOuterRoughSurface",
                                                                    outerVol,
                                                                    physFiber[i],
                                                                    fiberOuterRoughOpSurface);
                
                
                fiberOuterRoughOpSurface->SetModel(glisur);
                fiberOuterRoughOpSurface->SetFinish(ground);
                fiberOuterRoughOpSurface->SetType(dielectric_dielectric);
                fiberOuterRoughOpSurface->SetPolish(fFiberOuterSurfaceRoughness);
                
            }
            
        }
        
    }
    
    //////////////////////////
    // Std Light Guide Detector Logicals
    //////////////////////////
    
    G4String stdDetName = "StdDet";
    
    G4double stdDetThickness = .1*mm;
    G4double stdDetZPos = fCellLength/2.+stdDetThickness/2.;
    
    G4Box* stdDet_Solid = new G4Box(stdDetName,
                                    fCellWidth/2.,
                                    fCellThickness/2.,
                                    stdDetThickness/2.);
    
    G4LogicalVolume* stdDetLog = new G4LogicalVolume(stdDet_Solid,fMaterials->GetMaterial("PMMA"),stdDetName);
    
    G4VisAttributes* stdDetVis=new G4VisAttributes(G4Color(1.0,0.0,0.0));
    stdDetVis->SetVisibility(true);
    stdDetVis->SetForceWireframe(false);
    stdDetLog->SetVisAttributes(stdDetVis);
    
    // Back Face on Fiber Detectors to block stray photons
    G4String stdDetBackFaceName = stdDetName + "/BackFace";
    
    G4Box* stdDetBackFaceSolid = new G4Box(stdDetBackFaceName,
                                           fCellWidth/2.,
                                           fCellThickness/2.,
                                           stdDetThickness/4);
    
    G4LogicalVolume* stdDetBackFaceLog = new G4LogicalVolume(stdDetBackFaceSolid,fMaterials->GetMaterial("PMMA"),stdDetBackFaceName);
    
    G4ThreeVector stdDetBackFacePos = G4ThreeVector(0.,0.,stdDetThickness/4);
    
    new G4PVPlacement(0,stdDetBackFacePos,stdDetBackFaceLog,stdDetBackFaceName,stdDetLog,false,0,fCheckOverlaps);
    
    G4VisAttributes* stdDetBackFaceVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    stdDetBackFaceVis->SetVisibility(true);
    stdDetBackFaceLog->SetVisAttributes(stdDetBackFaceVis);

    
    
    // Detecor Placement
    
    new G4PVPlacement(0,                     // rotation
                      G4ThreeVector(0.,0.,stdDetZPos),  // position
                      stdDetLog,        // logical volume
                      stdDetName+"1",   // name
                      fLogicHall,             // mother volume
                      false,                 // no boolean operations
                      0,fCheckOverlaps);                    // not a copy
    
    G4RotationMatrix* det2Rot = new G4RotationMatrix();
    det2Rot->rotateY(180*deg);
    
    new G4PVPlacement(det2Rot,                     // rotation
                      G4ThreeVector(0.,0.,-1*stdDetZPos),  // position
                      stdDetLog,        // logical volume
                      stdDetName+"2",   // name
                      fLogicHall,             // mother volume
                      false,                 // no boolean operations
                      0,fCheckOverlaps);                    // not a copy
    
}

void nEDMSimpleDetectorConstruction::ConstructFullTentReflector()
{
    // Reflector Wrapping
    G4Box* BottomSolid = new G4Box("BottomReflector",fCellWidth,0.1*cm,fCellLength/2*1.5);
    
    
    G4double topReflRad = fCellWidth;
    G4Tubs* TopSolid = new G4Tubs("InnerReflector",topReflRad,topReflRad+.1*cm,fCellLength/2*1.5,.215*CLHEP::pi,.57*CLHEP::pi);
    
    G4Tubs* EndSolid = new G4Tubs("EndSolid",0,topReflRad+.1*cm,.1*cm,.215*CLHEP::pi,.57*CLHEP::pi);
    
    G4UnionSolid* TempRefl1 = new G4UnionSolid("tempRefl1",BottomSolid,EndSolid,0,G4ThreeVector(0.,-5./8.*topReflRad,fCellLength/2*1.5));
    
    G4UnionSolid* TempRefl2 = new G4UnionSolid("tempRefl2",TempRefl1,EndSolid,0,G4ThreeVector(0.,-5./8.*topReflRad,-1*fCellLength/2*1.5));
    
    G4UnionSolid* SolidReflector = new G4UnionSolid("Reflector", TempRefl2, TopSolid,0,G4ThreeVector(0.,-5./8.*topReflRad,0.));
    
    
    G4LogicalVolume* Reflector_Log = new G4LogicalVolume(SolidReflector, G4Material::GetMaterial("PMMA"), "Reflector");
    
    
    // Photon Energies for which mirror properties will be given
    const G4int kEnergies = 3;
    G4double the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 16*eV};
    
    // Optical Surface for mirror
    G4OpticalSurface* mirror_surface_ =
    new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                         dielectric_dielectric);
    
    // Reflectivity of mirror for each photon energy
    G4double mirror_REFL[kEnergies] = {0.96, 0.96, 0.96};
    
    //Table of Surface Properties for Mirror
    G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
    mirrorSurfaceProperty->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
    mirror_surface_->SetMaterialPropertiesTable(mirrorSurfaceProperty);
    
    new G4LogicalSkinSurface("Reflector_surface", Reflector_Log, mirror_surface_);
    
    
    G4VisAttributes* ReflectVis=new G4VisAttributes(G4Color(1.0,1.0,1.0));
    ReflectVis->SetVisibility(true);
    ReflectVis->SetForceWireframe(true);
    Reflector_Log->SetVisAttributes(ReflectVis);
    
    G4ThreeVector reflectorPos = G4ThreeVector(0.,-fCellThickness,0.);
    
    new G4PVPlacement(0,                            //no rotation
                      reflectorPos,              //at (0,0,0)
                      Reflector_Log,                     //its logical volume
                      "Reflector",            //its name
                      fLogicHall,                //its mother  volume
                      false,                        //no boolean operation
                      0,fCheckOverlaps);                            //copy number
    
}

void nEDMSimpleDetectorConstruction::ConstructSquareTubeReflector(){
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
                      fLogicHall,                //its mother  volume
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
