//////////////////////////////////////////
// nEDMWorldConstructor.hh //
// Constructs the nEDM world geometry. //
//////////////////////////////////////////

#ifndef nEDMWorldConstructor_h
#define nEDMWorldConstructor_h 1

#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>
#include "G4Box.hh"
#include "G4VisAttributes.hh"

#include "nEDMConstructor.hh"
#include "nEDMSimplePhysVolManager.hh"


class nEDMWorldConstructor : public nEDMConstructor
{
public:
  nEDMWorldConstructor(G4String name, nEDMDetectorConstruction* construct) : nEDMConstructor(name,construct){Init();};
  virtual ~nEDMWorldConstructor();

  /// Construct and return a G4 volume for the object.
  virtual G4LogicalVolume* GetPiece(void);

  /// Set the width of the world.
  void SetWidth(double width) {fWidth = width;};

  /// Set the height of the world.
  void SetHeight(double height) {fHeight = height;};

  /// Set the length of the world.
  void SetLength(double length) {fLength = length;};

  /// Get the width of the world.
  double GetWidth(void) {return fWidth;};

  /// Get the height of the world.
  double GetHeight(void) {return fHeight;};

  /// Get the length of the world.
  double GetLength(void) {return fLength;};

private:
  void Init(void);
  
  /// The width of the world to be simulated
  double fWidth;
  
  /// The heigth of the world to be simulated
  double fHeight;
  
  /// The length of the world to be simulated
  double fLength;
  
};
#endif
