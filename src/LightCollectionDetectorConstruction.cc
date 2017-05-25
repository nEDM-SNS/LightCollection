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

LightCollectionDetectorConstruction::LightCollectionDetectorConstruction()
{
    // G4 Specific Flags
    m_StepLimit = NULL;
    m_CheckOverlaps = false;
    
    // nEDM Geometry Flags
    m_EmbeddedFibers = true;
    m_FiberReflector = false;
    m_SqureTubeReflector = false;
    m_FullTentReflector = true;
    
    // Cell Size
    m_CellWidth= 2*5.08*cm;
    m_CellThickness = 2*0.635*cm;
    m_CellLength= 2*20.64*cm;

    // Fiber Params
    m_NumberOfFibers = 98;
    //m_NumberOfFibers = 0;
    m_FiberSpacing = 0.103*cm;
    m_FiberOuterSurfaceRoughness = 0.01;
    
    // TPB Params
    G4double tpbOuterFraction = 0.01;
    m_TPB_Thickness = .1*mm;
    m_TPB_outerThickness = tpbOuterFraction*m_TPB_Thickness;
//    m_TPB_outerThickness = 0.099*mm;
    
    // Outer Reflector reflectivity (in the visible, 0 for UV)
    m_mirrorReflectivity = 1.0;
    
}

LightCollectionDetectorConstruction::~LightCollectionDetectorConstruction()
{
    delete m_StepLimit;
}

G4VPhysicalVolume* LightCollectionDetectorConstruction::Construct()
{
    
    m_Materials = nEDMMaterials::GetInstance();
    
    // World volume
    G4double world_x = 1.0*m;
    G4double world_y = 1.0*m;
    G4double world_z = 1.0*m;
    
    G4String worldName = "World";
    G4Box* solidHall = new G4Box(worldName, world_x/2., world_y/2., world_z/2.);
    
    // Why is World material SuperfluidHelium for room temperature tests?
    // I think I remember a bug with no ouput when you use air.
    // Someone should test this.
    
    m_LogicHall = new G4LogicalVolume(solidHall, m_Materials->GetMaterial("SuperfluidHelium"), worldName);

    //m_LogicHall = new G4LogicalVolume(solidHall, m_Materials->GetMaterial("G4Air"), worldName);
    
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
    ConstructSinglePlate();
    if (m_SqureTubeReflector) {ConstructSquareTubeReflector();}
    else if (m_FullTentReflector) {ConstructFullTentReflector();}
    
    // Always return the physical World
    return m_PhysHall;
    
}

void LightCollectionDetectorConstruction::ConstructSinglePlate(){
    
    //////////////////////////
    // Cell Side 1
    //////////////////////////
    G4String side1Name = "CellSide1";
    
    // Cell Side Solid, Logical, and TPB interface
    
    G4Box* cellSide1_solid = new G4Box(side1Name,
                                       m_CellWidth/2.,
                                       m_CellThickness/2.,
                                       m_CellLength/2.);
    
    G4LogicalVolume* logicCellSide1 = new G4LogicalVolume(cellSide1_solid,m_Materials->GetMaterial("PMMA"),side1Name);
    
    G4String TPBInterface1Name = side1Name + "/TPBInterface";
    
    // Create TPB Interface Layer
    G4Box* TPBInterface1_solid = new G4Box(TPBInterface1Name,
                                           m_CellWidth/2.,
                                           m_TPB_Thickness/2.,
                                           m_CellLength/2.);
    
    G4LogicalVolume* TPBInterface1_log = new G4LogicalVolume(TPBInterface1_solid,m_Materials->GetMaterial("TPB_inner"),TPBInterface1Name);
    
    G4ThreeVector TPBInterface1_pos = G4ThreeVector(0.,(m_CellThickness-m_TPB_Thickness)/2.,0.);
    
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
                                                 m_CellWidth/2,
                                                 m_TPB_outerThickness/2,
                                                 m_CellLength/2);
    
    
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
    G4VPhysicalVolume* physCellSide1 =
        new G4PVPlacement(0,                     // rotation
                          G4ThreeVector(0,0,0),  // position
                          logicCellSide1,        // logical volume
                          side1Name,                  // name
                          m_LogicHall,             // mother volume
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
            const G4int kEnergies = 4;
            G4double the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 5*eV, 16*eV};
            
            // Optical Surface for mirror
            G4OpticalSurface* mirror_surface_ =
            new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                                 dielectric_dielectric);
            
            // Reflectivity of mirror for each photon energy
            G4double mirror_REFL[kEnergies] = {0.998, 0.998, 0., 0.};
            
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

