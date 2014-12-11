/* 
 * File:   RewardMtCar.h
 * Author: Adam
 *
 * Created on 24 June 2014, 12:03
 */
#include "DWL/include/Reward.h"
#include <string>
#ifndef REWARDMTCAR_H
#define	REWARDMTCAR_H

class RewardMtCar : public Reward {
public:
    RewardMtCar();
    RewardMtCar(const RewardMtCar& orig);
    virtual ~RewardMtCar();

    void calcReward();
    void calcReward(std::string oldState);
    void calcReward(std::string oldState, std::string worldState);
private:

};

#endif	/* REWARDMTCAR_H */

