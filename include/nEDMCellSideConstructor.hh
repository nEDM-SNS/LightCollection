//////////////////////////////////////////////
// nEDMCellSideConstructor.hh //
// Class to construct nEDM cell wall plate. //
//////////////////////////////////////////////

#ifndef nEDMCellSideConstructor_h
#define nEDMCellSideConstructor_h 1

#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>
#include "G4Box.hh"
#include "G4VisAttributes.hh"

#include "nEDMComponentConstructor.hh"
#include "nEDMSimplePhysVolManager.hh"

class nEDMCellSideConstructor : public nEDMComponentConstructor
{
public:
    nEDMCellSideConstructor(G4String name, nEDMDetectorConstruction* construct) : nEDMComponentConstructor(name,construct){Init();};
    nEDMCellSideConstructor(G4String name, nEDMConstructor* construct) : nEDMComponentConstructor(name,construct){Init();};
    virtual ~nEDMCellSideConstructor();
    
    /// Construct and return a G4 volume for the object.
    virtual G4LogicalVolume* GetPiece(void);
    
    // Get the dimensions of the cell side
    G4double GetWidth() {return fWidth;};
    G4double GetThickness() {return fThickness;};
    G4double GetLength() {return fLength;};
    
    G4int GetNumFib() {return fNum_fibers;};
    G4double GetFibSpacing() {return fFiber_spacing;};
    G4bool GetEmbedded_fibers() {return fEmbedded_fibers;};
    
    /// Set the dimensions of the cell side.
    void SetWidth(G4double width) {fWidth = width;};
    void SetThickness(G4double thickness) {fThickness = thickness;};
    void SetLength(G4double length) {fLength = length;};
    
    void SetTPB_Thickness(G4double thickness) {fTPB_Thickness = thickness;};
    void SetTPB_OuterFraction(G4double fraction) {fTPB_outerFraction = fraction;}
    
    void SetTPB_On(G4bool tpb) {fTPB_On = tpb;};
    void SetEmbedded_Fibers(G4bool embedded) {fEmbedded_fibers = embedded;};
    void SetFiber_Reflector(G4bool reflector) {fFiber_Reflector = reflector;};
    void SetFiber_Spacing(G4double spacing) {fFiber_spacing = spacing;};
    void SetNum_Fibers(G4int num) {fNum_fibers = num;};
    
    
private:
    void Init(void);
    
    
    void ConstructTPBInterface();
    void ConstructEmbeddedFibers();

    /// The dimensions of the cell side
    G4double fWidth;
    G4double fThickness;
    G4double fLength;
    
    G4double fTPB_Thickness;
    G4double fTPB_outerFraction;
    
    G4bool fTPB_On;
    G4bool fEmbedded_fibers;
    G4bool fFiber_Reflector;
    G4double fFiber_spacing;
    G4int fNum_fibers;
    
    G4LogicalVolume* fCellSide_log;

    
};
#endif
