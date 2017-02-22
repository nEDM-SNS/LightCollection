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
    m_NumberOfFibers = 16;
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
    if (m_NumberOfFibers>0) ConstructFibersNew();
    
    if (m_OuterReflector) ConstructCylindricalReflector();
    
    //ConstructPhotonDet();
    //ConstructSiliconWafers();
    //ConstructClearFibers();

    
    
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
    G4double fiberRmax = m_FiberThickness/2;
    
    G4double Clad1_rmax = fiberRmax - 0.003*cm;
    
    G4double Core_rmax = Clad1_rmax - 0.003*cm;
    
    G4String OuterCladdingName = "WLSFiberOuterCladding";
    G4String InnerCladdingName = "WLSFiberInnerCladding";
    G4String CoreName = "WLSFiberCore";

    G4VPhysicalVolume* physFiber[1000] = {0};
    
    G4double FibXPos, FibYPos, FibZPos;
   
    G4double R_pos = m_CircleOuter_rad + m_FiberThickness/2;
    FibZPos = -71.755*cm;
    
    
    // Loop over number of Fibers
    for(G4int i=0;i<m_NumberOfFibers;i++){
        
        std::stringstream i_plusOne;
        i_plusOne << i+1;
        
        // Cladding (outer layer)
        //
        
        G4Tubs* fiberTube =
        new G4Tubs(OuterCladdingName+i_plusOne.str(),0.*cm,fiberRmax,m_FiberHalfLength,0.00*deg,
                   360.*deg);
        
        G4LogicalVolume* fiberLog =
        new G4LogicalVolume(fiberTube,m_Materials->GetMaterial("FPethylene"),
                            OuterCladdingName+i_plusOne.str());
        
        //Set Vis attributes
        //
        G4VisAttributes* FiberVis=new G4VisAttributes(G4Color(0.0,1.0,0.0));
        FiberVis->SetVisibility(true);
        fiberLog->SetVisAttributes(FiberVis);
        
        // Cladding (first layer)
        //
        
        
        G4Tubs* clad1_tube =
        new G4Tubs(InnerCladdingName+i_plusOne.str(),0.*cm,Clad1_rmax,m_FiberHalfLength,0.00*deg,
                   360.*deg);
        
        G4LogicalVolume* clad1_log =
        
        new G4LogicalVolume(clad1_tube,m_Materials->GetMaterial("Pethylene"),
                            InnerCladdingName+i_plusOne.str());
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),clad1_log,
                          InnerCladdingName+i_plusOne.str(),fiberLog,false,0,m_CheckOverlaps);
        
        
        
        // Fiber Core
        //
        
        
        G4Tubs* core_tube =
        new G4Tubs(CoreName+i_plusOne.str(),0.*cm,Core_rmax,m_FiberHalfLength,0.00*deg,
                   360.*deg);
        
        G4LogicalVolume* core_log =
        new G4LogicalVolume(core_tube,m_Materials->GetMaterial("WLSPMMA"),
                            CoreName+i_plusOne.str());
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),core_log,
                          CoreName+i_plusOne.str(), clad1_log,false,0,m_CheckOverlaps);
        
        
        G4double Phi_pos = (i*CLHEP::twopi)/m_NumberOfFibers;
        
        FibXPos = R_pos*cos(Phi_pos);
        FibYPos = R_pos*sin(Phi_pos);;
        
        
        physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,FibZPos),fiberLog,fiberName+i_plusOne.str(),m_LogicHall,false,0,m_CheckOverlaps);
        
        
    }
    
    if (m_FiberOuterSurfaceRoughness < 1.){
        // Boundary Surface Properties
        
        G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
        
        G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
        
        G4VPhysicalVolume* outerVol = m_PhysHall;
        
        for(G4int i=0;i<m_NumberOfFibers;i++){
            
            fiberOuterRoughSurface = new G4LogicalBorderSurface("fiberOuterRoughSurface",
                                                                outerVol,
                                                                physFiber[i],fiberOuterRoughOpSurface);
            
            
            fiberOuterRoughOpSurface->SetModel(glisur);
            fiberOuterRoughOpSurface->SetFinish(ground);
            fiberOuterRoughOpSurface->SetType(dielectric_dielectric);
            fiberOuterRoughOpSurface->SetPolish(m_FiberOuterSurfaceRoughness);
            
        }
        
    }
    
    // Construct Wrap around pieces
    G4String wrapAroundOuterName = "wrapAroundOuter";
    G4String wrapAroundInnerName = "wrapAroundInner";
    G4String wrapAroundCoreName = "wrapAroundCore";
    G4double displacement = -2.111375*cm;
    
    for(G4int i=0;i<m_NumberOfFibers/2;i++){
        
        std::stringstream i_plusOne;
        i_plusOne << i+1;
        
        G4double wrapAroundZPos = m_CellHalfZ + (i+1)*m_FiberThickness;
        
        // Outer Cladding
        //
        G4Torus* wrapAroundOuter_torus = new G4Torus(wrapAroundOuterName+i_plusOne.str(),0.*cm,fiberRmax,R_pos,0.,180.*deg);
        
        G4LogicalVolume* wrapAroundOuter_log = new G4LogicalVolume(wrapAroundOuter_torus,m_Materials->GetMaterial("FPethylene"),  wrapAroundOuterName+i_plusOne.str());
        
        //Set Vis attributes
        //
        G4VisAttributes* FiberVis=new G4VisAttributes(G4Color(0.0,1.0,0.0));
        FiberVis->SetVisibility(true);
        wrapAroundOuter_log->SetVisAttributes(FiberVis);
        
        
        // Inner Cladding
        //
        G4Torus* wrapAroundInner_torus = new G4Torus(wrapAroundInnerName+i_plusOne.str(),0.*cm,Clad1_rmax,R_pos,0.,180.*deg);
        
        G4LogicalVolume* wrapAroundInner_log = new G4LogicalVolume(wrapAroundInner_torus,m_Materials->GetMaterial("Pethylene"),  wrapAroundInnerName+i_plusOne.str());
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),wrapAroundInner_log,
                          wrapAroundInnerName+i_plusOne.str(),wrapAroundOuter_log,false,0,m_CheckOverlaps);
        
        // Core
        //
        
        G4Torus* wrapAroundCore_torus = new G4Torus(wrapAroundCoreName+i_plusOne.str(),0.*cm,Core_rmax,R_pos,0.,180.*deg);
        
        G4LogicalVolume* wrapAroundCore_log = new G4LogicalVolume(wrapAroundCore_torus,m_Materials->GetMaterial("WLSPMMA"),  wrapAroundCoreName+i_plusOne.str());
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),wrapAroundCore_log,
                          wrapAroundCoreName+i_plusOne.str(),wrapAroundInner_log,false,0,m_CheckOverlaps);
        
        G4RotationMatrix* xRot = new G4RotationMatrix;
        xRot->rotateX(-90.*deg);

        // Place Wrap Around
        new G4PVPlacement(xRot,G4ThreeVector(0.,0.,wrapAroundZPos),wrapAroundOuter_log,wrapAroundOuterName+i_plusOne.str(),m_LogicHall,false,0,m_CheckOverlaps);
        
        
        
    }
    
    
}

