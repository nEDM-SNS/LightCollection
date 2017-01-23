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
    void ConstructSinglePlate();
    void ConstructSquareTubeReflector();
    void ConstructFullTentReflector();
    
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool );
    
protected:
    
    /// This really constructs the detector, but doesn't define materials before it is constructed.
    /// This is called by Construct()
    G4VPhysicalVolume* ConstructDetector();
    
    
private:
    
    nEDMMaterials* m_Materials;
    G4VPhysicalVolume* m_PhysHall;
    G4LogicalVolume* m_LogicHall;
    
    G4UserLimits* m_StepLimit;
    
    G4bool  m_CheckOverlaps;
    
    G4bool m_EmbeddedFibers;
    G4bool m_FiberReflector;
    G4bool m_SqureTubeReflector;
    G4bool m_FullTentReflector;
    G4int m_NumberOfFibers;
    G4double m_FiberSpacing;
    G4double m_TPB_outerThickness;
    G4double m_TPB_Thickness;
    G4double m_FiberOuterSurfaceRoughness;
    G4double m_CellWidth;
    G4double m_CellThickness;
    G4double m_CellLength;
};

#endif
