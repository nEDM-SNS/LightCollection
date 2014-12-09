#ifndef GPSPrimaryGeneratorAction_h
#define GPSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

class GPSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    GPSPrimaryGeneratorAction();
    ~GPSPrimaryGeneratorAction();
    
public:
    void GeneratePrimaries(G4Event* anEvent);
    
private:
    
    G4GeneralParticleSource* particleGun;
    
};

#endif


