
#include "nEDMDetectorMessenger.hh"
#include "nEDMDetectorConstruction.hh"

nEDMDetectorMessenger::nEDMDetectorMessenger(nEDMDetectorConstruction* Det)
: G4UImessenger(),
fDetectorConstruction(Det)
{
    fDetDirectory = new G4UIdirectory("/nEDM/det/");
    fDetDirectory->SetGuidance("Detector construction control");
    
    fCheckOverlapCmd = new G4UIcmdWithABool("/nEDM/det/checkOverlap", this);
    fCheckOverlapCmd->SetGuidance("Toggle CheckOverlap flag");
    fCheckOverlapCmd->AvailableForStates(G4State_PreInit);
    
    fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/nEDM/det/stepMax",this);
    fStepMaxCmd->SetGuidance("Define a step max");
    fStepMaxCmd->SetParameterName("stepMax",false);
    fStepMaxCmd->SetUnitCategory("Length");
    fStepMaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fPrintAllVolumesCmd = new G4UIcmdWithoutParameter("/nEDM/det/printAllVolumes",this);
    fPrintAllVolumesCmd->SetGuidance("Print current volumes in manager");
    
    fPrintLogicVolumeNamesCmd = new G4UIcmdWithoutParameter("/nEDM/det/printLogicVolumeNames",this);
    fPrintLogicVolumeNamesCmd->SetGuidance("Print names of the logic volumes associated to the physical volumes in manager");

    fPrintDetectorVolumesCmd = new G4UIcmdWithoutParameter("/nEDM/det/printDetectorVolumes",this);
    fPrintDetectorVolumesCmd->SetGuidance("Print current detector volumes in manager");
    
}

nEDMDetectorMessenger::~nEDMDetectorMessenger()
{
    delete fDetDirectory;
    
    delete fCheckOverlapCmd;
    delete fStepMaxCmd;
    delete fPrintAllVolumesCmd;
    delete fPrintLogicVolumeNamesCmd;
    delete fPrintDetectorVolumesCmd;
    
}

void nEDMDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if( command == fCheckOverlapCmd) {
        fDetectorConstruction
        ->SetCheckOverlaps(fCheckOverlapCmd->GetNewBoolValue(newValue));
    }
    else if( command == fStepMaxCmd ) {
        fDetectorConstruction
        ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
    }
    else if (command == fPrintAllVolumesCmd) {
        nEDMSimplePhysVolManager::GetInstance()->PrintAllVolumes();
    }
    else if (command == fPrintLogicVolumeNamesCmd){
        nEDMSimplePhysVolManager::GetInstance()->PrintLogicVolumeNames();
    }
    else if (command == fPrintDetectorVolumesCmd){
        nEDMSimplePhysVolManager::GetInstance()->PrintDetectorVolumeNames();
    }
}
