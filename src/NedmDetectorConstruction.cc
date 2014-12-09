// light guides + central cell - ASK-17apr09
//
// $Id: guidesDetectorConstruction.cc,v 1.14 2004/03/17 22:41:12 gum Exp $
// GEANT4 tag $Name: geant4-08-00 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NedmDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4SubtractionSolid.hh"

#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"

#include "NedmDetectorParameters.hh"
#include "NedmDetectorMessenger.hh"
#include "NedmDetectorComponent.hh"
#include "NedmCellSide.hh"
#include "NedmLightGuide.hh"
#include "NedmPMT.hh"
#include "NedmQuartzWindow.hh"
#include "NedmWLSFiber.hh"
#include "NedmCompoundSolid.hh"

//#include <G4GDMLParser.hh>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


const G4int NedmDetectorConstruction::kEnergies = 3;
G4double NedmDetectorConstruction::the_photon_energies_[kEnergies] = {2.034*eV, 4.136*eV, 16*eV};

SurfaceTypeMap NedmDetectorConstruction::the_border_surface_map_;

NedmDetectorConstruction::NedmDetectorConstruction()
{
    NedmDetectorParameters::instance()->Compute();
    
    SetDefaults();
    
    detMessenger = new NedmDetectorMessenger(this);
    
    //The component classes need access to the detector in order to
    //access configuration parameters. The initialize method does this.
    NedmDetectorComponent::Initialize(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NedmDetectorConstruction::~NedmDetectorConstruction() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NedmDetectorConstruction::UpdateGeometry() {
    //Forget our border surfaces since we will be deleting them
    the_border_surface_map_.clear();
    
    //Remove everything we can think of from the detector geometry
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    G4LogicalSkinSurface::CleanSurfaceTable();
    G4LogicalBorderSurface::CleanSurfaceTable();
    G4SurfaceProperty::CleanSurfacePropertyTable();
    
    //Recalculate the parameters as a changed value is one of the most likely reasons
    //for this method to be called
    NedmDetectorParameters::instance()->Compute();
    PrintParameters();
    
    //Rebuild the world and tell Geant we have done so.
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructGeometry());
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    
    
}


/*! Does the actual work of initializing the geometry.
 */
G4VPhysicalVolume* NedmDetectorConstruction::Construct()
{
    //Make the materials we will use. This only needs to be done once,
    //as we can change the optical properties of the materials elsewhere
    //dynamically.
    ConstructMaterials();
    
    //Build the detector
    return ConstructGeometry();
}

G4VPhysicalVolume* NedmDetectorConstruction::ConstructGeometry() {
    // The experimental Hall
    G4double hall_side = 5*m;
    G4Box* expHall_box = new G4Box("World", hall_side, hall_side, hall_side);
    
    expHall_log
    = new G4LogicalVolume(expHall_box, SuperfluidHelium, "World");
    
    expHall_log->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    expHall_phys
    = new G4PVPlacement(0, G4ThreeVector(), expHall_log, "World", 0, false, 0);
    
    //Create/recreate our optical surfaces so that they have the correct optical
    //properties, which might have changed from the last time this method was
    //called.
    ConstructOpticalSurfaces();
    
    //Rebuild the logical volumes for our components so that they have correct
    //properties and so we are sure they exist.
    CreateComponentLogicalVolumes();
    
    //Build the majority of the detector apparatus
    ConstructBox();
    
    //Add in the endcaps for the box
    //ConstructEndCaps();
    
    //always return the physical World
    return expHall_phys;
}

void NedmDetectorConstruction::CreateComponentLogicalVolumes() {
    
    NedmCellSide::CreateLogicalVolumes();
    NedmLightGuide::CreateLogicalVolumes();
    NedmPMT::CreateLogicalVolumes();
    NedmQuartzWindow::CreateLogicalVolumes();
    
}


void NedmDetectorConstruction::ConstructBox() {
    NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
    G4ThreeVector pos1(0.,
                       -parameters->cell_disty(),
                       0.);
    G4ThreeVector pos3(-parameters->cell_distx(),
                       0.,
                       0.);
    
    G4RotationMatrix* zRot = new G4RotationMatrix;
    zRot->rotateZ(90*deg);
    
    G4RotationMatrix* nzRot = new G4RotationMatrix;
    nzRot->rotateZ(-90*deg);
    
    G4RotationMatrix* oneEighty = new G4RotationMatrix;
    oneEighty->rotateZ(180*deg);
    
    G4ThreeVector positions[3] = { pos1,-pos1, pos3 };
    G4RotationMatrix* rotations[3] = { 0, oneEighty, zRot };
    
    
    G4ThreeVector pmt_offset;
    
    if(parameters->use_lightguide_files()) {
        pmt_offset = NedmLightGuide::lightguide_end_offset();
    }
    
    G4int numLG;
    
    if (parameters->use_single_plate()) {numLG=1;}
    else {numLG=3;}
    
    for(int i = 0; i < numLG; i++) {
        
        new NedmCellSide(rotations[i],
                         positions[i],
                         expHall_log,
                         i,
                         false);
        
        G4ThreeVector pmt_position = positions[i] + parameters->pmt_position();
        G4ThreeVector window_position = positions[i] + parameters->window_position();
        
        if (i!=2) {
            NedmLightGuide::ConstructLightGuides(rotations[i],
                                                 positions[i],
                                                 expHall_log,
                                                 i);
            
            if(parameters->use_lightguide_files()) {
                G4ThreeVector pmt_offset_1 = pmt_offset;
                if(rotations[i])
                    pmt_offset_1 *= rotations[i]->inverse();
                pmt_position += pmt_offset_1;
                window_position += pmt_offset_1;
            }
        }
        else {
            pmt_position += G4ThreeVector(-1*parameters->cell_size().y(),0.,72*cm+parameters->pmt_z()/2.);
            
            // Place fibers on outside of cell wall
            G4ThreeVector fiber_position = positions[i] + G4ThreeVector(-1.*(parameters->cell_size().y()+.1*cm), -1.*parameters->cell_size().x(), 36.*cm);
            
            for (int j = 0; j < 98; j++) {
                
                new NedmWLSFiber(zRot,
                                 fiber_position,
                                 expHall_log,
                                 false,0);
                
                fiber_position += G4ThreeVector(0., parameters->cell_size().x()/49, 0.);
                
            }
        }
        
        
        
        if(parameters->use_window() && parameters->use_lightguide_files()) {
            new NedmQuartzWindow(rotations[i],
                                 window_position,
                                 expHall_log,
                                 i);
        }
        
        new NedmPMT(rotations[i],
                    pmt_position,
                    expHall_log,
                    i);
        
        
    }
    

    // Reflector Wrapping
    G4Box* OuterReflector = new G4Box("OuterReflector",8.*cm,8.*cm,parameters->cell_size().z());
    G4Box* InnerReflector = new G4Box("InnerReflector",7.*cm,7.*cm,parameters->cell_size().z());
    G4SubtractionSolid* SolidReflector = new G4SubtractionSolid("Reflector",OuterReflector,InnerReflector);
    
    G4LogicalVolume* Reflector_Log = new G4LogicalVolume(SolidReflector, acrylic_, "Reflector");
    
    G4LogicalSkinSurface* Reflector_surface =
    new G4LogicalSkinSurface("Reflector_surface", Reflector_Log, mirror_surface());
    
    NedmDetectorConstruction::RegisterSurface(Reflector_surface, "mirror");

    
    G4VisAttributes* ReflectVis=new G4VisAttributes(G4Color(1.0,1.0,1.0));
    ReflectVis->SetVisibility(true);
    ReflectVis->SetForceWireframe(true);
    Reflector_Log->SetVisAttributes(ReflectVis);
    
    new G4PVPlacement(0,                            //no rotation
                      G4ThreeVector(),              //at (0,0,0)
                      Reflector_Log,                     //its logical volume
                      "Reflector",            //its name
                      expHall_log,                //its mother  volume
                      false,                        //no boolean operation
                      0);                            //copy number
    
    
    // Fiber Reflector
    G4Box* FiberReflectorSolid = new G4Box("FiberReflector", 5.*cm,
                                           .2*cm, .05*cm);
    G4LogicalVolume* FiblerReflector_Log = new G4LogicalVolume(FiberReflectorSolid,acrylic_, "FiberReflector");
    
    G4LogicalSkinSurface* FiberReflector_surface =
    new G4LogicalSkinSurface("FiberReflector_surface", FiblerReflector_Log, mirror_surface());

    NedmDetectorConstruction::RegisterSurface(FiberReflector_surface, "mirror");

    G4VisAttributes* FibReflectVis=new G4VisAttributes(G4Color(0.0,0.0,1.0));
    FibReflectVis->SetVisibility(true);
    FiblerReflector_Log->SetVisAttributes(FibReflectVis);
    
    G4ThreeVector FibReflPos = pos3 - G4ThreeVector(parameters->cell_size().y()+.2*cm, 0., parameters->cell_size().z());
    
    new G4PVPlacement(zRot,                            //no rotation
                      FibReflPos,              //position
                      FiblerReflector_Log,                     //its logical volume
                      "FiberReflector",            //its name
                      expHall_log,                //its mother  volume
                      false,                        //no boolean operation
                      0);                            //copy number


    
    /*
     if(!parameters->use_lightguide_files() && parameters->use_window()) {
     new NedmQuartzWindow(0,
     parameters->window_position(),
     expHall_log,
     0);
     
     }*/
    
    
}



/*!
 
 Looks up/builds the materials which the system components are made from.
 Light transmission proprties of the materials are also set here.
 */
void NedmDetectorConstruction::ConstructMaterials() {
    //	------------- Materials -------------
    
    G4double a, z, n, density;
    G4int nelements;
    
    G4int polyPMMA = 1;
    G4int nC_PMMA = 3+2*polyPMMA;
    G4int nH_PMMA = 6+2*polyPMMA;
    
    G4int polyeth = 1;
    G4int nC_eth = 2*polyeth;
    G4int nH_eth = 4*polyeth;
    
    //***Elements
    fH = new G4Element("H", "H", z=1., a=1.01*g/mole);
    fC = new G4Element("C", "C", z=6., a=12.01*g/mole);
    fO = new G4Element("O"  , "O", z=8., a= 16.00*g/mole);
    
    //Fiber(PMMA)
    fPMMA = new G4Material("PMMA", density=1190*kg/m3,3);
    fPMMA->AddElement(fH,nH_PMMA);
    fPMMA->AddElement(fC,nC_PMMA);
    fPMMA->AddElement(fO,2);
    //Cladding(polyethylene)
    fPethylene1 = new G4Material("Pethylene1", density=1200*kg/m3,2);
    fPethylene1->AddElement(fH,nH_eth);
    fPethylene1->AddElement(fC,nC_eth);
    //Double cladding(flourinated polyethylene)
    fPethylene2 = new G4Material("Pethylene2", density=1400*kg/m3,2);
    fPethylene2->AddElement(fH,nH_eth);
    fPethylene2->AddElement(fC,nC_eth);

    
    G4NistManager* man = G4NistManager::Instance();
    
    // Air
    //
    air_ = man->FindOrBuildMaterial("G4_AIR");
    
    
    // Acrylic
    //
    G4Element* C = new G4Element("Carbon", "C", z=6 , a=12.00*g/mole);
    G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);
    G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
    
    acrylic_ = new G4Material("Acrylic", density= 1.18*g/cm3, nelements=3);
    acrylic_->AddElement(C, 0.59984);
    acrylic_->AddElement(H, 0.08055);
    acrylic_->AddElement(O, 0.31961);
    
    
    // Quartz
    
    G4Element* Si = new G4Element("Silicon", "Si", z=14, a=28.0855*g/mole);
    
    quartz_ = new G4Material("Quartz", density= 2.65*g/cm3, nelements=2);
    quartz_->AddElement(Si, 1);
    quartz_->AddElement(O, 2);
    
    torlon_ = man->FindOrBuildMaterial("G4_BAKELITE");
    
    
    //
    // ------------ Generate & Add Material Properties Table ------------
    //
    
    
    //
    // Air
    //
    G4double RefractiveIndex2[kEnergies] = { 1.00, 1.00, 1.00};
    
    G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
    myMPT2->AddProperty("RINDEX", the_photon_energies_, RefractiveIndex2, kEnergies);
    
    air_->SetMaterialPropertiesTable(myMPT2);
    
    //
    // Acrylic
    //
    
    G4double RefractiveIndex3[kEnergies] = { acrylic_index_, acrylic_index_ , 1.00};
    G4double Absorption3[kEnergies] = { acrylic_absorption_length_, acrylic_absorption_length_, 100.*m };
    
    acrylic_mt_ = new G4MaterialPropertiesTable();
    acrylic_mt_->AddProperty("RINDEX", the_photon_energies_, RefractiveIndex3, kEnergies);
    acrylic_mt_->AddProperty("ABSLENGTH", the_photon_energies_, Absorption3, kEnergies);
    
    acrylic_->SetMaterialPropertiesTable(acrylic_mt_);
    
    
    //
    // Quartz
    //
    G4double RefractiveIndexQ[kEnergies] =
    { 1.545, 1.545, 1.00 };
    
    G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
    QuartzMPT->AddProperty("RINDEX", the_photon_energies_, RefractiveIndexQ, kEnergies);
    
    quartz_->SetMaterialPropertiesTable(QuartzMPT);
    
    //
    // Helium
    //
    // Helium-4He
    G4Isotope* He4 = new G4Isotope("He4", z=2, n=4, a=4.0026*g/mole);
    G4Element* elHe  = new G4Element("Helium", "He", 1);
    elHe->AddIsotope(He4, 100.*perCent);

    // Superfluid Helium: Density from Tito (07/13/10)
    SuperfluidHelium = new G4Material("SuperfluidHelium", density=0.145*g/cm3, 1, kStateLiquid, 0.4*kelvin);
    SuperfluidHelium->AddElement(elHe, 1.0);
    
    G4double RefractiveIndex4[kEnergies] = { 1.024, 1.024, 1.024};
    
    G4MaterialPropertiesTable* myMPT4 = new G4MaterialPropertiesTable();
    myMPT4->AddProperty("RINDEX", the_photon_energies_, RefractiveIndex4, kEnergies);
    SuperfluidHelium->SetMaterialPropertiesTable(myMPT4);
    
    
    // WLS properties
    const G4int wlsnum = 4;
    G4double wls_Energy[] = {2.00*eV,2.87*eV,2.90*eV,3.47*eV};

    G4double RefractiveIndexFiber[wlsnum]={ 1.59, 1.59, 1.59, 1.59};
    G4double AbsFiber[wlsnum]={9.00*m,9.00*m,0.1*mm,0.1*mm};
    G4double EmissionFib[wlsnum]={1.0, 1.0, 0.0, 0.0};
    G4MaterialPropertiesTable* fiberProperty = new G4MaterialPropertiesTable();
    fiberProperty->AddProperty("RINDEX",wls_Energy,RefractiveIndexFiber,wlsnum);
    fiberProperty->AddProperty("WLSABSLENGTH",wls_Energy,AbsFiber,wlsnum);
    fiberProperty->AddProperty("WLSCOMPONENT",wls_Energy,EmissionFib,wlsnum);
    fiberProperty->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);
    fPMMA->SetMaterialPropertiesTable(fiberProperty);
    
    G4double RefractiveIndexClad1[wlsnum]={ 1.49, 1.49, 1.49, 1.49};
    G4MaterialPropertiesTable* clad1Property = new G4MaterialPropertiesTable();
    clad1Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad1,wlsnum);
    //clad1Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);
    fPethylene1->SetMaterialPropertiesTable(clad1Property);
    
    G4double RefractiveIndexClad2[wlsnum]={ 1.42, 1.42, 1.42, 1.42};
    G4MaterialPropertiesTable* clad2Property = new G4MaterialPropertiesTable();
    clad2Property->AddProperty("RINDEX",wls_Energy,RefractiveIndexClad2,wlsnum);
    //clad2Property->AddProperty("ABSLENGTH",wls_Energy,AbsFiber,wlsnum);
    fPethylene2->SetMaterialPropertiesTable(clad2Property);

    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NedmDetectorConstruction::ConstructOpticalSurfaces() {
    
    machined_surface_ = new G4OpticalSurface("MachinedAcrylicSurface");
    machined_surface_->SetType(dielectric_dielectric);
    machined_surface_->SetFinish(polished);
    machined_surface_->SetModel(glisur);
    
    G4double machined_RINDEX[kEnergies]= { acrylic_index_, acrylic_index_, 1.00 };
    
    G4double mREFL = 1.-machined_lpb_;
    
    G4double machined_REFL[kEnergies]={mREFL, mREFL, mREFL};
    machined_mt_ = new G4MaterialPropertiesTable();
    machined_mt_->AddProperty("RINDEX", the_photon_energies_, machined_RINDEX, kEnergies);
    machined_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, machined_REFL, kEnergies);
    machined_surface_->SetMaterialPropertiesTable(machined_mt_);
    
    
    cast_surface_ = new G4OpticalSurface("CastAcrylicSurface");
    cast_surface_->SetType(dielectric_dielectric);
    cast_surface_->SetFinish(polished);
    cast_surface_->SetModel(glisur);
    
    G4double cREFL = 1-cast_lpb_;
    G4double cast_REFL[kEnergies]={cREFL, cREFL, cREFL};
    cast_mt_ = new G4MaterialPropertiesTable();
    cast_mt_->AddProperty("RINDEX", the_photon_energies_, machined_RINDEX, kEnergies);
    cast_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, cast_REFL, kEnergies);
    cast_surface_->SetMaterialPropertiesTable(cast_mt_);
    
    tpb_surface_ = new G4OpticalSurface("TPBSurface");
    tpb_surface_->SetType(dielectric_dielectric);
    tpb_surface_->SetFinish(polished);
    tpb_surface_->SetModel(glisur);
    
    G4double tpbREFL = 1. - tpb_lpb_;
    G4double TPB_REFL[kEnergies] = { tpbREFL, tpbREFL, tpbREFL };
    G4double TPB_RINDEX[kEnergies]={ acrylic_index_, acrylic_index_, 1.00};
    tpb_mt_ = new G4MaterialPropertiesTable();
    tpb_mt_->AddProperty("RINDEX", the_photon_energies_, TPB_RINDEX, kEnergies);
    tpb_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, TPB_REFL, kEnergies);
    
    tpb_surface_->SetMaterialPropertiesTable(tpb_mt_);
    
    conversion_surface_ = new G4OpticalSurface("TPBConversionSurface");
    conversion_surface_->SetType(dielectric_dielectric);
    conversion_surface_->SetFinish(polished);
    conversion_surface_->SetModel(glisur);
    
    tpbc_mt_ = new G4MaterialPropertiesTable();
    tpbc_mt_->AddProperty("RINDEX", the_photon_energies_, TPB_RINDEX, kEnergies);
    //tpbc_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, TPB_REFL, kEnergies);
    tpbc_mt_->AddConstProperty("TPBCONVERSIONEFFICIENCY", 0.24);
    
    conversion_surface_->SetMaterialPropertiesTable(tpbc_mt_);
    
    torlon_surface_ = new G4OpticalSurface("TorlonSurface",
                                           glisur, torlon_type_, dielectric_metal);
    G4double Torlon_Eff[kEnergies] = {0., 0., 0.};
    
    G4double tREFL = 1. - torlon_lpb_;
    G4double Torlon_Refl[kEnergies]={tREFL, tREFL, 0.};
    torlon_mt_ = new G4MaterialPropertiesTable;
    torlon_mt_->AddProperty("EFFICIENCY", the_photon_energies_, Torlon_Eff, kEnergies);
    torlon_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, Torlon_Refl, kEnergies);
    torlon_surface_->SetMaterialPropertiesTable(torlon_mt_);
    
    
    mirror_surface_ =
    new G4OpticalSurface("MirrorSurface", glisur, mirror_type_,
                         dielectric_dielectric);
    G4double mirror_REFL[kEnergies] = {mirror_reflectivity_, mirror_reflectivity_, mirror_reflectivity_};
    
    mirror_mt_ = new G4MaterialPropertiesTable();
    mirror_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
    mirror_surface_->SetMaterialPropertiesTable(mirror_mt_);
    
    pmt_surface_ =
    new G4OpticalSurface("photocath_opsurf", glisur, polished,
                         dielectric_metal);
    G4double photocath_EFF[kEnergies]={pmt_efficiency_, pmt_efficiency_, 0};
    G4double photocath_REFL[kEnergies]={0., 0., 0.};
    photocath_mt_ = new G4MaterialPropertiesTable();
    photocath_mt_->AddProperty("EFFICIENCY", the_photon_energies_, photocath_EFF, kEnergies);
    photocath_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, photocath_REFL, kEnergies);
    pmt_surface_->SetMaterialPropertiesTable(photocath_mt_);
}


