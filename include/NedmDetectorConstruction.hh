////////////////////////////////////////////////////////
// nEDMDetectorConstruction.hh                        //
// Coarse nEDM Geometry header file                   //
////////////////////////////////////////////////////////

#ifndef nEDMDetectorConstruction_h
#define nEDMDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"

#include "nEDMSimplePhysVolManager.hh"

class G4VPhysicalVolume;
class G4UserLimits;

class nEDMMaterials;

class nEDMDetectorMessenger;

class nEDMConstructor;

class nEDMDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  nEDMDetectorConstruction();
  virtual ~nEDMDetectorConstruction();

  // Construct the detector and define the world volume.
  virtual G4VPhysicalVolume* Construct();

  /// Return the detector construction messenger.
  virtual nEDMDetectorMessenger* GetMessenger(void){
    return fDetectorMessenger;
  }

  /// Update the geometry information to match stuff read from the macro file.
  void UpdateGeometry();

  void SetMaxStep (G4double );
  void SetCheckOverlaps(G4bool );
  
protected:

  /// This really constructs the detector, but doesn't define materials before it is constructed.
  /// This is called by Construct()
  G4VPhysicalVolume* ConstructDetector();
  
  /// A messenger for the DetectorConstruction object.
  nEDMDetectorMessenger* fDetectorMessenger;
  
  /// A constructor to create the world.
  nEDMConstructor* fWorldConstructor;
  
private:

  G4UserLimits* fStepLimit;

  G4bool  fCheckOverlaps;


};

#endif
