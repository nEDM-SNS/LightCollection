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
            
            G4String originVolumeName = aTrack->GetOriginTouchable()->GetVolume()->GetName();
            
            if (originVolumeName.contains("TPBInterface"))
            {
                analysisManager->FillH1(0, 1);
                analysisManager->FillH1(1, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
            }

            else if (originVolumeName.contains("Core"))
            {
                analysisManager->FillH1(0, 2);
                analysisManager->FillH1(2, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
                
            }
            else
            {
                G4cout << "Secondary produced by OpWLS in volume: " << originVolumeName << G4endl;
            }
            
        }
        else{
            G4cout << "Process Name = " << aTrack->GetCreatorProcess()->GetProcessName() << G4endl;
        }
        
        
    }
    
    else {
        G4cout << "!!!!! Negative Parent ID? !!!!" << G4endl;
    }
    
    
}


void LightCollectionTrackingAction::PostUserTrackingAction(const G4Track*)
{
//    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
//    
//    LightCollectionTrackInformation* trackInfo = (LightCollectionTrackInformation*)aTrack->GetUserInformation();
//    
//    G4int status = trackInfo->GetStatus();
//    
//    analysisManager->FillH1(5, status);
//    
//    if (status == 3 || status ==4 || status == 5) {
//        analysisManager->FillH1(5, 9);
//    }
//    else if (status == 1 || status ==2)
//    {
//        analysisManager->FillH1(5, 8);
//    }
}
