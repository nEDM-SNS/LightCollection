#include "nEDMWLSFiberConstructor.hh"

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
    
    fReflector = false;
    //fMirrorRmax  = fClad2_rmax;
    fMirrorRmax  = fFiber_rmax;
    fMirrorRmin  = 0.*cm;
    fMirrorThick = 1.*mm;
    fMirrorSPhi  = fFiber_sphi;
    fMirrorEPhi  = fFiber_ephi;
    
    fMirrorPosZ  = -1*(fFiber_z - fMirrorThick)/2;
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
    
    
    if (fReflector) {
        // Fiber Reflector
        G4Tubs* solidMirror = new G4Tubs("Mirror",
                                         fMirrorRmin,
                                         fMirrorRmax,
                                         fMirrorThick,
                                         fMirrorSPhi,
                                         fMirrorEPhi);
        
        
        G4LogicalVolume* logicMirror = new G4LogicalVolume(solidMirror,G4Material::GetMaterial("PMMA"),"Mirror");
        
        
        // Photon Energies for which mirror properties will be given
        const G4int kEnergies = 3;
        G4double the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 16*eV};
        
        // Optical Surface for mirror
        G4OpticalSurface* mirror_surface_ =
        new G4OpticalSurface("MirrorSurface", glisur, groundfrontpainted,
                             dielectric_dielectric);
        
        // Reflectivity of mirror for each photon energy
        G4double mirror_REFL[kEnergies] = {0.998, 0.998, 0.998};
        
        //Table of Surface Properties for Mirror
        G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
        mirrorSurfaceProperty->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
        mirror_surface_->SetMaterialPropertiesTable(mirrorSurfaceProperty);
        
        // Place Mirror
        new G4PVPlacement(0,                                 //no rotation
                          G4ThreeVector(0.,0.,fMirrorPosZ),   //position
                          logicMirror,            //its logical volume
                          "Mirror",               //its name
                          //Clad2_log,            //its mother  volume
                          core_log,               //its mother  volume
                          false,                  //no boolean operation
                          0,                    //copy number
                          fCheckOverlaps);
        
        // Create Skin Surface to link logical surface and optical surface
        new G4LogicalSkinSurface("MirrorSurface",logicMirror,mirror_surface_);
        
        // Set Visualization Properties of the Mirror
        G4VisAttributes* MirrorVis=new G4VisAttributes(G4Color(0.0,0.0,1.0));
        MirrorVis->SetVisibility(true);
        logicMirror->SetVisAttributes(MirrorVis);
        
    }

    return clad2_log;
    
    

}


void nEDMWLSFiberConstructor::SetOpticalSurface(G4String volName){

    if (fSurfaceRoughness < 1.){
        // Boundary Surface Properties
        G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
        G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
        fiberOuterRoughSurface = new G4LogicalBorderSurface("fiberOuterRoughSurface",
                                                            nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume(volName),
                                                            nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName()),
                                                            fiberOuterRoughOpSurface);
        
        
        fiberOuterRoughOpSurface->SetModel(glisur);
        fiberOuterRoughOpSurface->SetFinish(ground);
        fiberOuterRoughOpSurface->SetType(dielectric_dielectric);
        fiberOuterRoughOpSurface->SetPolish(fSurfaceRoughness);
   
    }

}


void nEDMWLSFiberConstructor::SetOpticalSurface(G4VPhysicalVolume* fiberVol, G4VPhysicalVolume* outerVol)
{
    
    if (fSurfaceRoughness < 1.){
        // Boundary Surface Properties
        G4OpticalSurface* fiberOuterRoughOpSurface =new G4OpticalSurface("fiberOuterRoughOpSurface");
        G4LogicalBorderSurface* fiberOuterRoughSurface = NULL;
        fiberOuterRoughSurface = new G4LogicalBorderSurface("fiberOuterRoughSurface",
                                                            outerVol,
                                                            fiberVol,
                                                            fiberOuterRoughOpSurface);
        
        
        fiberOuterRoughOpSurface->SetModel(glisur);
        fiberOuterRoughOpSurface->SetFinish(ground);
        fiberOuterRoughOpSurface->SetType(dielectric_dielectric);
        fiberOuterRoughOpSurface->SetPolish(fSurfaceRoughness);

    }
}



