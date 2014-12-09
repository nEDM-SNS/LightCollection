#include "GPSPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

GPSPrimaryGeneratorAction::GPSPrimaryGeneratorAction()
{
    particleGun = new G4GeneralParticleSource();
}

GPSPrimaryGeneratorAction::~GPSPrimaryGeneratorAction()
{
    delete particleGun;
}

void GPSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    particleGun->GeneratePrimaryVertex(anEvent);
    
}
