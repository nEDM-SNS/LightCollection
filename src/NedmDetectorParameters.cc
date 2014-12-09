#include "NedmDetectorParameters.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


NedmDetectorParameters* NedmDetectorParameters::instance_;

NedmDetectorParameters::NedmDetectorParameters() {
  SetDefaults();
}

void NedmDetectorParameters::SetDefaults() {
  //LG Defaults
  inner_lightguide_size_ = G4ThreeVector(2.2225, 0.635, 76.2) * cm;
  lightguide_gap_ = 5.*um;
  lightguide_files_[0] = lightguide_files_[1] = lightguide_files_[2] = "";
  lightguide_loss_ = true;

  //PMT Defaults
  pmt_radius_ = 2.55 * cm;

  //Geometry Defaults
  mirror_airgap_ = single_airgap_ = 0.02*cm;
  use_mirror_ = true;
  use_hole_ = false;
  use_plug_ = false;
  use_window_ = false;
  use_full_pmt_ = false;

  //Cell hole defaults
  hole_position_ = G4ThreeVector(0, 0, 0);
  hole_radius_ = 1.905*cm;
}

void NedmDetectorParameters::Compute() {
  ComputeCellParameters();
  ComputeMirrorParameters();
  ComputeEndcapParameters();
  ComputeLightguideParameters();
  ComputeWindowParameters();
  ComputePmtParameters();
}



void NedmDetectorParameters::ComputeCellParameters() {
  G4double cell_x = 5.08*cm; 
  G4double cell_y = 0.635*cm;
  G4double cell_z = 20.0*cm;

  cell_size_ = G4ThreeVector(cell_x, cell_y, cell_z);

  G4double surface_thickness = 0.01*cm;

  //The 4 here comes from the fact that there are two full surfaces
  //of thickness 2*surface_thickness between the plates. These are the 
  //surface volumes
  cell_distx_ = cell_x + cell_y + 4*surface_thickness;
  cell_disty_ = cell_x + cell_y + 4*surface_thickness;

  cell_x_surface_size_ = G4ThreeVector(surface_thickness, cell_y, cell_z);
  cell_y_surface_size_ = G4ThreeVector(cell_x, surface_thickness, cell_z);

  cell_x_surface_position_.setX(cell_x + surface_thickness);
  cell_y_surface_position_.setY(cell_y + surface_thickness);
}

void NedmDetectorParameters::ComputeLightguideParameters() {
  G4double outer_x = (2*cell_size_.x() - 2*inner_lightguide_size_.x())/4
                      - lightguide_gap_/2.;


  G4double surface_thickness = 0.01*cm;

  outer_lightguide_size_ = inner_lightguide_size_;
  outer_lightguide_size_.setX(outer_x);

  G4double inner_lightguide_z = cell_size_.z();

  if(!use_lightguide_files())
    inner_lightguide_z += inner_lightguide_size_.z();

  inner_lightguide_position_.setZ(inner_lightguide_z);

  G4double outer_x_position = outer_x + inner_lightguide_size_.x() + lightguide_gap_;
  outer_lightguide_position_ = inner_lightguide_position_;
  outer_lightguide_position_.setX(outer_x_position);

  lightguide_x_surface_size_ = inner_lightguide_size_;
  lightguide_x_surface_size_.setX(lightguide_gap_ / 2.);

  lightguide_inner_y_surface_size_ = inner_lightguide_size_;
  lightguide_inner_y_surface_size_.setY(surface_thickness);

  lightguide_outer_y_surface_size_ = outer_lightguide_size_;
  lightguide_outer_y_surface_size_.setY(surface_thickness);

  lightguide_inner_x_surface_position_ =
    G4ThreeVector(inner_lightguide_size_.x() + lightguide_x_surface_size_.x(),
                  0,
                  inner_lightguide_position_.z());

  lightguide_outer_x_surface_position_ = outer_lightguide_position_
    + G4ThreeVector(outer_x + lightguide_x_surface_size_.x(), 0, 0);

  lightguide_y_surface_position_ = inner_lightguide_position_ +
    G4ThreeVector(0., inner_lightguide_size_.y() + surface_thickness, 0.);

}

void NedmDetectorParameters::ComputePmtParameters() {
  pmt_z_ = inner_lightguide_size_.y();
  G4double cell_z = cell_size_.z();
  G4double lg_z = inner_lightguide_size_.z();
  G4double pmt_posz;

  if(use_window_)
    pmt_posz = cell_z + 2*cold_gap_z_ + 2*window_z_ + 2*window_to_pmt_z_ + pmt_z_;
  else
    pmt_posz = cell_z + pmt_z_ + single_airgap_; 

  if(!use_lightguide_files())
    pmt_posz += 2*lg_z;

  pmt_position_.setZ(pmt_posz);
}

void NedmDetectorParameters::ComputeMirrorParameters() {
  mirror_size_ = G4ThreeVector(5.08, 0.635, 0.5) * cm;
  mirror_z_position_ = -(cell_size_.z() + mirror_size_.z() + mirror_airgap_);
}

void NedmDetectorParameters::ComputeEndcapParameters() {
  endcap_size_ = G4ThreeVector(cell_size_.x(),
                              cell_size_.x() - 2*cell_size_.y(),
                              0.635*cm);

  endcap_position_ = G4ThreeVector(0, 0, cell_size_.z() - endcap_size_.z());
}

void NedmDetectorParameters::ComputeWindowParameters() {
  cold_gap_z_ = 0.03175*cm;          // half of cold gap
  window_to_pmt_z_ = 0.03175*cm;     // half of gap b/w quartz win and PMT
  window_z_ = 0.3175*cm;       // half of quartz win thickness
  if(use_lightguide_files())
    window_radius_ = 2.54*cm;
  else
    window_radius_ = 10. * cm;

  window_position_ = G4ThreeVector(0, 0, 
      cell_size_.z() + 2*inner_lightguide_size_.z() + 2*cold_gap_z_ + window_z_);
                      
}

void NedmDetectorParameters::Print() {

  if(!use_lightguide_files()) {
    G4cout << "Simulating " << G4BestUnit(2*inner_lightguide_size_.z(),"Length")
           << " lightguide" << G4endl;

    G4cout << "Width: " << G4BestUnit(2*inner_lightguide_size_.x(), "Length") << G4endl;
  } else {
    G4cout << "Simulating lightguides defined in files: " << G4endl;
    for(int i = 0; i < 3; i++) {
      G4cout << lightguide_files(i) << G4endl;
    }
    G4cout << G4endl;
  }


  G4cout << "Hole enabled?: " << use_hole_ << G4endl;

  G4cout << "Mirror Enabled?: " << use_mirror_ << G4endl;

  G4cout << "Width of Single Airgap: " <<
    G4BestUnit(single_airgap_, "Length") << G4endl;

  G4cout << "Width of Mirror Airgap: " <<
    G4BestUnit(mirror_airgap_, "Length") << G4endl;


}
