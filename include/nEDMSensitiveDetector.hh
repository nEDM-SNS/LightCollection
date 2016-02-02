//////////////////////////////////////////
// nEDMSensitiveDetector.hh //
// Sensitive detector for nEDM to interact and extract hit information  //
//////////////////////////////////////////

#ifndef nEDMSensitiveDetector_h
#define nEDMSensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;

class nEDMSensitiveDetector : public G4VSensitiveDetector
{
public:
  nEDMSensitiveDetector(G4String SDname);
  ~nEDMSensitiveDetector();

  void Initialize(G4HCofThisEvent *hce);
  G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);
  void EndOfEvent(G4HCofThisEvent *hce);

};

#endif