void NedmDetectorConstruction::PrintParameters() {
    G4cout << G4endl << G4endl
    <<"[------USER CONFIGURABLE PARAMETERS------]" << G4endl;
    
    G4cout << "Loss per bounce on Acrylic Cast Face: "
    << cast_lpb_*100 << " percent." << G4endl;
    
    G4cout << "Loss per bounce on Acrylic Machined Face: "
    << machined_lpb_*100 << " percent." << G4endl;
    
    G4cout << "Loss per bounce on Torlon Plug: "
    << torlon_lpb_*100 << " percent." << G4endl;
    
    G4cout << "Loss per bounce on TPB: "
    << tpb_lpb_*100 << " percent." << G4endl;
    
    G4cout << "Mirror Reflectivity: "
    << mirror_reflectivity_ * 100 << " percent." << G4endl;
    
    G4cout << "Acrylic Attenuation Length: " <<
    G4BestUnit(acrylic_absorption_length_, "Length") << G4endl;
    
    G4cout << "Acrylic Refractive Index: " <<
    acrylic_index_ << G4endl;
    
    G4cout << "Mirror Type: ";
    if(mirror_type_ == groundfrontpainted)
        G4cout << "diffuse" << G4endl;
    else if (mirror_type_ == polishedfrontpainted)
        G4cout << "specular" << G4endl;
    
    G4cout << "Torlon Surface Type: ";
    if(torlon_type_ == groundfrontpainted)
        G4cout << "diffuse" << G4endl;
    else if (torlon_type_ == polishedfrontpainted)
        G4cout << "specular" << G4endl;
    
    G4cout << "PMT Efficiency: " <<
    pmt_efficiency_ * 100. << " percent." << G4endl;
    
    
    NedmDetectorParameters::instance()->Print();
    
    G4cout << "[---------------------------------------]" << G4endl
    << G4endl;
    
    
}

