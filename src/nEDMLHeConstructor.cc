#include "nEDMLHeConstructor.hh"

#include "nEDMDetectorConstruction.hh"
#include "nEDMCellSideConstructor.hh"
#include "nEDMWLSFiberConstructor.hh"
#include "nEDMSimplePhotDetConstructor.hh"

#include "G4SubtractionSolid.hh"


nEDMLHeConstructor::~nEDMLHeConstructor(){;}

void nEDMLHeConstructor::Init(){
    
    SetMaterial("G4_AIR");
    
    fLHELength = 1.9*m;
    fLHERadius = 1.*m;
    
    AddConstructor(new nEDMCellSideConstructor("CellSide1",this));
    AddConstructor(new nEDMCellSideConstructor("CellSide2",this));
    AddConstructor(new nEDMCellSideConstructor("CellSide3",this));
    AddConstructor(new nEDMWLSFiberConstructor("WLSFiber",this));
    AddConstructor(new nEDMSimplePhotDetConstructor("PhotDet1",this));
    AddConstructor(new nEDMSimplePhotDetConstructor("PhotDet2",this));
    
}

G4LogicalVolume* nEDMLHeConstructor::GetPiece(void)
    {
        G4cout << "//################################################//" << G4endl;
        G4cout << "//################ LHE Volume ####################//" << G4endl;
        G4cout << "//################################################//" << G4endl;
        
        fLogicLHE = new G4LogicalVolume(new G4Tubs(GetName(),0.,fLHERadius,fLHELength/2.,0.,360*deg),
                                                       FindMaterial(GetMaterial()),
                                                       GetName());
        
        G4VisAttributes* LHeAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
        LHeAtt->SetVisibility(true);
        LHeAtt->SetForceWireframe(true);
        fLogicLHE->SetVisAttributes(LHeAtt);
        
        Construct3CellPlates();
        ConstructSquareTubeReflector();
        
        return fLogicLHE;
    }

