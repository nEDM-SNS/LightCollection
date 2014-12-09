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
LightCollectionStackingAction::ClassifyNewTrack(const G4Track * aTrack){
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    //LightCollectionEventInformation* eventInformation=(LightCollectionEventInformation*)G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetUserInformation();
    
    //Count what process generated the optical photons
    if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){
        // particle is optical photon
        if(aTrack->GetParentID()>0){
            // particle is secondary
            if(aTrack->GetCreatorProcess()->GetProcessName()=="Scintillation")
            {
                //eventInformation->IncPhotonCount_Scint();
            }
            
            else if(aTrack->GetCreatorProcess()->GetProcessName()=="Cerenkov")
            {
                //eventInformation->IncPhotonCount_Ceren();
            }
            else if(aTrack->GetCreatorProcess()->GetProcessName()=="OpTPB")
            {
                analysisManager->FillH1(12, 1);
            }
            else if(aTrack->GetCreatorProcess()->GetProcessName()=="OpWLS")
            {
                analysisManager->FillH1(13, 1);
            }

        }
    }
    else{
    }
    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionStackingAction::NewStage() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionStackingAction::PrepareNewEvent() {}
