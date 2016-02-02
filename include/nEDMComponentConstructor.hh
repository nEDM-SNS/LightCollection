//////////////////////////////////////////
// nEDMComponentConstructor.hh //
// Base class to construct nEDM components for nEDMModuleConstructor  //
//////////////////////////////////////////

#ifndef nEDMComponentConstructor_h
#define nEDMComponentConstructor_h 1

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4VSensitiveDetector.hh>
#include "G4UIcmdWithAString.hh"

#include "nEDMConstructor.hh"

/// Construct a component to be used by a nEDMModuleConstructor to build component
class nEDMComponentConstructor : public nEDMConstructor
{
public:
  nEDMComponentConstructor(G4String name, nEDMDetectorConstruction* construct) : nEDMConstructor(name,construct), fSensitiveDetector(NULL) {;};
  nEDMComponentConstructor(G4String name, nEDMConstructor* construct) : nEDMConstructor(name,construct), fSensitiveDetector(NULL) {;};
  virtual ~nEDMComponentConstructor(){;};

  /// Set the sensitive detector for this component
  virtual void SetSensitiveDetector(G4VSensitiveDetector* sd){fSensitiveDetector = sd;}
  /// Get the sensitive detector for this component
  virtual G4VSensitiveDetector* GetSensitiveDetector(void){return fSensitiveDetector;}

  /// Set the component material
  void SetMaterial(G4String material) {fComponentMaterialName = material;}
  /// Get the component material
  G4String GetMaterial(void) {return fComponentMaterialName;}

protected:

  /// The sensitive detector for this component
  G4VSensitiveDetector* fSensitiveDetector;

private:

  /// Component material name
  G4String fComponentMaterialName;

};

class nEDMComponentConstructorMessenger : public nEDMConstructorMessenger
{
private:
  nEDMComponentConstructor* fConstructor;

  G4UIcmdWithAString* fComponentMaterialNameCmd;

public:
  nEDMComponentConstructorMessenger(nEDMComponentConstructor* construct, const char* guide)
    : nEDMConstructorMessenger(construct,guide),
      fConstructor(construct)
    {
      fComponentMaterialNameCmd = new G4UIcmdWithAString(CommandName("setMaterial"),this);
      fComponentMaterialNameCmd->SetGuidance("Set the name of the material used to build the cryostat.");
      fComponentMaterialNameCmd->SetParameterName("setMaterial",false);
      fComponentMaterialNameCmd->AvailableForStates(G4State_PreInit);
    };

  virtual ~nEDMComponentConstructorMessenger(){;};

  void SetNewValue(G4UIcommand *cmd, G4String val){
    if( cmd == fComponentMaterialNameCmd)
    {
      fConstructor->SetMaterial(val);
    }
    else
    {
      nEDMConstructorMessenger::SetNewValue(cmd,val);
    }

  }

};
  
#endif
