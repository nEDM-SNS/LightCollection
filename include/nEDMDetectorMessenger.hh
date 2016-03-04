
#ifndef nEDMDetectorMessenger_h
#define nEDMDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "nEDMSimplePhysVolManager.hh"

class nEDMDetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for nEDMDetectorConstruction.
///
/// It implements commands:
/// - /nEDM/det/stepMax value unit

class nEDMDetectorMessenger: public G4UImessenger
{
public:
    nEDMDetectorMessenger(nEDMDetectorConstruction* );
    virtual ~nEDMDetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
    nEDMDetectorConstruction*  fDetectorConstruction;
    
    G4UIdirectory*           fDetDirectory;
    
    G4UIcmdWithABool* fCheckOverlapCmd;
    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
    G4UIcmdWithoutParameter* fPrintAllVolumesCmd;
    G4UIcmdWithoutParameter* fPrintDetectorVolumesCmd;
    G4UIcmdWithoutParameter* fPrintLogicVolumeNamesCmd;
};

#endif
