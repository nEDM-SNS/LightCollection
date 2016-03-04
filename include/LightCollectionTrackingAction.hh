#ifndef LightCollectionTrackingAction_h
#define LightCollectionTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class LightCollectionTrackingAction : public G4UserTrackingAction {
    
public:
    
    LightCollectionTrackingAction() { };
    virtual ~LightCollectionTrackingAction() { };
    
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
    
    
};

#endif
