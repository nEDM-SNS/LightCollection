////////////////////////////////////////////////////////
// LightCollectionDetectorConstruction.hh                        //
// Coarse nEDM Geometry header file                   //
////////////////////////////////////////////////////////

#ifndef LightCollectionDetectorConstruction_h
#define LightCollectionDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4UserLimits;

class nEDMMaterials;

class LightCollectionDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    LightCollectionDetectorConstruction();
    virtual ~LightCollectionDetectorConstruction();
    
    // Construct the detector and define the world volume.
    virtual G4VPhysicalVolume* Construct();
    void ConstructTestStand();
    
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool );
    
protected:
    
    /// This really constructs the detector, but doesn't define materials before it is constructed.
    /// This is called by Construct()
    G4VPhysicalVolume* ConstructDetector();
    
    
private:
    
    nEDMMaterials* fMaterials;
    G4VPhysicalVolume* fPhysHall;
    G4LogicalVolume* fLogicHall;
    
    G4UserLimits* fStepLimit;
    
    G4bool  fCheckOverlaps;
    
    
};

#endif
