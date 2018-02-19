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
    void ConstructSquarePMMA();
    void ConstructCirclePMMA();
    void ConstructFibers();
    void ConstructFibersNew();
    void ConstructCylindricalReflector();
    void ConstructEndFiberReflector();
    void ConstructPhotonDet();
    void ConstructSiliconWafers();
    void ConstructClearFibers();
    
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool );
    
protected:
    
private:
    
    nEDMMaterials* m_Materials;
    G4VPhysicalVolume* m_PhysHall;
    G4LogicalVolume* m_LogicHall;
    
    G4UserLimits* m_StepLimit;
    
    G4bool  m_CheckOverlaps;
    
    G4bool m_EmbeddedFibers;
    G4bool m_FiberReflector;
    G4bool m_OuterReflector;
    G4bool m_TPB_On;
    
    G4int m_NumberOfFibers;
    
    G4double m_CellHalfZ;
    G4double m_CellHalfThickness;
    G4double m_CircleInner_rad;
    G4double m_CircleOuter_rad;
    
    G4double m_FiberThickness;
    G4double m_FiberHalfLength;
    
    G4double m_TPB_Thickness;
    G4double m_FiberOuterSurfaceRoughness;
    
    G4double m_PlugHalfLength;
    G4double m_AcrylicSmoothness;
    G4double m_RelfectorOverhang;
    
    
};

#endif
