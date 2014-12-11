#ifndef _ActionSelection_H_
#define _ActionSelection_H_

#include <vector>
#include <string>

class DOP;

class ActionSelection {

public:

	ActionSelection();
	virtual ~ActionSelection();

	virtual std::string selectNextAction(std::vector<std::pair<std::string, double> >); 
	
	//virtual AbstractAction* selectNextActionGreedy(DOP*); 
	
protected:

	virtual std::string calculateNextAction(std::vector<std::pair<std::string, double> >)  = 0;


};
#endif
