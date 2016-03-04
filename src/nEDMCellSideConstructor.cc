#include "nEDMCellSideConstructor.hh"
#include "nEDMWLSFiberConstructor.hh"

#include <G4PVPlacement.hh>

nEDMCellSideConstructor::~nEDMCellSideConstructor(){;}

void nEDMCellSideConstructor::Init(){
    
    SetMaterial("PMMA");
    
    fWidth = 2*5.08*cm;
    fThickness = 2*0.635*cm;
    fLength = 2*20.64*cm;
    
    fTPB_Thickness = 0.001*cm;
    fTPB_outerFraction = 0.01;
    
    fTPB_On = true;
    fEmbedded_fibers = true;
    fFiber_Reflector = false;
    fFiber_spacing = 0.103*cm;
    fNum_fibers = 98;
    //fNum_fibers = 0;
    
    fCellSide_log = NULL;
    
    AddConstructor(new nEDMWLSFiberConstructor("WLSFiber",this));
    
    
}

G4LogicalVolume* nEDMCellSideConstructor::GetPiece(void)
{
    G4cout << "//################################################//" << G4endl;
    G4cout << "//################ Cell Side #####################//" << G4endl;
    G4cout << "//################################################//" << G4endl;
    
    
    G4Box* cellSide_solid = new G4Box(GetName(),
                                      fWidth/2.,
                                      fThickness/2.,
                                      fLength/2.);
    
    fCellSide_log =
    new G4LogicalVolume(cellSide_solid,FindMaterial(GetMaterial()),
                        GetName());
    
    G4VisAttributes* cellVis=new G4VisAttributes(G4Color(0.8,0.8,0.8));
    cellVis->SetVisibility(true);
    fCellSide_log->SetVisAttributes(cellVis);
    
    ConstructTPBInterface();
    if (fNum_fibers > 0 && fEmbedded_fibers) {ConstructEmbeddedFibers();}
    
    return fCellSide_log;
}

void nEDMCellSideConstructor::ConstructTPBInterface(){
    
    G4cout << "//################################################//" << G4endl;
    G4cout << "//################ TPB Interface #################//" << G4endl;
    G4cout << "//################################################//" << G4endl;
    
    G4String TPBInterfaceName = GetName() + "/TPBInterface";
    
    // Create TPB Interface Layer
    G4Box* TPBInterface_solid = new G4Box(TPBInterfaceName,
                                          fWidth/2.,
                                          fTPB_Thickness/2.,
                                          fLength/2.);
    
    G4LogicalVolume* TPBInterface_log = new G4LogicalVolume(TPBInterface_solid,FindMaterial("TPB_inner"),TPBInterfaceName);
    
    G4ThreeVector TPBInterface_pos = G4ThreeVector(0.,(fThickness-fTPB_Thickness)/2.,0.);
    
    G4VPhysicalVolume* TPBInterface_phys = new G4PVPlacement(0,TPBInterface_pos,
                                                             TPBInterface_log,
                                                             "TPBInterface",
                                                             fCellSide_log,false,0,fCheckOverlaps);
    
    G4VisAttributes* tpbVis=new G4VisAttributes(G4Color(1.0,0.0,1.0));
    tpbVis->SetVisibility(true);
    TPBInterface_log->SetVisAttributes(tpbVis);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(TPBInterfaceName,TPBInterface_phys);
    
    
    // Create outer TPB layer with low index of refraction
    
    G4double TPB_outerThickness = fTPB_outerFraction*fTPB_Thickness;
    G4String TPBInterfaceOuterName = GetName() + "/TPBInterface/Outer";
    
    G4Box* TPBInterface_outer_solid = new G4Box(TPBInterfaceOuterName,
                                                fWidth/2,
                                                TPB_outerThickness/2,
                                                fLength/2);
    
    
    G4LogicalVolume* TPBInterface_outer_log = new G4LogicalVolume(TPBInterface_outer_solid,FindMaterial("TPB_outer"),TPBInterfaceOuterName);
    
    G4ThreeVector TPBInterface_outer_pos = G4ThreeVector(0.,(fTPB_Thickness-TPB_outerThickness)/2.,0.);
    
    G4VPhysicalVolume* TPBInterface_outer_phys= new G4PVPlacement(0,TPBInterface_outer_pos,
                                                                  TPBInterface_outer_log,
                                                                  TPBInterfaceOuterName,
                                                                  TPBInterface_log,false,0,fCheckOverlaps);
    
    G4VisAttributes* tpbOuterVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
    tpbOuterVis->SetVisibility(true);
    TPBInterface_outer_log->SetVisAttributes(tpbOuterVis);
    
    nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(TPBInterfaceOuterName,TPBInterface_outer_phys);
    
    
}

void nEDMCellSideConstructor::ConstructEmbeddedFibers(){
    
    nEDMWLSFiberConstructor& fiber = Get<nEDMWLSFiberConstructor>("WLSFiber");
    fiber.SetCheckOverlaps(fCheckOverlaps);
    G4LogicalVolume* logicFiber = fiber.GetPiece();
    
    G4VPhysicalVolume* physFiber[1000] = {0};
    
    G4double X_pos;
    G4double Fiber_Thickness = fiber.GetFiberThickness();
    G4double Y_pos = -1*(fThickness/2.-Fiber_Thickness/2.-0.001*cm);
    
    for(G4int i=0;i<fNum_fibers;i++){
        X_pos=-(fFiber_spacing)*(fNum_fibers-1)*0.5 + i*fFiber_spacing;
        
        physFiber[i] = new G4PVPlacement(0,G4ThreeVector(X_pos,Y_pos,0.),logicFiber,fiber.GetName(),fCellSide_log,false,0,fCheckOverlaps);
        
        nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(fiber.GetName(),physFiber[i],i);
    }
    
    
    
}