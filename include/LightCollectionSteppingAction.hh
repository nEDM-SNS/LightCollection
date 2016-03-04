#ifndef LightCollectionSteppingAction_h
#define LightCollectionSteppingAction_h 1

#include <vector>
#include "G4UserSteppingAction.hh"
#include "G4VPhysicalVolume.hh"

typedef std::vector<G4VPhysicalVolume*> detectorVector;

class G4VPhysicalVolume;

//! User defined actions to be taken for each simulation step.
class LightCollectionSteppingAction : public G4UserSteppingAction
{
  public:
    LightCollectionSteppingAction();
    virtual ~LightCollectionSteppingAction();
    
    void UserSteppingAction(const G4Step*);

    
  private:
    
    void OldSteppingActionCode(const G4Step* aStep);
    
    std::vector<detectorVector*> detectorTypes;

    G4int detType;


};

#endif
