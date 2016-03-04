#ifndef LightCollectionTrackInformation_h
#define LightCollectionTrackInformation_h

#include "G4VUserTrackInformation.hh"


class LightCollectionTrackInformation : public G4VUserTrackInformation{
    
public:
    LightCollectionTrackInformation();
    virtual ~LightCollectionTrackInformation();
    
    void SetStatus(G4int newStatus) {fStatus = newStatus;}
    G4int GetStatus() {return fStatus;}

private:
    G4int fStatus;
    
};

#endif
