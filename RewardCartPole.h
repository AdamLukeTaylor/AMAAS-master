/* 
 * File:   RewardCartPole.h
 * Author: Adam
 *
 * Created on 17 June 2014, 14:34
 */
#include "DWL/include/Reward.h"
#include <string>
#ifndef REWARDCARTPOLE_H
#define	REWARDCARTPOLE_H

class RewardCartPole : public Reward {
public:
    RewardCartPole();
    ~RewardCartPole();

    void calcReward();
    void calcReward(std::string oldState);
    void calcReward(std::string oldState, std::string worldState);
private:

};

#endif	/* REWARDCARTPOLE_H */

