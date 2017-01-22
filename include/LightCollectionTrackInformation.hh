#ifndef LightCollectionTrackInformation_h
#define LightCollectionTrackInformation_h

#include "G4VUserTrackInformation.hh"


class LightCollectionTrackInformation : public G4VUserTrackInformation{
    
public:
    LightCollectionTrackInformation();
    virtual ~LightCollectionTrackInformation();
    
    void SetStatus(G4int newStatus) {m_Status = newStatus;}
    G4int GetStatus() {return m_Status;}

private:
    G4int m_Status;
    
};

#endif
