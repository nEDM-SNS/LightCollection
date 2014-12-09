#include "NedmPMT.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SystemOfUnits.hh"

#include "NedmDetectorConstruction.hh"
#include "NedmDetectorParameters.hh"

G4LogicalVolume* NedmPMT::the_pmt_log_;
G4LogicalVolume* NedmPMT::the_wide_pmt_log_;

NedmPMT::NedmPMT(G4RotationMatrix* pRot,
                 const G4ThreeVector& tlate,
                 G4LogicalVolume* pMother,
                 G4int copy_no) : G4PVPlacement(pRot, tlate,
                // Hack to make PMT for fibers
                ((copy_no==2) ? NedmPMT::the_wide_pmt_log_ : NedmPMT::the_pmt_log_),"PMT", pMother, false, copy_no) {
    
}

void NedmPMT::CreateLogicalVolumes() {
    G4VSolid* PMT_box;
    G4VSolid* PMT_Wide_box;
    NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
    
    if(parameters->use_full_pmt()) {
        PMT_box = new G4Tubs("PMT", 0., parameters->pmt_radius(),
                             parameters->pmt_z(), 0., 360.);
    } else {
        //The only part of the PMT which is uncovered is that which is
        //in contact with the Lightguide
        PMT_box = new G4Box("PMT", parameters->cell_size().x(),
                            parameters->cell_size().y(), parameters->pmt_z());
    }
    
    // Hack to make PMT for fibers
    PMT_Wide_box = new G4Box("PMT", 5.*cm,
                             .6*cm, parameters->pmt_z());
    
    NedmPMT::the_pmt_log_ = new G4LogicalVolume(PMT_box, the_detector_->acrylic(), "PMT");
    
    // Hack to make PMT for fibers

    NedmPMT::the_wide_pmt_log_ = new G4LogicalVolume(PMT_Wide_box, the_detector_->acrylic(), "PMT");
    
    G4VisAttributes* PMTVis=new G4VisAttributes(G4Color(1.0,0.0,0.0));
    PMTVis->SetVisibility(true);
    NedmPMT::the_pmt_log_->SetVisAttributes(PMTVis);
    NedmPMT::the_wide_pmt_log_->SetVisAttributes(PMTVis);
    
    
    new G4LogicalSkinSurface("photocath_surf", NedmPMT::the_pmt_log_,
                             the_detector_->pmt_surface());
    
    new G4LogicalSkinSurface("photocath_surf", NedmPMT::the_wide_pmt_log_,
                             the_detector_->pmt_surface());

}
