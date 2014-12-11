/* 
 * File:   QTable.cpp
 * Author: Adam
 * 
 * Created on October 8, 2012, 10:11 AM
 */

#include <map>
#include <string>
#include <vector>
#include "QTable.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <limits>

QTable::QTable()
{
    randomState = "";
}

void QTable::setGamma(double gamma)
{
    if (gamma < 0 || gamma > 1)
    {
        std::cerr << "in Qtable->setgamma out of range\n";
        exit(99);
    }
    this->gamma = gamma;
}

double QTable::getGamma() const
{
    return gamma;
}

void QTable::setAlpha(double alpha)
{
    if (alpha < 0 || alpha > 1)
    {
        std::cerr << "in Qtable->setalpha out of range\n";
        exit(99);
    }
    this->alpha = alpha;
}

double QTable::getAlpha() const
{
    return alpha;
}

std::multimap<std::string, std::pair<std::string, double> > QTable::getQTable() const
{
    return qTable;
}

QTable::~QTable()
{
    qTable.clear();
    visitCount.clear();
    converged.clear();
    qTablePrevious.clear();
    std::multimap<std::string, std::pair<std::string, double> > emptyMultimapSSD;
    std::multimap < std::string, std::pair < std::string, bool> > emptyMultimapSSB;
    std::multimap<std::string, std::pair<std::string, int> > emptyMultimapSSI;
    qTable.swap(emptyMultimapSSD);
    visitCount.swap(emptyMultimapSSI);
    converged.swap(emptyMultimapSSB);
    qTablePrevious.swap(emptyMultimapSSD);
}

/**
 * put in the new state
 * @param stateName its name
 * @param actionName its name
 * @param value how it should start
 */
void QTable::addStateAction(std::string stateName, std::string actionName, double value)
{
    //std::cout << stateName << "\n";
    if (qTable.size() == 0 || randomState.length() < 5)
    {
        randomState = stateName;
    }
    //see if this pair is there already
    std::pair< std::multimap< std::string, std::pair< std::string, double > >::iterator, std::multimap< std::string, std::pair< std::string, double > >::iterator > stateActionsRange = qTable.equal_range(stateName); //get first and last of the actions in that state
    std::multimap< std::string, std::pair< std::string, double> >::iterator it;
    bool valueSet = false;
    if ((stateActionsRange.first) != qTable.end())//&& (stateActionsRange.second) != qTable.end())//the second condition is not nessicary as find can ligitimatly give the end
    {
        for (it = (stateActionsRange.first); it != (stateActionsRange.second); it++)
        {//go through all actions in this state and see if there is a match
            if ((*it).second.first.compare(actionName) == 0)
            {//if they match update the q
                (*it).second.second = value;
                std::cerr << "in Qtable->addstateActin found state there already= " << stateName << " and action= " << actionName << ", added vakue anyway\n";
                valueSet = true;
                break; //it = stateActionsRange.second;
            }

        }
    }
    if (valueSet == false)
    {//if there was no state action by this name already
        std::pair<std::string, double> sub = std::pair<std::string, double>(actionName, value);
        std::pair<std::string, std::pair<std::string, double> > main = std::pair<std::string, std::pair<std::string, double> > (stateName, sub);
        //ADDED for transfer learning action visit count
        std::pair<std::string, int> sub2 = std::pair<std::string, int>(actionName, 0);
        std::pair<std::string, std::pair<std::string, int> > main2 = std::pair<std::string, std::pair<std::string, int> > (stateName, sub2);
        visitCount.insert(main2);
        std::pair < std::string, bool> sub3 = std::pair < std::string, bool>(actionName, false);
        std::pair < std::string, std::pair < std::string, bool> > main3 = std::pair < std::string, std::pair < std::string, bool> > (stateName, sub3);
        converged.insert(main3);
        //END
        qTable.insert(main);
    }

    this->qTablePrevious.clear();
    qTablePrevious.insert(qTable.begin(), qTable.end());
}

