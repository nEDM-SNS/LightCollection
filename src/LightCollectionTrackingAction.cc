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
    //    LightCollectionTrackInformation* trackInfo = new LightCollectionTrackInformation();
    
    //    fpTrackingManager->SetUserTrackInformation(trackInfo);
    
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    if (aTrack->GetParentID()==0) {
        // particle is a primary, store in underflow
        analysisManager->FillH1(0, -1);
    }
    
    else if(aTrack->GetParentID()>0){
        // particle is secondary
        
        
        if (aTrack->GetCreatorProcess()->GetProcessName() == "OpWLS") {
            
            G4String originVolumeName = aTrack->GetOriginTouchable()->GetVolume()->GetName();
            
            if (originVolumeName.contains("TPBInterface")) {
                if (originVolumeName.contains("CellSide1")) {
                    analysisManager->FillH1(0, 0);
                }
                else if (originVolumeName.contains("CellSide2")) {
                    analysisManager->FillH1(0, 2);
                }
                else if (originVolumeName.contains("CellSide3")) {
                    analysisManager->FillH1(0, 4);
                }
                else {
                    G4cout << "Something Unexpected Happend" << G4endl;
                }
                analysisManager->FillH1(1, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
            }
            else if (originVolumeName.contains("TPB_outer"))
            {
                if (originVolumeName.contains("CellSide1")) {
                    analysisManager->FillH1(0, 1);
                }
                else if (originVolumeName.contains("CellSide2")) {
                    analysisManager->FillH1(0, 3);
                }
                else if (originVolumeName.contains("CellSide3")) {
                    analysisManager->FillH1(0, 5);
                }
                else {
                    G4cout << "Something Unexpected Happend" << G4endl;
                }
                analysisManager->FillH1(1, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
            }
            else if (originVolumeName.contains("Fiber")){
                analysisManager->FillH1(0, 6);
                analysisManager->FillH1(1, h_Planck*c_light/aTrack->GetDynamicParticle()->GetKineticEnergy()/nm);
            }
            else
            {
                G4cout << "Something Unexpected Happend" << G4endl;
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


void LightCollectionTrackingAction::PostUserTrackingAction(const G4Track* )
{
}