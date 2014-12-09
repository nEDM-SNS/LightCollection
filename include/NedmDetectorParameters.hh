#ifndef NedmDetectorParameters_h
#define NedmDetectorParameters_h

#include "G4ThreeVector.hh"
#include "G4String.hh"

class NedmDetectorParameters {
    
public:
    
    void Compute();
    void Print();
    
    inline static NedmDetectorParameters* instance();
    
    //! @name Accessors
    //@{
    inline G4ThreeVector cell_size() const { return cell_size_; }
    inline G4double cell_distx() const { return cell_distx_; }
    inline G4double cell_disty() const { return cell_disty_; }
    
    inline G4ThreeVector cell_x_surface_size() const { return cell_x_surface_size_; }
    inline G4ThreeVector cell_y_surface_size() const { return cell_y_surface_size_; }
    inline G4ThreeVector cell_x_surface_position() const {return cell_x_surface_position_; }
    inline G4ThreeVector cell_y_surface_position() const {return cell_y_surface_position_; }
    
    inline G4ThreeVector inner_lightguide_size() const { return inner_lightguide_size_; }
    inline G4ThreeVector outer_lightguide_size() const { return outer_lightguide_size_; }
    inline G4ThreeVector inner_lightguide_position() const { return inner_lightguide_position_; }
    inline G4ThreeVector outer_lightguide_position() const { return outer_lightguide_position_; }
    inline G4ThreeVector lightguide_x_surface_size() const {
        return lightguide_x_surface_size_;
    }
    inline G4ThreeVector lightguide_inner_y_surface_size() const {
        return lightguide_inner_y_surface_size_;
    }
    inline G4ThreeVector lightguide_outer_y_surface_size() const {
        return lightguide_outer_y_surface_size_;
    }
    inline G4ThreeVector lightguide_inner_x_surface_position() const {
        return lightguide_inner_x_surface_position_;
    }
    inline G4ThreeVector lightguide_outer_x_surface_position() const {
        return lightguide_outer_x_surface_position_;
    }
    inline G4ThreeVector lightguide_y_surface_position() const {
        return lightguide_y_surface_position_;
    }
    inline const G4String lightguide_files(G4int i) const {
        return i < 3 ? lightguide_files_[i] : G4String("");
    }
    inline G4bool use_lightguide_files() const { return lightguide_files(0) != ""; }
    inline G4bool lightguide_loss() const { return lightguide_loss_; }
    
    inline G4ThreeVector mirror_size() const { return mirror_size_; }
    inline G4double mirror_z_position() const { return mirror_z_position_; }
    
    inline G4double hole_radius() const { return hole_radius_; }
    inline G4ThreeVector hole_position() const { return hole_position_; }
    
    inline G4double pmt_radius() const { return pmt_radius_; }
    inline G4double pmt_z() const { return pmt_z_; }
    inline G4ThreeVector pmt_position() const { return pmt_position_; }
    
    inline G4double window_radius() const { return window_radius_; }
    inline G4double window_z() const { return window_z_; }
    inline G4ThreeVector window_position() const { return window_position_; }
    
    inline G4ThreeVector endcap_size() const { return endcap_size_; }
    inline G4ThreeVector endcap_position() const { return endcap_position_; }
    
    inline G4bool use_mirror() const { return use_mirror_; }
    inline G4bool use_hole() const { return use_hole_; }
    inline G4bool use_plug() const { return use_plug_; }
    inline G4bool use_full_pmt() const { return use_full_pmt_; }
    inline G4bool use_window() const { return use_window_; }
    inline G4bool use_single_plate() const { return use_single_plate_; }
    //@}
    
    //! @name Mutators
    //@{
    inline void set_lightguide_x(G4double lightguide_x);
    inline void set_lightguide_z(G4double lightguide_z);
    inline void set_lightguide_files(G4String left,
                                     G4String center,
                                     G4String right);
    inline void set_lightguide_loss(G4bool on_off);
    inline void set_hole_x_position(G4double hole_x_position);
    inline void set_hole_z_position(G4double hole_z_position);
    inline void set_hole_radius(G4double hole_radius);
    inline void set_single_airgap(G4double single_airgap);
    inline void set_mirror_airgap(G4double mirror_airgap);
    inline void set_pmt_radius(G4double pmt_radius);
    inline void set_use_mirror(G4bool use_mirror);
    inline void set_use_hole(G4bool use_hole);
    inline void set_use_plug(G4bool use_plug);
    inline void set_use_window(G4bool use_window);
    inline void set_use_full_pmt(G4bool use_full_pmt);
    inline void set_use_single_plate(G4bool use_single_plate);
    //@}
    
private:
    NedmDetectorParameters();
    static NedmDetectorParameters* instance_;
    
    void SetDefaults();
    void ComputeCellParameters();
    void ComputeMirrorParameters();
    void ComputeEndcapParameters();
    void ComputeLightguideParameters();
    void ComputeWindowParameters();
    void ComputePmtParameters();
    
    
    //GEOMETRY
    
