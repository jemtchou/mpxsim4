#include "ActionInitialization.hh"
#include "Source.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new Source);
  RunAction* run = new RunAction;
  SetUserAction(run);
  EventAction* event = new EventAction(run);
  SetUserAction(event);
  SetUserAction(new SteppingAction(event));
}  
