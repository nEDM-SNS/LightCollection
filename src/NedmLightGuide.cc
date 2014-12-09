#include "NedmLightGuide.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalBorderSurface.hh"

#include "NedmDetectorConstruction.hh"
#include "NedmDetectorParameters.hh"
#include "NedmCompoundSolid.hh"
#include "G4SystemOfUnits.hh"

G4LogicalVolume* NedmLightGuide::lightGuide_log[3];

NedmLightGuide::NedmLightGuide(G4RotationMatrix* pRot,
    const G4ThreeVector& tlate,
    G4LogicalVolume* pLog,
    const G4String& pName,
    G4LogicalVolume* pMother,
    G4int copy_no) : G4PVPlacement(pRot, tlate,
			pLog, pName,
                       pMother, false, copy_no) {

    }


const G4ThreeVector& NedmLightGuide::lightguide_end_offset() {
  static const G4ThreeVector failure(0, 0, -1);

  NedmCompoundSolid::BentSolid* compound =
    dynamic_cast<NedmCompoundSolid::BentSolid*>(
        lightGuide_log[1]->GetSolid());

  if(!compound)
    return failure;

  return compound->final_slice().center(); 

}

void NedmLightGuide::CreateLogicalVolumes() {
  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();

  if(!parameters->use_lightguide_files()) {

    G4ThreeVector inner_size = parameters->inner_lightguide_size();
    G4ThreeVector outer_size = parameters->outer_lightguide_size();

    G4Box* lightGuideO_box = new G4Box("LGO_box", 
                                       outer_size.x(),
                                       outer_size.y(),
                                       outer_size.z());

    NedmLightGuide::lightGuide_log[0] = new G4LogicalVolume(lightGuideO_box,
        the_detector_->acrylic(),
        "LGO_log");

    NedmLightGuide::lightGuide_log[2] = NedmLightGuide::lightGuide_log[0];


    G4Box* lightGuideC_box = new G4Box("LGC_box",
                                       inner_size.x(),
                                       inner_size.y(),
                                       inner_size.z());

    NedmLightGuide::lightGuide_log[1] = new G4LogicalVolume(lightGuideC_box,
        the_detector_->acrylic(),"LGC_log");

  } else {
    NedmCompoundSolid::BentSolid* compound;
    char volume_name[32];
    
    for(int i = 0; i < 3; i ++) {
      sprintf(volume_name, "lightGuide_solid_%i", i);
      compound = NedmCompoundSolid::BentSolid::FromFile(volume_name, 
          parameters->lightguide_files(i));
      compound->set_surface_thickness(0.05 * cm);
      compound->Build();
      sprintf(volume_name, "lightGuide_log_%i", i);
      NedmLightGuide::lightGuide_log[i] = new G4LogicalVolume(compound,
          the_detector_->acrylic(), volume_name);
    }
  }

}

void NedmLightGuide::ConstructLightGuides(G4RotationMatrix* rotation, const G4ThreeVector& center, G4LogicalVolume* pMother, G4int copy_no) {

  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();

  G4ThreeVector inner_position = parameters->inner_lightguide_position();
  G4ThreeVector left_position = parameters->outer_lightguide_position();

  if(rotation) {
    inner_position *= rotation->inverse();
    left_position *= rotation->inverse();
  }

  G4ThreeVector right_position = G4ThreeVector(-left_position.x(), -left_position.y(),
                                   left_position.z());

  NedmLightGuide* guides[3];
  G4ThreeVector offsets[3] = { left_position,
                               inner_position,
                               right_position};

  for(int i = 0; i < 3; i++) {
    guides[i] = new NedmLightGuide(rotation, center + offsets[i],
        NedmLightGuide::lightGuide_log[i],
        "LG",
        pMother,
        copy_no*3 + i);
  }


  if(parameters->lightguide_loss()) {
    if (!parameters->use_lightguide_files())
      ConstructStraightSurfaceVolumes(guides);
    //Disabled for now because of bugs.
  // else
    //  ConstructCustomSurfaceVolumes(guides);
  }

  
}
  


