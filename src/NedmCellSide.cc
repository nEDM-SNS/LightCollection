#include "NedmCellSide.hh"

#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "NedmDetectorConstruction.hh"
#include "NedmDetectorParameters.hh"

G4LogicalVolume* NedmCellSide::the_first_side_;
G4LogicalVolume* NedmCellSide::the_side_;
G4LogicalVolume* NedmCellSide::the_plug_;
G4LogicalVolume* NedmCellSide::the_mirror_;


NedmCellSide::NedmCellSide(G4RotationMatrix* pRot,
    const G4ThreeVector& tlate,
    G4LogicalVolume* pMotherLogical,
    G4int copyNo,
    G4bool hole) : G4PVPlacement(pRot, tlate, hole ? NedmCellSide::the_first_side_  : NedmCellSide::the_side_, G4String("CellSide"), pMotherLogical, false, copyNo) { 

  ConstructTPBInterface(hole);
  ConstructSurfaceVolumes(hole);

  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
  
  if(parameters->use_plug() && hole) {
    G4ThreeVector displacement = parameters->hole_position();

    G4RotationMatrix* Rot = new G4RotationMatrix;
    Rot->rotateX(90.*deg);

    if(pRot) {
      displacement *= (*pRot);
      *Rot = Rot->transform(*pRot);
    }

    G4VisAttributes* plug_attr = new G4VisAttributes(G4Colour::Cyan());
    plug_attr->SetForceSolid(true);
    the_plug_->SetVisAttributes(plug_attr);

    displacement += tlate;


    new G4PVPlacement(Rot, displacement,
        the_plug_, "Plug",
        the_detector_->get_logical_world(), false, 0);
  }

  if(parameters->use_mirror())
    ConstructMirror();
  else
    ConstructRearSurface();
}

G4VSolid* NedmCellSide::MakeHole(G4VSolid* box, G4String solid_name, G4String hole_name, G4VSolid** hole_ptr) {
  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
  G4Tubs* hole = new G4Tubs(hole_name, 0., parameters->hole_radius(), 
      parameters->cell_size().y(), 0., twopi);

  if(hole_ptr)
    (*hole_ptr) = hole;

  G4RotationMatrix* Rot = new G4RotationMatrix;
  G4ThreeVector displacement = parameters->hole_position();
  Rot->rotateX(90.*deg);

  return new G4SubtractionSolid(solid_name, box,
      hole, Rot, displacement);

}

G4LogicalVolume* NedmCellSide::ConstructLogicalVolume(G4bool side1) {
  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
  
  G4VSolid* cellSide_solid;
  G4ThreeVector cell_size = parameters->cell_size();
  
  G4Box* cellSide_box = new G4Box("Cell",
                                  cell_size.x(),
                                  cell_size.y(),
                                  cell_size.z());

  if(parameters->use_hole() && side1) {
    G4VSolid* hole_ptr;
    cellSide_solid = MakeHole(cellSide_box, "CellWithHole", "CellHole", &hole_ptr);
    

    if(parameters->use_plug()) {
      
      the_plug_ = new G4LogicalVolume(hole_ptr,
          the_detector_->torlon(),
          "Plug");

      G4LogicalSkinSurface* torlonSurface =
	new G4LogicalSkinSurface("TorlonSurface", the_plug_,
				 the_detector_->torlon_surface());

      NedmDetectorConstruction::RegisterSurface(torlonSurface, "plug");
    }
  } else {
    cellSide_solid = cellSide_box;
  }
  
  G4LogicalVolume* cellSide_log = new G4LogicalVolume(cellSide_solid,
						      the_detector_->acrylic(), "Cell");

    G4VisAttributes* cellVis=new G4VisAttributes(G4Color(1.0,1.0,1.0));
    cellVis->SetVisibility(true);
    cellSide_log->SetVisAttributes(cellVis);
    
  return cellSide_log;
}

