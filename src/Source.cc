#include "Source.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

using namespace std;

Source* Source::fgInstance = 0;

Source* Source::GetInstance()
{
  return fgInstance;
}      

Source::Source()
: G4VUserPrimaryGeneratorAction()
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  particle = particleTable->FindParticle("gamma");

  fgInstance = this;
}

Source::~Source()
{
  fgInstance = 0;
}


G4ThreeVector Source::GetPosition()
{
/*   z = (2*G4UniformRand()-1)*12.25*mm;
   G4double r = 10.5*mm;
   while(true) {
       x=(2*G4UniformRand()-1)*r;
       y=(2*G4UniformRand()-1)*r;
       if(x*x+y*y < r*r) break;
   }*/ 
  
  G4ThreeVector position(0., 0., 0.); //x,y,z
  return position; 
}


G4ThreeVector Source::GetDirection(G4ThreeVector& pos)
{
   // phi = [0,2pi], theta=[0,pi]
  // G4double cost = G4UniformRand()*2-1;
  // G4double cost = G4UniformRand()*0.8-0.94; // 80-160 deg
  // G4double cost = G4UniformRand()*0.04-1; // 160-180 deg
   G4double cost = 1-G4UniformRand()*0.002; // 90+- 20 deg

   G4double sint = sqrt(1-cost*cost);

   G4double phi = 2*3.14159*G4UniformRand();
   G4ThreeVector dir(sint*cos(phi), sint*sin(phi), cost);

   return dir;
}


void Source::GeneratePrimaries(G4Event* evt)
{
  int i;
  for (i=0; i<2000; i++)
  {
    G4double ptime=0.0;
    G4ThreeVector position = GetPosition();

    G4PrimaryVertex* vertex = new G4PrimaryVertex(position, ptime); 

    G4double mass =  particle->GetPDGMass();

    G4ThreeVector direction = GetDirection(position);

    G4PrimaryParticle* p =
        new G4PrimaryParticle(particle);
    p->SetKineticEnergy( 59.5409*keV );
    p->SetMass( mass );
    p->SetMomentumDirection( direction );
    p->SetCharge( 0. );
    p->SetPolarization(0., 0., 0.);
    vertex->SetPrimary( p );

    evt->AddPrimaryVertex( vertex );
  }
}

