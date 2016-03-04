//////////////////////////////////////////
// nEDMLHeContructor.hh //
// Class to construct nEDM LHe Volume.
//////////////////////////////////////////

#ifndef nEDMLHeContructor_h
#define nEDMLHeContructor_h 1

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <G4ThreeVector.hh>

#include "nEDMComponentConstructor.hh"

/// Construct the Test Stand of the nEDM detector.
class nEDMLHeConstructor : public nEDMComponentConstructor
{
public:
    nEDMLHeConstructor(G4String name, nEDMDetectorConstruction* construct) : nEDMComponentConstructor(name,construct){Init();};
    nEDMLHeConstructor(G4String name, nEDMConstructor* construct) : nEDMComponentConstructor(name,construct){Init();};
    virtual ~nEDMLHeConstructor();
    
    virtual G4LogicalVolume* GetPiece(void);


private:
    void Init(void);

    void Construct3CellPlates();
    void ConstructSquareTubeReflector();
    
    G4double fLHERadius;
    G4double fLHELength;
    
    G4LogicalVolume* fLogicLHE;


};

#endif
