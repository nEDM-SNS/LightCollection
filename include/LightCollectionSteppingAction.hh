#ifndef LightCollectionSteppingAction_h
#define LightCollectionSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class nEDMDetectorConstruction;
class G4VPhysicalVolume;

//! User defined actions to be taken for each simulation step.
class LightCollectionSteppingAction : public G4UserSteppingAction
{
  public:
    LightCollectionSteppingAction();
    virtual ~LightCollectionSteppingAction();
    
    void UserSteppingAction(const G4Step*);

    
  private:
    
    G4VPhysicalVolume* detectorArray[1000];

    nEDMDetectorConstruction* detector = {0};
    
    void OldSteppingActionCode(const G4Step* aStep);

};

#endif
