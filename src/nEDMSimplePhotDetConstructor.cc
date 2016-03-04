#include "nEDMSimplePhotDetConstructor.hh"

nEDMSimplePhotDetConstructor::~nEDMSimplePhotDetConstructor(){;}

void nEDMSimplePhotDetConstructor::Init(){
    
    fXWidth = 0.100*cm;
    fYWidth = 0.100*cm;
    fThickness = .1*mm;
}

G4LogicalVolume* nEDMSimplePhotDetConstructor::GetPiece(void)
    {
        G4cout << "//################################################//" << G4endl;
        G4cout << "//########### Simple Photon Detector #############//" << G4endl;
        G4cout << "//################################################//" << G4endl;
    
        // Long Strip Detector
        G4Box* photDet_Solid = new G4Box(GetName(),
                                         fXWidth/2.,
                                         fYWidth/2.,
                                         fThickness/2.);
        
        G4LogicalVolume* photonDet_log = new G4LogicalVolume(photDet_Solid,FindMaterial("PMMA"),GetName());
                
        G4VisAttributes* DetVis=new G4VisAttributes(G4Color(1.0,0.0,0.0));
        DetVis->SetVisibility(true);
        DetVis->SetForceWireframe(true);
        photonDet_log->SetVisAttributes(DetVis);
        
        
        // Back Face to block stray photons
        G4String backFaceName = GetName() + "/BackFace";
        
        G4Box* backFace_solid = new G4Box(backFaceName,
                                              fXWidth/2.,
                                              fYWidth/2.,
                                              fThickness*.99/2);
        
        G4LogicalVolume* backFace_log = new G4LogicalVolume(backFace_solid,FindMaterial("PMMA"),backFaceName);
        
        G4ThreeVector backFace_pos = G4ThreeVector(0.,0.,.05*fThickness);
        
        G4VPhysicalVolume* backFace_phys = new G4PVPlacement(0,backFace_pos,
                                                                 backFace_log,
                                                                 backFaceName,
                                                                 photonDet_log,false,0,fCheckOverlaps);
        
        G4VisAttributes* faceVis=new G4VisAttributes(G4Color(1.0,1.0,0.0));
        faceVis->SetVisibility(true);
        backFace_log->SetVisAttributes(faceVis);
        
        nEDMSimplePhysVolManager::GetInstance()->AddPhysicalVolume(backFaceName,backFace_phys);
        
        return photonDet_log;



    }
