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
    int current = atoi(oldState.c_str());
    reward = 0;
    if (current == -1)
    {
        reward = -2000;
    }
    else
    {//reward 1
        //std::cout << "rewarding state " << oldState << " now " << worldState << std::endl;
        int begin = oldState.find("&", oldState.find("&") + 1) + 1;
        std::string angle = oldState.substr(begin, 2);
        //std::cout << "rewarding state " << worldState << " |" << angle << " with " << reward << std::endl;
        if (angle[0] == 'S')
        {//if a little bit either way
            reward = 100;
        }
        else if (angle[0] == 'L')
        {//if a little bit either way
            reward = -100;
        }
        else if (angle[0] == 'M')
        {
            reward = 0;
        }

        begin = oldState.find_last_of("&");
        std::string speed = oldState.substr(begin);
        //
        //std::cout << speed;
        if (speed[1] == 'S')
        {//if a little bit either way
            reward = reward + 20;
        }
        else
        {
            reward = reward - 20;
        }

        // std::cout << "rewarding state " << oldState << " |" << speed << " with " << reward << std::endl;
    }
    /* reward0int current = atoi(oldState.c_str());
     reward = 0;
     if (current == -1)
     {
         reward = -2000;
     }
     else
     {//reward 1
         //std::cout << "rewarding state " << oldState << " now " << worldState << std::endl;
         int begin = oldState.find("&", oldState.find("&") + 1) + 1;
         std::string angle = oldState.substr(begin, 2);
         //std::cout << "rewarding state " << worldState << " |" << angle << " with " << reward << std::endl;
         if (angle[0] == 'S')
         {//if a little bit either way
             reward = 1000;
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
     * */
}

void RewardCartPole::calcReward(std::string oldState, std::string worldState)
{

    //std::cout << "rewarding state " << oldState << " now " << worldState << std::endl;

    int current = atoi(oldState.c_str());
    reward = 0;
    if (current == -1)
    {
        reward = -2000;
    }
    else
    {//reward 1
        //std::cout << "rewarding state " << oldState << " now " << worldState << std::endl;
        int begin = oldState.find("&", oldState.find("&") + 1) + 1;
        std::string angle = oldState.substr(begin, 2);
        //std::cout << "rewarding state " << worldState << " |" << angle << " with " << reward << std::endl;
        if (angle[0] == 'S')
        {//if a little bit either way
            reward = 100;
        }
        else if (angle[0] == 'L')
        {//if a little bit either way
            reward = -100;
        }
        else if (angle[0] == 'M')
        {
            reward = 0;
        }

        begin = oldState.find_last_of("&");
        std::string speed = oldState.substr(begin);
        //
        //std::cout << speed;
        if (speed[1] == 'S')
        {//if a little bit either way
            reward = reward + 20;
        }
        else
        {
            reward = reward - 20;
        }

        // std::cout << "rewarding state " << oldState << " |" << speed << " with " << reward << std::endl;
    }
    /*reward 0
     int current = atoi(oldState.c_str());
     reward = 0;if (current == -1)
     {
         reward = -2000;
     }
     else
     {//reward 1
         //std::cout << "rewarding state " << oldState << " now " << worldState << std::endl;
         int begin = oldState.find("&", oldState.find("&") + 1) + 1;
         std::string angle = oldState.substr(begin, 2);
         //std::cout << "rewarding state " << worldState << " |" << angle << " with " << reward << std::endl;
         if (angle[0] == 'S')
         {//if a little bit either way
             reward = 1000;
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
     */
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