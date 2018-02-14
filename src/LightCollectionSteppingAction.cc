#include "LightCollectionSteppingAction.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "LightCollectionEventInformation.hh"

#include "LightCollectionAnalysis.hh"
#include "LightCollectionEventAction.hh"

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"

#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "G4PhysicsTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4ParticleDefinition.hh"

#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4TrackingManager.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpProcessSubType.hh"

#include "G4TrackStatus.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
using namespace std;

LightCollectionSteppingAction::LightCollectionSteppingAction(){
    
    
    
}

LightCollectionSteppingAction::~LightCollectionSteppingAction(){;}


/*! Called when the simulation steps forward. This handles detection of photons.
 */
void LightCollectionSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    
    // Kill tracks at first step for analyzing input
#if 0
    aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    return;
#endif
    
    
        G4StepPoint* thePrePoint  = aStep->GetPreStepPoint();
    G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
    
    
    // Ignore steps at world boundary
    if (thePostPoint->GetStepStatus()!= fWorldBoundary) {
        
                G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
        G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
        
        //        G4String thePrePVname  = " ";
        G4String thePostPVname = " ";
        
        if (thePostPV) {
            //            thePrePVname  = thePrePV->GetName();
            thePostPVname = thePostPV->GetName();
        }
        
        //G4cout << "Material:  " << thePostPV->GetLogicalVolume()->GetMaterial()->GetName() << G4endl;
        
        // Check for any optical surface crossings and print the name of the surface
#if 0
        G4LogicalSurface* surface = NULL;
        surface = G4LogicalBorderSurface::GetSurface(thePrePoint->GetPhysicalVolume(),
                                                     thePostPoint->GetPhysicalVolume());
        if (surface) {
            G4cout << "Surface: " << surface->GetName() << G4endl;
        }
#endif
        
        
        
        //Analysis code for single cell plate
        if (thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpWLS"){

            if(thePostPVname.contains("Fiber"))
            {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(6, aStep->GetNumberOfSecondariesInCurrentStep());
            }
            else if (thePostPVname.contains("CellSide"))
            {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(7, aStep->GetNumberOfSecondariesInCurrentStep());
            }
            else{
                G4cout << "Something Unexpected Happened" << G4endl;
            }

        }
        
        if (thePostPVname.contains("fibDet")){
            
            if (thePostPVname.contains("BackFace")){
                //G4cout << "Killed in BackFace" << G4endl;
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (thePostPVname.contains( "fibDet1_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 7);
                analysisManager->FillH1(3, h_Planck*c_light/aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy()/nm);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                
                 G4double sinTheta = sin(thePrePoint->GetMomentumDirection().theta());

                if(sinTheta<0.49) // sin(26.7deg)=0.49
                {analysisManager->FillH1(8, sinTheta);}
                analysisManager->FillH1(5, std::stoi(thePostPVname.substr(thePostPVname.rfind("_")+1,-1))); // replace the giant block of else and if
                
               
            }
            else if(thePostPVname.contains("fibDet2_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 8);
                analysisManager->FillH1(3, h_Planck*c_light/aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy()/nm);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                analysisManager->FillH1(5, std::stoi(thePostPVname.substr(thePostPVname.rfind("_")+1,-1))); // replace the giant block of else and if

              
                
            }
            
        }
        
        else if(thePostPVname.contains("CellSide2/StdDet1")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 9);
            analysisManager->FillH1(4, h_Planck*c_light/aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy()/nm);
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("CellSide2/StdDet2")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 10);
            analysisManager->FillH1(4, h_Planck*c_light/aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy()/nm);
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("CellSide3/StdDet1")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 11);
            analysisManager->FillH1(4, h_Planck*c_light/aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy()/nm);
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("CellSide3/StdDet2")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 12);
            analysisManager->FillH1(4, h_Planck*c_light/aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy()/nm);
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }

        
            // Kill Green Photons that exit fiber (not trapped)
        G4String originVolName = aStep->GetTrack()->GetOriginTouchableHandle()->GetVolume()->GetName();
        
        if (originVolName.contains("WLSFiber")) {
            
            if (thePrePVname.contains("WLSFiber") &&
                (!thePostPVname.contains("WLSFiber") && !thePostPVname.contains("fibDet")))
            {
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                
            }

        }
    
    }
    
}
