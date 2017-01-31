////////////////////////////////////////////////////////
// LightCollectionDetectorConstruction.cc                        //
// Coarse nEDM Geometry                               //
////////////////////////////////////////////////////////

#include "LightCollectionDetectorConstruction.hh"

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

LightCollectionDetectorConstruction::LightCollectionDetectorConstruction()
{
    m_StepLimit = NULL;
    
    m_CheckOverlaps = true;
    m_EmbeddedFibers = true;
    m_OuterReflector = true;
    m_FiberReflector = false;
    
    m_CellHalfZ = 4.445*cm;
    m_CellHalfThickness =  0.15875*cm;
    
    m_CircleInner_rad = 3.175*cm;
    m_CircleOuter_rad = m_CircleInner_rad + 0.3175*cm;
    
    m_FiberThickness = 0.100*cm;
    m_NumberOfFibers = 98;
    //m_NumberOfFibers = 0;
    m_FiberSpacing = 0.103*cm;
    m_FiberOuterSurfaceRoughness = 0.9;
    m_FiberHalfLength = 76.2*cm;

    m_TPB_On = true;
    m_TPB_Thickness = .1*mm;

    
}

LightCollectionDetectorConstruction::~LightCollectionDetectorConstruction()
{
    delete m_StepLimit;
}

G4VPhysicalVolume* LightCollectionDetectorConstruction::Construct()
{
    
    m_Materials = nEDMMaterials::GetInstance();
    
    // World volume
    G4double world_x = 10.0*m;
    G4double world_y = 10.0*m;
    G4double world_z = 10.0*m;
    
    G4String worldName = "World";
    G4Box* solidHall = new G4Box(worldName, world_x/2., world_y/2., world_z/2.);
    
    m_LogicHall = new G4LogicalVolume(solidHall, m_Materials->GetMaterial("SuperfluidHelium"), worldName);
    
    
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
    ConstructSquarePMMA();
    ConstructCirclePMMA();
    // Fibers //
    if (m_NumberOfFibers>0) ConstructFibers();
    
    if (m_OuterReflector) ConstructCylindricalReflector();
    
    //ConstructPhotonDet();
    //ConstructSiliconWafers();
    ConstructClearFibers();

    
    
    // Always return the physical World
    return m_PhysHall;
    
}

