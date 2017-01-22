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

    G4NistManager*     m_NistMan;

    G4Material*        m_Air;

    G4Material*        m_PMMA;
    G4Material*        m_WLSPMMA;
    G4Material*        m_Pethylene;
    G4Material*        m_FPethylene;
    G4Material*        m_Polystyrene;
    G4Material*        m_TPB_outer;
    G4Material*        m_TPB_inner;
    G4Material*        m_Silicone;
    G4Material*        m_SuperfluidHelium;

};

#endif /*nEDMMaterials_h*/
