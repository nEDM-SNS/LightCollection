#include "LightCollectionSteppingAction.hh"

#include "G4Step.hh"
#include "G4VProcess.hh"

#include "LightCollectionAnalysis.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

#include <iostream>
using namespace std;

LightCollectionSteppingAction::LightCollectionSteppingAction(){}

LightCollectionSteppingAction::~LightCollectionSteppingAction(){;}


/*! Called when the simulation steps forward. This handles detection of photons. */
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
        
        //G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
        G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
        
        //G4String thePrePVname  = " ";
        G4String thePostPVname = " ";
        
        if (thePostPV) {
            //thePrePVname  = thePrePV->GetName();
            thePostPVname = thePostPV->GetName();
        }
        
        // Count Secondaries from WLS process
        //
        if (thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpWLS"){
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(6, aStep->GetNumberOfSecondariesInCurrentStep());
        }
        
        // Analysis code for single fiber.
        //
        if (thePostPVname.contains("fibDet")){
            
            if (thePostPVname.contains("BackFace")){
                //G4cout << "Killed in BackFace" << G4endl;
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (thePostPVname.contains( "fibDet1")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                G4double sinTheta = sin(thePrePoint->GetMomentumDirection().theta());
                
                analysisManager->FillH1(0, 1);
                analysisManager->FillH1(2, sinTheta);
                analysisManager->FillH1(4, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
                analysisManager->FillH1(5, thePrePoint->GetKineticEnergy()/eV);
                
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if(thePostPVname.contains("fibDet2")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                G4double sinTheta = sin(thePrePoint->GetMomentumDirection().theta());
                
                analysisManager->FillH1(0, 2);
                analysisManager->FillH1(3, sinTheta);
                analysisManager->FillH1(4, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
                analysisManager->FillH1(5, thePrePoint->GetKineticEnergy()/eV);
                
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                
                
            }
        }
    }
}