void nEDMLHeConstructor::Construct3CellPlates(){

    
    nEDMSimplePhotDetConstructor& detector1 = Get<nEDMSimplePhotDetConstructor>("PhotDet1");
    detector1.SetCheckOverlaps(fCheckOverlaps);
    
    nEDMSimplePhotDetConstructor& detector2 = Get<nEDMSimplePhotDetConstructor>("PhotDet2");
    detector2.SetCheckOverlaps(fCheckOverlaps);
    G4double detZPos;


    
    //////////////////////////
    // Cell Side 1
    //////////////////////////
    
    nEDMCellSideConstructor& cellSide1 = Get<nEDMCellSideConstructor>("CellSide1");
    cellSide1.SetCheckOverlaps(fCheckOverlaps);
    cellSide1.SetEmbedded_Fibers(false);
    cellSide1.SetFiber_Reflector(false);
    cellSide1.SetNum_Fibers(98);

    G4LogicalVolume* logicCellSide1 = cellSide1.GetPiece();
    
    G4VPhysicalVolume* physCellSide1 = new G4PVPlacement(0,                     // rotation
                                                         G4ThreeVector(0,0,0),  // position
                                                         logicCellSide1,        // logical volume
                                                         cellSide1.GetName(),   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide1.GetName(),physCellSide1,0);
    
    //////////////////////////
    // External Fibers Side 1
    //////////////////////////
    
    if (cellSide1.GetNumFib()>0 && !cellSide1.GetEmbedded_fibers()) {
        G4VPhysicalVolume* physFiber[1000] = {0};
        G4VPhysicalVolume* physDetector1[1000] = {0};
        G4VPhysicalVolume* physDetector2[1000] = {0};
        
        nEDMWLSFiberConstructor& fiber = Get<nEDMWLSFiberConstructor>("WLSFiber");
        fiber.SetCheckOverlaps(fCheckOverlaps);
        G4LogicalVolume* logicFiber = fiber.GetPiece();
        
        G4double FibXPos, FibYPos;
        G4double CellThickness = cellSide1.GetThickness();
        G4double FibThickness = fiber.GetFiberThickness();
        G4int NumFibers = cellSide1.GetNumFib();
        G4double FiberSpacing = cellSide1.GetFibSpacing();
        
        FibYPos = -1*(CellThickness/2+FibThickness/2+.005);
        
        detZPos = fiber.GetLength()/2.+detector1.GetThickness()/2.;

        G4LogicalVolume* logicFibDet1 = detector1.GetPiece();
        G4LogicalVolume* logicFibDet2 = detector2.GetPiece();
        
        for(G4int i=0;i<NumFibers;i++){
            FibXPos=-(FiberSpacing)*(NumFibers-1)*0.5 + i*FiberSpacing;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),logicFiber,fiber.GetName(),fLogicLHE,false,0,fCheckOverlaps);
            
            fiber.SetOpticalSurface(physFiber[i], nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName()));
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide1.GetName()+"/"+fiber.GetLocalName(),physFiber[i],i);
            
            // Place +Z detectors
            physDetector1[i] = new G4PVPlacement(0, G4ThreeVector(FibXPos,FibYPos,detZPos),logicFibDet1,detector1.GetName(),fLogicLHE,false,0,fCheckOverlaps);
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide1.GetName()+"/"+detector1.GetLocalName(),physDetector1[i],0);
            
            // Place -Z detectors
            physDetector2[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,-detZPos),logicFibDet2,detector2.GetName(),fLogicLHE,false,0,fCheckOverlaps);
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide1.GetName()+"/"+detector2.GetLocalName(),physDetector2[i],0);
            

        }
        
    }
    
    //////////////////////////
    // Cell Side 2
    //////////////////////////
    
    nEDMCellSideConstructor& cellSide2 = Get<nEDMCellSideConstructor>("CellSide2");
    cellSide2.SetCheckOverlaps(fCheckOverlaps);
    cellSide2.SetNum_Fibers(0);
    G4LogicalVolume* logicCellSide2 = cellSide2.GetPiece();
    
    G4ThreeVector cell2pos = G4ThreeVector(cellSide2.GetWidth()*0.6,cellSide2.GetWidth()*0.6,0);
    G4RotationMatrix* cell2rotm = new G4RotationMatrix();
    cell2rotm->rotateZ(-90.*deg);
    
    G4VPhysicalVolume* physCellSide2 = new G4PVPlacement(cell2rotm,                     // rotation
                                                         cell2pos,  // position
                                                         logicCellSide2,        // logical volume
                                                         cellSide2.GetName(),   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide2.GetName(),physCellSide2,0);
    
    
    detector1.SetYWidth(cellSide2.GetWidth());
    detector1.SetXWidth(cellSide2.GetThickness());
    detector2.SetYWidth(cellSide2.GetWidth());
    detector2.SetXWidth(cellSide2.GetThickness());
    
    G4LogicalVolume* logicStdDet1 = detector1.GetPiece();
    G4LogicalVolume* logicStdDet2 = detector2.GetPiece();
    
    detZPos = cellSide2.GetLength()/2.+detector1.GetThickness()/2.;

    
    G4VPhysicalVolume* physStdDet2_1 = new G4PVPlacement(0,                     // rotation
                                                         cell2pos+G4ThreeVector(0.,0.,detZPos),  // position
                                                         logicStdDet1,        // logical volume
                                                         detector1.GetName(),   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide2.GetName()+"/"+detector1.GetLocalName(),physStdDet2_1,0);


    G4VPhysicalVolume* physStdDet2_2 = new G4PVPlacement(0,                     // rotation
                                                         cell2pos+G4ThreeVector(0.,0.,-1*detZPos),  // position
                                                         logicStdDet2,        // logical volume
                                                         detector2.GetName(),   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide2.GetName()+"/"+detector2.GetLocalName(),physStdDet2_2,0);

    
    //////////////////////////
    // Cell Side 3
    //////////////////////////
    
    nEDMCellSideConstructor& cellSide3 = Get<nEDMCellSideConstructor>("CellSide3");
    cellSide3.SetCheckOverlaps(fCheckOverlaps);
    cellSide3.SetNum_Fibers(0);
    G4LogicalVolume* logicCellSide3 = cellSide3.GetPiece();
    
    G4ThreeVector cell3pos = G4ThreeVector(-1*cellSide3.GetWidth()*0.6,cellSide3.GetWidth()*0.6,0);
    G4RotationMatrix* cell3rotm = new G4RotationMatrix();
    cell3rotm->rotateZ(90.*deg);
    
    G4VPhysicalVolume* physCellSide3 = new G4PVPlacement(cell3rotm,                     // rotation
                                                         cell3pos,  // position
                                                         logicCellSide3,        // logical volume
                                                         cellSide3.GetName(),   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide3.GetName(),physCellSide3,0);
    
    
    
    G4VPhysicalVolume* physStdDet3_1 = new G4PVPlacement(0,                     // rotation
                                                         cell3pos+G4ThreeVector(0.,0.,detZPos),  // position
                                                         logicStdDet1,        // logical volume
                                                         detector1.GetName(),   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide3.GetName()+"/"+detector1.GetLocalName(),physStdDet3_1,0);
    
    
    G4VPhysicalVolume* physStdDet3_2 = new G4PVPlacement(0,                     // rotation
                                                         cell3pos+G4ThreeVector(0.,0.,-1*detZPos),  // position
                                                         logicStdDet2,        // logical volume
                                                         detector2.GetName(),   // name
                                                         fLogicLHE,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide3.GetName()+"/"+detector2.GetLocalName(),physStdDet3_2,0);

    
    

}


void nEDMLHeConstructor::ConstructSquareTubeReflector()
{
    // Reflector Wrapping
    G4Box* OuterReflector = new G4Box("OuterReflector",8.*cm,8.*cm,2*20.64*cm);
    G4Box* InnerReflector = new G4Box("InnerReflector",7.*cm,7.*cm,2*20.64*cm);
    G4SubtractionSolid* SolidReflector = new G4SubtractionSolid("Reflector",OuterReflector,InnerReflector);
    
    G4LogicalVolume* Reflector_Log = new G4LogicalVolume(SolidReflector, G4Material::GetMaterial("PMMA"), "Reflector");
    
    
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
    
    new G4LogicalSkinSurface("Reflector_surface", Reflector_Log, mirror_surface_);
    
    
    G4VisAttributes* ReflectVis=new G4VisAttributes(G4Color(1.0,1.0,1.0));
    ReflectVis->SetVisibility(true);
    ReflectVis->SetForceWireframe(true);
    Reflector_Log->SetVisAttributes(ReflectVis);
    
    new G4PVPlacement(0,                            //no rotation
                      G4ThreeVector(0.,5.*cm,0.),              //at (0,0,0)
                      Reflector_Log,                     //its logical volume
                      "Reflector",            //its name
                      fLogicLHE,                //its mother  volume
                      false,                        //no boolean operation
                      0,                        //copy number
                      fCheckOverlaps);                    // Check Overlaps
    
}


