//////////////////////////////////////////
// nEDMSimplePhysVolManager.hh //
// Manager of added physical volumes //
// This class is designed as a singleton//
//////////////////////////////////////////

#ifndef nEDMSimplePhysVolManager_h
#define nEDMSimplePhysVolManager_h 1

#include <iostream>
#include <map>
#include <set>
#include <iomanip>

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"

class nEDMSimplePhysVolManager
{
public:
  static nEDMSimplePhysVolManager* GetInstance(){
    if(nEDMSimplePhysVolManager::fInstance == NULL) nEDMSimplePhysVolManager::fInstance = new nEDMSimplePhysVolManager();
    return nEDMSimplePhysVolManager::fInstance;
  }
  
  virtual ~nEDMSimplePhysVolManager();

  /// Get physical volume
  G4VPhysicalVolume* GetPhysicalVolume(G4String name, G4int index = 0);
  /// Add internal physical volume
  void AddPhysicalVolume(G4String name, G4VPhysicalVolume* volume, G4int index = 0);
  /// Print current volumes in manager
  void PrintAllVolumes();
  /// Print names of the logic volumes associated to the physical volumes in manager
  void PrintLogicVolumeNames();
  /// Print info about volumes in current geometry
  void PrintVolumes(G4int level);
  /// Print translation of given volume in current geometry
  void PrintLogicVolumeTranslation(G4String logVolName);

  /// Get translation of given volume in current geometry
  G4ThreeVector GetVolumeTranslation(G4String logVolName);
  
private:
  nEDMSimplePhysVolManager();
  static nEDMSimplePhysVolManager* fInstance;

  /// Physical volumes contained in a piece of this module
  std::map<G4String, G4VPhysicalVolume*> physVolumes;
};

#endif