void QTable::changeRandomState()
{
    std::multimap<std::string, std::pair<std::string, double> >::iterator qIterator = qTable.begin();
    int a = rand() % qTable.size(), b = 0;
    while (a > b)
    {
        qIterator++;
        b++;
    }
    randomState = (*qIterator).first;
}

/**
 * change the value
 * @param stateName
 * @param actionName
 * @param input what to change it to
 */
void QTable::setQValue(std::string stateName, std::string actionName, double input)
{
    std::pair< std::multimap< std::string, std::pair< std::string, double > >::iterator, std::multimap< std::string, std::pair< std::string, double > >::iterator > stateActionsRange = qTable.equal_range(stateName); //get first and last of the actions in that state
    std::multimap< std::string, std::pair< std::string, double> >::iterator it;
    bool valueSet = false;
    if ((stateActionsRange.first) != qTable.end())//&& (stateActionsRange.second) != qTable.end())//the second condition is not nessicary as find can ligitimatly give the end
    {
        for (it = (stateActionsRange.first); it != (stateActionsRange.second); it++)
        {//go through all actions in this state and see if there is a match
            if ((*it).second.first.compare(actionName) == 0)
            {//if they match update the q
                (*it).second.second = input;

                valueSet = true;
                break; //it = stateActionsRange.second;
            }

        }
    }
    if (valueSet == false)
    {
        std::cerr << "in Qtable->setQValue failed to find state= " << stateName << " and action= " << actionName << "\n";
        exit(11);
    }
}

double QTable::getQValue(std::string stateName, std::string actionName)
{
    std::pair<std::multimap<std::string, std::pair<std::string, double> >::iterator, std::multimap<std::string, std::pair<std::string, double> >::iterator> stateActionsRange = qTable.equal_range(stateName); //get first and last of the actions in that state
    std::multimap<std::string, std::pair<std::string, double> >::iterator it;
    for (it = stateActionsRange.first; it != stateActionsRange.second; it++)
    {//go through all actions in this state and see if there is a match
        if ((*it).second.first.compare(actionName) == 0)
        {//if they match return the q                   
            return (*it).second.second;
        }
    }
    this->writeStateActionToFile("error", "error");
    std::cerr << "in Qtable->getQValue failed to find state= " << stateName << " and action= " << actionName << "\n";
    exit(10);
    return -99999;
}

/*
 *return a not random state
 */
std::string QTable::getRandomState()
{
    return randomState;
}

/**
 * get what can be done in one state
 * @param stateName
 * @return the actions and their values
 */
std::vector<std::pair<std::string, double> > QTable::getActionsFromState(std::string stateName)
{
    std::vector<std::pair<std::string, double> > output;
    std::pair<std::multimap<std::string, std::pair<std::string, double> >::iterator, std::multimap<std::string, std::pair<std::string, double> >::iterator> stateActionsRange = qTable.equal_range(stateName); //get first and last of the actions in that state
    std::multimap<std::string, std::pair<std::string, double> >::iterator it;
    for (it = stateActionsRange.first; it != stateActionsRange.second; it++)
    {//go through all actions and add them
        output.push_back(std::pair<std::string, double>((*it).second.first, (*it).second.second));
    }
    if (output.size() == 0)
    {
        writeStateActionToFile("error", "error");
        std::cerr << "Warning QTable returned a state with zero actions.  It was " << stateName << ".\n";
    }
    //std::cerr<<"number of actions in qtable= "<<output.size()<<"\n";
    return output;
}

/**
 * write a file with the q table in it
 * @param filename
 */
