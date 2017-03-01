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
class G4OpticalSurface;

class nEDMMaterials;
class LightCollectionDetectorMessenger;

class LightCollectionDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    
    LightCollectionDetectorConstruction();
    virtual ~LightCollectionDetectorConstruction();
    
    // Construct the detector and define the world volume.
    virtual G4VPhysicalVolume* Construct();
    
    void ConstructSingleFiber();
    
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool checkOverlaps);
    void SetFiberHalfLength(G4double halfLength);
    void SetFiberSurfaceRoughness(G4double roughness);
    
protected:
    
private:
    
    LightCollectionDetectorMessenger* theMessenger;
    
    nEDMMaterials* m_Materials;
    G4VPhysicalVolume* m_PhysHall;
    G4LogicalVolume* m_LogicHall;
    
    G4VPhysicalVolume* m_PhysFiber;
    G4VPhysicalVolume* m_PhysClad1;
    G4VPhysicalVolume* m_PhysCore;
    G4VPhysicalVolume* m_PhysDet1;
    G4VPhysicalVolume* m_PhysDet2;
    
    G4UserLimits* m_StepLimit;
    
    G4bool  m_CheckOverlaps;
    
    G4bool m_FiberReflector;
    G4double m_FiberOuterSurfaceRoughness;
    G4double m_FiberHalfLength;
    G4double m_FibDetThickness;
    
    G4OpticalSurface* m_FiberOpSurface;
    
};

#endif
