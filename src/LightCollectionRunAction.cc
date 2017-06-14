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
    analysisManager->CreateH1("Photons", "Bins: Under=Prim, 0=InnerTPB, 1=OuterTPB, 6=FiberWLSPhot, 7=fibdet +z, 8=fibdet -z", 13, 0., 13.);
    
    // index 1
    analysisManager->CreateH1("wlsFibSpectrum", "Wavelength of photons produced by WLS Process in Fibers", 100, 300., 800.);
    
    // index 2
    analysisManager->CreateH1("wlsTPBSpectrum", "Wavelength of photons produced by WLS Process in TPB", 100, 300., 800.);
    
    // index 3
    analysisManager->CreateH1("detSpectrum", "Wavelength of photons detected", 100, 300., 800.);
    
    // index 4
    analysisManager->CreateH1("detIndex", "Detector Index", 100, 0, 100);
    
    //index 5
    analysisManager->CreateH1("numFibSecondaries", "Number of Secondaries in WLS in Fiber", 10, 0, 9);
    
    //index 6
    analysisManager->CreateH1("numTPBSecondaries", "Number of Secondaries in WLS in TPB", 10, 0, 9);
  
    // index 7
    analysisManager->CreateH1("fibConvSpectrum", "Wavelength of incoming photons converted by WLS Process in fibers", 100, 300., 800.);

    
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
