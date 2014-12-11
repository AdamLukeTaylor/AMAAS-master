/* 
 * File:   WTable.cpp
 * Author: Adam
 * 
 * Created on October 8, 2012, 10:11 AM
 */

#include <map>
#include <string>
#include <vector>
#include "WTable.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cerrno>

WTable::WTable(std::string idIn)
{
    ID = idIn;
}

WTable::WTable()
{//probably only used in mappings
    ID = "dummy";
}

void WTable::setGamma(double gamma)
{
    if (gamma < 0 || gamma > 1)
    {
        std::cerr << "in Wtable->setgamma out of range\n";
        exit(99);
    }
    this->gamma = gamma;
}

double WTable::getGamma() const
{
    return gamma;
}

void WTable::setAlpha(double alpha)
{
    if (alpha < 0 || alpha > 1)
    {
        std::cerr << "in Wtable->setalpha out of range\n";
        exit(99);
    }
    this->alpha = alpha;
}

double WTable::getAlpha() const
{
    return alpha;
}

void WTable::setID(std::string ID)
{
    this->ID = ID;
}

std::string WTable::getID() const
{
    return ID;
}

std::pair<std::string, double> WTable::getWTableEntry(std::string input)
{
    std::pair<std::string, double> output;
    output.first = input;
    output.second = wTable.find(input)->second;
    return output;
}

std::map<std::string, double> WTable::getWTable() const
{
    return wTable;
}

WTable::~WTable()
{
    std::map<std::string, double > emptyMapSD;
    wTable.clear();
    wTable.swap(emptyMapSD);
}

std::vector<std::string> WTable::getStateOfSomething()
{
    std::vector<std::string> output;
    output.push_back(wTable.begin()->first);
    return output;
}

/**
 *find which state has the greatest differance between argument and this
 *@param the thing to compare to
 */
std::vector<std::string> WTable::getStatesOfHighest()
{
    const int arraySize = 2;
    std::string bestSoFar[arraySize];
    int bestScore[arraySize]; //as abs will be greater
    for (int a = 0; a < arraySize; a++)
    {
        bestSoFar[a] = "";
        bestScore[a] = -99999;
    }
    std::map<std::string, double>::iterator wTableIterator = this->wTable.begin();
    while (wTableIterator != wTable.end())
    {//for all ws
        if (bestScore[0] < wTableIterator->second)
        {//is converged
            bestSoFar[0] = wTableIterator->first;
            bestScore[0] = wTableIterator->second;
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

        wTableIterator++;
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
 *get best highest w value
 */
std::vector<std::string> WTable::getStateOfHighestW()
{
    std::vector<std::string> output;
    std::map<std::string, double>::iterator wTableIterator = wTable.begin();
    std::string toOutput = "";
    double bestScore = -999999;
    while (wTableIterator != wTable.end())
    {//go through all and check for best
        std::pair<std::string, double> test = *wTableIterator;
        if (test.second > bestScore)
        {
            bestScore = test.second;
            toOutput = test.first;
        }
        wTableIterator++;
    }
    if (toOutput == "")
    {
        std::cout << "in wtable get highest state- failed to find a state\n";
        exit(382247);
    }
    output.push_back(toOutput);
    return output;
}

/**
 * put in the new state
 * @param stateName its name
 * @param value how it should start
 */
void WTable::addState(std::string stateName, double value)
{
    /*std::pair<std::string, double > main = std::pair<std::string, double> (stateName, value);
    if (wTable.find(stateName) != wTable.end())
    {//see if there already
    //if not add it
    wTable.insert(main);

    }
    else if(
    {//updqate it
    std::cerr<<"WTable add state " <<stateName<<" was there so changed value\n";

    wTable.find(stateName)->second=value;

    }
     */
    if (wTable.find(stateName) != wTable.end())
    {//if the state is there already
        //std::cerr<<"WTable add state " <<stateName<<" was there so changed value from "<<wTable.find(stateName)->second<<" to "<<value<<"\n";
        wTable.find(stateName)->second = value;

    }
    else
    {//if it is not
        std::pair<std::string, double > main = std::pair<std::string, double> (stateName, value);
        //std::cerr<<"WTable add state " <<stateName<<" was missing so addeded it with "<<value<<"\n";
        wTable.insert(main);
    }


}

/**
 * change the value
 * @param stateName
 * @param input what to change it to
 */
void WTable::setWValue(std::string stateName, double input)
{
    wTable.find(stateName)->second = input;
}

/**
 * 
 * @param stateName
 * @return the w value
 */
double WTable::getWValue(std::string stateName)
{

    return wTable.find(stateName)->second;
}

/**
 * write a file with the w table in it
 * @param filename
 */
void WTable::writeStateActionToFile(std::string filenameIn, std::string tag)
{
    std::string filename;
    if (tag == "error")
    {
        filename = filenameIn + "-w.txt.stats";
    }
    else
    {
        filename = filenameIn + "-w.txt." + tag + ".stats";
    }
    //std::cerr<<filenameIn<<"=write filename\n";
    std::ofstream outputfile(filename.c_str());
    if (outputfile.is_open())
    {
        std::map<std::string, double>::iterator wTableIterator = wTable.begin();
        while (wTableIterator != wTable.end())
        {
            outputfile << (*wTableIterator).first << "^^^^" << (*wTableIterator).second << "\r\n";
            wTableIterator++;
        }
        outputfile << ";;;;;;;;;;"; //eof
        outputfile.close();
    }
    else
    {
        std::cerr << "Wtable->writeStateActionToFile Unable to open file\n";
        //exit(89);
    }
}

/**
 * perform the w learning update based on the current alpha and gamma
 * @param previousStateName the state we were in
 * @param maxQForCurrentState the q value for the state we are in's successors
 * @param value the reward recived for above
 */
void WTable::wLearningUpdate(std::string previousStateName, double reward, double maxQForCurrentState, double qForCurrentStateLoosingAction)
{
    //find state to update        
    if (wTable.find(previousStateName) != wTable.end())
    {//if the state exixts
        //std::cerr<<"WLearning - was "<<wTable.find(previousStateName)->second;
        wTable.find(previousStateName)->second += (alpha * (qForCurrentStateLoosingAction - reward - (gamma * maxQForCurrentState) - wTable.find(previousStateName)->second));
        //std::cerr<<" now "<<wTable.find(previousStateName)->second<<" using alpha= "<<alpha<<" reward= "<<reward<<" gamma= "<<gamma<<" maxQ= "<<maxQForCurrentState<<"\n";
    }
    else
    {//if there was no match
        std::cerr << "in Wtable->wLearningUpdate failed to find state= " << previousStateName << "\n";
        exit(10);
    }

}

/**
 * read from the output of write
 * @param filenameIn
 */
bool WTable::readStateActionFromFile(std::string filenameIn)
{
    std::string line;
    filenameIn += ".txt";
    //std::cerr<<filenameIn<<"= read filename\n";
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
                line = backup;
                std::string value = line.substr(line.find("^^^^") + 4);
                value = value.substr(0, value.find("~~~~"));
                this->addState(state, atof(value.c_str())); //put it in
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
        std::cerr << "WTable  readStateActionFromFile Unable to open file" << "\n";
        // exit(89);
        return false;
    }
    return true;
}