void LightCollectionDetectorConstruction::ConstructFibersNew()
{

    G4String fiberName = "WLSFiber";

    // Parameters
    G4double fiberRmax = m_FiberThickness/2;
    G4double Clad1Rmax = fiberRmax - 0.003*cm;
    G4double CoreRmax = Clad1Rmax - 0.003*cm;
    G4RotationMatrix* xRot = new G4RotationMatrix;
    xRot->rotateX(90.*deg);
    
    G4double R_pos = m_CircleOuter_rad + m_FiberThickness/2;
    
    G4double ZOffset;
    
    // Parts of the Fiber solid that are the same for all fibers
    
    // Outer Cladding
    //
    G4Tubs* straightTube_Outer =
    new G4Tubs("straightTube_Outer",0.*cm,fiberRmax,m_FiberHalfLength,0.00*deg,
               360.*deg);
    
    G4Torus* bentTube_Outer = new G4Torus("fiberBend_Outer",0.*cm,fiberRmax,R_pos,0.,180.*deg);
    
    // Inner Cladding
    //
    G4Tubs* straightTube_Inner =
    new G4Tubs("straightTube_Inner",0.*cm,Clad1Rmax,m_FiberHalfLength,0.00*deg,
               360.*deg);
    
    G4Torus* bentTube_Inner = new G4Torus("fiberBend_Inner",0.*cm,Clad1Rmax,R_pos,0.,180.*deg);
    
    // Core
    //
    G4Tubs* straightTube_Core =
    new G4Tubs("straightTube_Core",0.*cm,CoreRmax,m_FiberHalfLength,0.00*deg,
               360.*deg);
    
    G4Torus* bentTube_Core = new G4Torus("fiberBend_Core",0.*cm,CoreRmax,R_pos,0.,180.*deg);

    
    
    
    G4VPhysicalVolume* physFiber[1000] = {0};
    
    for(G4int i=0;i<m_NumberOfFibers;i++){
        
        std::stringstream i_plusOne;
        i_plusOne << i+1;
        

        G4double displacement = 1.01*m_FiberThickness*(i+1);
        G4double extenderLength = (displacement+0.1*mm)/2;
        
        G4ThreeVector straightPos1 = G4ThreeVector(R_pos,-1*(m_FiberHalfLength+displacement),0.);
        G4ThreeVector straightPos2 = G4ThreeVector(-1*R_pos,-1*(m_FiberHalfLength+displacement),0.);
        
        G4ThreeVector extenderPos1 = G4ThreeVector(R_pos,-1*extenderLength,0.);
        G4ThreeVector extenderPos2 = G4ThreeVector(-1*R_pos,-1*extenderLength,0.);

        
        // ** Create Union Solids for bent Fibers **
        
        // Outer Cladding
        //
        G4Tubs* extender_Outer = new G4Tubs("extender_Outer",0.*cm,fiberRmax,extenderLength,0.*deg,360.*deg);
        
        G4UnionSolid* temp_outer1 = new G4UnionSolid("temp_outer1",bentTube_Outer,extender_Outer,xRot,extenderPos1);
        
        G4UnionSolid* temp_outer2 = new G4UnionSolid("temp_outer2",temp_outer1,extender_Outer,xRot,extenderPos2);
        
        G4UnionSolid* temp_Outer3 = new G4UnionSolid("temp_Outer3",temp_outer2,straightTube_Outer,xRot,straightPos1);

        G4UnionSolid* outerSolid = new G4UnionSolid("temp_Outer4",temp_Outer3,straightTube_Outer,xRot,straightPos2);
        
        // Inner Cladding
        //
        G4Tubs* extender_Inner = new G4Tubs("extender_Inner",0.*cm,Clad1Rmax,extenderLength,0.*deg,360.*deg);
        
        G4UnionSolid* temp_Inner1 = new G4UnionSolid("temp_Inner1",bentTube_Inner,extender_Inner,xRot,extenderPos1);
        
        G4UnionSolid* temp_Inner2 = new G4UnionSolid("temp_Inner2",temp_Inner1,extender_Inner,xRot,extenderPos2);

        G4UnionSolid* temp_Inner3 = new G4UnionSolid("temp_Inner3",temp_Inner2,straightTube_Inner,xRot,straightPos1);
        
        G4UnionSolid* innerSolid = new G4UnionSolid("temp_Inner4",temp_Inner3,straightTube_Inner,xRot,straightPos2);
        
        // Core
        //
        G4Tubs* extender_Core = new G4Tubs("extender_Core",0.*cm,CoreRmax,extenderLength,0.*deg,360.*deg);
        
        G4UnionSolid* temp_Core1 = new G4UnionSolid("temp_Core1",bentTube_Core,extender_Core,xRot,extenderPos1);
        
        G4UnionSolid* temp_Core2 = new G4UnionSolid("temp_Core2",temp_Core1,extender_Core,xRot,extenderPos2);
        
        G4UnionSolid* temp_Core3 = new G4UnionSolid("temp_Core3",temp_Core2,straightTube_Core,xRot,straightPos1);
        
        G4UnionSolid* coreSolid = new G4UnionSolid("temp_Core4",temp_Core3,straightTube_Core,xRot,straightPos2);
        
        
        // ** Create Logical Volumes and Placements **
        
        // Outer Cladding
        //
        G4LogicalVolume* outerCladLog =
        new G4LogicalVolume(outerSolid,m_Materials->GetMaterial("FPethylene"),"outerCladLog");
        
        G4VisAttributes* OuterVis=new G4VisAttributes(G4Color(0.0,1.0,0.0));
        OuterVis->SetVisibility(true);
        outerCladLog->SetVisAttributes(OuterVis);
        
        // Inner Cladding
        //
        G4LogicalVolume* innerCladLog =
        new G4LogicalVolume(innerSolid,m_Materials->GetMaterial("Pethylene"),"innerCladLog");
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),innerCladLog,
                          "InnerCladding"+i_plusOne.str(),outerCladLog,false,0,m_CheckOverlaps);
        
        G4VisAttributes* InnerVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
        InnerVis->SetVisibility(true);
        innerCladLog->SetVisAttributes(InnerVis);
        
        
        // Core
        //
        G4LogicalVolume* coreLog =
        new G4LogicalVolume(coreSolid,m_Materials->GetMaterial("WLSPMMA"),"coreLog");
        
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),coreLog,
                          "Core"+i_plusOne.str(),innerCladLog,false,0,m_CheckOverlaps);
        
        G4VisAttributes* CoreVis=new G4VisAttributes(G4Color(0.5,0.5,0.5));
        CoreVis->SetVisibility(true);
        coreLog->SetVisAttributes(CoreVis);
        
        
        // Place fibers
        //
        G4double Phi = (i*CLHEP::pi)/m_NumberOfFibers;
        G4RotationMatrix* fibRot = new G4RotationMatrix;
        fibRot->rotateX(-90*deg);
        fibRot->rotateY(Phi);
        
        physFiber[i] = new G4PVPlacement(fibRot,G4ThreeVector(0.,0.,m_CellHalfZ+displacement),outerCladLog,fiberName+i_plusOne.str(),m_LogicHall,false,0,m_CheckOverlaps);
    
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
