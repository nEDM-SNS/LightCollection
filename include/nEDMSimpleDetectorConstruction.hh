////////////////////////////////////////////////////////
// nEDMSimpleDetectorConstruction.hh                        //
// Coarse nEDM Geometry header file                   //
////////////////////////////////////////////////////////

#ifndef nEDMSimpleDetectorConstruction_h
#define nEDMSimpleDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4UserLimits;

class nEDMMaterials;

class nEDMSimpleDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    nEDMSimpleDetectorConstruction();
    virtual ~nEDMSimpleDetectorConstruction();
    
    // Construct the detector and define the world volume.
    virtual G4VPhysicalVolume* Construct();
    void ConstructSinglePlate();
    void ConstructSquareTubeReflector();
    
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
    
    G4bool fEmbeddedFibers;
    G4bool fFiberReflector;
    G4bool fSqureTubeReflector;
    G4int fNumberOfFibers;
    G4double fFiberSpacing;
    G4double fTPB_outerThickness;
    G4double fTPB_Thickness;
    G4double fFiberOuterSurfaceRoughness;
    
    
};

#endif
