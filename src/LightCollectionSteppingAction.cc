#include "LightCollectionSteppingAction.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "LightCollectionEventInformation.hh"

#include "LightCollectionAnalysis.hh"
#include "NedmDetectorConstruction.hh"
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

#include <iostream>
using namespace std;


/*! Called when the simulation steps forward. A lot is happening in this method and it is worth taking a look at. This handles detection of photons as well as bounce and absorption tracking.
 */
void LightCollectionSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    
    
    // Kill tracks at first step for analyzing input
#if 0
    aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    return;
#endif

    
    // get point of entry and exit
    const G4StepPoint *p_in  = aStep->GetPreStepPoint();
    const G4StepPoint *p_out = aStep->GetPostStepPoint();
    //const G4VProcess *p1 = p_in->GetProcessDefinedStep();
    //const G4VProcess *p2 = p_out->GetProcessDefinedStep();
    
    
    // Find boundary status
    
    G4OpBoundaryProcessStatus boundaryStatus=Undefined;
    static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;
    
    G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();
    
    if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
        
        if(!boundary){
            G4ProcessManager* pm
            = aStep->GetTrack()->GetDefinition()->GetProcessManager();
            G4int nprocesses = pm->GetProcessListLength();
            G4ProcessVector* pv = pm->GetProcessList();
            G4int i;
            for( i=0;i<nprocesses;i++){
                if((*pv)[i]->GetProcessName()=="OpBoundary"){
                    boundary = (G4OpBoundaryProcess*)(*pv)[i];
                    break;
                }
            }
        }
        
        boundaryStatus = boundary->GetStatus();
        
        G4LogicalVolume* aVol;
        G4LogicalSurface* surface = NULL;
        G4StepStatus stepStatus = p_out->GetStepStatus();
        
        switch( stepStatus )
        {
                
            case fGeomBoundary:
                
                switch( boundaryStatus )
            {
                case Detection:
                    if( p_out->GetMaterial() ) {
                        aVol = p_out->GetTouchableHandle()
                        ->GetVolume()->GetLogicalVolume();
                        
                        
                        if( aVol->GetName() == "PMT" ) {
                            G4int copy_no = p_out->GetPhysicalVolume()->GetCopyNo();
                            
                            G4Event* currentEvent = G4EventManager::GetEventManager()->GetNonconstCurrentEvent();
                            LightCollectionEventInformation* eventInfo = (LightCollectionEventInformation*)currentEvent->GetUserInformation();
                            
                            eventInfo->Detected(copy_no+1, aStep->GetTrack()->GetTotalEnergy());
                            
                            /*
                             trackInfo->set_detected(true);
                             trackInfo->set_pmt_number(copy_no+1);
                             trackInfo->set_time_detected(aStep->GetTrack()->GetGlobalTime());
                             */
                        }
                    }
                    break;
                    
                default:
                    break;
            }
                break;
                
            default:
                break;
        }
    }
    
}

