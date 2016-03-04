#include "nEDMWorldConstructor.hh"

#include "nEDMCellSideConstructor.hh"
#include "nEDMWLSFiberConstructor.hh"
#include "nEDMLHeConstructor.hh"
#include "nEDMSimplePhotDetConstructor.hh"

#include "G4PVPlacement.hh"

class nEDMWorldMessenger : public nEDMConstructorMessenger
{
private:
    nEDMWorldConstructor* fConstructor;
    
public:
    nEDMWorldMessenger(nEDMWorldConstructor* construct) :
    nEDMConstructorMessenger(construct,"Control of the nEDM world geometry."),
    fConstructor(construct){};
    
    virtual ~nEDMWorldMessenger(){};
    
    void SetNewValue(G4UIcommand* cmd, G4String val){
        nEDMConstructorMessenger::SetNewValue(cmd,val);
    };
};


void nEDMWorldConstructor::Init(void)
{
    SetMessenger(new nEDMWorldMessenger(this));
    
    fWidth  = 2.*m;
    fHeight = fWidth;
    fLength = fWidth;
    
    AddConstructor(new nEDMLHeConstructor("LHE",this));
    AddConstructor(new nEDMWLSFiberConstructor("WLSFiber",this));
    AddConstructor(new nEDMSimplePhotDetConstructor("PhotDet1",this));
    AddConstructor(new nEDMSimplePhotDetConstructor("PhotDet2",this));
    AddConstructor(new nEDMCellSideConstructor("CellSide",this));
    
}

nEDMWorldConstructor::~nEDMWorldConstructor(){;}

G4LogicalVolume* nEDMWorldConstructor::GetPiece(void)
{
    G4cout << "//******************************************************//" << G4endl;
    G4cout << "//********* CONSTRUCT THE HALL -> The World ************//" << G4endl;
    G4cout << "//******************************************************//" << G4endl;
    
    fLogicHall = new G4LogicalVolume(new G4Box(GetName(),
                                               fWidth/2.,
                                               fHeight/2.,
                                               fWidth/2.),
                                     FindMaterial("G4_AIR"),
                                     GetName());
    
    //logicHall->SetVisAttributes(G4VisAttributes::Invisible);
    G4VisAttributes* hallAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    hallAtt->SetVisibility(true);
    hallAtt->SetForceWireframe(true);
    fLogicHall->SetVisAttributes(hallAtt);
    
    
    // ***********************************************************************
    // ******************** Construction Function Calls **********************
    // ***********************************************************************
    
    //ConstructSingleFiber();
    //ConstructCellPlate();
    ConstructLHE();
    
    // ***********************************************************************
    // ***********************************************************************
    
    return fLogicHall;
}



void nEDMWorldConstructor::ConstructSingleFiber(){
    
    /////////////////////
    //Air Box for Fiber
    /////////////////////
    
    G4LogicalVolume* logicAirBox = new G4LogicalVolume(new G4Box(GetName()+"/AirBox",
                                                                 fWidth/2.1,
                                                                 fHeight/2.1,
                                                                 fWidth/2.1),
                                                       FindMaterial("G4_AIR"),
                                                       GetName()+"/AirBox");
    
    G4VPhysicalVolume* physAirBox = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicAirBox,GetName()+"/AirBox",fLogicHall,false,0,fCheckOverlaps);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(GetName()+"/AirBox",physAirBox,0);
    
    G4VisAttributes* airBoxAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    airBoxAtt->SetVisibility(true);
    airBoxAtt->SetForceWireframe(true);
    logicAirBox->SetVisAttributes(airBoxAtt);
    
    
    ///////////////
    //Single Fiber
    ///////////////
    
    nEDMWLSFiberConstructor& fiber = Get<nEDMWLSFiberConstructor>("WLSFiber");
    fiber.SetCheckOverlaps(fCheckOverlaps);
    G4LogicalVolume* logicFiber = fiber.GetPiece();
    
    G4VPhysicalVolume* physFiber = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicFiber,fiber.GetName(),logicAirBox,false,0,fCheckOverlaps);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fiber.GetName(),physFiber,0);
    
