#include "nEDMConstructor.hh"

nEDMConstructor::nEDMConstructor(G4String name, nEDMDetectorConstruction* construct)
  : fLocalName(name), fName(name), fConstruction(construct), fParent(NULL), fMessenger(NULL), fVisible(false)
{
  fMessenger = fConstruction->GetMessenger();
}

nEDMConstructor::nEDMConstructor(G4String name, nEDMConstructor* parent)
  : fLocalName(name), fName(name), fConstruction(NULL), fParent(parent), fMessenger(NULL), fVisible(false)
{
  fName = fParent->GetName() + "/" + fLocalName;
  fConstruction = fParent->GetConstruction();
  fMessenger = fParent->GetMessenger();
}

nEDMConstructor::~nEDMConstructor(){;}

G4LogicalVolume* nEDMConstructor::GetPiece(void)
{
  G4Exception("nEDMConstructor::GetPiece()","Not implemented",RunMustBeAborted,"");
  return NULL;
}

G4Material* nEDMConstructor::FindMaterial(G4String name)
{
  //G4Material* material = G4Material::GetMaterial(name,true);
  nEDMMaterials* materials = nEDMMaterials::GetInstance();
  G4Material* material = materials->GetMaterial(name);
  return material;
}

G4String nEDMConstructor::GetName(void)
{
  return fName;
}

G4String nEDMConstructor::GetLocalName(void)
{
  return fLocalName;
}
  
/// Set the local name of the object to be built.
/// The local name of the logical volume being set, and also sets the full name of the volume.
void nEDMConstructor::SetLocalName(const G4String& name)
{
  fLocalName = name;
  fName = name;
  if(fParent) fName = fParent->GetName() + "/" + fName;
}

/// Set the visibility of the constructed object.
void nEDMConstructor::SetVisible(bool vis)
{
  if(fVisible != vis)
    G4cout << "Set visibility for " << GetName() << " from " << fVisible << " to " << vis << G4endl;
  fVisible = vis;
}

// Set the visibility of the constructed object daughters.
void nEDMConstructor::SetVisibleDaughters(bool vis)
{
  for(std::map<G4String,nEDMConstructor*>::iterator sub = fSubConstructors.begin(); sub != fSubConstructors.end(); ++sub)
    sub->second->SetVisible(vis);
}

nEDMConstructorMessenger::nEDMConstructorMessenger(nEDMConstructor *construct, const char* guide)
{
  fConstructor = construct;
  fDirName = construct->GetName() + "/";
  fDirectory = new G4UIdirectory(fDirName);
  if(guide)
    fDirectory->SetGuidance(guide);
  else 
    fDirectory->SetGuidance("Commands for a nEDM geometry.");

}

void nEDMConstructorMessenger::SetNewValue(G4UIcommand* , G4String )
{
}

nEDMConstructorMessenger::~nEDMConstructorMessenger()
{
  delete fDirectory;
}
