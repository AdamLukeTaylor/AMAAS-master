/* 
 * File:   EGready.cpp
 * Author: Adam
 * 
 * Created on July 16, 2013, 9:49 AM
 */

#include "EGready.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

EGready::EGready()
{
    srand((unsigned) time(0));
    e = .1;
}

void EGready::setE(double e)
{
    this->e = e;
}

double EGready::getE() const
{
    return e;
}

std::string EGready::calculateNextAction(std::vector<std::pair<std::string, double> > possibleActions)
{//std::cerr << "1 "  <<  std::endl;
    std::string toReturn;
    if (possibleActions.empty())
    {

        std::cerr << "No actions to select from...exiting...";
        exit(1);
    }
    else
    {//test an e
        //find best action
        std::vector<std::pair<std::string, double> > copy = possibleActions;
        std::vector<std::pair<std::string, double> >::iterator actionsIterator = copy.begin();
        double bestValue = (*actionsIterator).second;
        std::vector<std::pair<std::string, double> >::iterator currentBest = actionsIterator;
        //std::cerr << "action  " <<(*actionsIterator).first<<"  "<<(*actionsIterator).second<<"\n";
        //actionsIterator++;std::cerr << "2 "  <<  std::endl;
        while (actionsIterator != copy.end())
        {//for each action
            //std::cerr << "3 "  <<  std::endl;
            // std::cerr << "action  " <<(*actionsIterator).first<<"  "<<(*actionsIterator).second<<"\n";
            if (bestValue < (*actionsIterator).second)
            {//if this one is better
                //std::cerr << "4 "  <<  std::endl;
                bestValue = (*actionsIterator).second;
                currentBest = actionsIterator;
            }
            actionsIterator++;
        }
        // std::cerr << "found best it was " << currentBest->first << "\n";
        //std::cerr << "5 "  <<  std::endl;
        //now we have best
        if ((double) rand() / (RAND_MAX) < e)
        {//if we drew a double smaller than e take a random action that is not best
            //std::cerr << "6 "  <<  std::endl;
            //std::cerr << "random action size =" << (copy.size()) << "\n";
            toReturn = copy.at(rand() % (copy.size())).first;
            // std::cerr << "1 ";
            if (toReturn == (*currentBest).first && copy.size() < 2)
            {//if we happened on the best try again
                //std::cerr << "2 ";
                //std::cerr << "7 "  <<  std::endl;
                toReturn = copy.begin()->first;
                //std::cerr << "3 ";
            }
        }
        else
        {//else the best
            // std::cerr << "best action ";
            toReturn = (*currentBest).first;
        }

    }
    //std::cerr <<toReturn<< " selcted\n ";
    return toReturn;

}
