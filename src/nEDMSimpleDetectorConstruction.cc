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
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

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
    fStepLimit = NULL;
    
    fCheckOverlaps = false;
    fEmbeddedFibers = true;
    fOuterReflector = false;
    fFiberReflector = false;
    
    fCellHalfZ = 4.445*cm;
    fCellHalfThickness =  0.15875*cm;
    
    fCircleInner_rad = 3.175*cm;
    fCircleOuter_rad = fCircleInner_rad + 0.3175*cm;
    
    fFiberThickness = 0.100*cm;
    fNumberOfFibers = 98;
    //fNumberOfFibers = 0;
    fFiberSpacing = 0.103*cm;
    fFiberOuterSurfaceRoughness = 0.9;
    
    fTPB_On = true;
    fTPB_Thickness = .1*mm;
    fFiberLength = 76.2*cm;
    
}

nEDMSimpleDetectorConstruction::~nEDMSimpleDetectorConstruction()
{
    delete fStepLimit;
}

G4VPhysicalVolume* nEDMSimpleDetectorConstruction::Construct()
{
    
    fMaterials = nEDMMaterials::GetInstance();
    
    // World volume
    G4double world_x = 10.0*m;
    G4double world_y = 10.0*m;
    G4double world_z = 10.0*m;
    
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
    ConstructSquarePMMA();
    ConstructCirclePMMA();
    // Fibers //
    if (fNumberOfFibers>0) ConstructFibers();
    
    //if (fOuterReflector) ConstructCylindricalReflector();
    //if (fFiberReflector) ConstructEndFiberReflector();
    
    
    //ConstructPhotonDet();
    //ConstructSiliconWafers();
    //ConstructClearFibers();

    
    
    // Always return the physical World
    return fPhysHall;
    
}

void nEDMSimpleDetectorConstruction::ConstructSquarePMMA()
{

    G4double cellSquareInner_half_ = 2.032*cm;
    G4double cellSquareOuter_half_ = cellSquareInner_half_ + fCellHalfThickness;
    G4double displacement = -2.111375*cm;
    
    G4ThreeVector move1((cellSquareOuter_half_ + cellSquareInner_half_)/2,
                        (cellSquareOuter_half_ + cellSquareInner_half_)/2,
                        0);
    
    
    G4ThreeVector move2(0,
                        cellSquareOuter_half_ + cellSquareInner_half_,
                        0);
    
    G4Box* cellSideHorz_solid = new G4Box("OuterCell",
                                          cellSquareOuter_half_,
                                          (cellSquareOuter_half_ - cellSquareInner_half_)/2,
                                          fCellHalfZ);
    
    
    G4Box* cellSideVert_solid = new G4Box("InnerCell",
                                          (cellSquareOuter_half_ - cellSquareInner_half_)/2,
                                          cellSquareOuter_half_,
                                          fCellHalfZ);
    
    
    G4UnionSolid* cellTR_solid = new G4UnionSolid("TopRight",
                                                  cellSideHorz_solid,
                                                  cellSideVert_solid,
                                                  0,
                                                  move1);
    
    G4UnionSolid* cellBL_solid = new G4UnionSolid("BottomLeft",
                                                  cellSideHorz_solid,
                                                  cellSideVert_solid,
                                                  0,
                                                  -1*move1);
    
    G4UnionSolid* cell_solid = new G4UnionSolid("SquareCell",
                                                cellTR_solid,
                                                cellBL_solid,
                                                0,
                                                move2);
    
    
    G4LogicalVolume* CellSide_log =
    new G4LogicalVolume(cell_solid,G4Material::GetMaterial("PMMA"),
                        "SquareCell");

    new G4PVPlacement(0,G4ThreeVector(0.,displacement,0.),CellSide_log,"SquareCell",fLogicHall,0,0,fCheckOverlaps);
    
    G4VisAttributes* cellVis=new G4VisAttributes(G4Color(0.8,0.8,0.8));
    cellVis->SetVisibility(true);
    CellSide_log->SetVisAttributes(cellVis);
    
    
    if (fTPB_On) {
        
        fTPB_Thickness = 0.75*um;
        
        G4double TPB_outer = 2*fTPB_Thickness + cellSquareInner_half_;
        
        G4Box* TPBHorz_solid = new G4Box("HorizontalTPB",
                                         TPB_outer,
                                         (TPB_outer - cellSquareInner_half_)/2,
                                         fCellHalfZ);
        
        G4Box* TPBVert_solid = new G4Box("VerticalTPB",
                                         (TPB_outer - cellSquareInner_half_)/2,
                                         TPB_outer,
                                         fCellHalfZ);
        
        G4ThreeVector moveTPB1 = G4ThreeVector((TPB_outer + cellSquareInner_half_)/2,
                                               (TPB_outer + cellSquareInner_half_)/2,
                                               0);
        
        G4ThreeVector moveTPB2 = G4ThreeVector(0,
                                               TPB_outer + cellSquareInner_half_,
                                               0);
        
        
        G4UnionSolid* TPBTR_solid = new G4UnionSolid("TopRightTPB",
                                                     TPBHorz_solid,
                                                     TPBVert_solid,
                                                     0,
                                                     moveTPB1);
						  
        G4UnionSolid* TPBBL_solid = new G4UnionSolid("BottomLeftTPB",
                                                     TPBHorz_solid,
                                                     TPBVert_solid,
                                                     0,
                                                     -1*moveTPB1);
						  
        G4UnionSolid* TPB_solid = new G4UnionSolid("TPB",
                                                   TPBTR_solid,
                                                   TPBBL_solid,
                                                   0,
                                                   moveTPB2);
        
        
        G4LogicalVolume* TPBInterface_log = new G4LogicalVolume(TPB_solid, G4Material::GetMaterial("TPB_evap"), "TPBInterface");
        
        G4ThreeVector TPBInterface_pos = G4ThreeVector(0.,0.79375*mm - fTPB_Thickness ,0.);
        
        new G4PVPlacement(0,TPBInterface_pos,
                          TPBInterface_log,
                          "TPBInterface",
                          CellSide_log,false,0);
        
        G4VisAttributes* tpbVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
        tpbVis->SetVisibility(true);
        TPBInterface_log->SetVisAttributes(tpbVis);

        G4VisAttributes* tpbOuterVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
        tpbOuterVis->SetVisibility(true);
        //  TPBInterface_outer_log->SetVisAttributes(tpbOuterVis);
        

    }

    
}

