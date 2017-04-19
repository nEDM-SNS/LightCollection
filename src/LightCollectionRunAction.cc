// Make this appear first!
#include "G4Timer.hh"

#include "LightCollectionRunAction.hh"
#include "LightCollectionAnalysis.hh"
#include "LightCollectionRunActionMessenger.hh"

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
    
    fMessenger = new LightCollectionRunActionMessenger(this);
    fileName = "";
    
    // Create analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFirstHistoId(0);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionRunAction::~LightCollectionRunAction()
{
    delete timer;
    delete G4AnalysisManager::Instance();

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
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    if(fileName==""){fileName = "output/LightCollection";}
    analysisManager->OpenFile(fileName);
    
    // index 0
    analysisManager->CreateH1("Photons", "Bins: Under=Prim, 1=fibdet +z, 2=fibdet -z", 10, 0., 10.);
    
    // index 1
    analysisManager->CreateH1("wlsFibSpectrum", "Wavelength of photons produced in fibers", 100, 300., 800.);
    
    // index 2
    analysisManager->CreateH1("sinThetaPosz", "Sine of the exit angle for +z end of fiber", 100, -1.001, 1.001);
    
    // index 3
    analysisManager->CreateH1("sinThetaNegz", "Sine of the exit angle for -z end of fiber", 100, -1.001, 1.001);
    
    // index 4
    analysisManager->CreateH1("detSpectrum", "Wavelength of photons detected", 100, 300., 800.);
    
    // index 5
    analysisManager->CreateH1("detEnergy", "Energy of photons detected", 100, 1., 4.);
    
    //index 6
    analysisManager->CreateH1("numSecondaries", "Number of Secondaries in WLS", 10, 0, 9);
    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionRunAction::EndOfRunAction(const G4Run*)
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
