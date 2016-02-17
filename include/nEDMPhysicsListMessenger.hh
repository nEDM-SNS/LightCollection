#ifndef nEDMPhysicsListMessenger_h
#define nEDMPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class nEDMPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//! Provides access to Physics process parameters interactively
class nEDMPhysicsListMessenger: public G4UImessenger
{
  public:  
    nEDMPhysicsListMessenger(nEDMPhysicsList* );
   ~nEDMPhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:  
    nEDMPhysicsList*     pPhysicsList;
    
    G4UIdirectory*        nEDMDir;
    G4UIdirectory*        physDir;
    G4UIcmdWithAnInteger* verboseCmd;
    G4UIcmdWithAnInteger* cerenkovCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

