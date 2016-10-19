#include "LightCollectionSteppingAction.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "LightCollectionEventInformation.hh"

#include "LightCollectionAnalysis.hh"
#include "nEDMDetectorConstruction.hh"
#include "LightCollectionEventAction.hh"

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"

#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "G4PhysicsTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4ParticleDefinition.hh"

#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4TrackingManager.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpProcessSubType.hh"

#include "G4TrackStatus.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
using namespace std;

LightCollectionSteppingAction::LightCollectionSteppingAction(){
    

    
}

LightCollectionSteppingAction::~LightCollectionSteppingAction(){;}


/*! Called when the simulation steps forward. A lot is happening in this method and it is worth taking a look at. This handles detection of photons.
 */
void LightCollectionSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    

    
    
    // Kill tracks at first step for analyzing input
#if 0
    aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    return;
#endif

    
    if(detectorTypes.size()==0){    detectorTypes = nEDMSimplePhysVolManager::GetInstance()->GetDetectorTypes();
    }
    
//    G4StepPoint* thePrePoint  = aStep->GetPreStepPoint();
    G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
    
    
    // Ignore steps at world boundary
    if (thePostPoint->GetStepStatus()!= fWorldBoundary) {
    
//        G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
        G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
        
//        G4String thePrePVname  = " ";
        G4String thePostPVname = " ";
        
        if (thePostPV) {
//            thePrePVname  = thePrePV->GetName();
            thePostPVname = thePostPV->GetName();
        }
  
        //G4cout << "Material:  " << thePostPV->GetLogicalVolume()->GetMaterial()->GetName() << G4endl;
        
        // Check for any optical surface crossings and print the name of the surface
#if 0
        G4LogicalSurface* surface = NULL;
        surface = G4LogicalBorderSurface::GetSurface(thePrePoint->GetPhysicalVolume(),
                                           thePostPoint->GetPhysicalVolume());

        if (surface) {
            G4cout << "Surface: " << surface->GetName() << G4endl;
        }
#endif
        

// Analysis code for single fiber
#if 0
        if (thePostPVname == "/nEDM/PhotDet_1") {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4double cosTheta = thePrePoint->GetMomentumDirection().z();
            
            analysisManager->FillH1(0, 6);
            analysisManager->FillH1(3, cosTheta);
            analysisManager->FillH1(6, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
            analysisManager->FillH1(7, thePrePoint->GetKineticEnergy()/eV);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);

        }
        else if(thePostPVname == "/nEDM/PhotDet_2") {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4double cosTheta = thePrePoint->GetMomentumDirection().z();
            
            analysisManager->FillH1(0, 7);
            analysisManager->FillH1(4, cosTheta);
            analysisManager->FillH1(6, h_Planck*c_light/thePrePoint->GetKineticEnergy()/nm);
            analysisManager->FillH1(7, thePrePoint->GetKineticEnergy()/eV);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);

        }
#endif


