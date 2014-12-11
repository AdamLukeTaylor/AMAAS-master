#include <string>
#ifndef Reward_H_
#define Reward_H_

typedef double Reward_Value;

class Reward {

	public:
		
	Reward() ;
	virtual ~Reward();

	void setReward( Reward_Value );
	
	virtual void calcReward() = 0 ;
	virtual void calcReward(std::string in) = 0 ;
	virtual void calcReward(std::string oldState, std::string worldState) = 0;
	Reward_Value getReward ();
	
	protected:
	
	Reward_Value reward;
	int max_reward;
	
};

#endif
