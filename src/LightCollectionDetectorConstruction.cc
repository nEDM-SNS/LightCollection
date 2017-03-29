////////////////////////////////////////////////////////
// LightCollectionDetectorConstruction.cc                        //
// Coarse nEDM Geometry                               //
////////////////////////////////////////////////////////

#include "LightCollectionDetectorConstruction.hh"

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

LightCollectionDetectorConstruction::LightCollectionDetectorConstruction()
{
    m_StepLimit = NULL;
    
    m_CheckOverlaps = false;
    m_EmbeddedFibers = true;
    m_FiberReflector = false;
    m_SqureTubeReflector = true;
    m_NumberOfFibers = 98;
    m_FiberSpacing = 0.103*cm;

    // TPB Params
    m_TPB_Thickness = .1*mm;
    m_TPB_outerThickness = 5*nm;
}

LightCollectionDetectorConstruction::~LightCollectionDetectorConstruction()
{
    delete m_StepLimit;
}

G4VPhysicalVolume* LightCollectionDetectorConstruction::Construct()
{
    
    m_Materials = nEDMMaterials::GetInstance();
    
    // World volume
    G4double world_x = 15.0*m;
    G4double world_y = 15.0*m;
    G4double world_z = 15.0*m;
    
    G4String worldName = "World";
    G4Box* solidHall = new G4Box(worldName, world_x/2., world_y/2., world_z/2.);
    
    m_LogicHall = new G4LogicalVolume(solidHall, m_Materials->GetMaterial("G4_Galactic"), worldName);
    
    
    m_PhysHall = new G4PVPlacement(0,                     // rotation
                                  G4ThreeVector(0,0,0),
                                  m_LogicHall,             // logical volume
                                  worldName,                  // name
                                  0,                     // mother volume
                                  false,                 // no boolean operations
                                  0,m_CheckOverlaps);     // not a copy
    
    //
    G4VisAttributes* HallAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    HallAtt->SetVisibility(true);
    HallAtt->SetForceWireframe(true);
    m_LogicHall->SetVisAttributes(HallAtt);
    
    
    // ************** Choose Volume to Construct Here *****************
    ConstructTestStand();
    
    // Always return the physical World
    return m_PhysHall;
    
}