void NedmLightGuide::ConstructStraightSurfaceVolumes(NedmLightGuide** lightGuide_phys) {
  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();

  G4ThreeVector center = lightGuide_phys[1]->GetTranslation();
  G4RotationMatrix* rot = lightGuide_phys[1]->GetRotation();

  G4ThreeVector x_size = parameters->lightguide_x_surface_size();
  G4ThreeVector inner_y_size = parameters->lightguide_inner_y_surface_size();
  G4ThreeVector outer_y_size = parameters->lightguide_outer_y_surface_size();
  G4ThreeVector inner_x_position = parameters->lightguide_inner_x_surface_position();
  G4ThreeVector outer_x_position = parameters->lightguide_outer_x_surface_position();
  G4ThreeVector y_position = parameters->lightguide_y_surface_position();


  G4Box* lgSurfVolCy_box = new G4Box("lgSurfVolCy",
                                     inner_y_size.x(),
                                     inner_y_size.y(),
                                     inner_y_size.z());

  G4Box* lgSurfVolOy_box = new G4Box("lgSurfVolOy",
                                     outer_y_size.x(),
                                     outer_y_size.y(),
                                     outer_y_size.z());


  G4LogicalVolume* lgSurfVolCy_log = new G4LogicalVolume(lgSurfVolCy_box,
      the_detector_->air(),
      "lgSurfVolCy");

  G4LogicalVolume* lgSurfVolOy_log = new G4LogicalVolume(lgSurfVolOy_box,
      the_detector_->air(),
      "lgSurfVolOy");

  
  //Construct Surface Volumes on the wide faces of the lightguides.
  G4double x_offsets[3] = { outer_x_position.x() - outer_y_size.x(),
                            0.,
                            -outer_x_position.x() + outer_y_size.x()};
  G4double y_offset = y_position.y();

  G4LogicalVolume* logicalVols[3] = { lgSurfVolOy_log,
    lgSurfVolCy_log,
    lgSurfVolOy_log };

  G4VPhysicalVolume* lgSurfVol;
  G4LogicalBorderSurface* surfVolBorder;

  char surfVolName[32];
  G4ThreeVector displacement;

  for(int i = 0; i < 6; i++) {
    displacement = G4ThreeVector(x_offsets[i / 2], pow(-1, i)*y_offset, 0.);

    if(rot)
      displacement *= (*rot);

    lgSurfVol = new G4PVPlacement(rot,
        center + displacement,
        logicalVols[ i / 2], "lgSurfVolY",
        the_detector_->get_logical_world(),
        false,
        i);

    sprintf(surfVolName, "lg%d_YSurface%d", lightGuide_phys[i / 2]->GetCopyNo(), i / 2);

    surfVolBorder =
      new G4LogicalBorderSurface(surfVolName,
          lightGuide_phys[i / 2],
          lgSurfVol,
          the_detector_->cast_surface());

    NedmDetectorConstruction::RegisterSurface(surfVolBorder, "cast");
  }



  G4Box* lgSurfVolx_box = new G4Box("lgSurfVolx",
                                    x_size.x(),
                                    x_size.y(),
                                    x_size.z());

  G4LogicalVolume* lgSurfVolx_log = new G4LogicalVolume(lgSurfVolx_box,
							  the_detector_->air(),
							 "lgSurfVolx");

  G4double x_offsets_2[2] = { inner_x_position.x(), outer_x_position.x()};
  G4int connections[4][3] = {
    {1, 1, 0},
    {0, 1, 1},
    {1, 0, 0},
    {0, 0, 1}
  };

  for(int i = 0; i < 4; i++) {
    displacement = G4ThreeVector(x_offsets_2[i / 2] * pow(-1, i), 0., 0.);

    if(rot)
      displacement *= (*rot);

    lgSurfVol = new G4PVPlacement(rot,
        center + displacement,
        lgSurfVolx_log, "lgSurfVolx",
        the_detector_->get_logical_world(),
        false,
        i);
    
    //The following logic makes sure the the surface volumes are attached
    //to the correct light guides with border surfaces.
    for(int j = 0; j < 3; j++) {
      if (connections[i][j]) {
        sprintf(surfVolName, "lg%d_XSurface%d", lightGuide_phys[j]->GetCopyNo(), i);
        surfVolBorder = new G4LogicalBorderSurface(surfVolName,
            lightGuide_phys[j],
            lgSurfVol,
            the_detector_->machined_surface());

        NedmDetectorConstruction::RegisterSurface(surfVolBorder, "machined");
      }

    }

  }

}

void NedmLightGuide::ConstructCustomSurfaceVolumes(NedmLightGuide** lightGuide_phys) {
  NedmCompoundSolid::BentSolid* solid;
  for(int i = 0; i < 3; i++) {
    solid =
      dynamic_cast<NedmCompoundSolid::BentSolid*>(
          lightGuide_phys[i]->GetLogicalVolume()->GetSolid());
    if(solid)
      solid->ConstructSurfaceVolumes(lightGuide_phys[i]);
  }
}
