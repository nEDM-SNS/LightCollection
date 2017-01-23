#ifndef LightCollectionRunActionMessenger_h
#define LightCollectionRunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class LightCollectionRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class LightCollectionRunActionMessenger: public G4UImessenger
{
  public:
    LightCollectionRunActionMessenger(LightCollectionRunAction* runAction);
    virtual ~LightCollectionRunActionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    LightCollectionRunAction* 	 m_RunAction;
    G4UIdirectory*           m_Directory;
    G4UIcmdWithAString*      m_SetFileNameCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