//Analysis code for 3 cell plates
#if 1
  /*
        for (std::vector<detectorVector*>::iterator it=detectorTypes.begin(); it !=detectorTypes.end(); it++) {
            
            std::vector<G4VPhysicalVolume*>::iterator jt;
            
            jt = find((*it)->begin(), (*it)->end(), thePostPV);
            if (jt !=(*it)->end()){
                detType = distance(detectorTypes.begin(), it)+1;
                break;}
            else {detType=0;}
        }
   */

        if (thePostPVname.contains("PhotDet")){
        
            if (thePostPVname.contains("BackFace")){
                //G4cout << "Killed in BackFace" << G4endl;
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (thePostPVname.contains( "/nEDM/LHE/CellSide1/PhotDet1_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 5);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_0") {
                    analysisManager->FillH1(8, 0);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_1"){
                    analysisManager->FillH1(8, 1);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_2"){
                    analysisManager->FillH1(8, 2);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_3"){
                    analysisManager->FillH1(8, 3);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_4"){
                    analysisManager->FillH1(8, 4);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_5"){
                    analysisManager->FillH1(8, 5);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_6"){
                    analysisManager->FillH1(8, 6);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_7"){
                    analysisManager->FillH1(8, 7);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_8"){
                    analysisManager->FillH1(8, 8);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_9"){
                    analysisManager->FillH1(8, 9);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_10"){
                    analysisManager->FillH1(8, 10);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_11"){
                    analysisManager->FillH1(8, 11);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_12"){
                    analysisManager->FillH1(8, 12);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_13"){
                    analysisManager->FillH1(8, 13);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_14"){
                    analysisManager->FillH1(8, 14);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_15"){
                    analysisManager->FillH1(8, 15);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_16"){
                    analysisManager->FillH1(8, 16);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_17"){
                    analysisManager->FillH1(8, 17);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_18"){
                    analysisManager->FillH1(8, 18);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_19"){
                    analysisManager->FillH1(8, 19);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_20"){
                    analysisManager->FillH1(8, 20);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_21"){
                    analysisManager->FillH1(8, 21);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_22"){
                    analysisManager->FillH1(8, 22);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_23"){
                    analysisManager->FillH1(8, 23);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_24"){
                    analysisManager->FillH1(8, 24);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_25"){
                    analysisManager->FillH1(8, 25);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_26"){
                    analysisManager->FillH1(8, 26);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_27"){
                    analysisManager->FillH1(8, 27);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_28"){
                    analysisManager->FillH1(8, 28);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_29"){
                    analysisManager->FillH1(8, 29);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_30"){
                    analysisManager->FillH1(8, 30);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_31"){
                    analysisManager->FillH1(8, 31);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_32"){
                    analysisManager->FillH1(8, 32);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_33"){
                    analysisManager->FillH1(8, 33);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_34"){
                    analysisManager->FillH1(8, 34);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_35"){
                    analysisManager->FillH1(8, 35);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_36"){
                    analysisManager->FillH1(8, 36);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_37"){
                    analysisManager->FillH1(8, 37);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_38"){
                    analysisManager->FillH1(8, 38);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_39"){
                    analysisManager->FillH1(8, 39);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_40"){
                    analysisManager->FillH1(8, 40);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_41"){
                    analysisManager->FillH1(8, 41);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_42"){
                    analysisManager->FillH1(8, 42);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_43"){
                    analysisManager->FillH1(8, 43);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_44"){
                    analysisManager->FillH1(8, 44);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_45"){
                    analysisManager->FillH1(8, 45);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_46"){
                    analysisManager->FillH1(8, 46);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_47"){
                    analysisManager->FillH1(8, 47);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_48"){
                    analysisManager->FillH1(8, 48);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_49"){
                    analysisManager->FillH1(8, 49);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_50"){
                    analysisManager->FillH1(8, 50);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_51"){
                    analysisManager->FillH1(8, 51);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_52"){
                    analysisManager->FillH1(8, 52);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_53"){
                    analysisManager->FillH1(8, 53);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_54"){
                    analysisManager->FillH1(8, 54);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_55"){
                    analysisManager->FillH1(8, 55);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_56"){
                    analysisManager->FillH1(8, 56);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_57"){
                    analysisManager->FillH1(8, 57);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_58"){
                    analysisManager->FillH1(8, 58);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_59"){
                    analysisManager->FillH1(8, 59);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_60"){
                    analysisManager->FillH1(8, 60);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_61"){
                    analysisManager->FillH1(8, 61);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_62"){
                    analysisManager->FillH1(8, 62);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_63"){
                    analysisManager->FillH1(8, 63);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_64"){
                    analysisManager->FillH1(8, 64);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_65"){
                    analysisManager->FillH1(8, 65);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_66"){
                    analysisManager->FillH1(8, 66);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_67"){
                    analysisManager->FillH1(8, 67);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_68"){
                    analysisManager->FillH1(8, 68);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_69"){
                    analysisManager->FillH1(8, 69);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_70"){
                    analysisManager->FillH1(8, 70);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_71"){
                    analysisManager->FillH1(8, 71);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_72"){
                    analysisManager->FillH1(8, 72);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_73"){
                    analysisManager->FillH1(8, 73);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_74"){
                    analysisManager->FillH1(8, 74);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_75"){
                    analysisManager->FillH1(8, 75);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_76"){
                    analysisManager->FillH1(8, 76);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_77"){
                    analysisManager->FillH1(8, 77);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_78"){
                    analysisManager->FillH1(8, 78);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_79"){
                    analysisManager->FillH1(8, 79);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_80"){
                    analysisManager->FillH1(8, 80);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_81"){
                    analysisManager->FillH1(8, 81);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_82"){
                    analysisManager->FillH1(8, 82);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_83"){
                    analysisManager->FillH1(8, 83);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_84"){
                    analysisManager->FillH1(8, 84);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_85"){
                    analysisManager->FillH1(8, 85);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_86"){
                    analysisManager->FillH1(8, 86);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_87"){
                    analysisManager->FillH1(8, 87);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_88"){
                    analysisManager->FillH1(8, 88);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_89"){
                    analysisManager->FillH1(8, 89);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_90"){
                    analysisManager->FillH1(8, 90);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_91"){
                    analysisManager->FillH1(8, 91);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_92"){
                    analysisManager->FillH1(8, 92);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_93"){
                    analysisManager->FillH1(8, 93);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_94"){
                    analysisManager->FillH1(8, 94);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_95"){
                    analysisManager->FillH1(8, 95);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_96"){
                    analysisManager->FillH1(8, 96);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_97"){
                    analysisManager->FillH1(8, 97);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_98"){
                    analysisManager->FillH1(8, 98);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet1_99"){
                    analysisManager->FillH1(8, 99);
                }
            }
            else if(thePostPVname.contains("/nEDM/LHE/CellSide1/PhotDet2_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 6);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_0") {
                    analysisManager->FillH1(8, 0);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_1"){
                    analysisManager->FillH1(8, 1);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_2"){
                    analysisManager->FillH1(8, 2);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_3"){
                    analysisManager->FillH1(8, 3);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_4"){
                    analysisManager->FillH1(8, 4);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_5"){
                    analysisManager->FillH1(8, 5);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_6"){
                    analysisManager->FillH1(8, 6);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_7"){
                    analysisManager->FillH1(8, 7);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_8"){
                    analysisManager->FillH1(8, 8);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_9"){
                    analysisManager->FillH1(8, 9);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_10"){
                    analysisManager->FillH1(8, 10);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_11"){
                    analysisManager->FillH1(8, 11);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_12"){
                    analysisManager->FillH1(8, 12);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_13"){
                    analysisManager->FillH1(8, 13);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_14"){
                    analysisManager->FillH1(8, 14);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_15"){
                    analysisManager->FillH1(8, 15);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_16"){
                    analysisManager->FillH1(8, 16);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_17"){
                    analysisManager->FillH1(8, 17);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_18"){
                    analysisManager->FillH1(8, 18);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_19"){
                    analysisManager->FillH1(8, 19);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_20"){
                    analysisManager->FillH1(8, 20);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_21"){
                    analysisManager->FillH1(8, 21);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_22"){
                    analysisManager->FillH1(8, 22);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_23"){
                    analysisManager->FillH1(8, 23);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_24"){
                    analysisManager->FillH1(8, 24);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_25"){
                    analysisManager->FillH1(8, 25);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_26"){
                    analysisManager->FillH1(8, 26);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_27"){
                    analysisManager->FillH1(8, 27);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_28"){
                    analysisManager->FillH1(8, 28);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_29"){
                    analysisManager->FillH1(8, 29);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_30"){
                    analysisManager->FillH1(8, 30);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_31"){
                    analysisManager->FillH1(8, 31);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_32"){
                    analysisManager->FillH1(8, 32);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_33"){
                    analysisManager->FillH1(8, 33);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_34"){
                    analysisManager->FillH1(8, 34);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_35"){
                    analysisManager->FillH1(8, 35);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_36"){
                    analysisManager->FillH1(8, 36);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_37"){
                    analysisManager->FillH1(8, 37);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_38"){
                    analysisManager->FillH1(8, 38);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_39"){
                    analysisManager->FillH1(8, 39);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_40"){
                    analysisManager->FillH1(8, 40);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_41"){
                    analysisManager->FillH1(8, 41);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_42"){
                    analysisManager->FillH1(8, 42);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_43"){
                    analysisManager->FillH1(8, 43);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_44"){
                    analysisManager->FillH1(8, 44);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_45"){
                    analysisManager->FillH1(8, 45);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_46"){
                    analysisManager->FillH1(8, 46);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_47"){
                    analysisManager->FillH1(8, 47);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_48"){
                    analysisManager->FillH1(8, 48);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_49"){
                    analysisManager->FillH1(8, 49);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_50"){
                    analysisManager->FillH1(8, 50);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_51"){
                    analysisManager->FillH1(8, 51);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_52"){
                    analysisManager->FillH1(8, 52);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_53"){
                    analysisManager->FillH1(8, 53);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_54"){
                    analysisManager->FillH1(8, 54);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_55"){
                    analysisManager->FillH1(8, 55);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_56"){
                    analysisManager->FillH1(8, 56);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_57"){
                    analysisManager->FillH1(8, 57);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_58"){
                    analysisManager->FillH1(8, 58);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_59"){
                    analysisManager->FillH1(8, 59);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_60"){
                    analysisManager->FillH1(8, 60);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_61"){
                    analysisManager->FillH1(8, 61);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_62"){
                    analysisManager->FillH1(8, 62);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_63"){
                    analysisManager->FillH1(8, 63);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_64"){
                    analysisManager->FillH1(8, 64);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_65"){
                    analysisManager->FillH1(8, 65);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_66"){
                    analysisManager->FillH1(8, 66);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_67"){
                    analysisManager->FillH1(8, 67);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_68"){
                    analysisManager->FillH1(8, 68);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_69"){
                    analysisManager->FillH1(8, 69);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_70"){
                    analysisManager->FillH1(8, 70);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_71"){
                    analysisManager->FillH1(8, 71);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_72"){
                    analysisManager->FillH1(8, 72);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_73"){
                    analysisManager->FillH1(8, 73);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_74"){
                    analysisManager->FillH1(8, 74);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_75"){
                    analysisManager->FillH1(8, 75);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_76"){
                    analysisManager->FillH1(8, 76);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_77"){
                    analysisManager->FillH1(8, 77);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_78"){
                    analysisManager->FillH1(8, 78);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_79"){
                    analysisManager->FillH1(8, 79);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_80"){
                    analysisManager->FillH1(8, 80);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_81"){
                    analysisManager->FillH1(8, 81);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_82"){
                    analysisManager->FillH1(8, 82);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_83"){
                    analysisManager->FillH1(8, 83);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_84"){
                    analysisManager->FillH1(8, 84);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_85"){
                    analysisManager->FillH1(8, 85);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_86"){
                    analysisManager->FillH1(8, 86);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_87"){
                    analysisManager->FillH1(8, 87);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_88"){
                    analysisManager->FillH1(8, 88);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_89"){
                    analysisManager->FillH1(8, 89);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_90"){
                    analysisManager->FillH1(8, 90);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_91"){
                    analysisManager->FillH1(8, 91);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_92"){
                    analysisManager->FillH1(8, 92);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_93"){
                    analysisManager->FillH1(8, 93);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_94"){
                    analysisManager->FillH1(8, 94);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_95"){
                    analysisManager->FillH1(8, 95);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_96"){
                    analysisManager->FillH1(8, 96);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_97"){
                    analysisManager->FillH1(8, 97);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_98"){
                    analysisManager->FillH1(8, 98);
                }
                else if (thePostPVname== "/nEDM/LHE/CellSide1/PhotDet2_99"){
                    analysisManager->FillH1(8, 99);
                }

            }
            else if(thePostPVname.contains("/nEDM/LHE/CellSide2/PhotDet1_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 7);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if(thePostPVname.contains("/nEDM/LHE/CellSide2/PhotDet2_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 8);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if(thePostPVname.contains("/nEDM/LHE/CellSide3/PhotDet1_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 9);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if(thePostPVname.contains("/nEDM/LHE/CellSide3/PhotDet2_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 10);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
        }

//        
//        if (thePostPVname.contains("BackFace")){
//            //G4cout << "Killed in BackFace" << G4endl;
//            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
//        }

        //  New Approach
        
//        for (std::vector<detectorVector*>::iterator it=detectorTypes.begin(); it !=detectorTypes.end(); it++) {
//
//            std::vector<G4VPhysicalVolume*>::iterator jt;
//
//            jt = find((*it)->begin(), (*it)->end(), thePostPoint->GetPhysicalVolume());
//            if (jt !=(*it)->end()){
////                detType = distance(detectorTypes.begin(), it)+1;
//                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
//                analysisManager->FillH1(0, distance(detectorTypes.begin(), it)+5);
//                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
//                break;
//            }
//            else {detType=0;}
//        }

        //if (detType!=0) {G4cout << "Detector Type:  " << detType << G4endl;}
  
//        if (detType!=0){

 /*
            if (detType==1) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 5);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (detType==2) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 6);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (detType==3) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 7);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (detType==4) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 8);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (detType==5) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 9);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (detType==6) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 10);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else {G4cout << "DetType out of range: " << detType << G4endl;
            }
  */
//        }
  

#endif
  
        
    }

}

#if 0
void LightCollectionSteppingAction::OldSteppingActionCode(const G4Step* aStep){
    
    
    // Kill tracks at first step for analyzing input
#if 0
    aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    return;
#endif
    
    
    // get point of entry and exit
    const G4StepPoint *p_in  = aStep->GetPreStepPoint();
    const G4StepPoint *p_out = aStep->GetPostStepPoint();
    //const G4VProcess *p1 = p_in->GetProcessDefinedStep();
    //const G4VProcess *p2 = p_out->GetProcessDefinedStep();
    
    
    // Find boundary status
    
    G4OpBoundaryProcessStatus boundaryStatus=Undefined;
    static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;
    
    G4ParticleDefinition* particleType = aStep->GetTrack()->GetDefinition();
    
    if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
        
        if(!boundary){
            G4ProcessManager* pm
            = aStep->GetTrack()->GetDefinition()->GetProcessManager();
            G4int nprocesses = pm->GetProcessListLength();
            G4ProcessVector* pv = pm->GetProcessList();
            G4int i;
            for( i=0;i<nprocesses;i++){
                if((*pv)[i]->GetProcessName()=="OpBoundary"){
                    boundary = (G4OpBoundaryProcess*)(*pv)[i];
                    break;
                }
            }
        }
        
        boundaryStatus = boundary->GetStatus();
        
        G4LogicalVolume* aVol;
        G4LogicalSurface* surface = NULL;
        G4StepStatus stepStatus = p_out->GetStepStatus();
        
        switch( stepStatus )
        {
                
            case fGeomBoundary:
                
                switch( boundaryStatus )
            {
                case Detection:
                    if( p_out->GetMaterial() ) {
                        aVol = p_out->GetTouchableHandle()
                        ->GetVolume()->GetLogicalVolume();
                        
                        
                        if( aVol->GetName() == "PMT" ) {
                            G4int copy_no = p_out->GetPhysicalVolume()->GetCopyNo();
                            
                            G4Event* currentEvent = G4EventManager::GetEventManager()->GetNonconstCurrentEvent();
                            LightCollectionEventInformation* eventInfo = (LightCollectionEventInformation*)currentEvent->GetUserInformation();
                            
                            eventInfo->Detected(copy_no+1, aStep->GetTrack()->GetTotalEnergy());
                            
                            /*
                             trackInfo->set_detected(true);
                             trackInfo->set_pmt_number(copy_no+1);
                             trackInfo->set_time_detected(aStep->GetTrack()->GetGlobalTime());
                             */
                        }
                    }
                    break;
                    
                default:
                    break;
            }
                break;
                
            default:
                break;
        }
    }
}
#endif



