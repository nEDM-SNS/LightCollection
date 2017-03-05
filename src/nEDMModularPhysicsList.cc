#include "nEDMModularPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4StepLimiterPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEDMModularPhysicsList::nEDMModularPhysicsList()
: G4VModularPhysicsList(){
    SetVerboseLevel(0);
    
    // Default physics
    RegisterPhysics(new G4DecayPhysics());
    
    // Radioactive decay
    //RegisterPhysics(new G4RadioactiveDecayPhysics());
    
    // EM physics
    //RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4EmLivermorePhysics());
    
    // Optical Physics
    RegisterPhysics(new G4OpticalPhysics());
    
    // Step Limiter
    RegisterPhysics(new G4StepLimiterPhysics());
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

nEDMModularPhysicsList::~nEDMModularPhysicsList()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void nEDMModularPhysicsList::SetCuts()
{
    G4VUserPhysicsList::SetCuts();
}  