#if 1
    // Rough Surface on outside of Fiber
    fiber.SetOpticalSurface(physFiber, physAirBox);
#elif 0
    // Rough Surface between cladding layers
    fiber.SetOpticalSurface(nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume("/nEDM/WLSFiber/InnerCladding"), physFiber);
#elif  0
    // Rough Surface between core and inner cladding
    fiber.SetOpticalSurface(nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume("/nEDM/WLSFiber/Core"), nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume("/nEDM/WLSFiber/InnerCladding"));
#endif
    
    
    ///////////////
    //Detectors
    ///////////////
    
    nEDMSimplePhotDetConstructor& detector = Get<nEDMSimplePhotDetConstructor>("PhotDet");
    detector.SetCheckOverlaps(fCheckOverlaps);
    G4LogicalVolume* logicDet = detector.GetPiece();
    
    
    G4double detZPos = fiber.GetLength()/2.+detector.GetThickness()/2.;
    G4VPhysicalVolume* physDet1 = new G4PVPlacement(0, G4ThreeVector(0.,0.,detZPos),logicDet,detector.GetName(),logicAirBox,false,0,fCheckOverlaps);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(detector.GetName(),physDet1,1);
    
    
    G4VPhysicalVolume* physDet2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-detZPos),logicDet,detector.GetName(),logicAirBox,false,0,fCheckOverlaps);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(detector.GetName(),physDet2,2);
    
}

void nEDMWorldConstructor::ConstructCellPlate()
{
    ///////////////////////////////
    //Air Box for Plate and Fibers
    ///////////////////////////////
    
    G4LogicalVolume* logicAirBox = new G4LogicalVolume(new G4Box(GetName()+"/AirBox",
                                                                 fWidth/2.1,
                                                                 fHeight/2.1,
                                                                 fWidth/2.1),
                                                       FindMaterial("G4_AIR"),
                                                       GetName()+"/AirBox");
    
    G4VPhysicalVolume* physAirBox = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicAirBox,GetName()+"/AirBox",fLogicHall,false,0,fCheckOverlaps);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(GetName()+"/AirBox",physAirBox,0);
    
    G4VisAttributes* airBoxAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    airBoxAtt->SetVisibility(true);
    airBoxAtt->SetForceWireframe(true);
    logicAirBox->SetVisAttributes(airBoxAtt);
    
    ///////////////
    //Single Plate
    ///////////////
    
    nEDMCellSideConstructor& cellSide = Get<nEDMCellSideConstructor>("CellSide");
    cellSide.SetCheckOverlaps(fCheckOverlaps);
    
    cellSide.SetEmbedded_Fibers(false);
    cellSide.SetFiber_Reflector(false);
    cellSide.SetNum_Fibers(98);
    
    
    G4LogicalVolume* logicPlate = cellSide.GetPiece();
    
    G4VPhysicalVolume* physPlate = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicPlate,cellSide.GetName(),logicAirBox,false,0,fCheckOverlaps);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide.GetName(), physPlate);
    
    if (cellSide.GetNumFib()>0 && !cellSide.GetEmbedded_fibers()) {
        G4VPhysicalVolume* physFiber[1000] = {0};
        G4VPhysicalVolume* physDetector1[1000] = {0};
        G4VPhysicalVolume* physDetector2[1000] = {0};

        nEDMWLSFiberConstructor& fiber = Get<nEDMWLSFiberConstructor>("WLSFiber");
        fiber.SetCheckOverlaps(fCheckOverlaps);
        G4LogicalVolume* logicFiber = fiber.GetPiece();
        
        nEDMSimplePhotDetConstructor& detector1 = Get<nEDMSimplePhotDetConstructor>("PhotDet1");
        detector1.SetCheckOverlaps(fCheckOverlaps);
        G4LogicalVolume* logicDet1 = detector1.GetPiece();

        nEDMSimplePhotDetConstructor& detector2 = Get<nEDMSimplePhotDetConstructor>("PhotDet2");
        detector2.SetCheckOverlaps(fCheckOverlaps);
        G4LogicalVolume* logicDet2 = detector2.GetPiece();

        G4double FibXPos, FibYPos;
        G4double CellThickness = cellSide.GetThickness();
        G4double FibThickness = fiber.GetFiberThickness();
        G4int NumFibers = cellSide.GetNumFib();
        G4double FiberSpacing = cellSide.GetFibSpacing();
        G4double detZPos;
        
        FibYPos = -1*(CellThickness/2+FibThickness/2+.005);
        
        detZPos = fiber.GetLength()/2.+detector1.GetThickness()/2.;

        for(G4int i=0;i<NumFibers;i++){
            
            // Place Fibers
            FibXPos=-(FiberSpacing)*(NumFibers-1)*0.5 + i*FiberSpacing;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),logicFiber,fiber.GetName(),logicAirBox,false,0,fCheckOverlaps);
            
            fiber.SetOpticalSurface(physFiber[i], nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName()));
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide.GetName()+"/"+fiber.GetLocalName(),physFiber[i],i);
            
            // Place +Z detectors
            physDetector1[i] = new G4PVPlacement(0, G4ThreeVector(FibXPos,FibYPos,detZPos),logicDet1,detector1.GetName(),logicAirBox,false,0,fCheckOverlaps);
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide.GetName()+"/"+detector1.GetLocalName(),physDetector1[i],i);
            
            // Place -Z detectors
            physDetector2[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,-detZPos),logicDet2,detector2.GetName(),logicAirBox,false,0,fCheckOverlaps);
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide.GetName()+"/"+detector2.GetLocalName(),physDetector2[i],i);

            
        }
        
    }

    
}

