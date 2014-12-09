#ifndef LightCollectionStackingAction_H
#define LightCollectionStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class LightCollectionStackingAction : public G4UserStackingAction
{
  public:

    LightCollectionStackingAction();
    virtual ~LightCollectionStackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
};

#endif
