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
    analysisManager->CreateH1("Photons", "Bins: Under=Prim, 0=C1_Inner(fib), 1=C1_Outer(fib), 2=C2_Inner, 3=C2_Outer, 4=C3_Inner, 5=C3_Outer, 6=fibWLSPhot, 7=fibdet +z, 8=fibdet -z, 9=C2det +z, 10=C2det -z,  11=C3 +z, 12=C3 -z", 13, 0., 13.);
    
    // index 1
    analysisManager->CreateH1("wlsFibSpectrum", "Wavelength of photons produced in Fiber by WLS Process", 100, 300., 800.);
    
    // index 2
    analysisManager->CreateH1("wlsTPBSpectrum", "Wavelength of photons produced in TPB by WLS Process", 100, 300., 800.);

    // index 3
    analysisManager->CreateH1("fibDetSpectrum", "Wavelength of photons detected in Fiber Detector", 100, 300., 800.);
    
    // index 4
    analysisManager->CreateH1("stdDetSpectrum", "Wavelength of photons detected in Standard Detector", 100, 300., 800.);

    // index 5
    analysisManager->CreateH1("fibDetIndex", "Fiber Detector Index", 100, 0, 100);
    
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
