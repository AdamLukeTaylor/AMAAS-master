/* 
 * File:   CartPoleAgent.h
 * Author: Adam
 *
 * Created on 17 June 2014, 14:17
 */
#include <string>
#include "RewardCartPole.h"
#include "DWL/include/DWLAgent.h"

#ifndef CARTPOLEAGENT_H
#define	CARTPOLEAGENT_H

class CartPoleAgent : public DWLAgent {
public:
    CartPoleAgent(std::string name);
    virtual ~CartPoleAgent();
private:
RewardCartPole* rewardCartPole;
};

#endif	/* CARTPOLEAGENT_H */

