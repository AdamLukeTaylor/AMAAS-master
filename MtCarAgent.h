/* 
 * File:   MtCarAgent.h
 * Author: Adam
 *
 * Created on 24 June 2014, 11:53
 */
#include <string>
#include "RewardMtCar.h"
#include "DWL/include/DWLAgent.h"

#ifndef MTCARAGENT_H
#define	MTCARAGENT_H

class MtCarAgent : public DWLAgent {
public:
    MtCarAgent(std::string name);
    virtual ~MtCarAgent();
private:
RewardMtCar* rewardMtCar;
};

#endif	/* MTCARAGENT_H */

