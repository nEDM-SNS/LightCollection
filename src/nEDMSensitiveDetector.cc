////////////////////////////////
// nEDMSensitiveDetector.cc //
///////////////////////////////

#include "nEDMSensitiveDetector.hh"

nEDMSensitiveDetector::nEDMSensitiveDetector(G4String SDname)
  : G4VSensitiveDetector(SDname)
{
  G4cout << "Creating SD with name: " << SDname << G4endl;
}

nEDMSensitiveDetector::~nEDMSensitiveDetector()
{
}

void nEDMSensitiveDetector::Initialize(G4HCofThisEvent * /*hce*/)
{
}

G4bool nEDMSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  const G4VPhysicalVolume* preStepVolume = preStepPoint->GetPhysicalVolume();
  const G4String preVolName = preStepVolume->GetName();

  G4StepPoint* postStepPoint = step->GetPostStepPoint();   
  const G4VPhysicalVolume* postStepVolume = postStepPoint->GetPhysicalVolume();
  const G4String postVolName = postStepVolume->GetName();

  //G4cout << "Pre volume " << preVolName << " post volume " << postVolName << G4endl;

  return true;
}

void nEDMSensitiveDetector::EndOfEvent(G4HCofThisEvent * /*hce*/)
{
}
