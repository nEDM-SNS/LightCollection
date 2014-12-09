#ifndef LightCollectionEventAction_h
#define LightCollectionEventAction_h 1

#include "G4UserEventAction.hh"

class G4Event;

class LightCollectionEventAction : public G4UserEventAction
{
public:

    LightCollectionEventAction();
    virtual ~LightCollectionEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
};

#endif