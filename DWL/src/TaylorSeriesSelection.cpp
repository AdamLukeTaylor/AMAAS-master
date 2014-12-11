#include "TaylorSeriesSelection.h"
#include <time.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <float.h>
using std::vector;

#include <iostream>
using std::cerr;

TaylorSeriesSelection::TaylorSeriesSelection()
{
    srand((unsigned) time(0));
}

TaylorSeriesSelection::~TaylorSeriesSelection()
{
}

std::string TaylorSeriesSelection::calculateNextAction(std::vector<std::pair<std::string, double> > possibleActions)
{
    std::string output = "";
    random_shuffle(possibleActions.begin(), possibleActions.end());
    if (possibleActions.empty())
    {

        cerr << "No actions to select from...exiting...";
        exit(1);
    }

    else
    {
        double randomProb = 1.0 - (double) (1.0 / temperature);

        double randomDraw = (double) rand() / RAND_MAX;
        //std::cout << "rand= " << randomDraw << " place= " << randomProb << " temprature= " << temperature << std::endl;
        if (randomDraw < randomProb && temperature > 1)
        {//chose randomly
            //std::cout << "in random\n";
            std::vector<std::pair<std::string, double> > ordered;
            //sort them best to worst
            while (possibleActions.size() > 0)
            {//while some to order
                //std::cout << "1" << std::endl;

                std::vector<std::pair<std::string, double> >::iterator currentBestAction;
                std::vector<std::pair<std::string, double> >::iterator actionIterator = possibleActions.begin();
                double currentBest = (*actionIterator).second;
                currentBestAction = actionIterator; //save a copy
                actionIterator++;
                while (actionIterator != possibleActions.end())
                {
                    //  std::cout << "2" << std::endl;
                    if ((*actionIterator).second >= currentBest)
                    {//is better
                        //     std::cout << "3" << std::endl;
                        currentBest = (*actionIterator).second;
                        currentBestAction = actionIterator; //save a copy
                    }
                    actionIterator++;
                }
                //done now move the best
                ordered.push_back((*currentBestAction));
                possibleActions.erase(currentBestAction);
            }
            std::vector<int> hat;
            //now sorted weighted from rest
            for (int a = 0; a < ordered.size(); a++)//set a=1 to start if you want more randomness
            {//put possitions in hat
                for (int b = 0; b < (ordered.size() - a); b++)
                {//put elements possition in hat more for higher place
                    hat.push_back(a);
                }
            }
            // std::cout << "4" << std::endl;
            random_shuffle(hat.begin(), hat.end()); //shake it much like a poleroid picture
            int draw = hat[0]; //something from nicely? shuffeled hat
            output = ordered[draw].first;
            //std::cout << "drew " << draw << "th element which is " << output << "\n";

        }
        else
        {//choose best
            //std::cout << "in not random\n";
            std::vector<std::pair<std::string, double> >::iterator actionIterator = possibleActions.begin();
            double currentBest = (*actionIterator).second;
            output = (*actionIterator).first;
            actionIterator++;
            while (actionIterator != possibleActions.end())
            {
                if ((*actionIterator).second >= currentBest)
                {//is better

                    currentBest = (*actionIterator).second;
                    output = (*actionIterator).first;
                }
                actionIterator++;
            }
        }
    }
    return output;
}

double TaylorSeriesSelection::getTemperature()
{

    return temperature;
}

void TaylorSeriesSelection::setTemperature(double tmp)
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
