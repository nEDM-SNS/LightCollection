//////////////////////////////////////////
// nEDMSimplePhotDetContructor.hh //
// Class to construct nEDM water tank.
//////////////////////////////////////////

#ifndef nEDMSimplePhotDetContructor_h
#define nEDMSimplePhotDetContructor_h 1

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

#include "nEDMDetectorConstruction.hh"
#include "nEDMComponentConstructor.hh"



/// Construct the Test Stand of the nEDM detector.
class nEDMSimplePhotDetConstructor : public nEDMComponentConstructor
{
public:
    nEDMSimplePhotDetConstructor(G4String name, nEDMDetectorConstruction* construct) : nEDMComponentConstructor(name,construct){Init();};
    nEDMSimplePhotDetConstructor(G4String name, nEDMConstructor* construct) : nEDMComponentConstructor(name,construct){Init();};
    virtual ~nEDMSimplePhotDetConstructor();
    
    virtual G4LogicalVolume* GetPiece(void);
    
    void SetXWidth(G4double width) {fXWidth = width;};
    void SetYWidth(G4double width) {fYWidth = width;};
    void SetThickness(G4double thick) {fThickness = thick;};
    
    G4double GetXWidth() {return fXWidth;};
    G4double GetYWidth() {return fYWidth;};
    G4double GetThickness() {return fThickness;};


private:
    void Init(void);
    
    G4double fXWidth;
    G4double fYWidth;
    G4double fThickness;

    


};

#endif
