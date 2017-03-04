#ifndef nEDMModularPhysicsList_h
#define nEDMModularPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics

class nEDMModularPhysicsList: public G4VModularPhysicsList
{
public:
  nEDMModularPhysicsList();
  virtual ~nEDMModularPhysicsList();

  virtual void SetCuts();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

