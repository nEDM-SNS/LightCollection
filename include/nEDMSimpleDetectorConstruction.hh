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
    void ConstructSquarePMMA();
    void ConstructCirclePMMA();
    void ConstructFibers();
    void ConstructCylindricalReflector();
    void ConstructEndFiberReflector();
    void ConstructPhotonDet();
    void ConstructSiliconWafers();
    void ConstructClearFibers();
    
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool );
    
protected:
    
private:
    
    nEDMMaterials* fMaterials;
    G4VPhysicalVolume* fPhysHall;
    G4LogicalVolume* fLogicHall;
    
    G4UserLimits* fStepLimit;
    
    G4bool  fCheckOverlaps;
    
    G4bool fEmbeddedFibers;
    G4bool fFiberReflector;
    G4bool fOuterReflector;
    G4bool fTPB_On;
    
    G4int fNumberOfFibers;
    
    G4double fCellHalfZ;
    G4double fCellHalfThickness;
    G4double fCircleInner_rad;
    G4double fCircleOuter_rad;
    
    G4double fFiberSpacing;
    G4double fFiberThickness;
    G4double fFiberLength;
    
    G4double fTPB_Thickness;
    G4double fFiberOuterSurfaceRoughness;
    
    
};

#endif
