/* 
 * File:   TransferMapping.cpp
 * Author: Adam
 * 
 * Created on November 5, 2012, 3:42 PM
 */

#include "TransferMapping.h"
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <climits> 
#include <string.h>

TransferMapping::TransferMapping()
{
    deletedCount = 0;
    sourceSet = false;
    targetSet = false;
    wSourceSet = false;
    wTargetSet = false;
    sourceName = "";
    TargetName = "";
}

TransferMapping::~TransferMapping()
{
    //std::cout << "t1" << std::endl;
    if (sourceSet)
    {
        delete source;
    }
    //std::cout << "t2" << std::endl;
    if (targetSet)
    {
        delete target;
    }
    //std::cout << "t3" << std::endl;
    if (wSourceSet)
    {
        delete wSource;
    }
    //std::cout << "t4" << std::endl;
    if (wTargetSet)
    {
        delete wTarget;
    }
    //std::cout << "t5" << std::endl;
    mappings.clear(); //this is source state+action, target state+action, scaling factor for new info
    //std::cout << "t6" << std::endl;
    votes.clear(); //if it should change a mapping
    //std::cout << "t7" << std::endl;
    sourceUnallocatedPool.clear();
    matchless.clear();
    deleted.clear();
    sourceAllocatedPool.clear();


    //free memory
    std::map<std::pair<std::string, std::string>, int> emptyMapSSI;
    std::multimap<std::string, std::pair<std::string, double> > emptyMultimapSSD;
    std::multimap<std::string, std::string> emptyMultimapSS;
    std::vector<std::string> emptyVectorS;
    sourceAllocatedPool.swap(emptyVectorS);
    sourceUnallocatedPool.swap(emptyVectorS);
    matchless.swap(emptyVectorS);
    deleted.swap(emptyMultimapSS);
    votes.swap(emptyMapSSI);
    mappings.swap(emptyMultimapSSD);

}

void TransferMapping::setTarget(QTable* target)
{
    this->target = target;
    targetSet = true;
}

QTable* TransferMapping::getTarget() const
{
    return target;
}

void TransferMapping::setSource(QTable* source)
{
    this->source = source;
    sourceSet = true;
}

/**
 * load a previous mapping source to target
 * @param filename where to look
 */
