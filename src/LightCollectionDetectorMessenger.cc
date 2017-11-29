#include "LightCollectionDetectorMessenger.hh"
#include "LightCollectionDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionDetectorMessenger::LightCollectionDetectorMessenger(LightCollectionDetectorConstruction* Det)
: G4UImessenger(),
fDetectorConstruction(Det)
{
    
    m_DetDirectory = new G4UIdirectory("/LightCollection/det/");
    m_DetDirectory->SetGuidance("Detector construction control");
    
    //  fTargMatCmd = new G4UIcmdWithAString("/B2/det/setTargetMaterial",this);
    //  fTargMatCmd->SetGuidance("Select Material of the Target.");
    //  fTargMatCmd->SetParameterName("choice",false);
    //  fTargMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    //
    //  fChamMatCmd = new G4UIcmdWithAString("/B2/det/setChamberMaterial",this);
    //  fChamMatCmd->SetGuidance("Select Material of the Chamber.");
    //  fChamMatCmd->SetParameterName("choice",false);
    //  fChamMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    //
    
    m_FiberHLCmd = new G4UIcmdWithADoubleAndUnit("/LightCollection/det/fiberHalfLength",this);
    m_FiberHLCmd->SetGuidance("Set half length of Fiber");
    m_FiberHLCmd->SetParameterName("halfLength", false);
    m_FiberHLCmd->SetUnitCategory("Length");
    m_FiberHLCmd->AvailableForStates(G4State_Idle);
    
    m_FiberSmoothCmd = new G4UIcmdWithADouble("/LightCollection/det/fiberSmoothness",this);
    m_FiberSmoothCmd->SetGuidance("Set Smoothness of Outer Fiber Surface");
    m_FiberSmoothCmd->SetParameterName("smoothness", false);
    m_FiberHLCmd->AvailableForStates(G4State_Idle);
    
    m_StepMaxCmd = new G4UIcmdWithADoubleAndUnit("/LightCollection/det/stepMax",this);
    m_StepMaxCmd->SetGuidance("Define a step max");
    m_StepMaxCmd->SetParameterName("stepMax",false);
    m_StepMaxCmd->SetUnitCategory("Length");
    m_StepMaxCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionDetectorMessenger::~LightCollectionDetectorMessenger()
{
    //  delete m_TargMatCmd;
    //  delete m_ChamMatCmd;
    delete m_StepMaxCmd;
    delete m_DetDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LightCollectionDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    //  if( command == fTargMatCmd )
    //   { fDetectorConstruction->SetTargetMaterial(newValue);}
    //
    //  if( command == fChamMatCmd )
    //   { fDetectorConstruction->SetChamberMaterial(newValue);}
    
    if (command==m_FiberHLCmd) {
        fDetectorConstruction->SetFiberHalfLength(m_FiberHLCmd->GetNewDoubleValue(newValue));
    }
    
    if (command==m_FiberSmoothCmd) {
        fDetectorConstruction->SetFiberSurfaceSmoothness(m_FiberSmoothCmd->GetNewDoubleValue(newValue));
    }
    
    if( command == m_StepMaxCmd ) {
        fDetectorConstruction
        ->SetMaxStep(m_StepMaxCmd->GetNewDoubleValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
