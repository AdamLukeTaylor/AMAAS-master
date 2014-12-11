/* 
 * File:   TestReward.h
 * Author: Adam
 *
 * Created on July 12, 2012, 11:20 AM
 */
#include "Reward.h"

#ifndef TESTREWARD_H
#define	TESTREWARD_H

class TestReward:public Reward {
public:
    TestReward();
 ~TestReward();
 void calcReward();
void calcReward(std::string in)  ;
void calcReward(std::string oldState, std::string worldState) ;
private:

};

#endif	/* TESTREWARD_H */