void NedmDetectorConstruction::SetDefaults() {
    //Optical Defaults
    machined_lpb_ = 0.01;
    cast_lpb_ = 0.001;
    torlon_lpb_ = 0.5;
    tpb_lpb_ = 0.0285;
    acrylic_index_ = 1.49;
    acrylic_absorption_length_ = 650.*cm;
    pmt_efficiency_ = 1.0;
    mirror_reflectivity_ = 0.998;
    mirror_type_ = groundfrontpainted;
    torlon_type_ = polishedfrontpainted;
}

void NedmDetectorConstruction::ConstructEndCaps() {
    NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
    
    G4ThreeVector size = parameters->endcap_size();
    
    G4Box* endcap_box = new G4Box("Endcap", size.x(), size.y(), size.z());
    G4LogicalVolume* endcap_log = new G4LogicalVolume(endcap_box,
                                                      acrylic_,
                                                      "Endcap_log");
    
    
    G4ThreeVector position = parameters->endcap_position();
    //G4VPhysicalVolume* nzEndcap =
    new G4PVPlacement(0,
                      position,
                      endcap_log,
                      "Endcap",
                      expHall_log,
                      false,
                      0);
    
    position.rotateX(pi);
    
    //G4VPhysicalVolume* pzEndcap =
    new G4PVPlacement(0,
                      position,
                      endcap_log,
                      "Endcap",
                      expHall_log,
                      false,
                      1);
}

