/* 
 * File:   MtCarAgent.cpp
 * Author: Adam
 * 
 * Created on 24 June 2014, 11:53
 */

#include "MtCarAgent.h"
#include "Constants.h"
#include "RewardMtCar.h"
#include <string>
#include <sstream>

MtCarAgent::MtCarAgent(std::string name) : DWLAgent(name)
{
    this->setUsingTransferLearning(true);

    //std::cerr << "Creating Agent " << this->getName() << std::endl;

    // instantiate the specific Reward for the policy which you have already defined
    RewardMtCar* rewardMtCar = new RewardMtCar();

    // add the local policy to the agent and return a handle to it for creating the state space 
    std::string policyName = std::string(this->getName() + "+MtCar");
    Policy* mtCarPolicy = this->addLocalPolicy(policyName, rewardMtCar);




    std::string carPos[4] = {"1", "2", "3", "4"}; //left right centre
    std::string carSpeed[4] = {"FL", "FR", "SL", "SR"}; //fast left, slow left
    std::string actions[3] = {"0", "1", "-1"};

    //for state
    for (int a = 0; a < sizeof (carPos) / sizeof ( *carPos); a++)
    {
        for (int b = 0; b < sizeof (carSpeed) / sizeof ( *carSpeed); b++)
        {
            for (int c = 0; c < sizeof (actions) / sizeof ( *actions); c++)
            {//for all the actions
                std::stringstream ss;
                ss.str("");
                ss << carPos[a] << "&" << carSpeed[b];
                ((WLearningProcess*) mtCarPolicy)->addStateAction(ss.str(), actions[c]);
            }
        }
    }


    //add goal states
    ((WLearningProcess*) mtCarPolicy)->addStateAction("0", actions[0]);
    ((WLearningProcess*) mtCarPolicy)->addStateAction("0", actions[1]);
    ((WLearningProcess*) mtCarPolicy)->addStateAction("0", actions[2]);
    int boltzTemp = 10000; //BOTLZTEMP

    ((WLearningProcess*) mtCarPolicy)->setBoltzmannTemperature(boltzTemp);
    ((WLearningProcess*) mtCarPolicy)->setUseBoltzmann(true);

    ((WLearningProcess*) mtCarPolicy)->setQAlpha(.8);
    ((WLearningProcess*) mtCarPolicy)->setQGamma(.4);

}

MtCarAgent::~MtCarAgent()
{
}

