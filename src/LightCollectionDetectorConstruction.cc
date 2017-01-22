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

nEDMSimpleDetectorConstruction::nEDMSimpleDetectorConstruction()
{
    fStepLimit = NULL;

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
    G4cout << "!!!!!!No Geometry Defined On Master Branch!!!!!!!" << G4endl;
    G4cout << "Please git checkout <specific geometry branch> or" << G4endl;
    G4cout << "git checkout -b <name of new geometry branch>" << G4endl;

    // Always return the physical World
    return fPhysHall;
    
}

void nEDMSimpleDetectorConstruction::SetMaxStep(G4double maxStep)
{
    if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void nEDMSimpleDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
    fCheckOverlaps = checkOverlaps;
}
