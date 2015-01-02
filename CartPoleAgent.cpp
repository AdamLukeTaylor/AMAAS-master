/* 
 * File:   CartPoleAgent.cpp
 * Author: Adam
 * 
 * Created on 17 June 2014, 14:17
 */

#include "CartPoleAgent.h"
#include "Constants.h"
#include "RewardCartPole.h"
#include <string>
#include <sstream>

CartPoleAgent::CartPoleAgent(std::string name) : DWLAgent(name)
{
    this->setUsingTransferLearning(true);

    std::cerr << "Creating Agent " << this->getName() << std::endl;

    // instantiate the specific Reward for the policy which you have already defined
    rewardCartPole = new RewardCartPole();

    // add the local policy to the agent and return a handle to it for creating the state space 
    std::string policyName = std::string(this->getName() + "+CartPole");
    Policy* cartPolePolicy = this->addLocalPolicy(policyName, rewardCartPole);




    std::string cartPos[3] = {"L", "R", "C"}; //left right centre
    std::string cartSpeed[4] = {"FL", "FR", "SL", "SR"}; //fast left, slow left
    std::string polePos[6] = {"LL", "ML", "SL", "LR", "MR", "SR",}; //large lest, medium l, small l
    std::string poleSpeed[4] = {"FL", "FR", "SL", "SR"}; //fast left, slow left
    std::string actions[2] = {"0", "1"};

    //for boxes
    /* for (int a = 0; a < Constants::N_BOXES; a++)
     {//for all the boxes
         for (int b = 0; b < 2; b++)
         {//for all the actions
             std::stringstream ss;
             ss << a;
             ((WLearningProcess*) cartPolePolicy)->addStateAction(ss.str(), actions[b]);
         }
     }
     //add fail states
     ((WLearningProcess*) cartPolePolicy)->addStateAction("-1", actions[0]);
     ((WLearningProcess*) cartPolePolicy)->addStateAction("-1", actions[1]);*/
    //for state
    for (int a = 0; a < sizeof (cartPos) / sizeof ( *cartPos); a++)
    {
        for (int b = 0; b < sizeof (cartSpeed) / sizeof ( *cartSpeed); b++)
        {
            for (int c = 0; c < sizeof (polePos) / sizeof ( *polePos); c++)
            {
                for (int d = 0; d < sizeof (poleSpeed) / sizeof ( *poleSpeed); d++)
                {
                    for (int e = 0; e < 2; e++)
                    {//for all the actions
                        std::stringstream ss;
                        ss.str("");
                        ss << cartPos[a] << "&" << cartSpeed[b] << "&" << polePos[c] << "&" << poleSpeed[d];
                        ((WLearningProcess*) cartPolePolicy)->addStateAction(ss.str(), actions[e]);
                    }
                }
            }
        }
    }
    //add fail states
    ((WLearningProcess*) cartPolePolicy)->addStateAction("-1", actions[0]);
    ((WLearningProcess*) cartPolePolicy)->addStateAction("-1", actions[1]);
    int boltzTemp = 10000; //BOTLZTEMP

    ((WLearningProcess*) cartPolePolicy)->setBoltzmannTemperature(boltzTemp);
    ((WLearningProcess*) cartPolePolicy)->setUseBoltzmann(true);

    ((WLearningProcess*) cartPolePolicy)->setQAlpha(.8);
    ((WLearningProcess*) cartPolePolicy)->setQGamma(.2);

}

CartPoleAgent::~CartPoleAgent()
{
}