//        G4Tubs* fibDetSolid = new G4Tubs(fibDetName,
//                                         0, fibDetWidth/2.,
//                                         fibDetThickness/2.,
//                                         0*deg, 360*deg);
        
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
        
//        G4Tubs* fibDetBackFaceSolid = new G4Tubs(fibDetName,
//                                         0, fibDetWidth/2.,
//                                         fibDetThickness/4.,
//                                         0*deg, 360*deg);
        
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
            FibYPos = -1*(m_CellThickness/2.-FibThickness/2.-0.001*cm);
        }
        else{
            fiberParentLog = m_LogicHall;
            FibYPos = -1*(m_CellThickness/2+FibThickness/2+.005*cm);
        }
        
        // Loop over number of Fibers
        
        for(G4int i=0;i<m_NumberOfFibers;i++){
            FibXPos=-(m_FiberSpacing)*(m_NumberOfFibers-1)*0.5 + i*m_FiberSpacing;
            
            std::stringstream i_plusOne;
            i_plusOne << i+1;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),fiberLog,fiberName+i_plusOne.str(),fiberParentLog,false,0,m_CheckOverlaps);
            
            
            // Place +Z detectors
            physDetector1[i] = new G4PVPlacement(0, G4ThreeVector(FibXPos,FibYPos,fibDetZPos),fibDetLog,fibDetName+"1_"+i_plusOne.str(),m_LogicHall,false,0,m_CheckOverlaps);
            
            
            // Place -Z detectors
            physDetector2[i] = new G4PVPlacement(det2Rot,G4ThreeVector(FibXPos,FibYPos,-fibDetZPos),fibDetLog,fibDetName+"2_"+i_plusOne.str(),m_LogicHall,false,0,m_CheckOverlaps);
            
            
        }
        
        if (m_FiberOuterSurfaceRoughness < 1.){
            // Boundary Surface Properties
            
            G4OpticalSurface* fiberOpticalSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
            
            fiberOpticalSurface->SetModel(glisur);
            fiberOpticalSurface->SetFinish(ground);
            fiberOpticalSurface->SetType(dielectric_dielectric);
            fiberOpticalSurface->SetPolish(m_FiberOuterSurfaceRoughness);
            
            G4VPhysicalVolume* outerVol;
            if (m_EmbeddedFibers) {outerVol = physCellSide1;}
            else {outerVol = m_PhysHall; }
            
            for(G4int i=0;i<m_NumberOfFibers;i++){
                
                new G4LogicalBorderSurface("fiberOuterRoughSurface1",
                                            outerVol,
                                            physFiber[i],
                                            fiberOpticalSurface);
                
                new G4LogicalBorderSurface("fiberOuterRoughSurface2",
                                           physFiber[i],
                                           outerVol,
                                           fiberOpticalSurface);
                
                
            }
            
        }
        
    }
    

    
}

