#include "LightCollectionStackingAction.hh"
#include "LightCollectionEventInformation.hh"
#include "LightCollectionSteppingAction.hh"
#include "LightCollectionAnalysis.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionStackingAction::LightCollectionStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionStackingAction::~LightCollectionStackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
LightCollectionStackingAction::ClassifyNewTrack(const G4Track *){
    
    //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    //LightCollectionEventInformation* eventInformation=(LightCollectionEventInformation*)G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();
    
    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionStackingAction::NewStage() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionStackingAction::PrepareNewEvent() {}
