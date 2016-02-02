#include "nEDMSimplePhysVolManager.hh"
#include "G4SystemOfUnits.hh"

nEDMSimplePhysVolManager* nEDMSimplePhysVolManager::fInstance = 0;

nEDMSimplePhysVolManager::nEDMSimplePhysVolManager()
{
  physVolumes.clear();
}

nEDMSimplePhysVolManager::~nEDMSimplePhysVolManager(){;}

void nEDMSimplePhysVolManager::AddPhysicalVolume(G4String name, G4VPhysicalVolume* volume, G4int index)
{
  char fullNameChar[1024];
  sprintf(fullNameChar,"%s_%d",name.data(),index);
  G4String fullName = fullNameChar;
  if(physVolumes.count(fullName) == 0)
    physVolumes.insert(std::pair<G4String, G4VPhysicalVolume*>(fullName,volume));
}

G4VPhysicalVolume* nEDMSimplePhysVolManager::GetPhysicalVolume(G4String name, G4int index)
{
  char fullNameChar[1024];
  sprintf(fullNameChar,"%s_%d",name.data(),index);
  G4String fullName = fullNameChar;
  if(physVolumes.count(fullName) > 0)
    return physVolumes.at(fullName);

  return NULL;
}

void nEDMSimplePhysVolManager::PrintAllVolumes()
{
  G4cout << "############# ALL VOLUMES IN SIMPLE MANAGER #############" << G4endl;
  for(std::map<G4String, G4VPhysicalVolume*>::iterator physVol = physVolumes.begin(); physVol != physVolumes.end(); physVol++)
    G4cout << "#### Name: " << physVol->first << " at address " << physVol->second << G4endl;    
}

void nEDMSimplePhysVolManager::PrintLogicVolumeNames()
{
  std::set<G4String> logicVols;
  
  G4cout << "############# LOGIC VOLUME NAMES IN SIMPLE MANAGER #############" << G4endl;
  
  for(std::map<G4String, G4VPhysicalVolume*>::iterator physVol = physVolumes.begin(); physVol != physVolumes.end(); physVol++)
    logicVols.insert(physVol->second->GetLogicalVolume()->GetName());

  for(std::set<G4String>::iterator volName = logicVols.begin(); volName != logicVols.end(); volName++)
    G4cout << *volName << G4endl;    
}

void nEDMSimplePhysVolManager::PrintVolumes(G4int )
{
  std::map<G4String, G4int> logicVolQty;

  for(std::map<G4String, G4VPhysicalVolume*>::iterator physVol = physVolumes.begin(); physVol != physVolumes.end(); physVol++)
  {
    G4String logicName = physVol->second->GetLogicalVolume()->GetName();
    if(logicVolQty.count(logicName) == 0)
      logicVolQty.insert(std::pair<G4String, G4int>(logicName,1));
    else
      logicVolQty.at(logicName) += 1;
  }

  //G4cout << " VOLUME NAME| QUANTITY | MATERIAL | INDIVIDUAL MASS (kg)" << G4endl;
  G4cout << std::left << std::setw(50) << "VOLUME NAME"
         << std::left << std::setw(10) << "QUANTITY"
         << std::left << std::setw(20) << "MATERIAL"
         << std::right << std::setw(16) << "DENSITY (g/cm3)"
         << std::right << std::setw(20) << "IND. MASS (kg)"
         << std::right << std::setw(20) << "IND. VOL (cm3)"
         << G4endl;
  
  for(std::map<G4String, G4int>::iterator volMap = logicVolQty.begin(); volMap != logicVolQty.end(); volMap++)
  {
    G4cout << std::left << std::setw(50) << volMap->first
           << std::left << std::setw(10) << volMap->second;
    
    G4VPhysicalVolume* physVol = GetPhysicalVolume(volMap->first,0);
    if(physVol)
    {
      G4LogicalVolume* logicVol = physVol->GetLogicalVolume();
      if(logicVol)
      {
        G4Material* matVol = logicVol->GetMaterial();
        if(matVol)
        {
          G4cout << std::left << std::setw(20) << matVol->GetName();
          G4cout << std::right << std::setw(16) << std::fixed << std::setprecision(6) << matVol->GetDensity()*cm3/g;
        }

        G4double daugMass = 0.;
        G4double daugCubVol = 0.;
        G4int nDaugthers = logicVol->GetNoDaughters();
        for(int i = 0; i < nDaugthers; i++)
        {
          G4VPhysicalVolume* daugVol = logicVol->GetDaughter(i);
          if(daugVol)
          {
            //G4cout << "Daughter " << i << " name: " << daugVol->GetName() << G4endl;
            G4LogicalVolume* logicDaug = daugVol->GetLogicalVolume();
            if(logicDaug)
            {
              //if(volMap->first.contains("/nEDM/TPCExternals/InnerSphericalCryostat"))
              //  std::cout << volMap->first << " daughter name " << logicDaug->GetName() << " mass " << logicDaug->GetMass()/kg << " vol " << logicDaug->GetSolid()->GetCubicVolume()/cm3 << std::endl;
              daugMass += logicDaug->GetMass();
              daugCubVol += logicDaug->GetSolid()->GetCubicVolume();
            }
          }
        }

        G4cout << std::right << std::setw(20) << std::fixed << std::setprecision(6) << (logicVol->GetMass() - daugMass)/kg;
        G4cout << std::right << std::setw(20) << std::fixed << std::setprecision(6) << (logicVol->GetSolid()->GetCubicVolume() - daugCubVol)/cm3;
      }
    }

    

    G4cout << "\n";
  }

  
  //for(std::map<G4String, G4VPhysicalVolume*>::iterator physVol = physVolumes.begin(); physVol != physVolumes.end(); physVol++)
  //  G4cout << "#### Name: " << physVol->first << " with mass " << physVol->second->GetLogicalVolume()->GetMass() << G4endl;       
}

void nEDMSimplePhysVolManager::PrintLogicVolumeTranslation(G4String logVolName)
{
  G4ThreeVector totalOffset = GetVolumeTranslation(logVolName);
    
  std::cout << "Total translation of " << logVolName << ": (x,y,z) = (" << totalOffset.x() << "," << totalOffset.y() << "," << totalOffset.z() << ") mm" << std::endl;
}

G4ThreeVector nEDMSimplePhysVolManager::GetVolumeTranslation(G4String logVolName)
{
  G4ThreeVector totalOffset;
  
  std::map<G4String, G4VPhysicalVolume*>::iterator physVol = physVolumes.begin();
  while(physVol != physVolumes.end())
  {
    G4String logicName = physVol->second->GetLogicalVolume()->GetName();
    if(logicName == logVolName)
      break;
    physVol++;
  }
  
  if(physVol == physVolumes.end())
  {
    std::cerr << "Could not find volume named: " << logVolName << std::endl;
    return totalOffset;
  }

  totalOffset = physVol->second->GetTranslation();
  G4LogicalVolume* mother = physVol->second->GetMotherLogical();

  while(mother->GetName() != "/nEDM")
  {
    //std::cout << "Mother name: " << mother->GetName() << std::endl;
    physVol = physVolumes.begin();
    while(physVol != physVolumes.end())
    {
      G4String logicName = physVol->second->GetLogicalVolume()->GetName();
      if(logicName == mother->GetName())
        break;
      physVol++;
    }
    if(physVol == physVolumes.end())
    {
      std::cerr << "Could not find logical volume named: " << mother->GetName() << std::endl;
      return totalOffset;
    }
    totalOffset += physVol->second->GetTranslation();
    mother = physVol->second->GetMotherLogical();
  }

  return totalOffset;
}