void LightCollectionDetectorConstruction::ConstructFullTentReflector()
{
    // Reflector Wrapping
    G4Box* BottomSolid = new G4Box("BottomReflector",m_CellWidth,0.1*cm,m_CellLength/2*1.5);
    
    
    G4double topReflRad = m_CellWidth;
    G4Tubs* TopSolid = new G4Tubs("InnerReflector",topReflRad,topReflRad+.1*cm,m_CellLength/2*1.5,.215*CLHEP::pi,.57*CLHEP::pi);
    
    G4Tubs* EndSolid = new G4Tubs("EndSolid",0,topReflRad+.1*cm,.1*cm,.215*CLHEP::pi,.57*CLHEP::pi);
    
    G4UnionSolid* TempRefl1 = new G4UnionSolid("tempRefl1",BottomSolid,EndSolid,0,G4ThreeVector(0.,-5./8.*topReflRad,m_CellLength/2*1.5));
    
    G4UnionSolid* TempRefl2 = new G4UnionSolid("tempRefl2",TempRefl1,EndSolid,0,G4ThreeVector(0.,-5./8.*topReflRad,-1*m_CellLength/2*1.5));
    
    G4UnionSolid* SolidReflector = new G4UnionSolid("Reflector", TempRefl2, TopSolid,0,G4ThreeVector(0.,-5./8.*topReflRad,0.));
    
    
    G4LogicalVolume* Reflector_Log = new G4LogicalVolume(SolidReflector, G4Material::GetMaterial("PMMA"), "Reflector");
    
    
    // Photon Energies for which mirror properties will be given
    const G4int kEnergies = 6;
    G4double the_photon_energies_[kEnergies] = {0.0*eV, 2.64*eV, 2.65*eV, 3.10*eV, 3.11*eV, 16*eV};
    // Wavelengths {inf, 469nm, 468nm, 400, 399nm, 78nm}
    
    // Optical Surface for mirror
    G4OpticalSurface* mirror_surface_ =
    new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                         dielectric_dielectric);
    
    // Reflectivity of mirror for each photon energy
    G4double mirror_REFL[kEnergies] = {0., 0., m_mirrorReflectivity, m_mirrorReflectivity, 0., 0.};
    
    //Table of Surface Properties for Mirror
    G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
    mirrorSurfaceProperty->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
    mirror_surface_->SetMaterialPropertiesTable(mirrorSurfaceProperty);
    
    new G4LogicalSkinSurface("Reflector_surface", Reflector_Log, mirror_surface_);
    
    
    G4VisAttributes* ReflectVis=new G4VisAttributes(G4Color(1.0,1.0,1.0));
    ReflectVis->SetVisibility(true);
    ReflectVis->SetForceWireframe(true);
    Reflector_Log->SetVisAttributes(ReflectVis);
    
    G4ThreeVector reflectorPos = G4ThreeVector(0.,-m_CellThickness,0.);
    
    new G4PVPlacement(0,                            //no rotation
                      reflectorPos,              //at (0,0,0)
                      Reflector_Log,                     //its logical volume
                      "Reflector",            //its name
                      m_LogicHall,                //its mother  volume
                      false,                        //no boolean operation
                      0,m_CheckOverlaps);                            //copy number
    
}

void LightCollectionDetectorConstruction::ConstructSquareTubeReflector(){
    // Square Tube Reflector Around Volume
    
    // Reflector Wrapping
    G4Box* OuterReflector = new G4Box("OuterReflector",8.*cm,8.*cm,2*20.64*cm);
    G4Box* InnerReflector = new G4Box("InnerReflector",7.*cm,7.*cm,2*20.64*cm);
    G4SubtractionSolid* SolidReflector = new G4SubtractionSolid("Reflector",OuterReflector,InnerReflector);
    
    G4LogicalVolume* Reflector_Log = new G4LogicalVolume(SolidReflector, G4Material::GetMaterial("PMMA"), "Reflector");
    
    
    // Photon Energies for which mirror properties will be given
    const G4int kEnergies = 4;
    G4double the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 5*eV, 16*eV};
    
    // Optical Surface for mirror
    G4OpticalSurface* mirror_surface_ =
    new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                         dielectric_dielectric);
    
    // Reflectivity of mirror for each photon energy
    G4double mirror_REFL[kEnergies] = {0.998, 0.998, 0., 0.};
    
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
                      m_LogicHall,                //its mother  volume
                      false,                        //no boolean operation
                      0,                        //copy number
                      m_CheckOverlaps);                    // Check Overlaps
}

void LightCollectionDetectorConstruction::SetMaxStep(G4double maxStep)
{
    if ((m_StepLimit)&&(maxStep>0.)) m_StepLimit->SetMaxAllowedStep(maxStep);
}

void LightCollectionDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
    m_CheckOverlaps = checkOverlaps;
}
