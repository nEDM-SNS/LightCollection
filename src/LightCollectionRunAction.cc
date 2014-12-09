// Make this appear first!
#include "G4Timer.hh"

#include "LightCollectionRunAction.hh"
#include "LightCollectionAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionRunAction::LightCollectionRunAction()
 : G4UserRunAction(),
   timer(0)
{
    if (IsMaster()) {
        timer = new G4Timer();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionRunAction::~LightCollectionRunAction()
{
  delete timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4int runID = aRun->GetRunID();
    
    if (IsMaster()){
        G4cout << "### Run " << runID << " start." << G4endl;
        timer->Start();
    }
    
    //inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
    
    //LightCollectionAnalysisManager::GetInstance()->BeginOfRun(runID);
    // get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    analysisManager->OpenFile("StndPlate_0.9");
    
    analysisManager->CreateH1("Detector1", "Number of Photons from LightGuide1", 10, 0, 9);
    analysisManager->CreateH1("Detector2", "Number of Photons from LightGuide2", 10, 0, 9);
    analysisManager->CreateH1("Detector3", "Number of Photons from LightGuide3", 10, 0, 9);
    
    analysisManager->CreateH1("Energy1", "Energy of Photons from LightGuide1", 100, 0., 20.);
    analysisManager->CreateH1("Energy2", "Energy of Photons from LightGuide2", 100, 0., 20.);
    analysisManager->CreateH1("Energy3", "Energy of Photons from LightGuide3", 100, 0., 20.);
    
    analysisManager->CreateH1("PenetrationLength", "Sampled penetration length", 300, 0., 120);

    analysisManager->CreateH1("PenetrationDepth", "Sampled penetration Depth", 300, 0., 60);
    
    analysisManager->CreateH1("MFPtraveled", "Number of MFPs traveled", 200, 0, 5);

    analysisManager->CreateH1("MFPdeep", "Number of MFPs deep", 200, 0, 3);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionRunAction::EndOfRunAction(const G4Run* aRun)
{

    //LightCollectionAnalysisManager::GetInstance()->EndOfRun();
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // save histograms
    //
    analysisManager->Write();
    analysisManager->CloseFile();
    
    // complete cleanup
    //
    delete G4AnalysisManager::Instance();

    
    if (IsMaster()){
        timer->Stop();
        G4cout << "*****G4 RUN TIMER*****" << G4endl;
        G4cout << *timer << G4endl;
        
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
