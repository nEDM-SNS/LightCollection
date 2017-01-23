#include "LightCollectionRunActionMessenger.hh"
#include "LightCollectionRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

LightCollectionRunActionMessenger::LightCollectionRunActionMessenger(LightCollectionRunAction* runAction)
 : G4UImessenger(),
   m_RunAction(runAction),
   m_Directory(0),
   m_SetFileNameCmd(0)
{
  m_Directory = new G4UIdirectory("/LightCollection/run/");
  m_Directory->SetGuidance("run control");
   
  m_SetFileNameCmd
    = new G4UIcmdWithAString("/LightCollection/run/setFileName",this);
  m_SetFileNameCmd->SetGuidance("Set the analysis file name (default: LightCollection");
  m_SetFileNameCmd->SetParameterName("fileName",false);
  m_SetFileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

LightCollectionRunActionMessenger::~LightCollectionRunActionMessenger()
{
  delete m_SetFileNameCmd;
  delete m_Directory;   
}

void LightCollectionRunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if ( command == m_SetFileNameCmd ) {
    m_RunAction->SetFileName(newValue);
  }

}