void NedmDetectorConstruction::set_cast_loss(G4double lpb) {
    cast_lpb_ = lpb;
    G4double cREFL = 1. - lpb;
    G4double cast_REFL[kEnergies]={cREFL, cREFL, cREFL};
    cast_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, cast_REFL, kEnergies);
}

void NedmDetectorConstruction::set_machined_loss(G4double lpb) {
    machined_lpb_ = lpb;
    G4double mREFL = 1. - lpb;
    G4double machined_REFL[kEnergies]={mREFL, mREFL, mREFL};
    machined_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, machined_REFL, kEnergies);
}

void NedmDetectorConstruction::set_tpb_loss(G4double lpb) {
    tpb_lpb_ = lpb;
    G4double tREFL = 1. - lpb;
    G4double tpb_REFL[kEnergies]={tREFL, tREFL, tREFL};
    tpb_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, tpb_REFL, kEnergies);
}

void NedmDetectorConstruction::set_acrylic_index(G4double index) {
    acrylic_index_ = index;
    G4double RefractiveIndex[kEnergies] = { acrylic_index_, acrylic_index_, 1.00 };
    acrylic_mt_->AddProperty("RINDEX", the_photon_energies_, RefractiveIndex, kEnergies);
}

void NedmDetectorConstruction::set_mirror_reflectivity(G4double eps) {
    mirror_reflectivity_ = eps;
    G4double mirror_REFL[kEnergies] = { mirror_reflectivity_, mirror_reflectivity_, mirror_reflectivity_ };
    mirror_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, mirror_REFL, kEnergies);
}