void QTable::writeStateActionToFile(std::string filenameIn, std::string tag)
{
    std::string filename;
    if (tag == "error")
    {//if erro no tag
        filename = filenameIn + "-q.txt.stats";
    }
    else
    {//has a tag
        filename = filenameIn + "-q.txt." + tag + ".stats";
    }

    std::cerr << "writing " << filenameIn << "\n";
    std::ofstream outputfile(filename.c_str());
    //std::cerr<<filename<<"\n";
    if (outputfile.is_open())
    {
        std::multimap<std::string, std::pair<std::string, double> >::iterator qTableIterator = qTable.begin();
        while (qTableIterator != qTable.end())
        {
            outputfile << (*qTableIterator).first << "^^^^" << (*qTableIterator).second.first << "~~~~" << (*qTableIterator).second.second << "\r\n";
            qTableIterator++;
        }
        outputfile << ";;;;;;;;;;"; //eof
        outputfile.close();
    }
    else
    {
        std::cerr << "\nQtable->writeStateActionToFile Unable to open file\n";
        exit(89);
    }
    std::cerr << "done!" << "\n";
}

/**
 * perform the q learning update based on the current alpha and gamma
 * @param previousStateName the state we were in
 * @param actionName the action taken
 * @param currentStateName the state we were in
 * @param value the reward recived for above
 */
void QTable::qLearningUpdate(std::string previousStateName, std::string actionName, std::string currentStateName, double reward)
{
    this->qTablePrevious.clear();
    qTablePrevious.insert(qTable.begin(), qTable.end());
    // std::cout << " in q update " << previousStateName << "  " << actionName << "  " << currentStateName << "  " << reward << "\n";
    //find state to update
    std::pair<std::multimap<std::string, std::pair<std::string, double> >::iterator, std::multimap<std::string, std::pair<std::string, double> >::iterator> stateActionsRange = qTable.equal_range(previousStateName); //get first and last of the actions in that state
    std::multimap<std::string, std::pair<std::string, double> >::iterator it;
    bool foundMatch = false;
    it = stateActionsRange.first;

    while (it != stateActionsRange.second)
    {//go through all actions in this state and see if there is a match
        if ((*it).second.first.compare(actionName) == 0)
        {//if they match do the update      
            // std::cerr << "Qlearning - " << (*it).first << " + " << (*it).second.first << "'s value " << (*it).second.second;
            //q new =                        q old +                         alpha (reward +gamma (max next action)                                                    - q old)
            //ADDED FOR EXP
            double oldValue = (*it).second.second;
            //end
            (*it).second.second = (*it).second.second + alpha * (reward + gamma * (this->getBestAction(currentStateName).second)- (*it).second.second);

            //ADDED FOR EXP
            double newValue = (*it).second.second;
            //end
            //std::cerr << " to " << (*it).second.second << " alpha= " << alpha << " gamma= " << gamma << " reward= " << reward << " qmax+1= " << (this->getBestAction(currentStateName).second) << "\n";

            it = stateActionsRange.second; //end loop if found
            foundMatch = true;
            //ADDED FOR EXP
            std::multimap< std::string, std::pair< std::string, int> >::iterator it1;
            std::pair< std::multimap< std::string, std::pair< std::string, int > >::iterator, std::multimap< std::string, std::pair< std::string, int > >::iterator > visitCountRange = visitCount.equal_range(previousStateName); //get first and last of the actions in that state
            for (it1 = (visitCountRange.first); it1 != (visitCountRange.second); it1++)
            {//go through all actions in this state and see if there is a match
                if ((*it1).second.first.compare(actionName) == 0)
                {//matched
                    (*it1).second.second++; //add to visit count
                }
            }
            //END
            //ADDED FOR EXP
            if (abs(oldValue - newValue) < 1 && reward > 3)
            {//if converged
                std::multimap < std::string, std::pair < std::string, bool> >::iterator it2;
                std::pair < std::multimap < std::string, std::pair < std::string, bool > >::iterator, std::multimap < std::string, std::pair < std::string, bool > >::iterator > convergedRange = converged.equal_range(previousStateName); //get first and last of the actions in that state
                for (it2 = (convergedRange.first); it2 != (convergedRange.second); it2++)
                {//go through all actions in this state and see if there is a match
                    if ((*it2).second.first.compare(actionName) == 0)
                    {//matched
                        (*it2).second.second = true;
                    }
                }
            }
            //END
            //std::cerr<<" learning!!!\n";
        }
        else
        {
            it++;
        }
    }
    if (foundMatch == false)
    {//if there was no match
        std::cerr << "in Qtable->qLearningUpdate failed to find state= " << previousStateName << " and action= " << actionName << "\n";
        exit(10);
    }

}

