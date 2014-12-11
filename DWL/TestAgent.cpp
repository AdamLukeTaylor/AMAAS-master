/*
 * WaterHeaterAgent.cpp
 *
 *  Created on: 16 Jul 2012
 *      Author: charris
 */

#include "DWLAgent.h"
#include "TestAgent.h"
#include "TestReward.h"
#include "Policy.h"
#include "WLearningProcess.h"

#include <string>

/** charris
 *  This example class inherits from DWLAgent and is used to defines the agents
 *  policies.
 *
 *
 */

TestAgent::TestAgent(std::string id) : DWLAgent(id)
{

        //

        // instantiate the specific Reward for the policy which you have already defined
        TestReward* tReward1 = new TestReward();

        // add the local policy to the agent and return a handle to it for creating the state space??
        std::string policyName = std::string(id);
        Policy* testPolicy = this->addLocalPolicy(policyName, tReward1);
        this->manageLearning(true,true);//do this so that the bools controling learnign are set on
        // generate the policy state action space
        std::string states[10] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
        std::string actions[2] = {"[", "]"};
        std::cout<<"about to add states"<<"\n";
        for (int a = 0; a < 10; a++)
        {//for all the states
                for (int b = 0; b < 2; b++)
                {//for all the actions
                        //put them into the state space
                        // down cast to the type of Policy for dealing with it's specifics
                        ((WLearningProcess*) testPolicy)->addStateAction(states[a], actions[b]);

                }
        }
}

TestAgent::~TestAgent()
{
        // TODO Auto-generated destructor stub
}


