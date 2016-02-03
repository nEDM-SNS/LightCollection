#include "LightCollectionRunActionMessenger.hh"
#include "LightCollectionRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

LightCollectionRunActionMessenger::LightCollectionRunActionMessenger(LightCollectionRunAction* runAction)
 : G4UImessenger(),
   fRunAction(runAction),
   fDirectory(0),
   fSetFileNameCmd(0)
{
  fDirectory = new G4UIdirectory("/LightCollection/run/");
  fDirectory->SetGuidance("run control");
   
  fSetFileNameCmd 
    = new G4UIcmdWithAString("/LightCollection/run/setFileName",this);
  fSetFileNameCmd->SetGuidance("Set the analysis file name (default: LightCollection");
  fSetFileNameCmd->SetParameterName("fileName",false);
  fSetFileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

LightCollectionRunActionMessenger::~LightCollectionRunActionMessenger()
{
  delete fSetFileNameCmd;
  delete fDirectory;   
}

void LightCollectionRunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if ( command == fSetFileNameCmd ) {
    fRunAction->SetFileName(newValue);
  }

}