void nEDMSimpleDetectorConstruction::ConstructCirclePMMA()
{
    // Circle PMMA //

    G4double half_z = fCellHalfZ + 3.175*cm;
    
    G4Tubs* cellSide_solid = new G4Tubs("CircleCell",
                                        fCircleInner_rad,
                                        fCircleOuter_rad,
                                        half_z,
                                        0*deg,
                                        360*deg);
    
    G4LogicalVolume* cellSide_log =
    new G4LogicalVolume(cellSide_solid, G4Material::GetMaterial("PMMA"), "CircleCell");
    
    G4ThreeVector circle_pos = G4ThreeVector(0, 0, -3.175*cm);
    
    new G4PVPlacement(0,
                      circle_pos,
                      cellSide_log,
                      "CircularCell",
                      fLogicHall,
                      false,
                      0,
                      fCheckOverlaps);
    
    G4VisAttributes* cellVis = new G4VisAttributes(G4Color(0.8,0.8,0.8));
    cellVis->SetVisibility(true);
    cellSide_log->SetVisAttributes(cellVis);
    
    
}

void nEDMSimpleDetectorConstruction::ConstructFibers()
{

    
    G4String fiberName = "WLSFiber";
    
    // Parameters
    G4double fiberRmin = 0.*cm;
    G4double fiberRmax = fFiberThickness/2;
    
    G4double fiberSphi = 0.00*deg;
    G4double fiberEphi = 360.*deg;
    
    G4double Clad1_rmin = 0.*cm;
    G4double Clad1_rmax = fiberRmax - 0.003*cm;
    
    G4double Clad1_z    = fFiberLength;
    G4double Clad1_sphi = fiberSphi;
    G4double Clad1_ephi = fiberEphi;
    
    G4double Fiber_rmin = 0.00*cm;
    G4double Fiber_rmax = Clad1_rmax - 0.003*cm;
    G4double Fiber_z    = fFiberLength;
    G4double Fiber_sphi = Clad1_sphi;
    G4double Fiber_ephi = Clad1_ephi;
    
    G4double MirrorRmax  = Fiber_rmax;
    G4double MirrorRmin  = 0.*cm;
    G4double MirrorThick = 1.*mm;
    G4double MirrorSPhi  = Fiber_sphi;
    G4double MirrorEPhi  = Fiber_ephi;
    
    G4double MirrorPosZ  = -1*(Fiber_z - MirrorThick)/2;
    
    // Cladding (outer layer)
    //
    G4String OuterCladdingName = "WLSFiberOuterCladding";
    
    G4Tubs* fiberTube =
    new G4Tubs(OuterCladdingName,fiberRmin,fiberRmax,fFiberLength/2,fiberSphi,
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
    new G4Tubs(InnerCladdingName,Clad1_rmin,Clad1_rmax,Clad1_z/2,Clad1_sphi,
               Clad1_ephi);
    
    G4LogicalVolume* clad1_log =
    
    new G4LogicalVolume(clad1_tube,fMaterials->GetMaterial("Pethylene"),
                        InnerCladdingName,0,0,0);
    
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),clad1_log,
                      InnerCladdingName,fiberLog,false,0,fCheckOverlaps);
    
    
    
    // Fiber Core
    //
    
    G4String CoreName = "WLSFiberCore";
    
    G4Tubs* core_tube =
    new G4Tubs(CoreName,Fiber_rmin,Fiber_rmax,Fiber_z/2,Fiber_sphi,Fiber_ephi);
    
    G4LogicalVolume* core_log =
    new G4LogicalVolume(core_tube,fMaterials->GetMaterial("WLSPMMA"),
                        CoreName);
    
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),core_log,
                      CoreName, clad1_log,false,0,fCheckOverlaps);
    
    
    if (fFiberReflector) {
        // Fiber Reflector
        G4Tubs* solidMirror = new G4Tubs("Mirror",
                                         MirrorRmin,
                                         MirrorRmax,
                                         MirrorThick,
                                         MirrorSPhi,
                                         MirrorEPhi);
        
        
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
                          G4ThreeVector(0.,0.,MirrorPosZ),   //position
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
    //G4VPhysicalVolume* physDetector1[1000] = {0};
    //G4VPhysicalVolume* physDetector2[1000] = {0};
    
    G4double FibXPos, FibYPos, FibZPos;
    
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
    
    //       G4double fibDetZPos = fiberLength/2.+fibDetThickness/2.;
    
    //       G4RotationMatrix* det2Rot = new G4RotationMatrix();
    //       det2Rot->rotateY(180*deg);
    
    G4LogicalVolume* fiberParentLog;
    //Embedded Fibers not implemented yet
    //        if (fEmbeddedFibers) {
    //            fiberParentLog = logicCellSide1;
    //            FibYPos = -1*(fCellHalfThickness-FibThickness/2.-0.001*cm);
    //        }
    //        else{}
    fiberParentLog = fLogicHall;
    
    G4double R_pos = fCircleOuter_rad + fFiberThickness/2;
    FibZPos = -71.755*cm;
    
    // Loop over number of Fibers
    for(G4int i=0;i<fNumberOfFibers;i++){
        
        G4double Phi_pos = (i*CLHEP::twopi)/fNumberOfFibers;
        
        FibXPos = R_pos*cos(Phi_pos);
        FibYPos = R_pos*sin(Phi_pos);;
        
        std::stringstream i_plusOne;
        i_plusOne << i+1;
        
        physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,FibZPos),fiberLog,fiberName+i_plusOne.str(),fiberParentLog,false,0,fCheckOverlaps);
        
        
        //            // Place +Z detectors
        //            physDetector1[i] = new G4PVPlacement(0, G4ThreeVector(FibXPos,FibYPos,fibDetZPos),fibDetLog,fibDetName+"1_"+i_plusOne.str(),fLogicHall,false,0,fCheckOverlaps);
        //
        //
        //            // Place -Z detectors
        //            physDetector2[i] = new G4PVPlacement(det2Rot,G4ThreeVector(FibXPos,FibYPos,-fibDetZPos),fibDetLog,fibDetName+"2_"+i_plusOne.str(),fLogicHall,false,0,fCheckOverlaps);
        
        
    }
    
    if (fFiberOuterSurfaceRoughness < 1.){
        // Boundary Surface Properties
        
        G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
        
        G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
        
        G4VPhysicalVolume* outerVol;
        //            if (fEmbeddedFibers) {outerVol = physCellSide1;}
        //            else {outerVol = fPhysHall; }
        outerVol = fPhysHall;
        
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

void nEDMSimpleDetectorConstruction::SetMaxStep(G4double maxStep)
{
    if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void nEDMSimpleDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
    fCheckOverlaps = checkOverlaps;
}
