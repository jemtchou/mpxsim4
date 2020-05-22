#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Threading.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{
}

SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
 
  G4String name = volume->GetName();

  auto postvolume
    = step->GetPostStepPoint()->GetTouchableHandle()
      ->GetVolume();

  if(postvolume)
   {
      G4String postname = postvolume->GetLogicalVolume()->GetName();
      if(name!="Det" && postname(0,3)=="Det")
      {
        G4int id = step->GetTrack()->GetTrackID();
        G4int pdg = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
        auto energy = step->GetTrack()->GetKineticEnergy();
	auto x = step->GetTrack()->GetPosition().x();
	auto y = step->GetTrack()->GetPosition().y();
        
        fEventAction->AddKine(id, energy);
        fEventAction->AddPdg(id, pdg);
        fEventAction->AddX(id, x);
        fEventAction->AddY(id, y);

        G4cout << "Enter detector " << pdg << " " <<  energy/keV << " keV"<< G4endl;
      }

      G4int id = step->GetTrack()->GetTrackID();
      G4int parent = step->GetTrack()->GetParentID();
      fEventAction->AddKids(parent, id);
      G4double edepStep = step->GetTotalEnergyDeposit();
      fEventAction->AddEdep(id, edepStep);
   }
}