void TransferMapping::loadMapping(std::string filename)
{

    //clear the old crap
    mappings.clear(); //this is source state+action, target state+action, scaling factor for new info
    votes.clear(); //if it should change a mapping
    sourceUnallocatedPool.clear();
    matchless.clear();
    deleted.clear();
    sourceAllocatedPool.clear();
    //free memory
    std::map<std::pair<std::string, std::string>, int> emptyMapSSI;
    std::multimap<std::string, std::pair<std::string, double> > emptyMultimapSSD;
    std::multimap<std::string, std::string> emptyMultimapSS;
    std::vector<std::string> emptyVectorS;
    sourceAllocatedPool.swap(emptyVectorS);
    sourceUnallocatedPool.swap(emptyVectorS);
    matchless.swap(emptyVectorS);
    deleted.swap(emptyMultimapSS);
    votes.swap(emptyMapSSI);
    mappings.swap(emptyMultimapSSD);
    std::string line;
    //filename = filename.substr(0, filename.find(".txt")); //clip of any tags or lables
    // filename += ".txt";
    std::ifstream myfile(filename.c_str());
    if (myfile.is_open())
    {
        getline(myfile, line);
        while (myfile.good())
        {//while there is stuff to get, get it split it and put it into model
            if (line.find(";;;;") == std::string::npos)
            {//stop it trying to segment eof   
                if (line.find("deleted") == std::string::npos)
                {//if not the first line read
                    //std::cout << "line= " << line << std::endl;
                    std::vector<std::string> splitV = split(line, '|');
                    //std::cout << "size= " << splitV.size() << "\n";
                    if (splitV.size() > 3)
                    {//prevent null adding
                        std::string source = splitV[0];
                        //std::cout << "source= " << source << std::endl;
                        std::string target = splitV[2];
                        //std::cout << "target= " << target << std::endl;
                        std::string value = split(splitV[3], ' ')[0];
                        //std::cout << "value= " << value << " part 3 size= " << split(splitV[3], ' ').size() << std::endl;
                        std::string voteNumber = split(splitV[3], ' ')[2];
                        //std::cout << "voteNumber= " << voteNumber << std::endl;
                        //add to mappings
                        std::pair<std::string, std::pair<std::string, double> > toAddMappings;
                        toAddMappings.first = source;
                        toAddMappings.second = std::pair<std::string, double>(target, atof(value.c_str()));
                        mappings.insert(toAddMappings);
                        //add to votes
                        std::pair<std::pair<std::string, std::string>, int> toAddVotes;
                        toAddVotes.first = std::pair<std::string, std::string>(source, target);
                        toAddVotes.second = atoi(voteNumber.c_str());
                        votes.insert(toAddVotes);
                        //add to allocated pool
                        sourceAllocatedPool.push_back(source);
                        std::vector<std::string> emptyVectorS;
                        splitV.swap(emptyVectorS);
                    }
                }
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
        std::cerr << "Transfermapping  loadMapping Unable to open file:" << filename << "\n";
        //exit(89);
    }
}

/**
 * return the highest voted pipes or some random ones
 * @param numberOfPairs how many pipes
 * @param greadiness 1 is full gready
 * @param qIn where to find actuall values
 * @return 
 */
std::vector<std::pair<std::string, double> > TransferMapping::choosePairsBasedOnVotes(const int numberOfPairs, double greadiness, QTable* qIn)
{
    //std::cout << "start choose by votes" << std::endl;
    std::vector<std::pair<std::string, double> > output;


    std::map<std::pair<std::string, std::string>, int>::iterator votesIterator = votes.begin();
    if ((double) rand() / RAND_MAX > greadiness)
    {//if should be not gready
        // std::cout << "not gready" << std::endl;
        int loopCount = 0;
        while (output.size() < numberOfPairs)
        {//until we have all
            //std::cout << " 1" << std::endl;
            std::advance(votesIterator, rand() % votes.size());
            while (votesIterator != votes.end())
            {
                loopCount++;
                //std::cout << " 2" << std::endl;
                if ((*votesIterator).second == 0)
                {//if untried add one
                    //std::cout << " 3" << std::endl;
                    std::string targetStateAction = (*votesIterator).first.second; //get the name for the actuall table
                    std::string sourceStateAction = (*votesIterator).first.first;
                    std::string sourceState = sourceStateAction.substr(0, sourceStateAction.find(":"));
                    std::string sourceAction = sourceStateAction.substr(sourceStateAction.find(":") + 1);
                    //find in table
                    double value = qIn->getQValue(sourceState, sourceAction);
                    std::pair < std::string, double > toAdd;
                    toAdd.first = targetStateAction;
                    toAdd.second = value;
                    output.push_back(toAdd);
                    //std::cout << " adding g v=0 " << targetStateAction << std::endl;
                    votesIterator = votes.end();

                }
                else if (loopCount > votes.size() - numberOfPairs)
                {//if not enough zero votes just add what ever
                    std::string targetStateAction = (*votesIterator).first.second; //get the name for the actuall table
                    std::string sourceStateAction = (*votesIterator).first.first;
                    std::string sourceState = sourceStateAction.substr(0, sourceStateAction.find(":"));
                    std::string sourceAction = sourceStateAction.substr(sourceStateAction.find(":") + 1);
                    //find in table
                    double value = qIn->getQValue(sourceState, sourceAction);
                    std::pair < std::string, double > toAdd;
                    toAdd.first = targetStateAction;
                    toAdd.second = value;
                    output.push_back(toAdd);
                    //std::cout << " adding g v!=0 " << targetStateAction << std::endl;
                    votesIterator = votes.end();
                }
                else
                {
                    //std::cout << " 4" << std::endl;
                    votesIterator++;
                }

            }
            votesIterator = votes.begin(); //reset it to fin another if needs be
        }
    }
    else
    {//nom nom nom
        //std::cout << " gready" << std::endl;
        //scan for the best and add it
        std::string *winningNames = new std::string[numberOfPairs];
        int* winningVotes = new int[numberOfPairs];
        double* winningQs = new double[numberOfPairs];
        int added = 0;
        for (int a = 0; a < numberOfPairs; a++)
        {//init arrays
            winningNames[a] = "null:null";
            winningVotes[a] = INT_MIN;
            winningQs[a] = 0;
        }
        while (votesIterator != votes.end())
        {//for all votes
            if ((*votesIterator).second > winningVotes[numberOfPairs - 1])
            {//if bigger than last one see where it should go
                //put it in store
                std::string toAddName = (*votesIterator).first.second; //target's state
                int toAddVotes = (*votesIterator).second; //votes
                std::string sourceStateAction = (*votesIterator).first.first;
                std::string sourceState = sourceStateAction.substr(0, sourceStateAction.find(":"));
                std::string sourceAction = sourceStateAction.substr(sourceStateAction.find(":") + 1);
                //find in table
                double toAddQ = qIn->getQValue(sourceState, sourceAction);

                for (int a = 0; a < numberOfPairs; a++)
                {//go through all in arrays and add
                    //std::cout << a << " comparing " << winningNames[a] << " with " << winningVotes[a] << " votes and " << toAddName << " with " << toAddVotes << std::endl;
                    if (toAddVotes > winningVotes[a])
                    {//if should be here
                        //std::cout << "added one" << std::endl;
                        //save old ones
                        std::string tempName = winningNames[a];
                        double tempQ = winningQs[a];
                        int tempVotes = winningVotes[a];
                        //add to be added (either new one or displaced old one)
                        winningNames[a] = toAddName;
                        winningQs[a] = toAddQ;
                        winningVotes[a] = toAddVotes;
                        //now put displaced one into to add space
                        toAddVotes = tempVotes;
                        toAddQ = tempQ;
                        toAddName = tempName;
                    }
                    else if ((toAddVotes == winningVotes[a])&&(rand() % 10 == 0))
                    {//if might be here
                        //std::cout << "added one =" << std::endl;
                        //save old ones
                        std::string tempName = winningNames[a];
                        double tempQ = winningQs[a];
                        int tempVotes = winningVotes[a];
                        //add to be added (either new one or displaced old one)
                        winningNames[a] = toAddName;
                        winningQs[a] = toAddQ;
                        winningVotes[a] = toAddVotes;
                        //now put displaced one into to add space
                        toAddVotes = tempVotes;
                        toAddQ = tempQ;
                        toAddName = tempName;
                    }
                    else
                    {
                        //std::cout << "the other thing" << std::endl;
                    }

                }

            }

            votesIterator++;
        }
        //std::cout << "end lop" << std::endl;
        for (int a = 0; a < numberOfPairs; a++)
        {//add the winners
            std::pair < std::string, double > toAdd;
            toAdd.first = winningNames[a];
            toAdd.second = winningQs[a];
            output.push_back(toAdd);
        }
        //std::cout << "after print" << std::endl;
        delete[] winningNames;
        delete[] winningVotes;
        delete[] winningQs;
        //std::cout << "deleted" << std::endl;
    }
    //std::cout << "output:\n";
    std::vector<std::pair<std::string, double> >::iterator outputIterator = output.begin();
    /*while (outputIterator != output.end())
    {
        std::cout << (*outputIterator).first << "\n";
        outputIterator++;
    }*/
    //std::cout << "end choose by votes" << std::endl;
    return output;
}

/**
 * change the mapping from the target side
 * @param stateToUpdate
 */
void TransferMapping::updateLearnedMappingFromTarget(std::string targetStateToUpdate, bool good)
{
    //std::cout << mappings.size() << "  " << sourceUnallocatedPool.size() << "   " << sourceAllocatedPool.size() << "  " << votes.size() << "\n";
    //std::cout << "start update learned mapping" << std::endl;
    //find all pipes that go to the target
    //apply voting to them
    //if good ok
    //if bad and >0 delete
    //reallocate as needed
    //std::cout << "1 " << targetStateToUpdate << std::endl;
    std::vector<std::string> sourcesOfPipesToTarget;
    std::multimap<std::string, std::pair<std::string, double> >::iterator mappingsIterator = mappings.begin();
    while (mappingsIterator != mappings.end())
    {//look through all for those pipes to this target
        if ((*mappingsIterator).second.first == targetStateToUpdate)
        {//found a pipe
            sourcesOfPipesToTarget.push_back((*mappingsIterator).first); //save the match
        }
        mappingsIterator++;
    }
    //std::cout << "2" << std::endl;
    //apply votes
    if (sourcesOfPipesToTarget.size() == 0)
    {//if nothing to vote on
        std::cout << "couldn't find " << targetStateToUpdate << " in mappings\n";
    }
    else
    {
        std::vector<std::string>::iterator votableIterator = sourcesOfPipesToTarget.begin();

        while (votableIterator != sourcesOfPipesToTarget.end())
        {
            //std::cout << "votable size " << sourcesOfPipesToTarget.size() << std::endl;
            //std::cout << "2a" << std::endl;
            std::pair<std::string, std::string> key;
            key.first = (*votableIterator);
            key.second = targetStateToUpdate;
            if (good)
            {//upvote
                //         std::cout << "2b" << std::endl;
                votes.find(key)->second++;
                sourcesOfPipesToTarget.erase(votableIterator); //done with that one 
                votableIterator = sourcesOfPipesToTarget.begin(); //re calabrate iterator#
                if (sourcesOfPipesToTarget.size() == 0)
                {//exit loop without status access
                    break;
                }
            }
            else
            {//downvote
                //      std::cout << "2c" << std::endl;
                votes.find(key)->second--;
            }
            //std::cout << "2d" << std::endl;
            votableIterator++;
        }
        // std::cout << "3 sourcesOfPipesToTarget " << sourcesOfPipesToTarget.size() << std::endl;
        //now sourcesOfPipesToTarget only has things to be deleted
        std::vector<std::string>::iterator deleteIterator = sourcesOfPipesToTarget.begin();
        while (deleteIterator != sourcesOfPipesToTarget.end())
        {//for all delteable
            //   std::cout << "3a" << std::endl;
            std::pair<std::multimap<std::string, std::pair<std::string, double> >::iterator, std::multimap<std::string, std::pair<std::string, double> >::iterator > range = mappings.equal_range((*deleteIterator));
            while (range.first != range.second)
            {//for all potential pipes
                // std::cout << (*range.first).first << "  " << (*range.first).second.first << " to deleter points to = " << targetStateToUpdate << std::endl;
                //       std::cout << "3b" << std::endl;
                if ((*range.first).second.first == targetStateToUpdate)
                {//if weve found the right pipe
                    deletedCount++;
                    //       std::cout << "3ba" << std::endl;

                    //     std::cout << "3bab" << std::endl;
                    //delete from votes
                    std::pair<std::string, std::string> toFind;
                    //     std::cout << "3bac" << std::endl;
                    toFind.first = (*range.first).first;
                    //    std::cout << "3bad " << (*range.first).second.first.length() << " " << (*range.first).second.first << std::endl;
                    toFind.second = (*range.first).second.first;
                    //    std::cout << "deleting " << (*range.first).first << " " << (*range.first).second.first << std::endl;
                    deleted.insert(std::make_pair(range.first->second.first, range.first->first)); //prevent re-adding one
                    mappings.erase(range.first); //delete it
                    //    std::cout << "3bae" << std::endl;
                    if (votes.find(toFind) != votes.end())
                    {//if it is there
                        votes.erase(votes.find(toFind));
                    }
                    else
                    {
                        std::cout << "couldnt find " << toFind.first << "  " << toFind.second << " in votes" << std::endl;
                        exit(5);
                    }
                    //  std::cout << "3bb" << std::endl;
                    //delete from allocpool
                    std::vector<std::string>::iterator allocIt = sourceAllocatedPool.begin();
                    while (allocIt != sourceAllocatedPool.end())
                    {
                        if (((*deleteIterator) == (*allocIt)))
                        {//if first match delete
                            //        std::cout << "3bc" << std::endl;
                            sourceAllocatedPool.erase(allocIt);
                            //std::cout << "removing one from alloc" << std::endl;
                            break;
                        }
                        allocIt++;
                    }
                    // std::cout << "3bd" << std::endl;
                    //realocate this target to a new pipe
                    std::pair<std::string, std::pair<std::string, double> > toAdd;
                    std::pair<std::pair<std::string, std::string>, int > toAddInt;
                    if (sourceUnallocatedPool.size() > 0)
                    {//if an unallocated state is available use it
                        //     std::cout << "3d" << std::endl;
                        int random;
                        bool newMatch = !true;
                        std::vector<std::string>::iterator begin;
                        int tryCount = 0;
                        while (!newMatch)
                        {//untill we add one that hasnt been trued
                            random = rand() % sourceUnallocatedPool.size();
                            newMatch = true; //assume true
                            begin = sourceUnallocatedPool.begin();
                            //     std::cout << "3da" << std::endl;
                            for (int z = 0; z < random; z++)
                            {
                                begin++;
                            }
                            //check it wasnt tried already
                            std::pair < std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> range = deleted.equal_range(targetStateToUpdate);
                            while (range.first != range.second)
                            {
                                if ((*range.first).second == (*begin))
                                {
                                    //std::cout << "1 already tried adding " << (*range.first).second << " to " << (*range.first).first << "\n";
                                    newMatch = false;
                                    tryCount++;
                                    range.first = range.second;
                                }
                                else
                                {
                                    range.first++;
                                }
                                if (tryCount > sourceUnallocatedPool.size())
                                {//if tried all just end
                                    newMatch = true;
                                    range.first = range.second;
                                }
                            }
                        }
                        //   std::cout << "3db" << std::endl;
                        toAdd.first = (*begin); //source//a random from sourceUnallocatedPool
                        //    std::cout << "3dba" << std::endl;
                        sourceUnallocatedPool.erase((sourceUnallocatedPool.begin() + random));
                        //   std::cout << "3dbb" << std::endl;
                        sourceAllocatedPool.push_back(toAdd.first); //mark as allocated
                        //   std::cout << "3dc" << std::endl;
                        toAdd.second.first = targetStateToUpdate; //target
                        toAdd.second.second = 0; //value
                        mappings.insert(toAdd);
                        //   std::cout << "3dd" << std::endl;
                        toAddInt.first.first = toAdd.first;
                        toAddInt.first.second = toAdd.second.first;
                        toAddInt.second = 0;
                        votes.insert(toAddInt);
                        //  std::cout << "3de" << std::endl;
                        // std::cout << "adding " << toAdd.first << " " << toAdd.second.first << "\n";
                        // std::cout << "adding " << toAddInt.first.first << " " << toAddInt.first.second << "\n";
                    }
                    else
                    {//use an allocated one
                        //std::cout << "3e" << std::endl;
                        if (sourceAllocatedPool.size() == 0)
                        {
                            std::cout << "about to except on pool size division" << std::endl;
                        }
                        int random;
                        bool newMatch = !true;
                        std::vector<std::string>::iterator begin;
                        int tryCount = 0;
                        while (!newMatch)
                        {//untill we add one that hasnt been trued
                            random = rand() % sourceAllocatedPool.size();
                            newMatch = true; //assume true
                            begin = sourceAllocatedPool.begin();
                            //     std::cout << "3da" << std::endl;
                            for (int z = 0; z < random; z++)
                            {
                                begin++;
                            }
                            //check it wasnt tried already
                            std::pair < std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> range = deleted.equal_range(targetStateToUpdate);
                            while (range.first != range.second)
                            {
                                if ((*range.first).second == (*begin))
                                {
                                    //std::cout << "2 already tried adding " << (*range.first).second << " to " << (*range.first).first << "\n";
                                    newMatch = false;
                                    tryCount++;
                                    range.first = range.second;
                                }
                                else
                                {
                                    range.first++;
                                }
                                if (tryCount > sourceAllocatedPool.size())
                                {//if tried all just end
                                    newMatch = true;
                                    range.first = range.second;
                                }
                            }
                        }
                        // std::cout << "+++++++++++++++++++++++++++==================\n";
                        //  std::cout << "3ea- pool= " << sourceAllocatedPool.size() << " random= " << random << " drew " << (*begin) << std::endl;
                        if (sourceAllocatedPool.size() == 50)
                        {
                            this->printMappingSourceFirst("debug", "error");
                            exit(356);
                        }
                        toAdd.first = (*begin); //source//a random from sourceAllocatedPool
                        //std::cout << "3eb" << std::endl;
                        toAdd.second.first = targetStateToUpdate; //target
                        toAdd.second.second = 0; //value
                        sourceAllocatedPool.push_back(toAdd.first); //mark as allocated
                        mappings.insert(toAdd);
                        //std::cout << "3ec" << std::endl;
                        toAddInt.first.first = toAdd.first;
                        //std::cout << "3ed" << std::endl;
                        toAddInt.first.second = toAdd.second.first;
                        toAddInt.second = 0;
                        //std::cout << "3ee" << std::endl;
                        votes.insert(toAddInt);
                        //  std::cout << "adding " << toAdd.first << " " << toAdd.second.first << "\n";
                        //  std::cout << "adding " << toAddInt.first.first << " " << toAddInt.first.second << "\n";
                        //  std::cout << "adding " << toAdd.first << " " << toAdd.second.first << "\n";
                    }//
                    //std::cout << "3f" << std::endl;
                    //now check where the just deleted one needs to be in the unallocated pools
                    allocIt = sourceAllocatedPool.begin();
                    bool realloc = true;
                    while (allocIt != sourceAllocatedPool.end())
                    {
                        // std::cout << "3g" << std::endl;
                        if ((*deleteIterator) == (*allocIt))
                        {//if still in alloc
                            realloc = false; //do nothing later
                            //    std::cout << "no need to reallocate" << std::endl;
                            break;
                        }
                        else
                        {
                            allocIt++;
                        }
                    }
                    //  std::cout << "3h" << std::endl;
                    if (realloc == true)
                    {//if no longer in allocated pool
                        sourceUnallocatedPool.push_back((*deleteIterator));
                        //  std::cout << "reallocate" << std::endl;
                    }
                    range.first = range.second; //end loop
                    break;
                }
                else
                {

                    range.first++;
                }
            }
            // std::cout << "3aa" << std::endl;
            deleteIterator++;
        }
        //std::cout << "4" << std::endl;
    }
    //check if there are any blank pipes
    /* if (matchless.size() > 0)
     {
         std::pair<std::string, std::pair<std::string, double> > toAdd;
         std::pair<std::pair<std::string, std::string>, int > toAddInt;
         std::vector<std::string>::iterator matchlessIterator = matchless.begin();
         while (matchlessIterator != matchless.end())
         {//for all lonely states

             //std::cout << "3d" << std::endl;
             int random = rand() % mappings.size();
             std::multimap<std::string, std::pair<std::string, double> >::iterator begin = mappings.begin();
             //std::cout << "18a" << std::endl;
             for (int z = 0; z < random; z++)
             {
                 begin++;
             }
             toAdd.first = (*matchlessIterator); //source//the matchless one
             if (sourceUnallocatedPool.size() > 0)
             {//if it might be marked as unallocated (should be)
                 std::vector<std::string>::iterator unallocIt = sourceUnallocatedPool.begin();
                 while (unallocIt != sourceUnallocatedPool.end())
                 {//for all in unaloc
                     if ((*unallocIt) == (*matchlessIterator))
                     {//found it
                         sourceUnallocatedPool.erase(unallocIt);
                         unallocIt = sourceUnallocatedPool.end();
                     }
                     else
                     {
                         unallocIt++;
                     }
                 }
             }
             sourceAllocatedPool.push_back(toAdd.first); //mark as allocated
             toAdd.second.first = (*begin).second.first; //target//a random one
             toAdd.second.second = 0; //value
             mappings.insert(toAdd);
             toAddInt.first.first = toAdd.first;
             toAddInt.first.second = toAdd.second.first;
             toAddInt.second = 0;
             votes.insert(toAddInt);
             std::cout << "matchless adding " << toAdd.first << " " << toAdd.second.first << "\n";
             std::cout << "matchless adding " << toAddInt.first.first << " " << toAddInt.first.second << "\n";


             matchlessIterator++;
         }
     }*/
    // std::cout << "5" << std::endl;
    //  std::cout << "end update learned mapping" << std::endl;
}

QTable * TransferMapping::getSource() const
{

    return source;
}

void TransferMapping::setWTarget(WTable * wTarget)
{

    this->wTarget = wTarget;
    wTargetSet = true;
}

WTable * TransferMapping::getWTarget() const
{

    return wTarget;
}

void TransferMapping::setWSource(WTable * source)
{

    this->wSource = source;
    wSourceSet = true;
}

WTable * TransferMapping::getWSource() const
{

    return wSource;
}

bool TransferMapping::getWTargetSet()
{

    return wTargetSet;
}

void TransferMapping::setTargetName(std::string TargetName)
{

    this->TargetName = TargetName;
}

std::string TransferMapping::getTargetName() const
{

    return TargetName;
}

void TransferMapping::setSourceName(std::string sourceName)
{

    this->sourceName = sourceName;
}

std::string TransferMapping::getSourceName() const
{

    return sourceName;
}

bool TransferMapping::getSourceSet()
{

    return sourceSet;
}

void TransferMapping::populateLearningMapping()
{
    //std::cout << "start populate learned mapping" << std::endl;
    if (source != NULL && target != NULL)
    {//it ready

        //init pools
        std::multimap<std::string, std::pair<std::string, double> > sourceTable = source->getQTable();
        std::multimap<std::string, std::pair<std::string, double> >::iterator qIterator = sourceTable.begin();
        while (qIterator != sourceTable.end())
        {//for all source states
            std::stringstream ss;
            ss << (*qIterator).first << ":" << (*qIterator).second.first; //make state+action
            this->sourceUnallocatedPool.push_back(ss.str()); //add to pool
            qIterator++;
        }
        //go through all target states and give them a source
        std::multimap<std::string, std::pair<std::string, double> > targetTable = target->getQTable();
        qIterator = targetTable.begin();
        while (qIterator != targetTable.end())
        {//for all target states
            if (sourceUnallocatedPool.size() > 0)
            {//if someone needs pairing
                std::pair<std::string, std::pair<std::string, double> > toAdd;
                int location = rand() % sourceUnallocatedPool.size(); //random position
                toAdd.first = sourceUnallocatedPool.at(location); //add source
                sourceAllocatedPool.push_back(sourceUnallocatedPool.at(location)); //mark as allocated
                sourceUnallocatedPool.erase(sourceUnallocatedPool.begin() + location); //delete it
                std::stringstream ss;
                ss << (*qIterator).first << ":" << (*qIterator).second.first; //make state+action
                toAdd.second.first = ss.str(); //add target
                toAdd.second.second = 1; //add weighting
                mappings.insert(toAdd); //done map it
                qIterator++;
            }
            else if (sourceUnallocatedPool.size() == 0)
            {//if no one needs pairing
                if (sourceAllocatedPool.size() == 0)
                {
                    std::cout << "sourceAllocatedPool size=0 will through exception now\n";
                }
                std::pair<std::string, std::pair<std::string, double> > toAdd;
                int location = rand() % sourceAllocatedPool.size(); //random position
                toAdd.first = sourceAllocatedPool.at(location); //add source
                sourceAllocatedPool.push_back(sourceAllocatedPool.at(location)); //mark as allocated
                std::stringstream ss;
                ss << (*qIterator).first << ":" << (*qIterator).second.first; //make state+action
                toAdd.second.first = ss.str(); //add target
                toAdd.second.second = 1; //add weighting
                mappings.insert(toAdd); //done map it
                qIterator++;
            }
        }
        while (sourceUnallocatedPool.size() > 0)
        {//if source had more than target alocate remainder
            //go through all target states and give them a source
            std::multimap<std::string, std::pair<std::string, double> > targetTable = target->getQTable();
            qIterator = targetTable.begin();
            while (qIterator != targetTable.end())
            {//for all target states
                if (sourceUnallocatedPool.size() > 0)
                {//if someone needs pairing
                    std::pair<std::string, std::pair<std::string, double> > toAdd;
                    int location = rand() % sourceUnallocatedPool.size(); //random position
                    toAdd.first = sourceUnallocatedPool.at(location); //add source
                    sourceAllocatedPool.push_back(sourceUnallocatedPool.at(location)); //mark as allocated
                    sourceUnallocatedPool.erase(sourceUnallocatedPool.begin() + location); //delete it
                    //TODO try just not using and adding a new one
                    std::stringstream ss;
                    ss << (*qIterator).first << ":" << (*qIterator).second.first; //make state+action
                    toAdd.second.first = ss.str(); //add target
                    toAdd.second.second = 1; //add weighting
                    mappings.insert(toAdd); //done map it
                    qIterator++;
                }
            }
        }
        //set up the votes
        std::multimap<std::string, std::pair<std::string, double> >::iterator mappingIterator = mappings.begin();
        while (mappingIterator != mappings.end())
        {//get all the allocated source states

            std::pair<std::string, std::string> states;
            states.first = (*mappingIterator).first;
            states.second = (*mappingIterator).second.first;
            std::pair<std::pair<std::string, std::string>, int> temp;
            temp.first = states;
            temp.second = 0;
            votes.insert(temp); //init to zero votes so easy to change
            mappingIterator++;
        }

    }
    //std::cout << "end populate learned mapping" << std::endl;
}

/**
 * put the states into the mapping in any order
 */
void TransferMapping::populateMappingRandomly()
{
    bool bothFinished = false;
    bool sourceFinished = false;
    bool targetFinished = false;
    if (source != NULL && target != NULL)
    {//if something there
        //for all source's states make them a partner in target
        std::multimap<std::string, std::pair<std::string, double> >sourceStates = source->getQTable();
        std::multimap<std::string, std::pair<std::string, double> >targetStates = target->getQTable();
        std::multimap<std::string, std::pair<std::string, double> >::iterator sourceStatesIterator = sourceStates.begin();
        std::multimap<std::string, std::pair<std::string, double> >::iterator targetStatesIterator = targetStates.begin();
        while (!bothFinished)
        {//while some left


            std::pair<std::string, std::pair<std::string, double> > toAdd;
            std::stringstream ss;
            ss << (*sourceStatesIterator).first << ":" << (*sourceStatesIterator).second.first; //make state+action
            toAdd.first = ss.str(); //add source
            std::stringstream ss1;
            ss1 << (*targetStatesIterator).first << ":" << (*targetStatesIterator).second.first; //make state+action
            toAdd.second.first = ss1.str(); //add target
            toAdd.second.second = 1; //add weighting
            mappings.insert(toAdd);
            targetStatesIterator++;
            sourceStatesIterator++;

            //see if ended
            if (sourceStatesIterator == sourceStates.end())
            {//if one finished
                if (targetFinished)
                {//if target already done
                    bothFinished = true;
                }
                else
                {
                    sourceStatesIterator = sourceStates.begin(); //reset it
                    sourceFinished = true;
                }
            }
            if (targetStatesIterator == targetStates.end())
            {//if one finished
                if (sourceFinished)
                {//if source already done
                    bothFinished = true;
                }
                else
                {
                    targetStatesIterator = targetStates.begin(); //reset it
                    targetFinished = true;
                }
            }
        }
        sourceStates.clear(); //clean up
        targetStates.clear();
    }
    else
    {

        std::cerr << "TransferMapping trying to map a null thing\r\n";
        exit(85);
    }

}

/**
 * put the states into the mapping in any order
 */
void TransferMapping::populateMappingBySearch()
{
    bool bothFinished = false;
    bool sourceFinished = false;
    bool targetFinished = false;

    if (source != NULL && target != NULL)
    {//if something there			
        //for all source's states make them a partner in target
        std::multimap<std::string, std::pair<std::string, double> >sourceStates = source->getQTable();
        std::multimap<std::string, std::pair<std::string, double> >targetStates = target->getQTable();
        std::multimap<std::string, std::pair<std::string, double> >::iterator sourceStatesIterator = sourceStates.begin();

        while (!bothFinished)
        {//while some left


            std::pair<std::string, std::pair<std::string, double> > toAdd;
            std::stringstream ss;
            ss << (*sourceStatesIterator).first << ":" << (*sourceStatesIterator).second.first; //make state+action
            toAdd.first = ss.str(); //add source

            if (targetStates.find((*sourceStatesIterator).first) != targetStates.end())
            {//if state match get all matches
                std::pair<std::multimap<std::string, std::pair<std::string, double> >::iterator, std::multimap<std::string, std::pair<std::string, double> >::iterator > targetStatesIterator = targetStates.equal_range((*sourceStatesIterator).first);
                while (targetStatesIterator.first != targetStatesIterator.second)
                {//for all possible correct states and actions
                    if ((*targetStatesIterator.first).second.first == (*sourceStatesIterator).second.first)
                    {//if actions match
                        std::stringstream ss1;
                        ss1 << (*targetStates.find((*sourceStatesIterator).first)).first << ":" << (*targetStatesIterator.first).second.first; //make state+action
                        toAdd.second.first = ss1.str(); //add target
                        toAdd.second.second = 1; //add weighting
                        targetStatesIterator.first = targetStatesIterator.second; //end loop
                        //std::cerr<<toAdd.second.first <<" matched  "<<toAdd.first<<"\n";
                    }
                    else
                    {
                        targetStatesIterator.first++;
                    }
                }

            }
            else
            {
                toAdd.second.first = "NO MATCH"; //add target
                toAdd.second.second = 1; //add weighting
            }
            mappings.insert(toAdd);
            //targetStatesIterator++;
            sourceStatesIterator++;
            //see if ended
            if (sourceStatesIterator == sourceStates.end())
            {//if one finished
                if (targetFinished)
                {//if target already done
                    bothFinished = true;
                }
                else
                {
                    sourceStatesIterator = sourceStates.begin(); //reset it
                    sourceFinished = true;
                    bothFinished = true; //add this so only this one needs to finish
                }
            }

        }
        sourceStates.clear(); //clean up
        targetStates.clear();
    }
    else
    {

        std::cerr << "TransferMapping trying to map a null thing\r\n";
        exit(85);
    }

}

/**
 * put the states into the mapping in any order
 */
void TransferMapping::populateWMappingBySearch()
{//std::cerr<<"trying to map ws\n";
    bool bothFinished = false;
    bool sourceFinished = false;
    bool targetFinished = false;

    if (source != NULL && target != NULL)
    {//if something there			
        //for all source's states make them a partner in target
        std::map<std::string, double> sourceStates = wSource->getWTable();
        std::map<std::string, double> targetStates = wTarget->getWTable();
        std::map<std::string, double> ::iterator sourceStatesIterator = sourceStates.begin();

        while (!bothFinished)
        {//while some left


            std::pair<std::string, std::pair<std::string, double> > toAdd;
            std::stringstream ss;
            ss << (*sourceStatesIterator).first; //make state
            toAdd.first = ss.str(); //add source

            if (targetStates.find((*sourceStatesIterator).first) != targetStates.end())
            {//if state match get all matches
                std::pair<std::map<std::string, double> ::iterator, std::map<std::string, double> ::iterator > targetStatesIterator = targetStates.equal_range((*sourceStatesIterator).first);
                while (targetStatesIterator.first != targetStatesIterator.second)
                {//for all possible correct states and actions
                    if ((*targetStatesIterator.first).first == (*sourceStatesIterator).first)
                    {//if actions match
                        std::stringstream ss1;
                        ss1 << (*targetStates.find((*sourceStatesIterator).first)).first; //make state
                        toAdd.second.first = ss1.str(); //add target
                        toAdd.second.second = 1; //add weighting
                        targetStatesIterator.first = targetStatesIterator.second; //end loop
                        //std::cerr<<toAdd.second.first <<" matched  "<<toAdd.first<<"\n";
                    }
                    else
                    {
                        targetStatesIterator.first++;
                    }
                }

            }
            else
            {
                toAdd.second.first = "NO MATCH"; //add target
                toAdd.second.second = 1; //add weighting
            }
            mappings.insert(toAdd);
            //targetStatesIterator++;
            sourceStatesIterator++;
            //see if ended
            if (sourceStatesIterator == sourceStates.end())
            {//if one finished
                if (targetFinished)
                {//if target already done
                    bothFinished = true;
                }
                else
                {
                    sourceStatesIterator = sourceStates.begin(); //reset it
                    sourceFinished = true;
                    bothFinished = true; //add this so only this one needs to finish
                }
            }

        }
        sourceStates.clear(); //clean up
        targetStates.clear();
    }
    else
    {

        std::cerr << "TransferMapping trying to map a null thing\r\n";
        exit(85);
    }

}

/**
 * take a q table state action pair and fit it into the mapping framework
 * @param input a qtable's pair state action value
 * @return state+action value
 */
std::pair<std::string, double> TransferMapping::makePairForMapper(std::pair<std::string, std::pair<std::string, double> > input)
{
    std::pair<std::string, double> output;
    std::stringstream ss;
    ss << input.first << ":" << input.second.first; //make state+action
    output.first = ss.str(); //add it
    output.second = input.second.second; //put in the q

    return output;
}

/**
 * take a q table state action pair and fit it into the mapping framework
 * @param input a qtable's pair state action value
 * @return state+action value
 */
std::vector<std::pair<std::string, double> > TransferMapping::makePairForMapper(std::vector<std::pair<std::string, std::pair<std::string, double> > > input)
{//std::cerr << "make pair for mapper\r\n";
    std::vector<std::pair<std::string, double> >output;
    std::vector<std::pair<std::string, std::pair<std::string, double> > >::iterator inputIterator = input.begin();
    while (inputIterator != input.end())
    {

        std::pair<std::string, double> toAdd;
        std::stringstream ss;
        ss << (*inputIterator).first << ":" << (*inputIterator).second.first; //make state+action
        toAdd.first = ss.str(); //add it
        toAdd.second = (*inputIterator).second.second; //put in the q
        output.push_back(toAdd);
        inputIterator++;
    }
    //std::cerr << "make pair for mapper end\r\n";
    return output;
}

/**
 * map the passed pair from source to target 
 * @param input the state action and its q value
 * @return the q value pair to add to target
 */
std::pair<std::string, std::pair<std::string, double> > TransferMapping::mapFromStateToTarget(std::pair<std::string, double> input)
{//std::cerr<<"about to map pair"<<" \n";
    std::pair<std::string, std::pair<std::string, double> > output;
    if ((mappings.find(input.first)) != mappings.end())
    {
        std::pair<std::string, double> targetState = (*(mappings.find(input.first))).second; //(*(mapping.find(input.first)));//get where this state should be
        std::string action = targetState.first;
        std::string state = action.replace(action.find(":"), (action.length() - 1), ""); //trim off action;
        action = targetState.first;
        action = action.substr(action.find(":") + 1, action.length() - 1);
        output.first = state;
        output.second.first = action;
        output.second.second = targetState.second * input.second; //weighting *q
    }
    else
    {

        //matchless.push_back(input.first);
        std::vector<std::string>::iterator sourceUnallocIt = sourceUnallocatedPool.begin();
        bool checker = false;
        while (sourceUnallocIt != sourceUnallocatedPool.end())
        {//check unalloc pool to see if it is there
            if ((*sourceUnallocIt) == input.first)
            {
                // std::cerr << " but it was in unalloc pool" << " \r\n";
                sourceUnallocIt = sourceUnallocatedPool.end();
                checker = true;
            }
            else
            {
                sourceUnallocIt++;
            }
        }
        if (!checker)
        {//if wasnt in unalloc pool
            this->printMappingSourceFirst("the error one", "error");
            std::cerr << "couldn't find the thing( " << input.first << " ) to map";
            std::cerr << " and it wasnt in unalloc pool" << " \r\n";
            exit(47892);
        }
    }
    //exit(47892);
    output.first = "NO MATCH";
    output.second.first = "NO MATCH";
    output.second.second = -10;

    //std::cerr<<"Mapped from "<<input.first<<" to "<<output.first<<"\n";

    return output;
}

/**
 * map the passed w pair from source to target 
 * @param input the state action and its w value
 * @return the q value pair to add to target
 */
std::pair<std::string, double> TransferMapping::mapWFromStateToTarget(std::pair<std::string, double> input)
{//std::cerr<<"about to map w"<<" \n";
    std::pair<std::string, double> output;
    if ((mappings.find(input.first)) != mappings.end())
    {
        std::pair<std::string, double> targetState = (*(mappings.find(input.first))).second; //(*(mapping.find(input.first)));//get where this state should be
        std::string state = targetState.first;
        output.first = state;
        output.second = targetState.second * input.second; //weighting *q
    }
    else
    {

        std::cerr << "w couldn't find the thing( " << input.first << " ) to map\r\n";
        exit(47892);
    }
    //std::cerr<<"Mapped w from "<<input.first<<" to "<<output.first<<"\n";
    return output;
}

/**
 * vector version map the passed pair from source to target 
 * @param input the state action and its q value
 * @return the q value pair to add to target
 */
std::vector<std::pair<std::string, double> > TransferMapping::mapWFromStateToTarget(std::vector<std::pair<std::string, double> >input)
{//std::cerr << "map from state to target\r\n";
    std::vector<std::pair<std::string, double> > output;
    std::vector<std::pair<std::string, double> >::iterator inputIterator = input.begin();
    while (inputIterator != input.end())
    {

        std::pair<std::string, double> temp = TransferMapping::mapWFromStateToTarget((*inputIterator));
        output.push_back(temp);
        inputIterator++;
    }

    // std::cerr << "ma from state to target end\r\n";
    return output;
}

/**
 * vector version map the passed pair from source to target 
 * @param input the state action and its q value
 * @return the q value pair to add to target
 */
std::vector<std::pair<std::string, std::pair<std::string, double> > > TransferMapping::mapFromStateToTarget(std::vector<std::pair<std::string, double> >input)
{//std::cerr << "map from state to target\r\n";
    std::vector<std::pair<std::string, std::pair<std::string, double> > > output;
    std::vector<std::pair<std::string, double> >::iterator inputIterator = input.begin();
    while (inputIterator != input.end())
    {

        std::pair<std::string, std::pair<std::string, double> > temp = TransferMapping::mapFromStateToTarget((*inputIterator));
        output.push_back(temp);
        inputIterator++;
    }

    //std::cerr << "ma from state to target end\r\n";
    return output;
}

void TransferMapping::printMappingSourceFirst(std::string input, std::string tag)
{
    std::stringstream ss;
    ss << getSourceName() << " to " << getTargetName() << " " << input << "-mapping.txt.stats";
    std::cerr << "writing " << input << "\n";
    std::string filename = ss.str();
    if (tag == "error")
    {//if erro no tag

    }
    else
    {//has a tag
        std::stringstream ss;
        ss << getSourceName() << " to " << getTargetName() << " " << input << "-mapping.txt." << tag << ".stats";
        filename = ss.str();
    }
    std::ofstream outputfile(filename.c_str());

    if (outputfile.is_open())
    {//std::cerr<<rewardLog.size() <<"= log size\n";
        outputfile << "deleted " << deletedCount << " elements\r\n";
        std::multimap<std::string, std::pair<std::string, double> >::iterator mappingIterator = mappings.begin();

        while (mappingIterator != mappings.end())
        {//for all maps
            outputfile << "|" << (*mappingIterator).first << "|-|" << (*mappingIterator).second.first << "| " << (*mappingIterator).second.second << " votes= ";
            std::pair<std::string, std::string> key;
            key.first = (*mappingIterator).first;
            key.second = (*mappingIterator).second.first;
            outputfile << (*votes.find(key)).second;
            outputfile << "\r\n";
            mappingIterator++;
        }
        outputfile << "\r\n;;;;;;;;;;"; //eof
        outputfile.close();
    }
    else
    {

        std::cerr << "\nmapping->printmappingsource Unable to open file\n";
        exit(89);
    }
}

void TransferMapping::printMappingTargetFirst(std::string input, std::string tag)
{
    std::stringstream ss;
    ss << getTargetName() << " to " << getSourceName() << " " << input << "-mapping.txt." << tag << ".stats";
    //std::cerr << "writing " << filename << "\n";
    std::ofstream outputfile(ss.str().c_str());

    if (outputfile.is_open())
    {//std::cerr<<rewardLog.size() <<"= log size\n";
        outputfile << "deleted " << deletedCount << " elements\r\n";
        std::multimap<std::string, std::pair<std::string, double> > mappingCopy = mappings;
        while (mappingCopy.size() > 0)
        {//for all pipes
            std::multimap<std::string, std::pair<std::string, double> >::iterator mappingCopyIterator = mappingCopy.begin();
            std::pair<std::string, std::pair<std::string, double> > test = (*mappingCopyIterator); //get first for match
            mappingCopyIterator++;
            //std::cout << "a " << test.second.first << "\n";
            //see if there are any matches (2 pipes to one location))
            while (mappingCopyIterator != mappingCopy.end())
            {//search the rest for match
                //std::cout << " compare to " << (*mappingCopyIterator).second.first << "\n";
                if (((*mappingCopyIterator).second.first) == (test.second.first))
                {//match
                    // std::cout << "b\n";
                    outputfile << "|" << (*mappingCopyIterator).second.first << "|-|" << (*mappingCopyIterator).first << "| " << (*mappingCopyIterator).second.second << " votes= ";
                    std::pair<std::string, std::string> key;
                    key.first = (*mappingCopyIterator).first;
                    key.second = (*mappingCopyIterator).second.first;
                    outputfile << (*votes.find(key)).second;
                    outputfile << "\r\n";
                    //std::cout << "about to delete " << (*mappingCopyIterator).second.first << "\n";
                    mappingCopy.erase(mappingCopyIterator); //delete the match
                    break;
                }
                //std::cout << "c\n";
                mappingCopyIterator++;
            }
            // std::cout << "d\n";
            if (mappingCopyIterator == mappingCopy.end())
            {//now put in this one

                outputfile << "|" << test.second.first << "|-|" << test.first << "| " << test.second.second << " votes= ";
                std::pair<std::string, std::string> key;
                key.first = test.first;
                key.second = test.second.first;
                outputfile << (*votes.find(key)).second;
                outputfile << "\r\n";
                mappingCopy.erase(mappingCopy.begin()); //delete the first
            }
        }
        outputfile << "\r\n;;;;;;;;;;"; //eof
        outputfile.close();
    }
    else
    {
        std::cerr << "\nmapping->printmappingtarget Unable to open file\n";
        exit(89);
    }
}

/**
 * splits the string
 * @param s what to split
 * @param delim where
 * @return a vector of non-zero length bits
 */
std::vector<std::string> TransferMapping::split(std::string s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        if (!item.empty())
        {//if not emply puch back
            elems.push_back(item);
        }
    }
    return elems;
}