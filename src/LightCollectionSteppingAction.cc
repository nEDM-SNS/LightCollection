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

LightCollectionSteppingAction::LightCollectionSteppingAction(){}

LightCollectionSteppingAction::~LightCollectionSteppingAction(){;}


/*! Called when the simulation steps forward. A lot is happening in this method and it is worth taking a look at. This handles detection of photons.
 */
void LightCollectionSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    
    // Kill tracks at first step for analyzing input
#if 0
    aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    return;
#endif

    
//    G4StepPoint* thePrePoint  = aStep->GetPreStepPoint();
    G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
    
    
    // Ignore steps at world boundary
    if (thePostPoint->GetStepStatus()!= fWorldBoundary)
    {
    
//        G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
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
        if (thePostPVname.contains("circleDet")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 4);
        }

        if (thePostPVname.contains("fibDet"))
        {
            if (thePostPVname.contains("BackFace"))
            {
                G4cout << "Killed in BackFace" << G4endl;
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            
            else
            {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                analysisManager->FillH1(0, 3);
                analysisManager->FillH1(3, h_Planck*c_light/aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy()/nm);
                analysisManager->FillH1(4, std::stoi(thePostPVname.substr(thePostPVname.rfind("_")+1,-1))); // replace the giant block of else and if
                analysisManager->FillH1(5, -1.*aStep->GetPreStepPoint()->GetMomentumDirection().cosTheta());
            }
            
        }
    
        if (thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpWLS")
        {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            if (thePostPVname.contains("TPBInterface"))
            {
                analysisManager->FillH1(6, aStep->GetNumberOfSecondariesInCurrentStep());
            }
            else if(thePostPVname.contains("Core"))
            {
                analysisManager->FillH1(7, aStep->GetNumberOfSecondariesInCurrentStep());
            }
            

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