/**
 * return the best action in the passed state
 * @param stateName
 * @return 
 */
std::pair<std::string, double> QTable::getBestAction(std::string stateName)
{
    std::pair<std::string, double> output;
    std::pair<std::multimap<std::string, std::pair<std::string, double> >::iterator, std::multimap<std::string, std::pair<std::string, double> >::iterator> stateActionsRange = qTable.equal_range(stateName); //get first and last of the actions in that state
    std::multimap<std::string, std::pair<std::string, double> >::iterator it;
    if (stateActionsRange.first != qTable.end())
    {
        for (it = stateActionsRange.first; it != stateActionsRange.second; it++)
        {//go through all actions and add them
            if (it == stateActionsRange.first)
            {//if the first one then it is best
                output = (*it).second;
            }
            else
            {
                if (output.second < (*it).second.second)
                {//this one is better
                    output = (*it).second;
                }
            }
        }
    }
    else
    {//badness
        std::cerr << "couldn't find " << stateName << " to get actions from\n";
        exit(72834729);
    }
    return output;
}

/**
 * read from the output of write
 * @param filenameIn
 */
void QTable::readStateActionFromFile(std::string filenameIn)
{//std::cerr<<"in readStateActionFromFile\n";
    std::string line;
    filenameIn += ".txt";
    std::ifstream myfile(filenameIn.c_str());
    if (myfile.is_open())
    {
        getline(myfile, line);
        while (myfile.good())
        {//while there is stuff to get, get it split it and put it into model
            if (line != ";;;;;;;;;;")
            {//stop it trying to segment eof
                std::string backup = line;
                std::string state = line.substr(0, line.find("^^^^"));
                if (state.length() > 5 && randomState == "")
                {
                    randomState = state; //std::cerr<<"setting random to "<<randomState<<"\n";
                }
                line = backup;
                std::string action = line.substr(line.find("^^^^") + 4);
                action = action.substr(0, action.find("~~~~"));
                line = backup;
                std::string value = line.substr(line.find("~~~~") + 4);
                this->addStateAction(state, action, atof(value.c_str())); //put it in
                getline(myfile, line);
            }
            else
            {//if at my end of file note
                myfile.close();
            }
        }
        myfile.close();
    }
    else
    {
        std::cerr << "QTable  readStateActionFromFile Unable to open file:" << filenameIn << "\n";
        //exit(89);
    }
    //std::cerr<<"end readStateActionFromFile\n";
}

/**
 * get a q table state and action
 * @param state
 * @param action
 * @return 
 */
std::pair<std::string, std::pair<std::string, double> > QTable::getQTableEntry(std::string state, std::string action)
{
    //find state to update
    std::pair<std::multimap<std::string, std::pair<std::string, double> >::iterator, std::multimap<std::string, std::pair<std::string, double> >::iterator> stateActionsRange = qTable.equal_range(state); //get first and last of the actions in that state
    std::multimap<std::string, std::pair<std::string, double> >::iterator it;
    bool foundMatch = false;
    it = stateActionsRange.first;

    while (it != stateActionsRange.second)
    {//go through all actions in this state and see if there is a match
        if ((*it).second.first.compare(action) == 0)
        {//if they match 
            return (*it);
        }
        else
        {
            it++;
        }
    }
    if (foundMatch == false)
    {//if there was no match
        std::cerr << "in Qtable->getQTableEntry failed to find state= " << state << " and action= " << action << "\n";
        exit(10);
    }
    return *stateActionsRange.first;

}

