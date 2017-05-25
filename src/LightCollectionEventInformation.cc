#include "LightCollectionEventInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LightCollectionEventInformation::LightCollectionEventInformation()
{
    detector1 = 0;
    detector2 = 0;
    detector3 = 0;
    TPBWavelength = 0;
}

LightCollectionEventInformation::~LightCollectionEventInformation() {}

void LightCollectionEventInformation::Detected(G4int detector, G4double ene) {
    
    switch (detector) {
        case 1:
            detector1++;
            energy1 = ene;
            break;
            
        case 2:
            detector2++;
            energy2 = ene;
            break;
            
        case 3:
            detector3++;
            energy3 = ene;
            break;
            
        default:
            G4cout << "***** EventInformation::Detected called with invalid detector number.*****" << G4endl;
            break;
    }
}

