/* 
 * File:   CollaborationProcess.cpp
 * Author: Adam
 * 
 * Created on July 12, 2012, 11:46 AM
 */

#include "CollaborationProcess.h"
#include "WLearningProcess.h"
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <sstream>

/**
 * 
 * @param d_ID
 * @param numberOfStates how many intervals to split c into
 * @param maxRange how high should c go
 */
CollaborationProcess::CollaborationProcess(std::string name, int numberOfStates, double maxRange) : WLearningProcess(name, numberOfStates, maxRange)
{
        numberOfRuns = 0;
        //std::cerr<<"created colab policy\n";
        changeBy = maxRange / numberOfStates; //save this for executing
        maxValue = maxRange;

}

CollaborationProcess::~CollaborationProcess()
{
}

/**
 * overload that of wprocess as this is somewhat limited and needs to perform action selection
 * @param worldState
 * @param rewardValueIn
 * @param winningAction
 */
void CollaborationProcess::update(std::string worldState, double rewardValueIn, bool winningAction)
{
        WLearningProcess::update(worldState, this->getCurrentAction(), rewardValueIn, true);
        
        //force action execution
        if (WLearningProcess::getCurrentAction().compare( "+")==0)
        {//if +
                if ((atof(this->getCurrentState().c_str()) + changeBy) <= maxValue)
                {//if resonable to +
                        std::stringstream ss;
                        ss << (atof(this->getCurrentState().c_str()) + changeBy); //new state is current + diff
                        WLearningProcess::setCurrentState(ss.str());
                }              
                        
        }
        else if (WLearningProcess::getCurrentAction().compare( "-")==0)
        {//if +
                if ((atof(this->getCurrentState().c_str()) - changeBy) >= 0)
                {//if resonable to -
                        std::stringstream ss;
                        ss << (atof(this->getCurrentState().c_str()) - changeBy); //new state is current + diff
                        WLearningProcess::setCurrentState(ss.str());
                }              
                        
        }
        else
        {//must be 0 do nothing
        }
        WLearningProcess::setCurrentAction(WLearningProcess::generateActionSuggestion().first);
}