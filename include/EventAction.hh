#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "RunAction.hh"

#include <vector>
#include <algorithm>
#include <map>

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* run);
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );
 
    void AddKine(int id, double e)
    { 
       std::vector<int> v;
       kids.insert(std::make_pair(id, v));
       kids[id].push_back(id);
       kine.insert(std::make_pair(id, e));
       edep.insert(std::make_pair(id, 0));
    }
    void AddPdg(int id, int e){ pdg.insert(std::make_pair(id, e));}
    void AddX(int id, double e){ x.insert(std::make_pair(id, e));}
    void AddY(int id, double e){ y.insert(std::make_pair(id, e));}

    void AddEdep(int id, double e){ 
       auto it = kids.begin();
       while(it != kids.end())
          {
            // if id among descendants of (*it).first particle
	    if(std::find((*it).second.begin(), (*it).second.end(), id) != (*it).second.end())
	    {
	//	std::cout << "ADD " << (*it).first << " edep " << e << std::endl; 
		edep[(*it).first] += e;
	    }
	    it++;
          }
    }

    void AddKids(int idp, int idk){ 
       if(kids.find(idp) != kids.end())
          kids[idp].push_back(idk);
       else
       {  
          auto it = kids.begin();
          while(it != kids.end())
          {
	    if(std::find((*it).second.begin(), (*it).second.end(), idp) != (*it).second.end())
            { 
		(*it).second.push_back(idk);
		break;
             } 
	    it ++;
          }
       }
    }

    void Reset() {
       kine.clear();
       pdg.clear();
       x.clear();
       y.clear();
       edep.clear();
       kids.clear();
    }

  private:
    RunAction* fRunAction;

    std::map<int,double>  kine;
    std::map<int,int>  pdg;
    std::map<int,double>  x;
    std::map<int,double>  y;
    std::map<int,double>  edep;
    std::map<int, std::vector<int> > kids; // descendants of the key particle

};

#endif