void nEDMWorldConstructor::ConstructLHE(){
    
    // Test Stand Configuration
    
    nEDMLHeConstructor& lHe = Get<nEDMLHeConstructor>("LHE");
    lHe.SetCheckOverlaps(fCheckOverlaps);
    G4LogicalVolume* logicTestStand = lHe.GetPiece();
    
    G4VPhysicalVolume* physTestStand = new G4PVPlacement(0,                     // rotation
                                                         G4ThreeVector(0,0,0),  // position
                                                         logicTestStand,        // logical volume
                                                         lHe.GetName(),   // name
                                                         fLogicHall,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,                    // not a copy
                                                         fCheckOverlaps);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(lHe.GetName(),physTestStand,0);
    
    
/*
    //////////////////////////
    // Cell Side 1
    //////////////////////////
    
    nEDMCellSideConstructor& cellSide1 = Get<nEDMCellSideConstructor>("CellSide1");
    cellSide1.SetCheckOverlaps(fCheckOverlaps);
    G4LogicalVolume* logicCellSide1 = cellSide1.GetPiece();
    
    G4VPhysicalVolume* physCellSide1 = new G4PVPlacement(0,                     // rotation
                                                         G4ThreeVector(0,0,0),  // position
                                                         logicCellSide1,        // logical volume
                                                         cellSide1.GetName(),   // name
                                                         logicTestStand,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide1.GetName(),physCellSide1,0);
    
    //////////////////////////
    // External Fibers Side 1
    //////////////////////////
    
    if (cellSide1.GetNumFib()>0 && !cellSide1.GetEmbedded_fibers()) {
        G4VPhysicalVolume* physFiber[1000] = {0};
        
        nEDMWLSFiberConstructor& fiber = Get<nEDMWLSFiberConstructor>("WLSFiber");
        fiber.SetCheckOverlaps(fCheckOverlaps);
        G4LogicalVolume* logicFiber = fiber.GetPiece();
        
        G4double FibXPos, FibYPos;
        G4double CellThickness = cellSide1.GetThickness();
        G4double FibThickness = fiber.GetFiberThickness();
        G4int NumFibers = cellSide1.GetNumFib();
        G4double FiberSpacing = cellSide1.GetFibSpacing();
        
        
        FibYPos = -1*(CellThickness/2+FibThickness/2+.005);
        
        for(G4int i=0;i<NumFibers;i++){
            FibXPos=-(FiberSpacing)*(NumFibers-1)*0.5 + i*FiberSpacing;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),logicFiber,fiber.GetName(),logicTestStand,false,0,fCheckOverlaps);
            
            fiber.SetOpticalSurface(physFiber[i], nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName()));
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide1.GetName()+"/"+fiber.GetLocalName(),physFiber[i],i);
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
                                                         logicTestStand,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide2.GetName(),physCellSide2,0);
    
    //////////////////////////
    // External Fibers Side 1
    //////////////////////////
    
    if (cellSide2.GetNumFib()>0 && !cellSide2.GetEmbedded_fibers()) {
        G4VPhysicalVolume* physFiber[1000] = {0};
        
        nEDMWLSFiberConstructor& fiber = Get<nEDMWLSFiberConstructor>("WLSFiber");
        fiber.SetCheckOverlaps(fCheckOverlaps);
        G4LogicalVolume* logicFiber = fiber.GetPiece();
        
        G4double FibXPos, FibYPos;
        G4double CellThickness = cellSide2.GetThickness();
        G4double FibThickness = fiber.GetFiberThickness();
        G4int NumFibers = cellSide2.GetNumFib();
        G4double FiberSpacing = cellSide2.GetFibSpacing();
        
        
        FibXPos = cell2pos.x() + 1*(CellThickness/2+FibThickness/2+.005);
        
        for(G4int i=0;i<NumFibers;i++){
            FibYPos=cell2pos.y()-(FiberSpacing)*(NumFibers-1)*0.5 + i*FiberSpacing;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),logicFiber,fiber.GetName(),logicTestStand,false,0,fCheckOverlaps);
            
            fiber.SetOpticalSurface(physFiber[i], nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName()));
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide2.GetName()+"/"+fiber.GetLocalName(),physFiber[i],i);
        }
        
    }
    
    
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
                                                         logicTestStand,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide3.GetName(),physCellSide3,0);
    
    
    //////////////////////////
    // External Fibers Side 1
    //////////////////////////
    
    
    if (cellSide3.GetNumFib()>0 && !cellSide3.GetEmbedded_fibers()) {
        G4VPhysicalVolume* physFiber[1000] = {0};
        
        nEDMWLSFiberConstructor& fiber = Get<nEDMWLSFiberConstructor>("WLSFiber");
        fiber.SetCheckOverlaps(fCheckOverlaps);
        G4LogicalVolume* logicFiber = fiber.GetPiece();
        
        G4double FibXPos, FibYPos;
        G4double CellThickness = cellSide3.GetThickness();
        G4double FibThickness = fiber.GetFiberThickness();
        G4int NumFibers = cellSide3.GetNumFib();
        G4double FiberSpacing = cellSide3.GetFibSpacing();
        
        
        FibXPos = cell3pos.x() - 1*(CellThickness/2+FibThickness/2+.005);
        
        for(G4int i=0;i<NumFibers;i++){
            FibYPos=cell3pos.y()-(FiberSpacing)*(NumFibers-1)*0.5 + i*FiberSpacing;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXPos,FibYPos,0.),logicFiber,fiber.GetName(),logicTestStand,false,0,fCheckOverlaps);
            
            fiber.SetOpticalSurface(physFiber[i], nEDMSimplePhysVolManager::GetInstance()->GetPhysicalVolume(GetName()));
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide3.GetName()+"/"+fiber.GetLocalName(),physFiber[i],i);
        }
        
    }
    
 */
}
