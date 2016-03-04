//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: guidesPhysicsList.hh,v 1.6 2003/01/23 15:34:23 maire Exp $
// GEANT4 tag $Name: geant4-08-00 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
//class nEDMOpTPB;
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
    //nEDMOpTPB*           theTPBProcess;
    
    nEDMPhysicsListMessenger* pMessenger;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /* nEDMPhysicsList_h */