void LightCollectionDetectorConstruction::ConstructTestStand()
{
    
    // Cell Parameters
    G4double cellWidth= 2*5.08*cm;
    G4double cellThickness = 2*0.635*cm;
    G4double cellLength= 2*20.64*cm;

    
    // Liquid Helium Volume
    
    G4String LHeName = "LHE";
    G4double fLHELength = 1.9*m;
    G4double fLHERadius = 1.*m;
    
    G4LogicalVolume* fLogicLHE = new G4LogicalVolume(new G4Tubs(LHeName,0.,fLHERadius,fLHELength/2.,0.,360*deg),m_Materials->GetMaterial("SuperfluidHelium"),LHeName);
    
    G4VPhysicalVolume* fPhysLHe = new G4PVPlacement(0,                          // rotation
                                                    G4ThreeVector(0,0,0),  // position
                                                    fLogicLHE,        // logical volume
                                                    LHeName,         // name
                                                    m_LogicHall,            // mother volume
                                                    false,                 // no boolean operations
                                                    0,                     // not a copy
                                                    m_CheckOverlaps);
    
    
    G4VisAttributes* LHeAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
    LHeAtt->SetVisibility(true);
    LHeAtt->SetForceWireframe(true);
    fLogicLHE->SetVisAttributes(LHeAtt);
    
    
    //////////////////////////
    // Cell Side 1
    //////////////////////////
    G4String side1Name = "CellSide1";
    
    // Cell Side Solid, Logical, and TPB interface
    
    G4Box* cellSide1_solid = new G4Box(side1Name,
                                      cellWidth/2.,
                                      cellThickness/2.,
                                      cellLength/2.);
    
    G4LogicalVolume* logicCellSide1 = new G4LogicalVolume(cellSide1_solid,m_Materials->GetMaterial("PMMA"),side1Name);
    
    G4String TPBInterface1Name = side1Name + "/TPBInterface";
    
    // Create TPB Interface Layer
    G4Box* TPBInterface1_solid = new G4Box(TPBInterface1Name,
                                          cellWidth/2.,
                                          m_TPB_Thickness/2.,
                                          cellLength/2.);
    
    G4LogicalVolume* TPBInterface1_log = new G4LogicalVolume(TPBInterface1_solid,m_Materials->GetMaterial("TPB_inner"),TPBInterface1Name);
    
    G4ThreeVector TPBInterface1_pos = G4ThreeVector(0.,(cellThickness-m_TPB_Thickness)/2.,0.);
    
    new G4PVPlacement(0,TPBInterface1_pos,
                      TPBInterface1_log,
                      TPBInterface1Name,
                      logicCellSide1,false,0,m_CheckOverlaps);
    
    G4VisAttributes* tpb1Vis=new G4VisAttributes(G4Color(1.0,0.0,1.0));
    tpb1Vis->SetVisibility(true);
    TPBInterface1_log->SetVisAttributes(tpb1Vis);
    
    // Create outer TPB layer with low index of refraction
    
    G4String TPBInterfaceOuter1Name = side1Name + "/TPB_outer";
    
    G4Box* TPBInterface_outer1_solid = new G4Box(TPBInterfaceOuter1Name,
                                                cellWidth/2,
                                                m_TPB_outerThickness/2,
                                                cellLength/2);
    
    
    G4LogicalVolume* TPBInterface_outer1_log = new G4LogicalVolume(TPBInterface_outer1_solid,m_Materials->GetMaterial("TPB_outer"),TPBInterfaceOuter1Name);
    
    G4ThreeVector TPBInterface_outer1_pos = G4ThreeVector(0.,(m_TPB_Thickness-m_TPB_outerThickness)/2.,0.);
    
    new G4PVPlacement(0,TPBInterface_outer1_pos,
                      TPBInterface_outer1_log,
                      TPBInterfaceOuter1Name,
                      TPBInterface1_log,false,0,m_CheckOverlaps);
    
    G4VisAttributes* tpbOuter1Vis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    tpbOuter1Vis->SetVisibility(true);
    TPBInterface_outer1_log->SetVisAttributes(tpbOuter1Vis);


    // Place CellSide1
    G4VPhysicalVolume* physCellSide1 = new G4PVPlacement(0,                     // rotation
                                                         G4ThreeVector(0,0,0),  // position
                                                         logicCellSide1,        // logical volume
                                                         side1Name,                  // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,m_CheckOverlaps);     // not a copy
    
    if (m_NumberOfFibers>0) {
        
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
        new G4LogicalVolume(fiberTube,m_Materials->GetMaterial("FPethylene"),
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
        
        new G4LogicalVolume(clad1_tube,m_Materials->GetMaterial("Pethylene"),
                            InnerCladdingName,0,0,0);
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),clad1_log,
                          InnerCladdingName,fiberLog,false,0,m_CheckOverlaps);
        
        
        
        // Fiber Core
        //
        
        G4String CoreName = "WLSFiberCore";
        
        G4Tubs* core_tube =
        new G4Tubs(CoreName,fFiber_rmin,fFiber_rmax,fFiber_z/2,fFiber_sphi,fFiber_ephi);
        
        G4LogicalVolume* core_log =
        new G4LogicalVolume(core_tube,m_Materials->GetMaterial("WLSPMMA"),
                            CoreName);
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),core_log,
                          CoreName, clad1_log,false,0,m_CheckOverlaps);
        
        
        if (m_FiberReflector) {
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
                              m_CheckOverlaps);
            
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
        
        G4LogicalVolume* fibDetLog = new G4LogicalVolume(fibDetSolid,m_Materials->GetMaterial("PMMA"),fibDetName);
        
        
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
        
        G4LogicalVolume* fibDetBackFaceLog = new G4LogicalVolume(fibDetBackFaceSolid,m_Materials->GetMaterial("PMMA"),fibDetBackFaceName);
        
        G4ThreeVector fibDetBackFacePos = G4ThreeVector(0.,0.,fibDetThickness/4);
        
        new G4PVPlacement(0,fibDetBackFacePos,fibDetBackFaceLog,fibDetBackFaceName,fibDetLog,false,0,m_CheckOverlaps);
        
        G4VisAttributes* fibDetBackFaceVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
        fibDetBackFaceVis->SetVisibility(true);
        fibDetBackFaceLog->SetVisAttributes(fibDetBackFaceVis);
        
        
        // Place Physical Fibers and Detectors
        
        G4double fibDetZPos = fiberLength/2.+fibDetThickness/2.;
        
        G4RotationMatrix* det2Rot = new G4RotationMatrix();
        det2Rot->rotateY(180*deg);
        
        G4LogicalVolume* fiberParentLog;
        if (m_EmbeddedFibers) {
            fiberParentLog = logicCellSide1;
            FibYPos = -1*(cellThickness/2.-FibThickness/2.-0.001*cm);
        }
        else{
            fiberParentLog = fLogicLHE;
            FibYPos = -1*(cellThickness/2+FibThickness/2+.005*cm);
        }

        // Loop over number of Fibers
        
        for(G4int i=0;i<m_NumberOfFibers;i++){
            FibXPos=-(m_FiberSpacing)*(m_NumberOfFibers-1)*0.5 + i*m_FiberSpacing;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),fiberLog,fiberName+std::to_string(i+1),fiberParentLog,false,0,m_CheckOverlaps);
            
            
            // Place +Z detectors
            physDetector1[i] = new G4PVPlacement(0, G4ThreeVector(FibXPos,FibYPos,fibDetZPos),fibDetLog,fibDetName+"1_"+std::to_string(i+1),fLogicLHE,false,0,m_CheckOverlaps);
            
            
            // Place -Z detectors
            physDetector2[i] = new G4PVPlacement(det2Rot,G4ThreeVector(FibXPos,FibYPos,-fibDetZPos),fibDetLog,fibDetName+"2_"+std::to_string(i+1),fLogicLHE,false,0,m_CheckOverlaps);
            

        }
        
        if (fOuterSurfaceRoughness < 1.){
            // Boundary Surface Properties
            
            G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
            
            G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
            
            G4VPhysicalVolume* outerVol;
            if (m_EmbeddedFibers) {outerVol = physCellSide1;}
            else {outerVol = fPhysLHe; }
            
            for(G4int i=0;i<m_NumberOfFibers;i++){
                
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
    
    
    //////////////////////////
    // Std Light Guide Detector Logicals
    //////////////////////////
    
    G4String stdDetName = "StdDet";
    
    G4double stdDetThickness = .1*mm;
    G4double stdDetZPos = cellLength/2.+stdDetThickness/2.;
    
    G4Box* stdDet_Solid = new G4Box(stdDetName,
                                    cellThickness/2.,
                                    cellWidth/2.,
                                    stdDetThickness/2.);
    
    G4LogicalVolume* stdDetLog = new G4LogicalVolume(stdDet_Solid,m_Materials->GetMaterial("PMMA"),stdDetName);
    
    G4VisAttributes* stdDetVis=new G4VisAttributes(G4Color(1.0,0.0,0.0));
    stdDetVis->SetVisibility(true);
    stdDetVis->SetForceWireframe(false);
    stdDetLog->SetVisAttributes(stdDetVis);
    
    // Back Face on Fiber Detectors to block stray photons
    G4String stdDetBackFaceName = stdDetName + "/BackFace";
    
    G4Box* stdDetBackFaceSolid = new G4Box(stdDetBackFaceName,
                                           cellThickness/2.,
                                           cellWidth/2.,
                                           stdDetThickness/4);
    
    G4LogicalVolume* stdDetBackFaceLog = new G4LogicalVolume(stdDetBackFaceSolid,m_Materials->GetMaterial("PMMA"),stdDetBackFaceName);
    
    G4ThreeVector stdDetBackFacePos = G4ThreeVector(0.,0.,stdDetThickness/4);
    
    new G4PVPlacement(0,stdDetBackFacePos,stdDetBackFaceLog,stdDetBackFaceName,stdDetLog,false,0,m_CheckOverlaps);
    
    G4VisAttributes* stdDetBackFaceVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    stdDetBackFaceVis->SetVisibility(true);
    stdDetBackFaceLog->SetVisAttributes(stdDetBackFaceVis);
    
    
    //////////////////////////
    // Cell Side 2
    //////////////////////////
    
    G4String side2Name = "CellSide2";
 
    // Cell Side Solid, Logical, and TPB interface
    
    G4Box* cellSide2_solid = new G4Box(side2Name,
                                       cellWidth/2.,
                                       cellThickness/2.,
                                       cellLength/2.);
    
    G4LogicalVolume* logicCellSide2 = new G4LogicalVolume(cellSide2_solid,m_Materials->GetMaterial("PMMA"),side2Name);
    
    G4String TPBInterface2Name = side2Name + "/TPBInterface";
    
    // Create TPB Interface Layer
    G4Box* TPBInterface2_solid = new G4Box(TPBInterface2Name,
                                           cellWidth/2.,
                                           m_TPB_Thickness/2.,
                                           cellLength/2.);
    
    G4LogicalVolume* TPBInterface2_log = new G4LogicalVolume(TPBInterface2_solid,m_Materials->GetMaterial("TPB_inner"),TPBInterface2Name);
    
    G4ThreeVector TPBInterface2_pos = G4ThreeVector(0.,(cellThickness-m_TPB_Thickness)/2.,0.);
    
    new G4PVPlacement(0,TPBInterface2_pos,
                      TPBInterface2_log,
                      TPBInterface2Name,
                      logicCellSide2,false,0,m_CheckOverlaps);
    
    G4VisAttributes* tpb2Vis=new G4VisAttributes(G4Color(1.0,0.0,1.0));
    tpb2Vis->SetVisibility(true);
    TPBInterface2_log->SetVisAttributes(tpb2Vis);
    
    // Create outer TPB layer with low index of refraction
    
    G4String TPBInterfaceOuter2Name = side2Name + "/TPB_outer";
    
    G4Box* TPBInterface_outer2_solid = new G4Box(TPBInterfaceOuter2Name,
                                                 cellWidth/2,
                                                 m_TPB_outerThickness/2,
                                                 cellLength/2);
    
    
    G4LogicalVolume* TPBInterface_outer2_log = new G4LogicalVolume(TPBInterface_outer2_solid,m_Materials->GetMaterial("TPB_outer"),TPBInterfaceOuter2Name);
    
    G4ThreeVector TPBInterface_outer2_pos = G4ThreeVector(0.,(m_TPB_Thickness-m_TPB_outerThickness)/2.,0.);
    
    new G4PVPlacement(0,TPBInterface_outer2_pos,
                      TPBInterface_outer2_log,
                      TPBInterfaceOuter2Name,
                      TPBInterface2_log,false,0,m_CheckOverlaps);
    
    G4VisAttributes* tpbOuter2Vis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    tpbOuter2Vis->SetVisibility(true);
    TPBInterface_outer2_log->SetVisAttributes(tpbOuter2Vis);
    
    // Place Cell 2
    G4ThreeVector cell2pos = G4ThreeVector(cellWidth*0.6,cellWidth*0.6,0);
    G4RotationMatrix* cell2rotm = new G4RotationMatrix();
    cell2rotm->rotateZ(-90.*deg);
    
    new G4PVPlacement(cell2rotm,                     // rotation
                                                         cell2pos,  // position
                                                         logicCellSide2,        // logical volume
                                                         side2Name,   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,m_CheckOverlaps);                    // not a copy
    
    
    // Detecor Placement
    
    new G4PVPlacement(0,                     // rotation
                      cell2pos+G4ThreeVector(0.,0.,stdDetZPos),  // position
                      stdDetLog,        // logical volume
                      side2Name+"/"+stdDetName+"1",   // name
                      fLogicLHE,             // mother volume
                      false,                 // no boolean operations
                      0,m_CheckOverlaps);                    // not a copy
    
    G4RotationMatrix* det2Rot = new G4RotationMatrix();
    det2Rot->rotateY(180*deg);
    
    new G4PVPlacement(det2Rot,                     // rotation
                      cell2pos+G4ThreeVector(0.,0.,-1*stdDetZPos),  // position
                      stdDetLog,        // logical volume
                      side2Name+"/"+stdDetName+"2",   // name
                      fLogicLHE,             // mother volume
                      false,                 // no boolean operations
                      0,m_CheckOverlaps);                    // not a copy
    
    
    
    //////////////////////////
    // Cell Side 3
    //////////////////////////
    
    G4String side3Name = "CellSide3";
    
    // Cell Side Solid, Logical, and TPB interface
    
    G4Box* cellSide3_solid = new G4Box(side3Name,
                                       cellWidth/2.,
                                       cellThickness/2.,
                                       cellLength/2.);
    
    G4LogicalVolume* logicCellSide3 = new G4LogicalVolume(cellSide3_solid,m_Materials->GetMaterial("PMMA"),side3Name);
    
    G4String TPBInterface3Name = side3Name + "/TPBInterface";
    
    // Create TPB Interface Layer
    G4Box* TPBInterface3_solid = new G4Box(TPBInterface3Name,
                                           cellWidth/2.,
                                           m_TPB_Thickness/2.,
                                           cellLength/2.);
    
    G4LogicalVolume* TPBInterface3_log = new G4LogicalVolume(TPBInterface3_solid,m_Materials->GetMaterial("TPB_inner"),TPBInterface3Name);
    
    G4ThreeVector TPBInterface3_pos = G4ThreeVector(0.,(cellThickness-m_TPB_Thickness)/2.,0.);
    
    new G4PVPlacement(0,TPBInterface3_pos,
                      TPBInterface3_log,
                      TPBInterface3Name,
                      logicCellSide3,false,0,m_CheckOverlaps);
    
    G4VisAttributes* tpb3Vis=new G4VisAttributes(G4Color(1.0,0.0,1.0));
    tpb3Vis->SetVisibility(true);
    TPBInterface3_log->SetVisAttributes(tpb3Vis);
    
    // Create outer TPB layer with low index of refraction
    
    G4String TPBInterfaceOuter3Name = side3Name + "/TPB_outer";
    
    G4Box* TPBInterface_outer3_solid = new G4Box(TPBInterfaceOuter3Name,
                                                 cellWidth/2,
                                                 m_TPB_outerThickness/2,
                                                 cellLength/2);
    
    
    G4LogicalVolume* TPBInterface_outer3_log = new G4LogicalVolume(TPBInterface_outer3_solid,m_Materials->GetMaterial("TPB_outer"),TPBInterfaceOuter3Name);
    
    G4ThreeVector TPBInterface_outer3_pos = G4ThreeVector(0.,(m_TPB_Thickness-m_TPB_outerThickness)/2.,0.);
    
    new G4PVPlacement(0,TPBInterface_outer3_pos,
                      TPBInterface_outer3_log,
                      TPBInterfaceOuter3Name,
                      TPBInterface3_log,false,0,m_CheckOverlaps);
    
    G4VisAttributes* tpbOuter3Vis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    tpbOuter3Vis->SetVisibility(true);
    TPBInterface_outer3_log->SetVisAttributes(tpbOuter3Vis);

    // Place Cell 3
    G4ThreeVector cell3pos = G4ThreeVector(-1*cellWidth*0.6,cellWidth*0.6,0);
    G4RotationMatrix* cell3rotm = new G4RotationMatrix();
    cell3rotm->rotateZ(90.*deg);
    
    new G4PVPlacement(cell3rotm,                     // rotation
                      cell3pos,  // position
                      logicCellSide3,        // logical volume
                      side3Name,   // name
                      fLogicLHE,             // mother volume
                      false,                 // no boolean operations
                      0,m_CheckOverlaps);                    // not a copy
    
    
    // Detector placement
    
    new G4PVPlacement(0,                     // rotation
                      cell3pos+G4ThreeVector(0.,0.,stdDetZPos),  // position
                      stdDetLog,        // logical volume
                      side3Name+"/"+stdDetName+"1",   // name
                      fLogicLHE,             // mother volume
                      false,                 // no boolean operations
                      0,m_CheckOverlaps);                    // not a copy
    
    
    
   new G4PVPlacement(det2Rot,                     // rotation
                     cell3pos+G4ThreeVector(0.,0.,-1*stdDetZPos),  // position
                     stdDetLog,        // logical volume
                     side3Name+"/"+stdDetName+"2",   // name
                     fLogicLHE,             // mother volume
                     false,                 // no boolean operations
                     0,m_CheckOverlaps);                    // not a copy

    //////////////////////////
    // Square Tube Reflector
    //////////////////////////
    
    if (m_SqureTubeReflector) {
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
                          m_CheckOverlaps);                    // Check Overlaps
    }
    
}

void LightCollectionDetectorConstruction::SetMaxStep(G4double maxStep)
{
    if ((m_StepLimit)&&(maxStep>0.)) m_StepLimit->SetMaxAllowedStep(maxStep);
}

void LightCollectionDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
    m_CheckOverlaps = checkOverlaps;
}
