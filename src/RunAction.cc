#include "RunAction.hh"

#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4AccumulableManager.hh"

#include "G4Run.hh"

#include <iostream>

RunAction::RunAction() : G4UserRunAction()
{ 
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetNtupleMerging(true);
  analysisManager->CreateNtuple("MpxSim", "Mpx Sim");
  analysisManager->CreateNtupleIColumn("FRAMEID");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("E");
  analysisManager->CreateNtupleIColumn("PDG");
  analysisManager->CreateNtupleDColumn("Edep"); 
  analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{

}

void RunAction::BeginOfRunAction(const G4Run* run)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("run_"+std::to_string(run->GetRunID())+".root");
}

void RunAction::EndOfRunAction(const G4Run* run)
{
 G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

 auto analysisManager = G4AnalysisManager::Instance();
      // save histograms & ntuple
 analysisManager->Write();
 analysisManager->CloseFile();
}

void RunAction::AddEdep(G4int n, G4double edep)
{
}



