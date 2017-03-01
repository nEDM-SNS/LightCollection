#include "LightCollectionSteppingAction.hh"

#include "G4EventManager.hh"
#include "G4Event.hh"
#include "LightCollectionEventInformation.hh"

#include "LightCollectionAnalysis.hh"
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
        // Report on logical surfaces
#if 0
        G4LogicalSurface* surface = NULL;
        surface = G4LogicalBorderSurface::GetSurface(thePrePoint->GetPhysicalVolume(),
                                                     thePostPoint->GetPhysicalVolume());
        if (surface) {
            G4OpticalSurface* optSurf = (G4OpticalSurface*)surface->GetSurfaceProperty();
//            optSurf->DumpInfo();
            G4cout << "Surface: " << surface->GetName() << "  Polish:  "
                << optSurf->GetPolish() << G4endl;
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
        if (thePostPoint->GetProcessDefinedStep()->GetProcessName()=="OpWLS"){
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(9, aStep->GetNumberOfSecondariesInCurrentStep());
        }
        
        if (thePostPVname.contains("fibDet")){
        
            if (thePostPVname.contains("BackFace")){
                //G4cout << "Killed in BackFace" << G4endl;
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
            }
            else if (thePostPVname.contains( "fibDet1_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 7);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);

                if (thePostPVname== "fibDet1_1"){
                    analysisManager->FillH1(8, 1);
                }
                else if (thePostPVname== "fibDet1_2"){
                    analysisManager->FillH1(8, 2);
                }
                else if (thePostPVname== "fibDet1_3"){
                    analysisManager->FillH1(8, 3);
                }
                else if (thePostPVname== "fibDet1_4"){
                    analysisManager->FillH1(8, 4);
                }
                else if (thePostPVname== "fibDet1_5"){
                    analysisManager->FillH1(8, 5);
                }
                else if (thePostPVname== "fibDet1_6"){
                    analysisManager->FillH1(8, 6);
                }
                else if (thePostPVname== "fibDet1_7"){
                    analysisManager->FillH1(8, 7);
                }
                else if (thePostPVname== "fibDet1_8"){
                    analysisManager->FillH1(8, 8);
                }
                else if (thePostPVname== "fibDet1_9"){
                    analysisManager->FillH1(8, 9);
                }
                else if (thePostPVname== "fibDet1_10"){
                    analysisManager->FillH1(8, 10);
                }
                else if (thePostPVname== "fibDet1_11"){
                    analysisManager->FillH1(8, 11);
                }
                else if (thePostPVname== "fibDet1_12"){
                    analysisManager->FillH1(8, 12);
                }
                else if (thePostPVname== "fibDet1_13"){
                    analysisManager->FillH1(8, 13);
                }
                else if (thePostPVname== "fibDet1_14"){
                    analysisManager->FillH1(8, 14);
                }
                else if (thePostPVname== "fibDet1_15"){
                    analysisManager->FillH1(8, 15);
                }
                else if (thePostPVname== "fibDet1_16"){
                    analysisManager->FillH1(8, 16);
                }
                else if (thePostPVname== "fibDet1_17"){
                    analysisManager->FillH1(8, 17);
                }
                else if (thePostPVname== "fibDet1_18"){
                    analysisManager->FillH1(8, 18);
                }
                else if (thePostPVname== "fibDet1_19"){
                    analysisManager->FillH1(8, 19);
                }
                else if (thePostPVname== "fibDet1_20"){
                    analysisManager->FillH1(8, 20);
                }
                else if (thePostPVname== "fibDet1_21"){
                    analysisManager->FillH1(8, 21);
                }
                else if (thePostPVname== "fibDet1_22"){
                    analysisManager->FillH1(8, 22);
                }
                else if (thePostPVname== "fibDet1_23"){
                    analysisManager->FillH1(8, 23);
                }
                else if (thePostPVname== "fibDet1_24"){
                    analysisManager->FillH1(8, 24);
                }
                else if (thePostPVname== "fibDet1_25"){
                    analysisManager->FillH1(8, 25);
                }
                else if (thePostPVname== "fibDet1_26"){
                    analysisManager->FillH1(8, 26);
                }
                else if (thePostPVname== "fibDet1_27"){
                    analysisManager->FillH1(8, 27);
                }
                else if (thePostPVname== "fibDet1_28"){
                    analysisManager->FillH1(8, 28);
                }
                else if (thePostPVname== "fibDet1_29"){
                    analysisManager->FillH1(8, 29);
                }
                else if (thePostPVname== "fibDet1_30"){
                    analysisManager->FillH1(8, 30);
                }
                else if (thePostPVname== "fibDet1_31"){
                    analysisManager->FillH1(8, 31);
                }
                else if (thePostPVname== "fibDet1_32"){
                    analysisManager->FillH1(8, 32);
                }
                else if (thePostPVname== "fibDet1_33"){
                    analysisManager->FillH1(8, 33);
                }
                else if (thePostPVname== "fibDet1_34"){
                    analysisManager->FillH1(8, 34);
                }
                else if (thePostPVname== "fibDet1_35"){
                    analysisManager->FillH1(8, 35);
                }
                else if (thePostPVname== "fibDet1_36"){
                    analysisManager->FillH1(8, 36);
                }
                else if (thePostPVname== "fibDet1_37"){
                    analysisManager->FillH1(8, 37);
                }
                else if (thePostPVname== "fibDet1_38"){
                    analysisManager->FillH1(8, 38);
                }
                else if (thePostPVname== "fibDet1_39"){
                    analysisManager->FillH1(8, 39);
                }
                else if (thePostPVname== "fibDet1_40"){
                    analysisManager->FillH1(8, 40);
                }
                else if (thePostPVname== "fibDet1_41"){
                    analysisManager->FillH1(8, 41);
                }
                else if (thePostPVname== "fibDet1_42"){
                    analysisManager->FillH1(8, 42);
                }
                else if (thePostPVname== "fibDet1_43"){
                    analysisManager->FillH1(8, 43);
                }
                else if (thePostPVname== "fibDet1_44"){
                    analysisManager->FillH1(8, 44);
                }
                else if (thePostPVname== "fibDet1_45"){
                    analysisManager->FillH1(8, 45);
                }
                else if (thePostPVname== "fibDet1_46"){
                    analysisManager->FillH1(8, 46);
                }
                else if (thePostPVname== "fibDet1_47"){
                    analysisManager->FillH1(8, 47);
                }
                else if (thePostPVname== "fibDet1_48"){
                    analysisManager->FillH1(8, 48);
                }
                else if (thePostPVname== "fibDet1_49"){
                    analysisManager->FillH1(8, 49);
                }
                else if (thePostPVname== "fibDet1_50"){
                    analysisManager->FillH1(8, 50);
                }
                else if (thePostPVname== "fibDet1_51"){
                    analysisManager->FillH1(8, 51);
                }
                else if (thePostPVname== "fibDet1_52"){
                    analysisManager->FillH1(8, 52);
                }
                else if (thePostPVname== "fibDet1_53"){
                    analysisManager->FillH1(8, 53);
                }
                else if (thePostPVname== "fibDet1_54"){
                    analysisManager->FillH1(8, 54);
                }
                else if (thePostPVname== "fibDet1_55"){
                    analysisManager->FillH1(8, 55);
                }
                else if (thePostPVname== "fibDet1_56"){
                    analysisManager->FillH1(8, 56);
                }
                else if (thePostPVname== "fibDet1_57"){
                    analysisManager->FillH1(8, 57);
                }
                else if (thePostPVname== "fibDet1_58"){
                    analysisManager->FillH1(8, 58);
                }
                else if (thePostPVname== "fibDet1_59"){
                    analysisManager->FillH1(8, 59);
                }
                else if (thePostPVname== "fibDet1_60"){
                    analysisManager->FillH1(8, 60);
                }
                else if (thePostPVname== "fibDet1_61"){
                    analysisManager->FillH1(8, 61);
                }
                else if (thePostPVname== "fibDet1_62"){
                    analysisManager->FillH1(8, 62);
                }
                else if (thePostPVname== "fibDet1_63"){
                    analysisManager->FillH1(8, 63);
                }
                else if (thePostPVname== "fibDet1_64"){
                    analysisManager->FillH1(8, 64);
                }
                else if (thePostPVname== "fibDet1_65"){
                    analysisManager->FillH1(8, 65);
                }
                else if (thePostPVname== "fibDet1_66"){
                    analysisManager->FillH1(8, 66);
                }
                else if (thePostPVname== "fibDet1_67"){
                    analysisManager->FillH1(8, 67);
                }
                else if (thePostPVname== "fibDet1_68"){
                    analysisManager->FillH1(8, 68);
                }
                else if (thePostPVname== "fibDet1_69"){
                    analysisManager->FillH1(8, 69);
                }
                else if (thePostPVname== "fibDet1_70"){
                    analysisManager->FillH1(8, 70);
                }
                else if (thePostPVname== "fibDet1_71"){
                    analysisManager->FillH1(8, 71);
                }
                else if (thePostPVname== "fibDet1_72"){
                    analysisManager->FillH1(8, 72);
                }
                else if (thePostPVname== "fibDet1_73"){
                    analysisManager->FillH1(8, 73);
                }
                else if (thePostPVname== "fibDet1_74"){
                    analysisManager->FillH1(8, 74);
                }
                else if (thePostPVname== "fibDet1_75"){
                    analysisManager->FillH1(8, 75);
                }
                else if (thePostPVname== "fibDet1_76"){
                    analysisManager->FillH1(8, 76);
                }
                else if (thePostPVname== "fibDet1_77"){
                    analysisManager->FillH1(8, 77);
                }
                else if (thePostPVname== "fibDet1_78"){
                    analysisManager->FillH1(8, 78);
                }
                else if (thePostPVname== "fibDet1_79"){
                    analysisManager->FillH1(8, 79);
                }
                else if (thePostPVname== "fibDet1_80"){
                    analysisManager->FillH1(8, 80);
                }
                else if (thePostPVname== "fibDet1_81"){
                    analysisManager->FillH1(8, 81);
                }
                else if (thePostPVname== "fibDet1_82"){
                    analysisManager->FillH1(8, 82);
                }
                else if (thePostPVname== "fibDet1_83"){
                    analysisManager->FillH1(8, 83);
                }
                else if (thePostPVname== "fibDet1_84"){
                    analysisManager->FillH1(8, 84);
                }
                else if (thePostPVname== "fibDet1_85"){
                    analysisManager->FillH1(8, 85);
                }
                else if (thePostPVname== "fibDet1_86"){
                    analysisManager->FillH1(8, 86);
                }
                else if (thePostPVname== "fibDet1_87"){
                    analysisManager->FillH1(8, 87);
                }
                else if (thePostPVname== "fibDet1_88"){
                    analysisManager->FillH1(8, 88);
                }
                else if (thePostPVname== "fibDet1_89"){
                    analysisManager->FillH1(8, 89);
                }
                else if (thePostPVname== "fibDet1_90"){
                    analysisManager->FillH1(8, 90);
                }
                else if (thePostPVname== "fibDet1_91"){
                    analysisManager->FillH1(8, 91);
                }
                else if (thePostPVname== "fibDet1_92"){
                    analysisManager->FillH1(8, 92);
                }
                else if (thePostPVname== "fibDet1_93"){
                    analysisManager->FillH1(8, 93);
                }
                else if (thePostPVname== "fibDet1_94"){
                    analysisManager->FillH1(8, 94);
                }
                else if (thePostPVname== "fibDet1_95"){
                    analysisManager->FillH1(8, 95);
                }
                else if (thePostPVname== "fibDet1_96"){
                    analysisManager->FillH1(8, 96);
                }
                else if (thePostPVname== "fibDet1_97"){
                    analysisManager->FillH1(8, 97);
                }
                else if (thePostPVname== "fibDet1_98"){
                    analysisManager->FillH1(8, 98);
                }
                else if (thePostPVname== "fibDet1_99"){
                    analysisManager->FillH1(8, 99);
                }
            }
            else if(thePostPVname.contains("fibDet2_")) {
                G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                analysisManager->FillH1(0, 8);
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);

                if (thePostPVname== "fibDet2_1"){
                    analysisManager->FillH1(8, 1);
                }
                else if (thePostPVname== "fibDet2_2"){
                    analysisManager->FillH1(8, 2);
                }
                else if (thePostPVname== "fibDet2_3"){
                    analysisManager->FillH1(8, 3);
                }
                else if (thePostPVname== "fibDet2_4"){
                    analysisManager->FillH1(8, 4);
                }
                else if (thePostPVname== "fibDet2_5"){
                    analysisManager->FillH1(8, 5);
                }
                else if (thePostPVname== "fibDet2_6"){
                    analysisManager->FillH1(8, 6);
                }
                else if (thePostPVname== "fibDet2_7"){
                    analysisManager->FillH1(8, 7);
                }
                else if (thePostPVname== "fibDet2_8"){
                    analysisManager->FillH1(8, 8);
                }
                else if (thePostPVname== "fibDet2_9"){
                    analysisManager->FillH1(8, 9);
                }
                else if (thePostPVname== "fibDet2_10"){
                    analysisManager->FillH1(8, 10);
                }
                else if (thePostPVname== "fibDet2_11"){
                    analysisManager->FillH1(8, 11);
                }
                else if (thePostPVname== "fibDet2_12"){
                    analysisManager->FillH1(8, 12);
                }
                else if (thePostPVname== "fibDet2_13"){
                    analysisManager->FillH1(8, 13);
                }
                else if (thePostPVname== "fibDet2_14"){
                    analysisManager->FillH1(8, 14);
                }
                else if (thePostPVname== "fibDet2_15"){
                    analysisManager->FillH1(8, 15);
                }
                else if (thePostPVname== "fibDet2_16"){
                    analysisManager->FillH1(8, 16);
                }
                else if (thePostPVname== "fibDet2_17"){
                    analysisManager->FillH1(8, 17);
                }
                else if (thePostPVname== "fibDet2_18"){
                    analysisManager->FillH1(8, 18);
                }
                else if (thePostPVname== "fibDet2_19"){
                    analysisManager->FillH1(8, 19);
                }
                else if (thePostPVname== "fibDet2_20"){
                    analysisManager->FillH1(8, 20);
                }
                else if (thePostPVname== "fibDet2_21"){
                    analysisManager->FillH1(8, 21);
                }
                else if (thePostPVname== "fibDet2_22"){
                    analysisManager->FillH1(8, 22);
                }
                else if (thePostPVname== "fibDet2_23"){
                    analysisManager->FillH1(8, 23);
                }
                else if (thePostPVname== "fibDet2_24"){
                    analysisManager->FillH1(8, 24);
                }
                else if (thePostPVname== "fibDet2_25"){
                    analysisManager->FillH1(8, 25);
                }
                else if (thePostPVname== "fibDet2_26"){
                    analysisManager->FillH1(8, 26);
                }
                else if (thePostPVname== "fibDet2_27"){
                    analysisManager->FillH1(8, 27);
                }
                else if (thePostPVname== "fibDet2_28"){
                    analysisManager->FillH1(8, 28);
                }
                else if (thePostPVname== "fibDet2_29"){
                    analysisManager->FillH1(8, 29);
                }
                else if (thePostPVname== "fibDet2_30"){
                    analysisManager->FillH1(8, 30);
                }
                else if (thePostPVname== "fibDet2_31"){
                    analysisManager->FillH1(8, 31);
                }
                else if (thePostPVname== "fibDet2_32"){
                    analysisManager->FillH1(8, 32);
                }
                else if (thePostPVname== "fibDet2_33"){
                    analysisManager->FillH1(8, 33);
                }
                else if (thePostPVname== "fibDet2_34"){
                    analysisManager->FillH1(8, 34);
                }
                else if (thePostPVname== "fibDet2_35"){
                    analysisManager->FillH1(8, 35);
                }
                else if (thePostPVname== "fibDet2_36"){
                    analysisManager->FillH1(8, 36);
                }
                else if (thePostPVname== "fibDet2_37"){
                    analysisManager->FillH1(8, 37);
                }
                else if (thePostPVname== "fibDet2_38"){
                    analysisManager->FillH1(8, 38);
                }
                else if (thePostPVname== "fibDet2_39"){
                    analysisManager->FillH1(8, 39);
                }
                else if (thePostPVname== "fibDet2_40"){
                    analysisManager->FillH1(8, 40);
                }
                else if (thePostPVname== "fibDet2_41"){
                    analysisManager->FillH1(8, 41);
                }
                else if (thePostPVname== "fibDet2_42"){
                    analysisManager->FillH1(8, 42);
                }
                else if (thePostPVname== "fibDet2_43"){
                    analysisManager->FillH1(8, 43);
                }
                else if (thePostPVname== "fibDet2_44"){
                    analysisManager->FillH1(8, 44);
                }
                else if (thePostPVname== "fibDet2_45"){
                    analysisManager->FillH1(8, 45);
                }
                else if (thePostPVname== "fibDet2_46"){
                    analysisManager->FillH1(8, 46);
                }
                else if (thePostPVname== "fibDet2_47"){
                    analysisManager->FillH1(8, 47);
                }
                else if (thePostPVname== "fibDet2_48"){
                    analysisManager->FillH1(8, 48);
                }
                else if (thePostPVname== "fibDet2_49"){
                    analysisManager->FillH1(8, 49);
                }
                else if (thePostPVname== "fibDet2_50"){
                    analysisManager->FillH1(8, 50);
                }
                else if (thePostPVname== "fibDet2_51"){
                    analysisManager->FillH1(8, 51);
                }
                else if (thePostPVname== "fibDet2_52"){
                    analysisManager->FillH1(8, 52);
                }
                else if (thePostPVname== "fibDet2_53"){
                    analysisManager->FillH1(8, 53);
                }
                else if (thePostPVname== "fibDet2_54"){
                    analysisManager->FillH1(8, 54);
                }
                else if (thePostPVname== "fibDet2_55"){
                    analysisManager->FillH1(8, 55);
                }
                else if (thePostPVname== "fibDet2_56"){
                    analysisManager->FillH1(8, 56);
                }
                else if (thePostPVname== "fibDet2_57"){
                    analysisManager->FillH1(8, 57);
                }
                else if (thePostPVname== "fibDet2_58"){
                    analysisManager->FillH1(8, 58);
                }
                else if (thePostPVname== "fibDet2_59"){
                    analysisManager->FillH1(8, 59);
                }
                else if (thePostPVname== "fibDet2_60"){
                    analysisManager->FillH1(8, 60);
                }
                else if (thePostPVname== "fibDet2_61"){
                    analysisManager->FillH1(8, 61);
                }
                else if (thePostPVname== "fibDet2_62"){
                    analysisManager->FillH1(8, 62);
                }
                else if (thePostPVname== "fibDet2_63"){
                    analysisManager->FillH1(8, 63);
                }
                else if (thePostPVname== "fibDet2_64"){
                    analysisManager->FillH1(8, 64);
                }
                else if (thePostPVname== "fibDet2_65"){
                    analysisManager->FillH1(8, 65);
                }
                else if (thePostPVname== "fibDet2_66"){
                    analysisManager->FillH1(8, 66);
                }
                else if (thePostPVname== "fibDet2_67"){
                    analysisManager->FillH1(8, 67);
                }
                else if (thePostPVname== "fibDet2_68"){
                    analysisManager->FillH1(8, 68);
                }
                else if (thePostPVname== "fibDet2_69"){
                    analysisManager->FillH1(8, 69);
                }
                else if (thePostPVname== "fibDet2_70"){
                    analysisManager->FillH1(8, 70);
                }
                else if (thePostPVname== "fibDet2_71"){
                    analysisManager->FillH1(8, 71);
                }
                else if (thePostPVname== "fibDet2_72"){
                    analysisManager->FillH1(8, 72);
                }
                else if (thePostPVname== "fibDet2_73"){
                    analysisManager->FillH1(8, 73);
                }
                else if (thePostPVname== "fibDet2_74"){
                    analysisManager->FillH1(8, 74);
                }
                else if (thePostPVname== "fibDet2_75"){
                    analysisManager->FillH1(8, 75);
                }
                else if (thePostPVname== "fibDet2_76"){
                    analysisManager->FillH1(8, 76);
                }
                else if (thePostPVname== "fibDet2_77"){
                    analysisManager->FillH1(8, 77);
                }
                else if (thePostPVname== "fibDet2_78"){
                    analysisManager->FillH1(8, 78);
                }
                else if (thePostPVname== "fibDet2_79"){
                    analysisManager->FillH1(8, 79);
                }
                else if (thePostPVname== "fibDet2_80"){
                    analysisManager->FillH1(8, 80);
                }
                else if (thePostPVname== "fibDet2_81"){
                    analysisManager->FillH1(8, 81);
                }
                else if (thePostPVname== "fibDet2_82"){
                    analysisManager->FillH1(8, 82);
                }
                else if (thePostPVname== "fibDet2_83"){
                    analysisManager->FillH1(8, 83);
                }
                else if (thePostPVname== "fibDet2_84"){
                    analysisManager->FillH1(8, 84);
                }
                else if (thePostPVname== "fibDet2_85"){
                    analysisManager->FillH1(8, 85);
                }
                else if (thePostPVname== "fibDet2_86"){
                    analysisManager->FillH1(8, 86);
                }
                else if (thePostPVname== "fibDet2_87"){
                    analysisManager->FillH1(8, 87);
                }
                else if (thePostPVname== "fibDet2_88"){
                    analysisManager->FillH1(8, 88);
                }
                else if (thePostPVname== "fibDet2_89"){
                    analysisManager->FillH1(8, 89);
                }
                else if (thePostPVname== "fibDet2_90"){
                    analysisManager->FillH1(8, 90);
                }
                else if (thePostPVname== "fibDet2_91"){
                    analysisManager->FillH1(8, 91);
                }
                else if (thePostPVname== "fibDet2_92"){
                    analysisManager->FillH1(8, 92);
                }
                else if (thePostPVname== "fibDet2_93"){
                    analysisManager->FillH1(8, 93);
                }
                else if (thePostPVname== "fibDet2_94"){
                    analysisManager->FillH1(8, 94);
                }
                else if (thePostPVname== "fibDet2_95"){
                    analysisManager->FillH1(8, 95);
                }
                else if (thePostPVname== "fibDet2_96"){
                    analysisManager->FillH1(8, 96);
                }
                else if (thePostPVname== "fibDet2_97"){
                    analysisManager->FillH1(8, 97);
                }
                else if (thePostPVname== "fibDet2_98"){
                    analysisManager->FillH1(8, 98);
                }
                else if (thePostPVname== "fibDet2_99"){
                    analysisManager->FillH1(8, 99);
                }

            }
       }
        
        else if(thePostPVname.contains("CellSide2/StdDet1")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 9);
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("CellSide2/StdDet2")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 10);
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("CellSide3/StdDet1")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 11);
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        else if(thePostPVname.contains("CellSide3/StdDet2")) {
            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillH1(0, 12);
            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        
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