/**
 * set the q value
 * @param input
 */
void QTable::setQValue(std::pair<std::string, std::pair<std::string, double> > input)
{
    this->setQValue(input.first, input.second.first, input.second.second);
}

/**
 *find which state has the greatest differance between argument and this
 *@param the thing to compare to
 */
std::vector<std::string> QTable::getStateOfGreatestChang()
{ //std::cerr<<"state of greatest change"<<std::endl;
    const int arraySize = 4;
    std::string bestSoFar[arraySize] = {""}; //,"","","",""};//,""};
    double bestScore[arraySize] = {-10}; //,-10,-10,-10,-10};//,-10};//as abs will be greater
    std::multimap<std::string, std::pair<std::string, double> >::iterator qTableIterator = this->qTable.begin();
    while (qTableIterator != qTable.end())
    {//for all qs//use search as the two tables are not nessicarilly identical
        double diff = std::numeric_limits<double>::quiet_NaN(); //= abs(qTableIterator->second.second - qTablePrevious->getQValue(qTableIterator->first, qTableIterator->second.first));
        //get the q value in previous
        std::pair<std::multimap<std::string, std::pair<std::string, double> >::iterator, std::multimap<std::string, std::pair<std::string, double> >::iterator> stateActionsRange = qTablePrevious.equal_range(qTableIterator->first); //get first and last of the actions in that state
        std::multimap<std::string, std::pair<std::string, double> >::iterator it;
        for (it = stateActionsRange.first; it != stateActionsRange.second;)
        {//go through all actions in this state and see if there is a match
            //std::cerr<<(*it).second.first<<"  "<<qTableIterator->second.first<<"\n";
            if ((*it).second.first.compare(qTableIterator->second.first) == 0)
            {//if they match return the q                   
                diff = abs(qTableIterator->second.second - (*it).second.second);
                it = stateActionsRange.second;
                //std::cerr<<diff<<" match\n";
            }
            else
            {
                it++;
            }
        }
        if (diff != diff)
        {//is nan
            std::cerr << "couldn't match state action in greatest chang\n";
            exit(883);
        }
        //std::cerr<<diff<<std::endl;
        if (diff > bestScore[0])
        {//if better
            //std::cerr<<"found abest score"<<std::endl;
            bestSoFar[0] = qTableIterator->first;
            bestScore[0] = diff;

            //sort
            for (int a = 0; a < (arraySize - 1); a++)
            {
                if (bestScore[a] > bestScore[a + 1])
                {//if 0 better than 1 swap //keep in order
                    int temp = bestScore[1];
                    std::string name = bestSoFar[a + 1];
                    bestSoFar[a + 1] = bestSoFar[a];
                    bestSoFar[a] = name;
                    bestScore[a + 1] = bestScore[a];
                    bestScore[a] = temp;
                }
            }
        }
        qTableIterator++;
    }
    //std::cerr<<bestSoFar<<"\n";
    std::vector<std::string> out;
    for (int a = 0; a < arraySize; a++)
    {
        out.push_back(bestSoFar[a]);
    }
    return out;

}
//ADDED FOE EXP

/**
 *find which state has the greatest differance between argument and this
 *@param the thing to compare to
 */
