#ifndef NedmCellSide_h
#define NedmCellSide_h 1

#include "globals.hh"
#include "G4PVPlacement.hh"

#include "NedmDetectorComponent.hh"

class G4LogicalVolume;
class G4VSolid;
class NedmDetectorConstruction;

class NedmCellSide : public NedmDetectorComponent, public G4PVPlacement {

  public:
    NedmCellSide(G4RotationMatrix* pRot,
        const G4ThreeVector& tlate,
        G4LogicalVolume* pMotherLogical,
        G4int copyNo,
        G4bool hole=false);

    static void CreateLogicalVolumes();

  private:
    static G4LogicalVolume* ConstructLogicalVolume(G4bool side1=false);
    static G4LogicalVolume* ConstructMirrorLogicalVolume();
    static G4VSolid* MakeHole(G4VSolid*, G4String, G4String, G4VSolid**);

    static G4LogicalVolume* the_first_side_;
    static G4LogicalVolume* the_side_;
    static G4LogicalVolume* the_plug_;
    static G4LogicalVolume* the_mirror_;

    G4VPhysicalVolume* ConstructTPBInterface(G4bool side1);
    void ConstructSurfaceVolumes(G4bool side1);
    G4VPhysicalVolume* ConstructMirror();
    G4VPhysicalVolume* ConstructRearSurface();

};

#endif
