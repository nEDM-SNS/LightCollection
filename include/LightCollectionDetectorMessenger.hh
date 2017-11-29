#ifndef LightCollectionDetectorMessenger_h
#define LightCollectionDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class LightCollectionDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for LightCollectionDetectorConstruction.
///
/// It implements commands:
/// - /B2/det/setTargetMaterial name
/// - /B2/det/setChamberMaterial name
/// - /B2/det/stepMax value unit

class LightCollectionDetectorMessenger: public G4UImessenger
{
  public:
    LightCollectionDetectorMessenger(LightCollectionDetectorConstruction* );
    virtual ~LightCollectionDetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    LightCollectionDetectorConstruction*  fDetectorConstruction;

    G4UIdirectory*              m_DetDirectory;

//    G4UIcmdWithAString*      m_TargMatCmd;
//    G4UIcmdWithAString*      m_ChamMatCmd;

    G4UIcmdWithADoubleAndUnit*  m_StepMaxCmd;
    G4UIcmdWithADoubleAndUnit*  m_FiberHLCmd;
    G4UIcmdWithADouble*         m_FiberSmoothCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
