#include "EventAction.hh"

#include "G4Event.hh"
#include "Analysis.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

#include <iostream>

EventAction::EventAction(RunAction* run)
: G4UserEventAction(),  fRunAction(run)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  Reset();
}

void EventAction::EndOfEventAction(const G4Event* event)
{
   // store particles to the ntuple
  // std::cout << "Edep " << edep.size() << " Kine " << kine.size() << std::endl;

   auto analysisManager = G4AnalysisManager::Instance();

   auto it = kine.begin();
   while(it != kine.end())
   {
      auto id = (*it).first;
      G4int counter = 0;
      analysisManager->FillNtupleIColumn(counter++, event->GetEventID());
      analysisManager->FillNtupleDColumn(counter++, x[id]);
      analysisManager->FillNtupleDColumn(counter++, y[id]);
      analysisManager->FillNtupleDColumn(counter++, kine[id]);
      analysisManager->FillNtupleIColumn(counter++, pdg[id]);
      analysisManager->FillNtupleDColumn(counter++, edep[id]);
      analysisManager->AddNtupleRow();
      it++;
   }
}  
