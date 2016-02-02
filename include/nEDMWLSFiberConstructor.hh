//////////////////////////////////////////////
// nEDMWLSFiberConstructor.hh //
// Class to construct nEDM cell wall plate. //
//////////////////////////////////////////////

#ifndef nEDMWLSFiberConstructor_h
#define nEDMWLSFiberConstructor_h 1

#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include "G4VisAttributes.hh"

#include "nEDMComponentConstructor.hh"
#include "nEDMSimplePhysVolManager.hh"

class nEDMWLSFiberConstructor : public nEDMComponentConstructor
{
public:
    nEDMWLSFiberConstructor(G4String name, nEDMDetectorConstruction* construct) : nEDMComponentConstructor(name,construct){Init();};
    nEDMWLSFiberConstructor(G4String name, nEDMConstructor* construct) : nEDMComponentConstructor(name,construct){Init();};
    virtual ~nEDMWLSFiberConstructor();
    
    /// Construct and return a G4 volume for the object.
    virtual G4LogicalVolume* GetPiece(void);
    
    G4double GetFiberThickness() {return fClad2_rmax*2;};
        
    
private:
    void Init(void);
    
    
    G4double fFiber_rmin;
    G4double fFiber_rmax;
    G4double fFiber_z;
    G4double fFiber_sphi;
    G4double fFiber_ephi;
    G4double fSurfaceRoughness;
    
    G4double fClad1_rmin;
    G4double fClad1_rmax;
    G4double fClad1_z;
    G4double fClad1_sphi;
    G4double fClad1_ephi;
    
    G4double fClad2_rmin;
    G4double fClad2_rmax;
    G4double fClad2_z;
    G4double fClad2_sphi;
    G4double fClad2_ephi;
    
    G4double fMirrorRmax;
    G4double fMirrorRmin;
    G4double fMirrorThick;
    G4double fMirrorSPhi;
    G4double fMirrorEPhi;
    
    G4double fMirrorPosZ;
    G4double fMirrorReflectivity;
    
    
    
};
#endif
