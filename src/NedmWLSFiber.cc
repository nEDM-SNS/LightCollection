#include "NedmWLSFiber.hh"
#include "NedmDetectorParameters.hh"

#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

G4LogicalVolume* NedmWLSFiber::fClad2_log=NULL;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NedmWLSFiber::NedmWLSFiber(G4RotationMatrix *pRot,
                           const G4ThreeVector &tlate,
                           G4LogicalVolume *pMotherLogical,
                           G4bool pMany,
                           G4int pCopyNo)
:G4PVPlacement(pRot,tlate,
               new G4LogicalVolume(new G4Box("temp",1,1,1),
                                   G4Material::GetMaterial("G4_AIR"),
                                   "temp",0,0,0),
               "Cladding2",pMotherLogical,pMany,pCopyNo)
{
    CopyValues();
    
    // The Fiber
    //
    G4Tubs* fiber_tube =
    new G4Tubs("Fiber",fFiber_rmin,fFiber_rmax,fFiber_z,fFiber_sphi,fFiber_ephi);
    
    G4LogicalVolume* fiber_log =
    new G4LogicalVolume(fiber_tube,G4Material::GetMaterial("PMMA"),
                        "Fiber",0,0,0);
    
    // Cladding (first layer)
    //
    G4Tubs* clad1_tube =
    new G4Tubs("Cladding1",fClad1_rmin,fClad1_rmax,fClad1_z,fClad1_sphi,
               fClad1_ephi);
    
    G4LogicalVolume* clad1_log =
    new G4LogicalVolume(clad1_tube,G4Material::GetMaterial("Pethylene1"),
                        "Cladding1",0,0,0);
    
    // Cladding (second layer)
    //
    G4Tubs* clad2_tube =
    new G4Tubs("Cladding2",fClad2_rmin,fClad2_rmax,fClad2_z,fClad2_sphi,
               fClad2_ephi);
    
    fClad2_log =
    new G4LogicalVolume(clad2_tube,G4Material::GetMaterial("Pethylene2"),
                        "Cladding2",0,0,0);
    
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fiber_log,
                      "Fiber", clad1_log,false,0);
    new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),clad1_log,
                      "Cladding1",fClad2_log,false,0);
    
    G4VisAttributes* FiberVis=new G4VisAttributes(G4Color(0.0,1.0,0.0));
    FiberVis->SetVisibility(true);
    fClad2_log->SetVisAttributes(FiberVis);

    
    SetLogicalVolume(fClad2_log);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NedmWLSFiber::CopyValues(){
    
    fFiber_rmin = 0.00*cm;
    fFiber_rmax = 0.088/2*cm;
    fFiber_z    = 112./2*cm;
    fFiber_sphi = 0.00*deg;
    fFiber_ephi = 360.*deg;
    
    fClad1_rmin = 0.;// fFiber_rmax;
    fClad1_rmax = fFiber_rmax + 0.003*cm;
    
    fClad1_z    = fFiber_z;
    fClad1_sphi = fFiber_sphi;
    fClad1_ephi = fFiber_ephi;
    
    fClad2_rmin = 0.;//fClad1_rmax;
    fClad2_rmax = fClad1_rmax + 0.003*cm;
    
    fClad2_z    = fFiber_z;
    fClad2_sphi = fFiber_sphi;
    fClad2_ephi = fFiber_ephi;
    
}
