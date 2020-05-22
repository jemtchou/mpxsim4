#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class PhysicsList : public G4VModularPhysicsList
{
public:
  PhysicsList(G4int ver = 1, G4String LEN_model="HP");
  virtual ~PhysicsList()=default;

  // delete copy constructor and assignment operator
  PhysicsList(const PhysicsList &)=delete;
  PhysicsList & operator=(const PhysicsList &right)=delete;

  virtual void SetCuts();
};

#endif