void NedmDetectorConstruction::set_mirror_type(G4String type) {
    if(type == "specular")
        mirror_type_ = polishedfrontpainted;
    else if (type == "diffuse")
        mirror_type_ = groundfrontpainted;
}

void NedmDetectorConstruction::set_torlon_type(G4String type) {
    if(type == "specular")
        torlon_type_ = polishedfrontpainted;
    else if (type == "diffuse")
        torlon_type_ = groundfrontpainted;
}

void NedmDetectorConstruction::set_bulk_attenuation(G4double length) {
    acrylic_absorption_length_ = length;
    G4double absLength[kEnergies] = { length, length, 100.*m};
    acrylic_mt_->AddProperty("ABSLENGTH", the_photon_energies_, absLength, kEnergies);
}

void NedmDetectorConstruction::set_torlon_loss(G4double lpb) {
    torlon_lpb_ = lpb;
    G4double tREFL = 1. - lpb;
    G4double torlon_REFL[kEnergies]={tREFL, tREFL, tREFL};
    torlon_mt_->AddProperty("REFLECTIVITY", the_photon_energies_, torlon_REFL, kEnergies);
}

void NedmDetectorConstruction::set_pmt_efficiency(G4double eff) {
    pmt_efficiency_ = eff;
    G4double photocath_EFF[kEnergies]={eff, eff, 0.};
    photocath_mt_->AddProperty("EFFICIENCY", the_photon_energies_, photocath_EFF, kEnergies);
}

