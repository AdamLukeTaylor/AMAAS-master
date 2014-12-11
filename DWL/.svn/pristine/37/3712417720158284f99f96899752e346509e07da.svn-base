#include "Boltzmann.h"
#include <time.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <float.h>
using std::vector;

#include <iostream>
using std::cerr;

Boltzmann::Boltzmann()
{
    srand((unsigned) time(0));
}

Boltzmann::~Boltzmann()
{
}

std::string Boltzmann::calculateNextAction(std::vector<std::pair<std::string, double> > possibleActions)
{
    std::string output = "";
    random_shuffle(possibleActions.begin(), possibleActions.end());
    if (possibleActions.empty())
    {

        cerr << "No actions to select from...exiting...";
        exit(1);
    }

    random_shuffle(possibleActions.begin(), possibleActions.end());
    double probSum = 0;
    double randNumber = (double) rand() / RAND_MAX;
    std::vector<std::pair<std::string, double> >::iterator actionIterator = possibleActions.begin();
    while (actionIterator != possibleActions.end())
    {
        std::cout << (*actionIterator).first << " has q= " << (*actionIterator).second;
        (*actionIterator).second = exp((*actionIterator).second / temperature); //get this contrabution to boltz
        std::cout << " it gets a weight of " << (*actionIterator).second << "\n";
        if ((*actionIterator).second == 0)
        {//if one is zero weighted
            std::cout << "error 1 in action selsction\n";
            exit(674);
        }
        if (isinf((*actionIterator).second))
        {//if is infinity
            std::cout << "error 2 in action selsction\n";
            exit(674);
        }
        probSum += (*actionIterator).second;

        actionIterator++;
    }
    std::cout << "prob sum= " << probSum << "\n";
    if (probSum == 0)
    {//if divide by zero
        std::cout << "prob sum ==0 which is bad because of maths\n";
    }
    //now allocate their areas of spectrum
    actionIterator = possibleActions.begin();
    double runningTotal = 0;
    bool found = false;
    while (actionIterator != possibleActions.end())
    {
        double temp = (*actionIterator).second / probSum;
        (*actionIterator).second = runningTotal; //set it so it is action 1+ action 2 +...+action n=1
        runningTotal += temp; //now accumulate for start of next
        if (found == false && randNumber <= runningTotal)
        {//if rand falls inside this actions range
            std::cout << "rand= " << randNumber << " falls into " << (*actionIterator).first << "'s range begining at " << ((*actionIterator).second) << " ending at " << (*actionIterator).second + temp << "\n";
            //return (*actionIterator).first;//for release
            output = (*actionIterator).first; //for debug
            found = true;
        }
        else
        {
            std::cout << "rand= " << randNumber << " not in " << (*actionIterator).first << "'s range begining at " << ((*actionIterator).second) << " ending at " << (*actionIterator).second + temp << "\n";

        }

        actionIterator++;
    }
    if (found)
    {
        return output;
    }
    if (runningTotal > 1)
    {
        std::cout << "accumulation failed in boltzman giving total= " << runningTotal << "\n";
        exit(35872);
    }
    std::cout << "failed to find action to return in boltzman " << "\n";
    exit(35873);
    return "brokenness";
}

double Boltzmann::getTemperature()
{

    return temperature;
}

void Boltzmann::setTemperature(double tmp)
{

    if (tmp > 0.0 && tmp < 1000)
    {
        temperature = tmp;
        //cerr<<"\nNew Boltzmann Temp : "<< temperature<<"\n" ;
    }
    else if (tmp > 1000)
    {
        temperature = 1000;
        //cerr<<"\nNew Boltzmann Temp : "<< temperature<<"\n" ;
    }
    else
    { //	cerr<<"Boltzmann Temperature cannot be Zero....Setting Temperature to 1.0 \n";
        temperature = 1.0;
    }

}