void LightCollectionDetectorConstruction::ConstructSquarePMMA()
{

    G4double cellSquareInner_half_ = 2.032*cm;
    G4double cellSquareOuter_half_ = cellSquareInner_half_ + m_CellHalfThickness;
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
                                          m_CellHalfZ);
    
    
    G4Box* cellSideVert_solid = new G4Box("InnerCell",
                                          (cellSquareOuter_half_ - cellSquareInner_half_)/2,
                                          cellSquareOuter_half_,
                                          m_CellHalfZ);
    
    
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

    new G4PVPlacement(0,G4ThreeVector(0.,displacement,0.),CellSide_log,"SquareCell",m_LogicHall,0,0,m_CheckOverlaps);
    
    G4VisAttributes* cellVis=new G4VisAttributes(G4Color(0.8,0.8,0.8));
    cellVis->SetVisibility(true);
    CellSide_log->SetVisAttributes(cellVis);
    
    
    if (m_TPB_On) {
        
        m_TPB_Thickness = 0.75*um;
        
        G4double TPB_outer = 2*m_TPB_Thickness + cellSquareInner_half_;
        
        G4Box* TPBHorz_solid = new G4Box("HorizontalTPB",
                                         TPB_outer,
                                         (TPB_outer - cellSquareInner_half_)/2,
                                         m_CellHalfZ);
        
        G4Box* TPBVert_solid = new G4Box("VerticalTPB",
                                         (TPB_outer - cellSquareInner_half_)/2,
                                         TPB_outer,
                                         m_CellHalfZ);
        
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
        
        G4ThreeVector TPBInterface_pos = G4ThreeVector(0.,0.79375*mm - m_TPB_Thickness ,0.);
        
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

void LightCollectionDetectorConstruction::ConstructCirclePMMA()
{
    // Circle PMMA //

    G4double half_z = m_CellHalfZ + 3.175*cm;
    
    G4Tubs* cellSide_solid = new G4Tubs("CircleCell",
                                        m_CircleInner_rad,
                                        m_CircleOuter_rad,
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
                      m_LogicHall,
                      false,
                      0,
                      m_CheckOverlaps);
    
    G4VisAttributes* cellVis = new G4VisAttributes(G4Color(0.8,0.8,0.8));
    cellVis->SetVisibility(true);
    cellSide_log->SetVisAttributes(cellVis);
    
    
}

void LightCollectionDetectorConstruction::ConstructFibers()
{

    
    G4String fiberName = "WLSFiber";
    
    // Parameters
    G4double fiberRmin = 0.*cm;
    G4double fiberRmax = m_FiberThickness/2;
    
    G4double fiberSphi = 0.00*deg;
    G4double fiberEphi = 360.*deg;
    
    G4double Clad1_rmin = 0.*cm;
    G4double Clad1_rmax = fiberRmax - 0.003*cm;
    
    G4double Clad1_sphi = fiberSphi;
    G4double Clad1_ephi = fiberEphi;
    
    G4double Fiber_rmin = 0.00*cm;
    G4double Fiber_rmax = Clad1_rmax - 0.003*cm;
    G4double Fiber_sphi = Clad1_sphi;
    G4double Fiber_ephi = Clad1_ephi;
    
    G4double MirrorRmax  = Fiber_rmax;
    G4double MirrorRmin  = 0.*cm;
    G4double MirrorThick = 1.*mm;
    G4double MirrorSPhi  = Fiber_sphi;
    G4double MirrorEPhi  = Fiber_ephi;
    
    G4double MirrorPosZ  = -1*(m_FiberHalfLength - MirrorThick/2);
    
    // Cladding (outer layer)
    //
    G4String OuterCladdingName = "WLSFiberOuterCladding";
    
    G4Tubs* fiberTube =
    new G4Tubs(OuterCladdingName,fiberRmin,fiberRmax,m_FiberHalfLength,fiberSphi,
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
    new G4Tubs(InnerCladdingName,Clad1_rmin,Clad1_rmax,m_FiberHalfLength,Clad1_sphi,
               Clad1_ephi);
    
    G4LogicalVolume* clad1_log =
    
    new G4LogicalVolume(clad1_tube,m_Materials->GetMaterial("Pethylene"),
                        InnerCladdingName,0,0,0);
    
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),clad1_log,
                      InnerCladdingName,fiberLog,false,0,m_CheckOverlaps);
    
    
    
    // Fiber Core
    //
    
    G4String CoreName = "WLSFiberCore";
    
    G4Tubs* core_tube =
    new G4Tubs(CoreName,Fiber_rmin,Fiber_rmax,m_FiberHalfLength,Fiber_sphi,Fiber_ephi);
    
    G4LogicalVolume* core_log =
    new G4LogicalVolume(core_tube,m_Materials->GetMaterial("WLSPMMA"),
                        CoreName);
    
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),core_log,
                      CoreName, clad1_log,false,0,m_CheckOverlaps);
    
    
    if (m_FiberReflector) {
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
                          m_CheckOverlaps);
        
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
    
    //       G4double fibDetZPos = fiberLength/2.+fibDetThickness/2.;
    
    //       G4RotationMatrix* det2Rot = new G4RotationMatrix();
    //       det2Rot->rotateY(180*deg);
    
    G4LogicalVolume* fiberParentLog;
    //Embedded Fibers not implemented yet
    //        if (m_EmbeddedFibers) {
    //            fiberParentLog = logicCellSide1;
    //            FibYPos = -1*(m_CellHalfThickness-FibThickness/2.-0.001*cm);
    //        }
    //        else{}
    fiberParentLog = m_LogicHall;
    
    G4double R_pos = m_CircleOuter_rad + m_FiberThickness/2;
    FibZPos = -71.755*cm;
    
    // Loop over number of Fibers
    for(G4int i=0;i<m_NumberOfFibers;i++){
        
        G4double Phi_pos = (i*CLHEP::twopi)/m_NumberOfFibers;
        
        FibXPos = R_pos*cos(Phi_pos);
        FibYPos = R_pos*sin(Phi_pos);;
        
        std::stringstream i_plusOne;
        i_plusOne << i+1;
        
        physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,FibZPos),fiberLog,fiberName+i_plusOne.str(),fiberParentLog,false,0,m_CheckOverlaps);
        
        
        //            // Place +Z detectors
        //            physDetector1[i] = new G4PVPlacement(0, G4ThreeVector(FibXPos,FibYPos,fibDetZPos),fibDetLog,fibDetName+"1_"+i_plusOne.str(),m_LogicHall,false,0,m_CheckOverlaps);
        //
        //
        //            // Place -Z detectors
        //            physDetector2[i] = new G4PVPlacement(det2Rot,G4ThreeVector(FibXPos,FibYPos,-fibDetZPos),fibDetLog,fibDetName+"2_"+i_plusOne.str(),m_LogicHall,false,0,m_CheckOverlaps);
        
        
    }
    
    if (m_FiberOuterSurfaceRoughness < 1.){
        // Boundary Surface Properties
        
        G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
        
        G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
        
        G4VPhysicalVolume* outerVol;
        //            if (m_EmbeddedFibers) {outerVol = physCellSide1;}
        //            else {outerVol = m_PhysHall; }
        outerVol = m_PhysHall;
        
        for(G4int i=0;i<m_NumberOfFibers;i++){
            
            fiberOuterRoughSurface = new G4LogicalBorderSurface("fiberOuterRoughSurface",
                                                                outerVol,
                                                                physFiber[i],
                                                                fiberOuterRoughOpSurface);
            
            
            fiberOuterRoughOpSurface->SetModel(glisur);
            fiberOuterRoughOpSurface->SetFinish(ground);
            fiberOuterRoughOpSurface->SetType(dielectric_dielectric);
            fiberOuterRoughOpSurface->SetPolish(m_FiberOuterSurfaceRoughness);
            
        }
        
    }
    
    
}

