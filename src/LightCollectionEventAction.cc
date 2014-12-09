#include "LightCollectionEventAction.hh"
#include "LightCollectionEventInformation.hh"
#include "LightCollectionAnalysis.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionEventAction::LightCollectionEventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionEventAction::~LightCollectionEventAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void LightCollectionEventAction::BeginOfEventAction(const G4Event* anEvent)
{
    //New event, add the user information object
    G4EventManager::
    GetEventManager()->SetUserInformation(new LightCollectionEventInformation());
    
    G4int eventID = anEvent->GetEventID();
    if ( eventID % 1000 == 0 )  {
        G4cout << anEvent->GetEventID() << " events simulated." << G4endl;
    }

}

void LightCollectionEventAction::EndOfEventAction(const G4Event * anEvent)
{
    LightCollectionEventInformation* eventInformation = (LightCollectionEventInformation*)anEvent->GetUserInformation();
    
    // get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    G4int detect;
    
    detect = eventInformation->GetDetector1();
    if (detect!=0) {
        analysisManager->FillH1(0, eventInformation->GetDetector1());
        analysisManager->FillH1(3, eventInformation->GetEnergy1()/eV);

    }

    detect = eventInformation->GetDetector2();
    if (detect!=0) {
        analysisManager->FillH1(1, eventInformation->GetDetector2());
        analysisManager->FillH1(4, eventInformation->GetEnergy2()/eV);
    }
    
    detect = eventInformation->GetDetector3();
    if (detect!=0) {
        analysisManager->FillH1(2, eventInformation->GetDetector3());
        analysisManager->FillH1(5, eventInformation->GetEnergy3()/eV);

    }


 }