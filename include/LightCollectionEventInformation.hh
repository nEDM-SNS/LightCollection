#ifndef LightCollectionEventInformation_h
#define LightCollectionEventInformation_h 1

#include "G4VUserEventInformation.hh"
#include "globals.hh"

class LightCollectionEventInformation : public G4VUserEventInformation
{
public:
    LightCollectionEventInformation();
    virtual ~LightCollectionEventInformation();
    
    inline virtual void Print()const{};
    
    G4double TPBWavelength;
    
    void Detected(G4int detector, G4double ene);
    
    G4int GetDetector1(){return detector1;}
    G4int GetDetector2(){return detector2;}
    G4int GetDetector3(){return detector3;}
    G4double GetEnergy1(){return energy1;}
    G4double GetEnergy2(){return energy2;}
    G4double GetEnergy3(){return energy3;}

private:
    G4int detector1;
    G4int detector2;
    G4int detector3;
    G4double energy1;
    G4double energy2;
    G4double energy3;
};

#endif