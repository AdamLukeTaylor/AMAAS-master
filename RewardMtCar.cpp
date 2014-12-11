/* 
 * File:   RewardMtCar.cpp
 * Author: Adam
 * 
 * Created on 24 June 2014, 12:03
 */

#include "RewardMtCar.h"
#include <iostream>
#include <cstdlib>

RewardMtCar::RewardMtCar()
{
}

RewardMtCar::RewardMtCar(const RewardMtCar& orig)
{
}

RewardMtCar::~RewardMtCar()
{
}

void RewardMtCar::calcReward()
{
    reward = -0.2;
    std::cerr << "called calc dummy 6\n";
    std::exit(1);
}

void RewardMtCar::calcReward(std::string oldState)
{
    reward = -0.5;
    std::cerr << "called calc dummy 5\n";
    std::exit(1);
}

void RewardMtCar::calcReward(std::string oldState, std::string worldState)
{

    std::string pos = oldState.substr(0, oldState.find("&"));
    std::string vel = oldState.substr(oldState.find("&") + 1, oldState.length() - (oldState.find("&") + 1));

    //std::cout << pos << "\n";
    // std::cout << vel << "\n";
    int position = atoi(pos.c_str());
    // std::cout << "pos num= " << position << "\n";
    if (position == 0)
    {
        reward = 1000;
    }
    else if (position == 1)
    {
        reward = -1;
    }
    else if (position == 2)
    {
        reward = -1;
    }
    else if (position == 3)
    {
        reward = -1;
    }
    else if (position == 4)
    {
        reward = -1;
    }
    else
    {
        reward = 0;
        std::cerr << "unknown state in reward\n";
        std::exit(1);
    }
    // std::cout << oldState;
    // std::cout << " reward= " << reward << "\n";
}