std::vector<std::string> QTable::getStateOfMostVisit()
{
    const int arraySize = 1;
    std::string bestSoFar[arraySize];
    int bestScore[arraySize]; //as abs will be greater
    for (int a = 0; a < arraySize; a++)
    {
        bestSoFar[a] = "";
        bestScore[a] = -10;
    }
    std::multimap<std::string, std::pair<std::string, int> >::iterator qTableIterator = this->visitCount.begin();
    while (qTableIterator != visitCount.end())
    {//for all qs//use search as the two tables are not nessicarilly identical
        if (bestScore[0] < qTableIterator->second.second)
        {//if might be worth sending see if is converged
            /*    std::pair<std::multimap<std::string, std::pair<std::string, bool> >::iterator,std::multimap<std::string, std::pair<std::string, bool> >::iterator> range=converged.equal_range(qTableIterator->first);
            std::multimap<std::string, std::pair<std::string, bool> >::iterator it=range.first;
            while(it!=range.second)
            {//for all in a  state
            if((*it).second.first==qTableIterator->second.first)
            {//if actions match
            break;
            }
            else
            {
            it++;
            }
            }
            if((*it).second.second==true)//for converged only turn on
             */
            {//is converged
                bestSoFar[0] = qTableIterator->first;
                bestScore[0] = qTableIterator->second.second;
                //sort
                for (int a = 0; a < (arraySize - 1); a++)
                {
                    if (bestScore[a] > bestScore[a + 1])
                    {//if 0 better than 1 swap //keep in order
                        int temp = bestScore[1];
                        std::string name = bestSoFar[a + 1];
                        bestSoFar[a + 1] = bestSoFar[a];
                        bestSoFar[a] = name;
                        bestScore[a + 1] = bestScore[a];
                        bestScore[a] = temp;
                    }
                }
            }
        }
        qTableIterator++;
    }
    //std::cerr<<bestSoFar<<"\n";
    std::vector<std::string> out;
    for (int a = 0; a < arraySize; a++)
    {
        out.push_back(bestSoFar[a]);
    }
    return out;

}

/**
 *find which state has the greatest differance between argument and this
 *@param the thing to compare to
 */
std::vector<std::string> QTable::getStateOfManyVisit()
{
    const int arraySize = 1;
    const int visitMin = 10;
    std::string bestSoFar[arraySize];
    int bestScore[arraySize]; //as abs will be greater
    for (int a = 0; a < arraySize; a++)
    {
        bestSoFar[a] = "";
        bestScore[a] = -10;
    }
    std::multimap<std::string, std::pair<std::string, int> >::iterator qTableIterator = this->visitCount.begin();
    while (qTableIterator != visitCount.end())
    {//for all qs//use search as the two tables are not nessicarilly identical
        if (bestScore[0] < qTableIterator->second.second && qTableIterator->second.second > visitMin)
        {//if might be worth sending see if is converged
            /*     std::pair<std::multimap<std::string, std::pair<std::string, bool> >::iterator,std::multimap<std::string, std::pair<std::string, bool> >::iterator> range=converged.equal_range(qTableIterator->first);
            std::multimap<std::string, std::pair<std::string, bool> >::iterator it=range.first;
            while(it!=range.second)
            {//for all in a  state
            if((*it).second.first==qTableIterator->second.first)
            {//if actions match
            break;
            }
            else
            {
            it++;
            }
            }
            if((*it).second.second==true)//for converged only turn on
             */
            {//is converged
                bestSoFar[0] = qTableIterator->first;
                bestScore[0] = qTableIterator->second.second;
                //sort
                for (int a = 0; a < (arraySize - 1); a++)
                {
                    if (bestScore[a] > bestScore[a + 1])
                    {//if 0 better than 1 swap //keep in order
                        int temp = bestScore[1];
                        std::string name = bestSoFar[a + 1];
                        bestSoFar[a + 1] = bestSoFar[a];
                        bestSoFar[a] = name;
                        bestScore[a + 1] = bestScore[a];
                        bestScore[a] = temp;
                    }
                }
            }
        }
        qTableIterator++;
    }
    //std::cerr<<bestSoFar<<"\n";
    std::vector<std::string> out;
    for (int a = 0; a < arraySize; a++)
    {
        out.push_back(bestSoFar[a]);
    }
    return out;

}

/**
 *find which state has the greatest differance between argument and this
 *@param the thing to compare to
 */