G4LogicalVolume* NedmCellSide::ConstructMirrorLogicalVolume() {

  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();

  G4ThreeVector mirror_size = parameters->mirror_size();

  G4Box* mirror_box = new G4Box("mirror", 
                                mirror_size.x(),
                                mirror_size.y(),
                                mirror_size.z());
  
  G4LogicalVolume* mirror_log = new G4LogicalVolume(mirror_box,
      the_detector_->air(),
      "mirror");

  G4LogicalSkinSurface* mirror_surface = 
    new G4LogicalSkinSurface("mirror_surface", mirror_log, the_detector_->mirror_surface());

  NedmDetectorConstruction::RegisterSurface(mirror_surface, "mirror");

    G4VisAttributes* mirrorVis=new G4VisAttributes(G4Color(0.0,0.0,1.0));
    mirrorVis->SetVisibility(true);
    mirror_log->SetVisAttributes(mirrorVis);

    
    return mirror_log;


}

void NedmCellSide::CreateLogicalVolumes() {
  NedmCellSide::the_first_side_ = ConstructLogicalVolume(true);
  NedmCellSide::the_side_ = ConstructLogicalVolume();
  NedmCellSide::the_mirror_ = ConstructMirrorLogicalVolume();
}

G4VPhysicalVolume* NedmCellSide::ConstructTPBInterface(G4bool side1) {
  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();

  G4VSolid* TPBInterface_solid;

  G4ThreeVector tpb_size = parameters->cell_y_surface_size();

  G4Box* TPBInterface_box = new G4Box("TPBInterface",
      tpb_size.x(),
      tpb_size.y(),
      tpb_size.z());

  if(parameters->use_hole() && side1) {
    TPBInterface_solid = MakeHole(TPBInterface_box,
        "TPBInterfaceWithHole", "TPBInterface_hole", NULL);
  } else {
    TPBInterface_solid = TPBInterface_box;
  }
  
  G4LogicalVolume* TPBInterface_log = new G4LogicalVolume(TPBInterface_solid,
							  the_detector_->air(),
							  "TPBInterface");

  G4int copy_no = GetCopyNo();
  G4RotationMatrix* rot = GetRotation();
  G4ThreeVector cell_position = GetTranslation();

  G4VPhysicalVolume* TPBInterface_phys
    = new G4PVPlacement(rot,
        cell_position - cell_position.unit() *
        (tpb_size.y() + parameters->cell_size().y()),
        TPBInterface_log,
        "TPBInterface",
        the_detector_->get_logical_world(), false, copy_no);

  
  G4LogicalBorderSurface* TPBMachinedSurf =
    new G4LogicalBorderSurface("MachinedSurfaceTPB",
        TPBInterface_phys,
        this,
        the_detector_->conversion_surface());

  G4LogicalBorderSurface* TPBMachinedSurf2 =
    new G4LogicalBorderSurface("MachinedSurfaceTPB2",
        this,
        TPBInterface_phys,
        the_detector_->tpb_surface());

  NedmDetectorConstruction::RegisterSurface(TPBMachinedSurf, "tpb");
  NedmDetectorConstruction::RegisterSurface(TPBMachinedSurf2, "tpb");


  return TPBInterface_phys;
}


