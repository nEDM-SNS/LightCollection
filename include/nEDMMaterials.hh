#ifndef nEDMMaterials_h
#define nEDMMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

class nEDMMaterials
{
  public:

    virtual ~nEDMMaterials();
 
    static nEDMMaterials* GetInstance();

    G4Material* GetMaterial(const G4String);
 
  private:
 
    nEDMMaterials();

    void CreateMaterials();

    void AddMatProperties();

  private:

    static nEDMMaterials* fInstance;

    G4NistManager*     fNistMan;

    G4Material*        fAir;

    G4Material*        fPMMA;
    G4Material*        fWLSPMMA;
    G4Material*        fPethylene;
    G4Material*        fFPethylene;
    G4Material*        fPolystyrene;
    G4Material*        fTPB_outer;
    G4Material*        fTPB_inner;
    G4Material*        fSilicone;
    G4Material*        fSuperfluidHelium;

};

#endif /*nEDMMaterials_h*/
