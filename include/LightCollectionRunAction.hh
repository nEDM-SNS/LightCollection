#ifndef LightCollectionRunAction_h
#define LightCollectionRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Timer;
class G4Run;
class LightCollectionRunActionMessenger;

class LightCollectionRunAction : public G4UserRunAction
{
public:
    LightCollectionRunAction();
    virtual ~LightCollectionRunAction();
    
public:
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);
    
    void SetFileName(G4String name);
    
    
private:
    G4Timer* timer;
    G4String fileName;
    
    LightCollectionRunActionMessenger*  fMessenger;   // messenger
    
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void LightCollectionRunAction::SetFileName(G4String name)
{
    fileName = name;
}

#endif /*LightCollectionRunAction_h*/
