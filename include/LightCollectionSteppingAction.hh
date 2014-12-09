#ifndef LightCollectionSteppingAction_h
#define LightCollectionSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class NedmDetectorConstruction;

//! User defined actions to be taken for each simulation step.
class LightCollectionSteppingAction : public G4UserSteppingAction
{
  public:
    void UserSteppingAction(const G4Step*);
    
  private:

    NedmDetectorConstruction* detector;

};

#endif