void NedmCellSide::ConstructSurfaceVolumes(G4bool side1) {
  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();

  G4int copy_no = GetCopyNo();
  G4RotationMatrix* rot = GetRotation();
  G4ThreeVector cell_position = GetTranslation();

  G4ThreeVector x_surface_size = parameters->cell_x_surface_size();
  G4ThreeVector y_surface_size = parameters->cell_y_surface_size();


  //Should make these static variables to avoid recreating this all the time.
  G4Box* surfVolX_box = new G4Box("cellSurfaceX",
                                  x_surface_size.x(),
                                  x_surface_size.y(),
                                  x_surface_size.z());

  G4LogicalVolume* surfVolX_log =
    new G4LogicalVolume(surfVolX_box, the_detector_->air(), "cellSurfaceX");

  G4ThreeVector displacement = parameters->cell_x_surface_position();
  if (rot)
    displacement *= (*rot);

  G4ThreeVector pos1 = cell_position + displacement;


  G4VPhysicalVolume* surfVolPx_phys =
    new G4PVPlacement(rot,
                      pos1,
		      surfVolX_log,
		      "cellSurfacePx",
		      the_detector_->get_logical_world(),
		      false,
                      2*copy_no);
 
  G4ThreeVector pos2 = cell_position - displacement;

  G4VPhysicalVolume* surfVolNx_phys =
    new G4PVPlacement(rot,
                      pos2,
		      surfVolX_log,
		      "cellSurfaceNx",
		      the_detector_->get_logical_world(),
		      false,
		      2*copy_no + 1);

  G4VSolid* surfVolY_solid;

  G4Box* surfVolY_box = new G4Box("cellSurfaceY",
                                  y_surface_size.x(),
                                  y_surface_size.y(),
                                  y_surface_size.z());

  if(parameters->use_hole() && side1) {
    G4Tubs* surfVol4_hole = new G4Tubs("cellSurfaceY_hole",
				       0., parameters->hole_radius(),
				       y_surface_size.y(), 0., twopi);

    G4RotationMatrix* Rot = new G4RotationMatrix;
    Rot->rotateX(90.*deg);
    G4ThreeVector displacement = parameters->hole_position();

    G4SubtractionSolid* surfVolY_w_hole =
      new G4SubtractionSolid("cellSurfaceYWithHole", surfVolY_box,
			     surfVol4_hole, Rot, displacement);

    surfVolY_solid = surfVolY_w_hole;
  } else {
    surfVolY_solid = surfVolY_box;
  }
  
  G4LogicalVolume* surfVolY_log =
    new G4LogicalVolume(surfVolY_solid, the_detector_->air(), "cellSurfaceY");

  G4VPhysicalVolume* surfVolY_phys =
    new G4PVPlacement(rot,
		      cell_position + cell_position.unit() *
                      parameters->cell_y_surface_position().y(),
		      surfVolY_log,
		      "cellSurfaceY",
		      the_detector_->get_logical_world(),
		      false,
		      copy_no);

 
  //G4LogicalBorderSurface* cellMachinedSurface1 =
    new G4LogicalBorderSurface("cellMachinedSurfacePx", this,
			       surfVolPx_phys, the_detector_->machined_surface());

  //G4LogicalBorderSurface* cellMachinedSurface2 =
    new G4LogicalBorderSurface("cellMachinedSurfaceNx", this,
			       surfVolNx_phys, the_detector_->machined_surface());

  //G4LogicalBorderSurface* cellCastSurface4 =
    new G4LogicalBorderSurface("cellCastSurfaceY", this,
			       surfVolY_phys, the_detector_->cast_surface());
 
}


/*! @warning Do not call if you are calling NedmCellSide::ConstructRearSurface
 */
G4VPhysicalVolume* NedmCellSide::ConstructMirror() {
  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();

  G4RotationMatrix* rot = GetRotation();
  G4ThreeVector translation = GetTranslation();
  G4int copy_no = GetCopyNo();

  G4ThreeVector position = translation + G4ThreeVector(0.,0.,
      parameters->mirror_z_position());

  G4VPhysicalVolume* mirror_phys
    = new G4PVPlacement(rot, position, the_mirror_, "mirror",
			the_detector_->get_logical_world(), false, copy_no);

  return mirror_phys;
}

/*! @warning Do not call this method if you are calling NedmCellSide::ConstructMirror
 */
G4VPhysicalVolume* NedmCellSide::ConstructRearSurface() {
  NedmDetectorParameters* parameters = NedmDetectorParameters::instance();
  G4RotationMatrix* rot = GetRotation();
  G4ThreeVector translation = GetTranslation();
  G4int copy_no = GetCopyNo();
  G4ThreeVector mirror_size = parameters->mirror_size();
  G4Box* rearSurf_box = new G4Box("rearSurf", 
                                  mirror_size.x(),
                                  mirror_size.y(),
                                  mirror_size.z());

  G4LogicalVolume* rearSurf_log = new G4LogicalVolume(rearSurf_box,
      the_detector_->acrylic(),
      "rear_surface");
  
  G4ThreeVector position = translation + G4ThreeVector(0.,0.,
      parameters->mirror_z_position());

  G4VPhysicalVolume* rearSurf_phys
    = new G4PVPlacement(rot, position, rearSurf_log, "rearSurf",
			the_detector_->get_logical_world(), false, copy_no);

    new G4LogicalBorderSurface("RearMachinedSurface", this,
			       rearSurf_phys, the_detector_->machined_surface());

  return rearSurf_phys;
}

