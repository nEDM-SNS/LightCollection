#include "globals.hh"
#include "G4RunManager.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4TrackingManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "LightCollectionTrackingAction.hh"
#include "LightCollectionTrackInformation.hh"
#include "LightCollectionAnalysis.hh"

void LightCollectionTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    LightCollectionTrackInformation* trackInfo = new LightCollectionTrackInformation();
    
    fpTrackingManager->SetUserTrackInformation(trackInfo);
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    if (aTrack->GetParentID()==0) {
        // particle is a primary, store in underflow
        analysisManager->FillH1(0, -1);
    }
    else if(aTrack->GetParentID()>0){
        // particle is secondary
        if (aTrack->GetCreatorProcess()->GetProcessName() == "OpWLS") {
            analysisManager->FillH1(1, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
        }
        else{G4cout << "Process Name = " << aTrack->GetCreatorProcess()->GetProcessName() << G4endl;
        }
    }
    else {
        G4cout << "!!!!! Negative Parent ID? !!!!" << G4endl;
    }    
}


void LightCollectionTrackingAction::PostUserTrackingAction(const G4Track*)
{
    //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    //LightCollectionTrackInformation* trackInfo = (LightCollectionTrackInformation*)aTrack->GetUserInformation();
    
    // G4int status = trackInfo->GetStatus();
    
}
