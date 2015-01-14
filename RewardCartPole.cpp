/* 
 * File:   RewardCartPole.cpp
 * Author: Adam
 * 
 * Created on 17 June 2014, 14:34
 */

#include "RewardCartPole.h"
#include <iostream>
#include <cstdlib>

RewardCartPole::RewardCartPole() : Reward()
{
}

RewardCartPole::~RewardCartPole()
{
}

void RewardCartPole::calcReward()
{
    reward = -0.2;
    std::cerr << "called calc dummy 6\n";
    std::exit(1);
}

void RewardCartPole::calcReward(std::string oldState)
{
    reward = -0.5;
    std::cerr << "called calc dummy 5\n";
    std::exit(1);
}

void RewardCartPole::calcReward(std::string oldState, std::string worldState)
{
    //std::cout << "rewarding state " << oldState << " now " << worldState << std::endl;
    int current = atoi(oldState.c_str());
    reward = 0;
    /* if (current == 75 || current == 74 || current == 83 || current == 131 || current == 133 || current == 142 || current == 143 || current == 30 || current == 19 || current == 27 || current == 35 || current == 18 || current == 134 || current == 86 || current == 140 || current == 77 || current == 137 || current == 21 || current == 72 || current == 128 || current == 26 || current == 126 || current == 84 || current == 31 || current == 80 || current == 130 || current == 135 || current == 22 || current == 76 || current == 25 || current == 136 || current == 88 || current == 139 || current == 79 || current == 34 || current == 127)
     {//known near upright states
         reward = 500;
     }
     else if (current == 75 || current == 80 || current == 142 || current == 30 || current == 86 || current == 77 || current == 81 || current == 84 || current == 82 || current == 85 || current == 73 || current == 89 || current == 79 || current == 73 || current == 88)
     {//known near upright states and slow
         reward = 1000;
     }
     else if (current == -1)
     {
         reward = -1000;
     }
     else
     {
         reward = 10;
     }
    if (current != -1)
    {
        reward = 1000;
    }
    else
    {
        reward = -1000;
    }*/
    if (current == -1)
    {
        reward = -2;
    }
    else
    {//reward 1
        //std::cout << "rewarding state " << oldState << " now " << worldState << std::endl;
        int begin = oldState.find("&", oldState.find("&") + 1) + 1;
        std::string angle = oldState.substr(begin, 2);
        //std::cout << "rewarding state " << worldState << " |" << angle << " with " << reward << std::endl;
        if (angle[0] == 'S')
        {//if a little bit either way
            reward = 1;
        }
        else if (angle[0] == 'L')
        {//if a little bit either way
            reward = -1;
        }
        else
        {
            //reward = -1;
        }

        begin = oldState.find_last_of("&");
        std::string speed = oldState.substr(begin);
        //
        //std::cout << speed;
        if (speed[1] == 'S')
        {//if a little bit either way
            //reward = reward + 200;
        }
        else
        {
            // reward = reward - 200;
        }

        // std::cout << "rewarding state " << oldState << " |" << speed << " with " << reward << std::endl;
    }
    //std::cout << "rewarding state " << s << " with " << reward << "\n";
}
/*//reward 1
 * if (angle[0] == 'S')
        {//if a little bit either way
            reward = 1000;
        }
        if (angle[0] == 'L')
        {//if a little bit either way
            reward = -500;
        }
        else
        {
            reward = -1;
        }

        begin = oldState.find_last_of("&");
        std::string speed = oldState.substr(begin);
        //
        if (speed[1]== 'S')
        {//if a little bit either way
            reward = reward + 200;
        }
        else
        {
            reward = reward - 200;
        }*/
/*rewad 2
 if (current == -1)
    {
        reward = -1000;
    }
    else
    {
        //std::cout << "rewarding state " << oldState << " now " << worldState << std::endl;
        int begin = oldState.find("&", oldState.find("&") + 1) + 1;
        std::string angle = oldState.substr(begin, 2);
        //std::cout << "rewarding state " << worldState << " |" << angle << " with " << reward << std::endl;
        if (angle[0] == 'S')
        {//if a little bit either way
         //   reward = 500;
        }
        if (angle[0] == 'L')
        {//if a lot bit either way
         //   reward = -500;
        }
        else
        {
         //   reward = -10;
        }

        begin = oldState.find_last_of("&");
        std::string speed = oldState.substr(begin);
        //
        if (speed[1] == 'S')
        {//if a little bit either way
            //reward = reward + 2;
        }
        else
        {
            //reward = reward - 2;
        }*/

/*reward3if (angle[0] == 'S')
        {//if a little bit either way
            reward = 1;
        }
        else if (angle[0] == 'L')
        {//if a little bit either way
            reward = -1;
        }
        else
        {
            //reward = -1;
        }

        begin = oldState.find_last_of("&");
        std::string speed = oldState.substr(begin);
        //
        //std::cout << speed;
        if (speed[1] == 'S')
        {//if a little bit either way
            //reward = reward + 200;
        }
        else
        {
            // reward = reward - 200;
        }*/