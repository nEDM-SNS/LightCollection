////////////////////////////////////////////////////////
// nEDMDetectorConstruction.cc                        //
// Coarse nEDM Geometry                               //
////////////////////////////////////////////////////////

#include "nEDMDetectorConstruction.hh"
#include "nEDMDetectorMessenger.hh"

#include "G4Material.hh"
#include "nEDMMaterials.hh"
//#include "nEDMAnalysis.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Cons.hh"

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

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh"

#include "nEDMSensitiveDetector.hh"

#include "nEDMWorldConstructor.hh"

nEDMDetectorConstruction::nEDMDetectorConstruction()
{
  fDetectorMessenger = new nEDMDetectorMessenger(this);
  fWorldConstructor = new nEDMWorldConstructor("/nEDM",this);
  
  fStepLimit = NULL;

  fCheckOverlaps = false;
  
}

nEDMDetectorConstruction::~nEDMDetectorConstruction()
{
  delete fStepLimit;
  delete fDetectorMessenger;
}

G4VPhysicalVolume* nEDMDetectorConstruction::Construct()
{
  /*
  if (fPhysiWorld) {
     G4cout << " in nEDMDetectorConstruction::Construct() " << G4endl;
     G4GeometryManager::GetInstance()->OpenGeometry();
     G4PhysicalVolumeStore::GetInstance()->Clean();
     G4LogicalVolumeStore::GetInstance()->Clean();
     G4SolidStore::GetInstance()->Clean();
  }
  */

  nEDMMaterials::GetInstance();
  return ConstructDetector();
}

G4VPhysicalVolume* nEDMDetectorConstruction::ConstructDetector()
{
  G4String name = fWorldConstructor->GetName();
  fWorldConstructor->SetCheckOverlaps(fCheckOverlaps);
  G4LogicalVolume* logicHall = fWorldConstructor->GetPiece();
  
  G4VPhysicalVolume* physHall = new G4PVPlacement(0,                     // rotation
                                                  G4ThreeVector(0,0,0),
                                                  logicHall,             // logical volume
                                                  name,                  // name
                                                  0,                     // mother volume
                                                  false,                 // no boolean operations
                                                  0,fCheckOverlaps);     // not a copy

  nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(name,physHall);  
  //nEDMSimplePhysVolManager::GetInstance()->PrintAllVolumes();
  
  //nEDMAnalysis::GetInstance()->SetGeometry(physHall);
  
  // Always return the physical World
  return physHall; 
}

void nEDMDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());
}

void nEDMDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void nEDMDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}
