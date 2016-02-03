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
        
        return photonDet_log;
    }