    //! @name LightGuide Parameters
    //@{
    G4ThreeVector inner_lightguide_size_;
    G4ThreeVector inner_lightguide_position_;
    G4ThreeVector outer_lightguide_size_;
    G4ThreeVector outer_lightguide_position_;
    G4double lightguide_gap_;
    
    //Looking from outside the box, leftmost, middle, rightmost
    G4String lightguide_files_[3];
    
    G4bool lightguide_loss_;
    //@}
    
    //! @name Cellside Dimensions
    //@{
    G4ThreeVector cell_size_;
    G4double cell_distx_;
    G4double cell_disty_;
    //@}
    
    //! @name PMT Dimensions
    //@{
    G4double pmt_radius_;
    G4double pmt_z_;
    //@}
    
    G4ThreeVector pmt_position_;
    
    //! @name Mirror Dimensions
    //@{
    G4ThreeVector mirror_size_;
    //@}
    G4double mirror_z_position_;
    
    G4double single_airgap_;
    G4double mirror_airgap_;
    
    /*! @name Surface Volume Propreties
     
     These allow surface properties to be set indepently for a given volume.
     */
    //@{
    G4ThreeVector cell_x_surface_size_;
    G4ThreeVector cell_x_surface_position_;
    G4ThreeVector cell_y_surface_size_;
    G4ThreeVector cell_y_surface_position_;
    
    G4ThreeVector lightguide_x_surface_size_;
    G4ThreeVector lightguide_inner_y_surface_size_;
    G4ThreeVector lightguide_outer_y_surface_size_;
    G4ThreeVector lightguide_inner_x_surface_position_;
    G4ThreeVector lightguide_outer_x_surface_position_;
    G4ThreeVector lightguide_y_surface_position_;
    //@}
    
    //! @name Cell side hole parameters
    //@{
    G4ThreeVector hole_position_;
    G4double hole_radius_;
    //@}
    
    //! @name Endcap parameters
    //@{
    G4ThreeVector endcap_size_;
    G4ThreeVector endcap_position_;
    //@}
    
    //! @name Quartz window parameters
    //@{
    G4double cold_gap_z_;
    G4double window_to_pmt_z_;
    G4double window_z_;
    G4double window_radius_;
    
    G4ThreeVector window_position_;
    //@}
    
    
    //Switchs
    //! @name Geometry Options
    //@{
    G4bool use_mirror_;
    G4bool use_hole_;
    G4bool use_plug_;
    G4bool use_window_;
    G4bool use_full_pmt_;
    G4bool use_single_plate_;
    //@}
};

inline
NedmDetectorParameters* NedmDetectorParameters::instance() {
    if (!instance_)
        instance_ = new NedmDetectorParameters;
    return instance_;
}

inline
void NedmDetectorParameters::set_lightguide_x(G4double lightguide_x) {
    inner_lightguide_size_.setX(lightguide_x);
}

inline
void NedmDetectorParameters::set_lightguide_z(G4double lightguide_z) {
    inner_lightguide_size_.setZ(lightguide_z);
    outer_lightguide_size_.setZ(lightguide_z);
}

inline
void NedmDetectorParameters::set_lightguide_files(G4String left,
                                                  G4String center,
                                                  G4String right) {
    lightguide_files_[0] = left.strip(2).strip(2, '"');
    lightguide_files_[1] = center.strip(2).strip(2, '"');
    lightguide_files_[2] = right.strip(2).strip(2, '"');
}

inline
void NedmDetectorParameters::set_lightguide_loss(G4bool on_off) {
    lightguide_loss_ = on_off;
}

inline
void NedmDetectorParameters::set_hole_x_position(G4double hole_x_position) {
    hole_position_.setX(hole_x_position);
}

inline
void NedmDetectorParameters::set_hole_z_position(G4double hole_z_position) {
    hole_position_.setZ(hole_z_position);
}

inline
void NedmDetectorParameters::set_hole_radius(G4double hole_radius) {
    hole_radius_ = hole_radius;
}

inline
void NedmDetectorParameters::set_single_airgap(G4double single_airgap) {
    single_airgap_ = single_airgap;
}

inline
void NedmDetectorParameters::set_mirror_airgap(G4double mirror_airgap) {
    mirror_airgap_ = mirror_airgap;
}

inline
void NedmDetectorParameters::set_pmt_radius(G4double pmt_radius) {
    pmt_radius_ = pmt_radius;
}

inline
void NedmDetectorParameters::set_use_mirror(G4bool use_mirror) {
    use_mirror_ = use_mirror; 
}

inline
void NedmDetectorParameters::set_use_hole(G4bool use_hole) {
    use_hole_ = use_hole; 
    use_plug_ &= use_hole_;
}

inline
void NedmDetectorParameters::set_use_plug(G4bool use_plug) {
    use_plug_ = use_plug; 
    use_hole_ |= use_hole_;
}

inline
void NedmDetectorParameters::set_use_window(G4bool use_window) {
    use_window_ = use_window; 
}

inline
void NedmDetectorParameters::set_use_full_pmt(G4bool use_full_pmt) {
    use_full_pmt_ = use_full_pmt; 
}

inline
void NedmDetectorParameters::set_use_single_plate(G4bool use_single_plate) {
    use_single_plate_ = use_single_plate;
}
#endif