void LightCollectionDetectorConstruction::ConstructCylindricalReflector()
{
    G4double refl_rad = 3.5925*cm + 0.35*mm;
    G4double refl_length = m_CellHalfZ + 6.985*cm;
    
    // Reflector Geometry
    G4Tubs* CylindricalReflector = new G4Tubs("CylindricalReflector", refl_rad,refl_rad + 0.1*mm, refl_length, 0.*deg, 360.*deg );
    
    G4LogicalVolume* Reflector_Log = new G4LogicalVolume(CylindricalReflector, G4Material::GetMaterial("PMMA"), "Reflector");
    
    
    // Photon Energies for which mirror properties will be given
    const G4int kEnergies = 3;
    G4double the_photon_energies_[kEnergies] = {2.*eV, 4.*eV, 16*eV};
    
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
    
    
    G4VisAttributes* ReflectVis=new G4VisAttributes(G4Color(1.0,0.2,1.0));
    ReflectVis->SetVisibility(true);
    //ReflectVis->SetForceWireframe(true);
    Reflector_Log->SetVisAttributes(ReflectVis);
    
    G4ThreeVector refl_pos = G4ThreeVector(0., 0.,-6.985*cm);
    
    new G4PVPlacement(0,                            //no rotation
                      refl_pos,              //at (0,0,0)
                      Reflector_Log,                     //its logical volume
                      "Reflector",            //its name
                      m_LogicHall,                //its mother  volume
                      false,                        //no boolean operation
                      0,                        //copy number
                      m_CheckOverlaps);                    // Check Overlaps
    

}

void LightCollectionDetectorConstruction::ConstructClearFibers(){
    
    G4double half_z = 15.24*cm; // 6 inches
    G4Tubs* clearFiberClad_solid = new G4Tubs("clearFiberClad",
                                              0,
                                              0.75*mm,
                                              half_z,
                                              0,
                                              360.*deg);
    
    G4Tubs* clearFiberCore_solid = new G4Tubs("ClearFiberCore",
                                              0,
                                              0.72*mm,
                                              half_z,
                                              0,
                                              360.*deg);
    
    G4LogicalVolume* clearFiberCore_log = new G4LogicalVolume(clearFiberCore_solid, G4Material::GetMaterial("ClearCore"), "ClearFiberCore");
    
    G4LogicalVolume* clearFiberClad_log = new G4LogicalVolume(clearFiberClad_solid, G4Material::GetMaterial("ClearClad"), "ClearFiberCladding");
    
    new G4PVPlacement(0,
                      G4ThreeVector(0.,0.,0.),
                      clearFiberCore_log,
                      "ClearFiberCore",
                      clearFiberClad_log,
                      false,
                      0,
                      m_CheckOverlaps);
    
    G4int nFibers = m_NumberOfFibers;
    G4double R_pos = m_CircleOuter_rad + (m_FiberThickness)/2;
    
    for(G4int i=0;i<nFibers;i++)
    {
        G4double Phi_pos = (i*CLHEP::twopi)/nFibers;
        G4double X_pos = R_pos*cos(Phi_pos);
        G4double Y_pos = R_pos*sin(Phi_pos);
        G4ThreeVector fiber_pos = G4ThreeVector(X_pos, Y_pos, -1*(half_z + 152.5*cm - m_CellHalfZ));
        
        new G4PVPlacement(0,
                          fiber_pos,
                          clearFiberClad_log,
                          "ClearFiberCladding",
                          m_LogicHall,
                          false,
                          0,
                          m_CheckOverlaps);
    };
    
    
    G4VisAttributes* clearCladVis = new G4VisAttributes(G4Color(1.0,1.0,1.0));
    clearCladVis->SetVisibility(true);
    clearFiberClad_log->SetVisAttributes(clearCladVis);
    clearFiberCore_log->SetVisAttributes(clearCladVis);
    
}

void LightCollectionDetectorConstruction::SetMaxStep(G4double maxStep)
{
    if ((m_StepLimit)&&(maxStep>0.)) m_StepLimit->SetMaxAllowedStep(maxStep);
}

void LightCollectionDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
    m_CheckOverlaps = checkOverlaps;
}
