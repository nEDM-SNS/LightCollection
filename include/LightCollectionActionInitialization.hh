#ifndef LightCollectionActionInitialization_h
#define LightCollectionActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.
///

class LightCollectionActionInitialization : public G4VUserActionInitialization
{
  public:
    LightCollectionActionInitialization();
    virtual ~LightCollectionActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

};

#endif
