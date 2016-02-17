#ifndef nEDMPhysicsList_h
#define nEDMPhysicsList_h 1

#include "globals.hh"
#include "G4VUserPhysicsList.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;
class G4OpWLS;
class nEDMPhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//! Defines which physics processes will be simulated.
class nEDMPhysicsList : public G4VUserPhysicsList
{
  public:
    nEDMPhysicsList();
   ~nEDMPhysicsList();

  public:
    void ConstructParticle();
    void ConstructProcess();

    void SetCuts();

  //! @name Particle Constructors
  //@{
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
  //@}

  //! @name Process Constructors
  //@{
    void ConstructGeneral();
    void ConstructEM();
    void ConstructOp();
  //@}
    
  /*! @name Messenger Methods
    These methods are to be called by an instance of nEDMPhysicsListMessenger
    */
  //@{
    void SetVerbose(G4int);
    void SetNbOfPhotonsCerenkov(G4int);
  //@}
    
  private:
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;
    G4OpWLS*             theWLSProcess;
    
    nEDMPhysicsListMessenger* pMessenger;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /* nEDMPhysicsList_h */