std::vector<std::string> QTable::getBestStates()
{
    const int arraySize = 4;
    std::string bestSoFar[arraySize];
    int bestScore[arraySize]; //as abs will be greater
    for (int a = 0; a < arraySize; a++)
    {
        bestSoFar[a] = "";
        bestScore[a] = -20;
    }
    std::multimap<std::string, std::pair<std::string, double> >::iterator qTableIterator = this->qTable.begin();
    while (qTableIterator != qTable.end())
    {//for all qs//use search as the two tables are not nessicarilly identical
        /* if (bestScore[0] < qTableIterator->second.second)
        {//if might be worth sending see if is converged
        std::pair<std::multimap<std::string, std::pair<std::string, bool> >::iterator,std::multimap<std::string, std::pair<std::string, bool> >::iterator> range=converged.equal_range(qTableIterator->first);
        std::multimap<std::string, std::pair<std::string, bool> >::iterator it=range.first;
        while(it!=range.second)
        {//for all in a  state
        if((*it).second.first==qTableIterator->second.first)
        {//if actions match
        break;
        }
        else
        {
        it++;
        }
        }
        if((*it).second.second==true)//for converged only turn on
         */
        {//is converged
            bestSoFar[0] = qTableIterator->first;
            bestScore[0] = qTableIterator->second.second;
            //sort
            for (int a = 0; a < (arraySize - 1); a++)
            {
                if (bestScore[a] > bestScore[a + 1])
                {//if 0 better than 1 swap //keep in order
                    int temp = bestScore[1];
                    std::string name = bestSoFar[a + 1];
                    bestSoFar[a + 1] = bestSoFar[a];
                    bestSoFar[a] = name;
                    bestScore[a + 1] = bestScore[a];
                    bestScore[a] = temp;
                }
            }
        }
        //}
        qTableIterator++;
    }
    //std::cerr<<bestSoFar<<"\n";
    std::vector<std::string> out;
    for (int a = 0; a < arraySize; a++)
    {
        out.push_back(bestSoFar[a]);
    }
    return out;

}
//END

void QTable::addFromQTable(std::multimap<std::string, std::pair<std::string, double> > in)
{//std::cerr<<"about to add form Qtable\n"<<std::endl;
    std::multimap<std::string, std::pair<std::string, double> >::iterator inputIterator = in.begin();
    while (inputIterator != in.end())
    {
        this->qTable.insert(*inputIterator);
        inputIterator++;
    }
    //std::cerr<<"added form Qtable\n"<<std::endl;
}

int QTable::getVisitCount(std::string stateName, std::string actionName)
{
    std::pair<std::multimap<std::string, std::pair<std::string, int> >::iterator, std::multimap<std::string, std::pair<std::string, int> >::iterator> stateActionsRange = visitCount.equal_range(stateName); //get first and last of the actions in that state
    std::multimap<std::string, std::pair<std::string, int> >::iterator it;
    for (it = stateActionsRange.first; it != stateActionsRange.second; it++)
    {//go through all actions in this state and see if there is a match
        if ((*it).second.first.compare(actionName) == 0)
        {//if they match return the q                   
            return (*it).second.second;
        }
    }
    this->writeStateActionToFile("error", "error");
    std::cerr << "in Qtable->getVisitcount failed to find state= " << stateName << " and action= " << actionName << "\n";
    exit(10);
    return -99999;

}

/**
 * return some random states
 * @return 
 */
std::vector<std::string> QTable::getStateOfRandom()
{
    int arraySize = 2;
    std::vector<std::string> out;
    for (int a = 0; a < arraySize; a++)
    {
        std::multimap<std::string, std::pair<std::string, double> >::iterator tableIterator = qTable.begin();
        int number = (rand() % qTable.size());
        for (int b = 0; b < number; b++)
        {
            tableIterator++;
        }
        out.push_back((*tableIterator).first);

    }

    return out;
}