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
    m_FiberReflector = false;

    // Fiber Params
    m_FiberOuterSurfaceRoughness = 0.9;
    
    
}

LightCollectionDetectorConstruction::~LightCollectionDetectorConstruction()
{
    delete m_StepLimit;
}

G4VPhysicalVolume* LightCollectionDetectorConstruction::Construct()
{
    
    m_Materials = nEDMMaterials::GetInstance();
    
    // World volume
    G4double world_x = 0.1*m;
    G4double world_y = 0.1*m;
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
    ConstructSingleFiber();

    
    // Always return the physical World
    return m_PhysHall;
    
}

void LightCollectionDetectorConstruction::ConstructSingleFiber()
{

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
    
    G4VPhysicalVolume* physFiber = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fiberLog,fiberName,m_LogicHall,false,0,m_CheckOverlaps);
    
    //Set Vis attributes
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
    
    
    
    // Create Reflector on the negative z end of fiber
    //
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
    
    // Place Physical Detectors
    G4double fibDetZPos = fiberLength/2.+fibDetThickness/2.;
    
    G4RotationMatrix* det2Rot = new G4RotationMatrix();
    det2Rot->rotateY(180*deg);
    
    G4LogicalVolume* fiberParentLog;
    fiberParentLog = m_LogicHall;
    
    // Place +Z detectors
    new G4PVPlacement(0, G4ThreeVector(0.,0.,fibDetZPos),fibDetLog,fibDetName+"1",m_LogicHall,false,0,m_CheckOverlaps);
    
    
    // Place -Z detectors
    new G4PVPlacement(det2Rot,G4ThreeVector(0.,0.,-fibDetZPos),fibDetLog,fibDetName+"2",m_LogicHall,false,0,m_CheckOverlaps);
    
    
    // Create Rough Surface on Outside of Fiber
    if (m_FiberOuterSurfaceRoughness < 1.){
        // Boundary Surface Properties
        
        G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
        
        G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
        
        G4VPhysicalVolume* outerVol;
        outerVol = m_PhysHall;
        
        fiberOuterRoughSurface = new G4LogicalBorderSurface("fiberOuterRoughSurface",
                                                            outerVol,
                                                            physFiber,
                                                            fiberOuterRoughOpSurface);
        
        fiberOuterRoughOpSurface->SetModel(glisur);
        fiberOuterRoughOpSurface->SetFinish(ground);
        fiberOuterRoughOpSurface->SetType(dielectric_dielectric);
        fiberOuterRoughOpSurface->SetPolish(m_FiberOuterSurfaceRoughness);
        
        
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
