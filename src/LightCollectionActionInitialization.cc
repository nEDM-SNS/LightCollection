#include "LightCollectionActionInitialization.hh"

#include "GPSPrimaryGeneratorAction.hh"

#include "LightCollectionRunAction.hh"
#include "LightCollectionEventAction.hh"
#include "LightCollectionSteppingAction.hh"
#include "LightCollectionStackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionActionInitialization::LightCollectionActionInitialization()
: G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionActionInitialization::~LightCollectionActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionActionInitialization::BuildForMaster() const
{
    SetUserAction(new LightCollectionRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionActionInitialization::Build() const
{
    SetUserAction(new LightCollectionRunAction());
    
    SetUserAction(new GPSPrimaryGeneratorAction());
    
    SetUserAction(new LightCollectionEventAction());
    
    //SetUserAction(new LightCollectionStackingAction());
    
    SetUserAction(new LightCollectionSteppingAction());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......