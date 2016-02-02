#include "nEDMWorldConstructor.hh"

#include "nEDMCellSideConstructor.hh"
#include "nEDMWLSFiberConstructor.hh"

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

  fWidth  = 2*5.*m;
  fHeight = fWidth;
  fLength = fWidth; 

  AddConstructor(new nEDMCellSideConstructor("CellSide",this));
    AddConstructor(new nEDMWLSFiberConstructor("WLSFiber",this));
  
}

nEDMWorldConstructor::~nEDMWorldConstructor(){;}

G4LogicalVolume* nEDMWorldConstructor::GetPiece(void)
{  
  G4cout << "//******************************************************//" << G4endl;
  G4cout << "//********* CONSTRUCT THE HALL -> The World ************//" << G4endl;
  G4cout << "//******************************************************//" << G4endl;

  G4LogicalVolume* logicHall = new G4LogicalVolume(new G4Box(GetName(),
                                                             fWidth/2., 
                                                             fHeight/2., 
                                                             fWidth/2.), 
                                                   FindMaterial("G4_AIR"),
                                                   GetName());

  //logicHall->SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes* hallAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  hallAtt->SetVisibility(true);
  hallAtt->SetForceWireframe(true);
  logicHall->SetVisAttributes(hallAtt);



    
    nEDMCellSideConstructor& cellSide = Get<nEDMCellSideConstructor>("CellSide");
    cellSide.SetCheckOverlaps(fCheckOverlaps);
    G4LogicalVolume* logicCellSide = cellSide.GetPiece();
    
    G4VPhysicalVolume* physCellSide1 = new G4PVPlacement(0,                     // rotation
                                                        G4ThreeVector(0,0,0),  // position
                                                        logicCellSide,        // logical volume
                                                        cellSide.GetName(),   // name
                                                        logicHall,             // mother volume
                                                        false,                 // no boolean operations
                                                        0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide.GetName(),physCellSide1,1);
    
    
    // Not Embedded Fibers

    if (!cellSide.GetEmbedded_fibers()) {
        G4VPhysicalVolume* physFiber[1000] = {0};
        
        nEDMWLSFiberConstructor& fiber = Get<nEDMWLSFiberConstructor>("WLSFiber");
        fiber.SetCheckOverlaps(fCheckOverlaps);
        G4LogicalVolume* logicFiber = fiber.GetPiece();
        
        G4double FibXpos, FibYPos;
        G4double CellThickness = cellSide.GetThickness();
        G4double FibThickness = fiber.GetFiberThickness();
        G4int NumFibers = cellSide.GetNumFib();
        G4double FiberSpacing = cellSide.GetFibSpacing();
        
        
        FibYPos = -1*(CellThickness/2+FibThickness/2+.005);
        
        for(G4int i=0;i<NumFibers;i++){
            FibXpos=-(FiberSpacing)*(NumFibers-1)*0.5 + i*FiberSpacing;
            
            physFiber[i] = new G4PVPlacement(0,G4ThreeVector(FibXpos,FibYPos,0.),logicFiber,fiber.GetName(),logicHall,false,0,fCheckOverlaps);
            
            nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fiber.GetName(),physFiber[i],i);
        }

    }

    
    
    G4ThreeVector cell2pos = G4ThreeVector(cellSide.GetWidth()*0.6,cellSide.GetWidth()*0.6,0);
    G4RotationMatrix* cell2rotm = new G4RotationMatrix();
    cell2rotm->rotateZ(-90.*deg);

    G4VPhysicalVolume* physCellSide2 = new G4PVPlacement(cell2rotm,                     // rotation
                                                         cell2pos,  // position
                                                         logicCellSide,        // logical volume
                                                         cellSide.GetName(),   // name
                                                         logicHall,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide.GetName(),physCellSide2,2);


    G4ThreeVector cell3pos = G4ThreeVector(-1*cellSide.GetWidth()*0.6,cellSide.GetWidth()*0.6,0);
    G4RotationMatrix* cell3rotm = new G4RotationMatrix();
    cell3rotm->rotateZ(90.*deg);
    
    G4VPhysicalVolume* physCellSide3 = new G4PVPlacement(cell3rotm,                     // rotation
                                                         cell3pos,  // position
                                                         logicCellSide,        // logical volume
                                                         cellSide.GetName(),   // name
                                                         logicHall,             // mother volume
                                                         false,                 // no boolean operations
                                                         0,fCheckOverlaps);                    // not a copy
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(cellSide.GetName(),physCellSide3,3);

  
  return logicHall;  
}
