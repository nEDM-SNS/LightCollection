#include "nEDMWLSFiberConstructor.hh"

#include <G4PVPlacement.hh>

nEDMWLSFiberConstructor::~nEDMWLSFiberConstructor(){;}

void nEDMWLSFiberConstructor::Init(void){
    fClad2_rmin = 0.*cm;
    fClad2_rmax = 0.100*cm/2;
    
    fClad2_z    = 2*20.64*cm;
    fClad2_sphi = 0.00*deg;
    fClad2_ephi = 360.*deg;
    
    fClad1_rmin = 0.*cm;
    fClad1_rmax = fClad2_rmax - 0.003*cm;
    
    fClad1_z    = fClad2_z;
    fClad1_sphi = fClad2_sphi;
    fClad1_ephi = fClad2_ephi;
    
    fFiber_rmin = 0.00*cm;
    fFiber_rmax = fClad1_rmax - 0.003*cm;
    fFiber_z    = fClad2_z;
    fFiber_sphi = fClad1_sphi;
    fFiber_ephi = fClad1_ephi;
    
    fSurfaceRoughness = 0.9;
    
    //fMirrorRmax  = fClad2_rmax;
    fMirrorRmax  = fFiber_rmax;
    fMirrorRmin  = 0.*cm;
    fMirrorThick = 1./2*mm;
    fMirrorSPhi  = fFiber_sphi;
    fMirrorEPhi  = fFiber_ephi;
    
    fMirrorPosZ  = -1*(fFiber_z - fMirrorThick);
    fMirrorReflectivity = 1;
    

}

G4LogicalVolume* nEDMWLSFiberConstructor::GetPiece(){

    
    
    G4cout << "//################################################//" << G4endl;
    G4cout << "//################ The Fiber  ####################//" << G4endl;
    G4cout << "//################################################//" << G4endl;
    
    // Cladding (outer layer)
    //
    G4String OuterCladdingName = GetName() + "/OuterCladding";
    
    G4Tubs* clad2_tube =
    new G4Tubs(OuterCladdingName,fClad2_rmin,fClad2_rmax,fClad2_z/2,fClad2_sphi,
               fClad2_ephi);
    
    G4LogicalVolume* clad2_log =
    new G4LogicalVolume(clad2_tube,FindMaterial("FPethylene"),
                        OuterCladdingName,0,0,0);
    

    // Cladding (first layer)
    //
    
    G4String InnerCladdingName = GetName() + "/InnerCladding";
    
    G4Tubs* clad1_tube =
    new G4Tubs(InnerCladdingName,fClad1_rmin,fClad1_rmax,fClad1_z/2,fClad1_sphi,
               fClad1_ephi);
    
    G4LogicalVolume* clad1_log =
    
    new G4LogicalVolume(clad1_tube,FindMaterial("Pethylene"),
                        InnerCladdingName,0,0,0);
    
    G4PVPlacement* clad1_phys = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),clad1_log,
                      "Cladding1",clad2_log,false,0,fCheckOverlaps);
 
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(InnerCladdingName,clad1_phys);

    

    // Fiber Core
    //
    
    G4String CoreName = GetName() + "/Core";

    G4Tubs* core_tube =
    new G4Tubs(CoreName,fFiber_rmin,fFiber_rmax,fFiber_z/2,fFiber_sphi,fFiber_ephi);
    
    G4LogicalVolume* core_log =
    new G4LogicalVolume(core_tube,FindMaterial("WLSPMMA"),
                        CoreName);
    
    G4PVPlacement* core_phys = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),core_log,
                      CoreName, clad1_log,false,0,fCheckOverlaps);

    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(CoreName,core_phys);

    
    //Set Vis attributes and return logical volume
    //
    G4VisAttributes* FiberVis=new G4VisAttributes(G4Color(0.0,1.0,0.0));
    FiberVis->SetVisibility(true);
    clad2_log->SetVisAttributes(FiberVis);
        
    return clad2_log;
    
    

}