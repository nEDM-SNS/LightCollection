#include "LightCollectionSteppingAction.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "LightCollectionEventInformation.hh"

#include "LightCollectionAnalysis.hh"
#include "nEDMDetectorConstruction.hh"
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


/*! Called when the simulation steps forward. A lot is happening in this method and it is worth taking a look at. This handles detection of photons.
 */
void LightCollectionSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    
    if(detectorTypes.size()==0){    detectorTypes = nEDMSimplePhysVolManager::GetInstance()->GetDetectorTypes();
    }

    
    
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
        
        G4String thePrePVname  = " ";
        G4String thePostPVname = " ";
        
        if (thePostPV) {
            thePrePVname  = thePrePV->GetName();
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
        

// Analysis code for single fiber
#if 0
        if (thePostPVname == "/nEDM/PhotDet_1") {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4double cosTheta = thePrePoint->GetMomentumDirection().z();
            
            analysisManager->FillH1(0, 6);
            analysisManager->FillH1(3, cosTheta);
            analysisManager->FillH1(6, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
            analysisManager->FillH1(7, thePrePoint->GetKineticEnergy()/eV);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);

        }
        else if(thePostPVname == "/nEDM/PhotDet_2") {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4double cosTheta = thePrePoint->GetMomentumDirection().z();
            
            analysisManager->FillH1(0, 7);
            analysisManager->FillH1(4, cosTheta);
            analysisManager->FillH1(6, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
            analysisManager->FillH1(7, thePrePoint->GetKineticEnergy()/eV);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);

        }
#endif


//Analysis code for 3 cell plates
#if 1

        if (thePostPVname.contains( "/nEDM/LHE/CellSide1/PhotDet1")) {
            
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4double cosTheta = thePrePoint->GetMomentumDirection().z();
            
            analysisManager->FillH1(0, 5);
            analysisManager->FillH1(3, cosTheta);
            analysisManager->FillH1(6, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
            analysisManager->FillH1(7, thePrePoint->GetKineticEnergy()/eV);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            
        }
        
        else if(thePostPVname.contains("/nEDM/LHE/CellSide1/PhotDet2")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4double cosTheta = thePrePoint->GetMomentumDirection().z();
            
            analysisManager->FillH1(0, 6);
            analysisManager->FillH1(4, cosTheta);
            analysisManager->FillH1(6, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
            analysisManager->FillH1(7, thePrePoint->GetKineticEnergy()/eV);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            
            
        }
        else if(thePostPVname.contains("/nEDM/LHE/CellSide2/PhotDet1")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillH1(0, 7);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("/nEDM/LHE/CellSide2/PhotDet2")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillH1(0, 8);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("/nEDM/LHE/CellSide3/PhotDet1")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillH1(0, 9);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("/nEDM/LHE/CellSide3/PhotDet2")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillH1(0, 10);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }

/*
        //  New Approach
        
        for (std::vector<detectorVector*>::iterator it=detectorTypes.begin(); it !=detectorTypes.end(); it++) {

            std::vector<G4VPhysicalVolume*>::iterator jt;

            jt = find((*it)->begin(), (*it)->end(), thePostPV);
            if (jt !=(*it)->end()){
                detType = distance(detectorTypes.begin(), it)+1;
                break;}
            else {detType=0;}
        }
        
        //if (detType!=0) {G4cout << "Detector Type:  " << detType << G4endl;}
  
        
        if (detType == 0){}
        else if (detType==1) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4double cosTheta = thePrePoint->GetMomentumDirection().z();
            
            analysisManager->FillH1(0, 5);
            analysisManager->FillH1(3, cosTheta);
            analysisManager->FillH1(6, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
            analysisManager->FillH1(7, thePrePoint->GetKineticEnergy()/eV);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            
        }
        else if (detType==2) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4double cosTheta = thePrePoint->GetMomentumDirection().z();
            
            analysisManager->FillH1(0, 6);
            analysisManager->FillH1(4, cosTheta);
            analysisManager->FillH1(6, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
            analysisManager->FillH1(7, thePrePoint->GetKineticEnergy()/eV);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            
            
        }
        else if (detType==3) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillH1(0, 7);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if (detType==4) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillH1(0, 8);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if (detType==5) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillH1(0, 9);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if (detType==6) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            
            analysisManager->FillH1(0, 10);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else {G4cout << "DetType out of range: " << detType << G4endl;}
*/
        

#endif
  
        
    }
    
}

void LightCollectionSteppingAction::OldSteppingActionCode(const G4Step* aStep){
    
    
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




