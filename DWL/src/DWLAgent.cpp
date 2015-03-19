/* 
 * File:   DWLAgent.cpp
 * Author: Adam
 * 
 * Created on July 11, 2012, 9:10 AM
 */

#include "DWLAgent.h"
#include "Policy.h"
#include "NeighbourReward.h"
#include "WLearningProcess.h"
#include <string>
#include <algorithm>
#include <iostream>
using std::string;
#include <fstream>
#include <set>
using std::pair;

#include <vector>
#include <locale>
#include <sstream>
using std::vector;

/**
 * 
 * @param ID
 */
DWLAgent::DWLAgent(std::string ID)
{
    lastQTable = NULL;
    this->collaborationPolicy = NULL; //do this so it doesn't get used by accident
    name = ID;
    totalReward = 0;
    historicReward = 0;
    this->previousWinner = ID;
    usingTransferLearning = false; //default to no transfer
}

/**
 * add in to the neighbour set what neighbours are passed 
 * @param name what to add
 */
void DWLAgent::addNeighbours(std::string name)
{
    neighbours.insert(name);
}

std::string DWLAgent::getName()
{
    return name;
}

DWLAgent::~DWLAgent()
{//TODO make sure the elements are deleter properly. clear only empties the vector, it does not call ~
    //std::cout << "1" << std::endl;
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {
        delete (*localIterator);
        localIterator++;
    }
    localPolicies.clear();
    //std::cout << "2" << std::endl;
    std::vector<Policy*>::iterator remoteIterator = remotePolicies.begin();
    while (remoteIterator != remotePolicies.end())
    {
        delete (*remoteIterator);
        remoteIterator++;
    }
    remotePolicies.clear();
    neighbours.clear();
    communications.clear();
    sugestedActions.clear();
    //std::cout << "3" << std::endl;
    if (collaborationPolicy != NULL)
    {
        delete collaborationPolicy;
    }
    //std::cout << "4" << std::endl;
    for (std::vector<TransferMapping*>::iterator i = mappings.begin(); i != mappings.end(); ++i)
        delete *i;
    for (std::vector<TransferMapping*>::iterator i = wMappings.begin(); i != wMappings.end(); ++i)
        delete *i;
    delete lastQTable;
    rewardLog.clear();
    //std::cout << "5" << std::endl;
    //free memory
    std::vector<double> emptyVectorD;
    std::vector<Policy*> emptyVectorP;
    std::vector<TransferMapping*> emptyVectorT;
    std::vector< std::pair< std::string, std::pair<std::string, double> > > emptyVectorSSD;
    std::set<std::string, Compare> emptySetS;
    remotePolicies.swap(emptyVectorP);
    localPolicies.swap(emptyVectorP);
    neighbours.swap(emptySetS);
    communications.swap(emptyVectorSSD);
    sugestedActions.swap(emptyVectorSSD);
    mappings.swap(emptyVectorT);
    wMappings.swap(emptyVectorT);
    rewardLog.swap(emptyVectorD);
}

/** charris - taken out DOP
 * add a policy to this agent
 * @param name what to call it should be unique
 * @param rewardIn how to tell it what is good
 */
Policy* DWLAgent::addLocalPolicy(std::string name, Reward* rewardIn)
{

    // create the Policy implementation
    WLearningProcess* wLearningProcess = new WLearningProcess(name);
    wLearningProcess->setReward(rewardIn);
    //wLearningProcess->configureLearningProcess();
    //rewardIn->calcReward();
    // cast into Policy container
    Policy* toAdd = (Policy*) wLearningProcess;

    // toAdd->setPolicyName(name);
    // toAdd->configureLearningProcess(rewardIn);
    localPolicies.push_back(toAdd);
    // return as a WLearningProcess for use by the sub-class
    return toAdd;
}

/**
 *add what comms gives
 */
void DWLAgent::addRemotePolicy(std::vector<std::pair<std::string, WTable*> > policies)
{//std::cerr<<"dwl in add remote\n";
    std::vector<std::pair<std::string, WTable*> >::iterator inputIterator = policies.begin();
    while (inputIterator != policies.end())
    {//for all the things to add
        WTable* wTableToAdd = (*inputIterator).second;
        if (neighbours.find(wTableToAdd->getID()) != neighbours.end())
        {//if is in neighbours set
            bool isLocal = false;
            //prevent adding of its own local policies if they are read from comms
            std::vector<Policy*>::iterator localIterator = localPolicies.begin();
            while (localIterator != localPolicies.end())
            {
                Policy* policy = *localIterator;
                if (policy->getPolicyName() == (*inputIterator).first)
                {
                    //std::cerr<<"DWLAgent addRemotePolicy got a local one\n";
                    isLocal = true;
                    localIterator = localPolicies.end(); //end loop
                }
                else
                {
                    localIterator++;
                }
            }
            if (!isLocal)
            {//if is a remote policy    
                std::vector<Policy*>::iterator remoteIterator;
                remoteIterator = remotePolicies.begin();
                bool isThereAlready = false;
                if (remotePolicies.begin() == remotePolicies.end())
                {
                    Policy* policyToAdd = (Policy*)new WLearningProcess((*inputIterator).first);
                    policyToAdd->setPolicyName((*inputIterator).first);
                    NeighbourReward* reward = new NeighbourReward(); //use this as it is allows a singe reward sample from a neighbour to be treated in the same way as 'normal'.
                    ((WLearningProcess*) policyToAdd)->addRemoteFromWTable(wTableToAdd, this->replaceActionsInRemoteStateSpace(wTableToAdd)); //this strips the actions and replaces them
                    ((WLearningProcess*) policyToAdd)->setReward(reward);
                    ((WLearningProcess*) policyToAdd)->setQAlpha(0.01); //TODO place holder//im not responsable for seting these in general//they need to be inited here
                    ((WLearningProcess*) policyToAdd)->setWAlpha(.9);
                    ((WLearningProcess*) policyToAdd)->setQGamma(.1);
                    ((WLearningProcess*) policyToAdd)->setWGamma(.1);
                    ((WLearningProcess*) policyToAdd)->initalise();
                    remotePolicies.push_back(policyToAdd);
                    remoteIterator = remotePolicies.end();
                    //std::cerr << "Added " << policyToAdd->getPolicyName() << " to " << this->getName() << "\n";
                    isThereAlready = true;
                }
                else
                {// a remote policy and we already have at least one
                    while (remoteIterator != remotePolicies.end())
                    {//for each see if it matches
                        if ((*remoteIterator)->getPolicyName() == (*inputIterator).first)
                        {
                            isThereAlready = true;
                            remoteIterator = remotePolicies.end();
                        }
                        else
                        {
                            remoteIterator++;
                        }
                    }
                    if (!isThereAlready)
                    {//only add if there was no same named one
                        Policy* policyToAdd = new Policy((*inputIterator).first);
                        policyToAdd->setPolicyName((*inputIterator).first);
                        NeighbourReward* reward = new NeighbourReward(); //use this as it is allows a singe reward sample from a neighbour to be treated in the same way as 'normal'.
                        ((WLearningProcess*) policyToAdd)->addRemoteFromWTable(wTableToAdd, this->replaceActionsInRemoteStateSpace(wTableToAdd)); //this strips the actions and replaces them
                        ((WLearningProcess*) policyToAdd)->setReward(reward);
                        ((WLearningProcess*) policyToAdd)->initalise();
                        remotePolicies.push_back(policyToAdd);
                    }
                }
            }
            else
            {

            }
        }
        inputIterator++;
    }
}

/**
 *return how many neighbours in set
 */
int DWLAgent::numberOfNeighbours()
{
    return neighbours.size();
}

void DWLAgent::setUsingTransferLearning(bool usingTransferLearning)
{
    this->usingTransferLearning = usingTransferLearning;
}

bool DWLAgent::getUsingTransferLearning() const
{
    return usingTransferLearning;
}

/**
 *add what comms gives
 */
void DWLAgent::addRemotePolicy(std::string path, std::string tag)
{//TODO add check for adding its self as a remote

    //drop the tag
    path = path.substr(11, (path.length() - 11));
    std::string test = "";
    //split sting by <>s
    while (path.find_first_of("<") != std::string::npos && path.find_first_of(">") != std::string::npos)
    {//while still adderesses that is properly formatted
        //std::cerr<<"path: "<<path<<"\n\n";
        test = path.substr(path.find_first_of("<") + 1, (path.find_first_of(">") - 1)); //get the path
        path = path.substr(path.find_first_of(">") + 1, (path.length()-((path.find_first_of(">") - 1)))); //clear handeled path
        std::string policyName = test.substr(test.find_last_of("\\") + 1, test.length() - 1);
        //std::cerr<<"before if add remote\n";
        //TODO a better find that looks for sub string, that being is h1 is neighbout h1+p1 will not match
        if (neighbours.find(policyName) != neighbours.end())//TODO turn neighbours back on//if this policy is in the neighbour set
        {
            test = test + "-w"; //add this so it finds the way it was printed to file
            WTable* wToAdd = new WTable(policyName);
            bool isLocal = false;
            //prevent adding of its own local policies if they are read from comms
            std::vector<Policy*>::iterator localIterator = localPolicies.begin();
            while (localIterator != localPolicies.end())
            {
                Policy* policy = *localIterator;
                if (policy->getPolicyName() == policyName)
                {
                    //std::cerr<<"DWLAgent addRemotePolicy got a local one\n";
                    isLocal = true;
                    localIterator = localPolicies.end(); //end loop
                }
                else
                {
                    localIterator++;
                }
            }
            if (!isLocal)
            {//if is a remote policy    
                std::vector<Policy*>::iterator remoteIterator;
                remoteIterator = remotePolicies.begin();
                bool isThereAlready = false;
                if (remotePolicies.begin() == remotePolicies.end())
                {
                    if (wToAdd->readStateActionFromFile(test, tag))
                    {
                        Policy* policyToAdd = (Policy*)new WLearningProcess(name);
                        policyToAdd->setPolicyName(policyName);
                        NeighbourReward* reward = new NeighbourReward(); //use this as it is allows a singe reward sample from a neighbour to be treated in the same way as 'normal'.
                        ((WLearningProcess*) policyToAdd)->addRemoteFromWTable(wToAdd, this->replaceActionsInRemoteStateSpace(wToAdd)); //this strips the actions and replaces them
                        ((WLearningProcess*) policyToAdd)->setReward(reward);
                        ((WLearningProcess*) policyToAdd)->setQAlpha(0.01); //TODO place holder//im not responsable for seting these in general//they need to be inited here
                        ((WLearningProcess*) policyToAdd)->setWAlpha(.9);
                        ((WLearningProcess*) policyToAdd)->setQGamma(.1);
                        ((WLearningProcess*) policyToAdd)->setWGamma(.1);
                        ((WLearningProcess*) policyToAdd)->initalise();
                        remotePolicies.push_back(policyToAdd);
                        remoteIterator = remotePolicies.end();
                        //std::cerr << "Added " << policyToAdd->getPolicyName() << " to " << this->getName() << "\n";
                        isThereAlready = true;
                        //std::cerr<<"\n added the Wtable in addremote end bit \n";
                    }
                    else
                    {
                        std::cerr << "\ncouldn't add the Wtable in addremote for some reason\n";
                    }
                }
                else
                {// a remote policy and we already have at least one
                    while (remoteIterator != remotePolicies.end())
                    {//for each see if it matches
                        if ((*remoteIterator)->getPolicyName() == policyName)
                        {
                            isThereAlready = true;
                            remoteIterator = remotePolicies.end();
                        }
                        else
                        {
                            remoteIterator++;
                        }
                    }
                    if (!isThereAlready)
                    {//only add if there was no same named one
                        //if this policy is not a local one
                        //TODO add a check for if already in remote


                        if (wToAdd->readStateActionFromFile(test, tag))
                        {
                            Policy* policyToAdd = (Policy*)new WLearningProcess(name);
                            policyToAdd->setPolicyName(policyName);
                            NeighbourReward* reward = new NeighbourReward(); //use this as it is allows a singe reward sample from a neighbour to be treated in the same way as 'normal'.
                            ((WLearningProcess*) policyToAdd)->addRemoteFromWTable(wToAdd, this->replaceActionsInRemoteStateSpace(wToAdd)); //this strips the actions and replaces them
                            ((WLearningProcess*) policyToAdd)->setReward(reward);
                            ((WLearningProcess*) policyToAdd)->setQAlpha(0.01); //TODO place holder//im not responsable for seting these in general//they need to be inited here
                            ((WLearningProcess*) policyToAdd)->setWAlpha(.9);
                            ((WLearningProcess*) policyToAdd)->setQGamma(.1);
                            ((WLearningProcess*) policyToAdd)->setWGamma(.1);
                            ((WLearningProcess*) policyToAdd)->initalise();
                            remotePolicies.push_back(policyToAdd);
                            remoteIterator = remotePolicies.end();
                            //std::cerr << "Added " << policyToAdd->getPolicyName() << " to " << this->getName() << "\n";
                            isThereAlready = true;
                            //std::cerr<<"\n added the Wtable in addremote end bit \n";
                        }
                        else
                        {
                            std::cerr << "\ncouldn't add the Wtable in addremote for some reason\n";
                        }
                    }
                }
                //delete wToAdd;
            }
            else
            {
                std::cerr << policyName + " wasn't in the neighbourhood of " << this->name << "\n";
            }
        }
    }
    //read each path and see if we have it

    //std::cerr<<"end add remote\n";
}

/**
 * place holder this should be called by nighbour in emulated comms or some automated way in real comms.  it will not add the same policy twice
 * @param name
 * @param the dop
 */
void DWLAgent::addRemotePolicy(std::string name, std::vector<WTable*> wTableIn)
{
    std::vector<WTable*>::iterator inputIterator = wTableIn.begin();
    while (inputIterator != wTableIn.end())
    {//for all the things to add
        WTable* wTableToAdd = (*inputIterator);
        if (neighbours.find(wTableToAdd->getID()) != neighbours.end())
        {//if is in neighbours set
            bool isLocal = false;
            //prevent adding of its own local policies if they are read from comms
            std::vector<Policy*>::iterator localIterator = localPolicies.begin();
            while (localIterator != localPolicies.end())
            {
                Policy* policy = *localIterator;
                if (policy->getPolicyName() == name)
                {
                    //std::cerr<<"DWLAgent addRemotePolicy got a local one\n";
                    isLocal = true;
                    localIterator = localPolicies.end(); //end loop
                }
                else
                {
                    localIterator++;
                }
            }
            if (!isLocal)
            {//if is a remote policy    
                std::vector<Policy*>::iterator remoteIterator;
                remoteIterator = remotePolicies.begin();
                bool isThereAlready = false;
                if (remotePolicies.begin() == remotePolicies.end())
                {
                    Policy* policyToAdd = (Policy*)new WLearningProcess(name);
                    policyToAdd->setPolicyName(name);
                    NeighbourReward* reward = new NeighbourReward(); //use this as it is allows a singe reward sample from a neighbour to be treated in the same way as 'normal'.
                    ((WLearningProcess*) policyToAdd)->addRemoteFromWTable(wTableToAdd, this->replaceActionsInRemoteStateSpace(wTableToAdd)); //this strips the actions and replaces them
                    ((WLearningProcess*) policyToAdd)->setReward(reward);
                    ((WLearningProcess*) policyToAdd)->setQAlpha(0.01); //TODO place holder//im not responsable for seting these in general//they need to be inited here
                    ((WLearningProcess*) policyToAdd)->setWAlpha(.9);
                    ((WLearningProcess*) policyToAdd)->setQGamma(.1);
                    ((WLearningProcess*) policyToAdd)->setWGamma(.1);

                    remotePolicies.push_back(policyToAdd);
                    remoteIterator = remotePolicies.end();
                    //std::cerr << "Added " << policyToAdd->getPolicyName() << " to " << this->getName() << "\n";
                    isThereAlready = true;
                }
                else
                {// a remote policy and we already have at least one
                    while (remoteIterator != remotePolicies.end())
                    {//for each see if it matches
                        if ((*remoteIterator)->getPolicyName() == name)
                        {
                            isThereAlready = true;
                            remoteIterator = remotePolicies.end();
                        }
                        else
                        {
                            remoteIterator++;
                        }
                    }
                    if (!isThereAlready)
                    {//only add if there was no same named one
                        Policy* policyToAdd = new Policy(name);
                        policyToAdd->setPolicyName(name);
                        NeighbourReward* reward = new NeighbourReward(); //use this as it is allows a singe reward sample from a neighbour to be treated in the same way as 'normal'.
                        ((WLearningProcess*) policyToAdd)->addRemoteFromWTable(wTableToAdd, this->replaceActionsInRemoteStateSpace(wTableToAdd)); //this strips the actions and replaces them
                        ((WLearningProcess*) policyToAdd)->setReward(reward);
                        remotePolicies.push_back(policyToAdd);
                    }
                }
            }
            else
            {

            }
        }
        inputIterator++;
    }
}

/**
 * WARNING: seems to have  a bug in which it strips a state from the state space use an atlernative
 * write out a vector of all the dops this agent has for local policies.  This is used to make policies avaliable to other agents.
 * @return a vector of policy's dops
 */
std::vector<WTable*> DWLAgent::publishLocalPolicies()
{
    std::cerr << "WARNING possible bug needs fixing.  Strips state from statespace intermitantly\n";
    std::vector<WTable*> output;
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {
        Policy* policy = *localIterator;
        output.push_back(((WLearningProcess*) policy)->getWTable());
        localIterator++;
    }
    return output;
}

/**
 * write out a vector of all the wtables to file ans return a string of all paths.  This is used to make policies avaliable to other agents.
 * @param where to put them
 * @return a string of form <path><ACTUAL PATH>
 */
std::string DWLAgent::publishLocalPolicies(std::string path, std::string tag)
{
    std::string output = "";
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {
        Policy* policy = *localIterator;
        output += "<path><" + path + policy->getPolicyName() + ">\n";
        ((WLearningProcess*) policy)->getQTable()->writeStateActionToFile(((WLearningProcess*) policy)->getPolicyName(), tag);
        ((WLearningProcess*) policy)->getWTable()->writeStateActionToFile(((WLearningProcess*) policy)->getPolicyName(), tag);
        localIterator++;
    }
    return output;
}

/**
 * this takes a dop and strips the states and add this agents actions to its statespace
 * @param wIn the w table fro the neighbour
 * @return the nice q table with my actions and its states
 */
QTable* DWLAgent::replaceActionsInRemoteStateSpace(WTable* wIn)
{
    QTable* remotePolicy = new QTable(); //make my representation
    std::map<std::string, double> localCopy = wIn->getWTable();
    std::map<std::string, double>::iterator wIterator = localCopy.begin();
    std::vector<std::string> actions = ((WLearningProcess*) (*localPolicies.begin()))->getActions();
    while (wIterator != localCopy.end())
    {//add all actions to the states as we go through
        std::vector<std::string>::iterator actionsIterator = actions.begin();

        while (actionsIterator != actions.end())
        {//for all actions
            //std::cerr<<"adding an action\n";
            remotePolicy->addStateAction(wIterator->first, (*actionsIterator), 0);
            actionsIterator++;
        }
        remotePolicy->randomState = wIterator->first;
        wIterator++;
    };
    localCopy.clear();
    actions.clear();

    return remotePolicy; //now give it back
}

/**
 * emmulate reciving a communication over a network into a buffer
 * @param name the name of the agent and policy giving this update
 * @param messageIn the  state and action and reward recieved at t-1
 */
void DWLAgent::addMessage(std::string name, std::pair<std::string, double> messageIn)
{
    std::pair< std::string, std::pair<std::string, double> > toAdd;
    toAdd.first = name;
    toAdd.second = messageIn;
    communications.push_back(toAdd);
}

/**
 * emmulate reciving a communication over a network into a buffer
 * @param messageIn string from comms
 */
void DWLAgent::addMessage(std::string messageIn)
{
    messageIn = messageIn.substr(messageIn.find_first_of(">") + 1, (messageIn.length()-(messageIn.find_first_of(">") + 1))); //clip of message type
    while (std::count(messageIn.begin(), messageIn.end(), '<') >= 3)
    {//while a properly formed message left
        std::string policy = messageIn.substr(messageIn.find_first_of("<") + 1, (messageIn.find_first_of(">")-(messageIn.find_first_of("<") + 1))); //name
        messageIn = messageIn.substr(messageIn.find_first_of(">") + 1, (messageIn.length()-(messageIn.find_first_of(">")) + 1));
        std::string state = messageIn.substr(messageIn.find_first_of("<") + 1, (messageIn.find_first_of(">")-(messageIn.find_first_of("<") + 1))); //state
        messageIn = messageIn.substr(messageIn.find_first_of(">") + 1, (messageIn.length()-(messageIn.find_first_of(">")) + 1));
        std::string value = messageIn.substr(messageIn.find_first_of("<") + 1, (messageIn.find_first_of(">")-(messageIn.find_first_of("<") + 1))); //value
        messageIn = messageIn.substr(messageIn.find_first_of(">") + 1, (messageIn.length()-(messageIn.find_first_of(">")) + 1));
        //std::cerr<<"add message reading as "<<policy<<"\nstate= "<<state<<"\nvalue ="<<value<<"\n";
        std::vector< std::pair< std::string, std::pair<std::string, double> > > ::iterator commsIterator = communications.begin();
        bool match = false;
        while (!match && commsIterator != communications.end())
        {//find if the message is there
            if (commsIterator->first.compare(policy) == 0)
            {//if match
                commsIterator->second.first = state;
                commsIterator->second.second = atof(value.c_str());
                match = true; //end this adding
            }
            commsIterator++;
        }
        if (!match)
        {//if no m,atch found force add
            std::pair< std::string, std::pair<std::string, double> > toAdd; //add the message
            toAdd.first = policy;
            toAdd.second.first = state;
            toAdd.second.second = atof(value.c_str());
            communications.push_back(toAdd);
        }
    }
}

/**
 * add both parts of messages at the same time
 * @param input many messages
 */
void DWLAgent::addMessage(std::pair< std::string, std::pair<std::string, double> > input)
{
    communications.push_back(input);
}

/**
 * place holder for writing out a comms update.  
 * @return the previous winners stuff
 */
std::pair< std::string, std::pair<std::string, double> > DWLAgent::writeMessage()
{
    //replaced as should be updating based on what state it is and how good that is
    /*std::vector< std::pair< std::string, KeyValuePair*> >::iterator sugestedIterator = sugestedActions.begin();
    while ((*sugestedIterator).first != this->previousWinner)
    {//while not the right pair
    sugestedIterator++;
    }
    (*sugestedIterator).second->second=this->calculatedRewardForPreviousAction;
    std::cerr<<"adding message "<<(*sugestedIterator).first<<" "<<(*sugestedIterator).second->first->getID()<<"  "<<(*sugestedIterator).second->second<<"\n";
    return (*sugestedIterator);*/
    std::pair< std::string, std::pair<std::string, double> > out;
    out.first = (*localPolicies.begin())->getPolicyName();
    out.second = std::pair<std::string, double>(((WLearningProcess*) (*this->localPolicies.begin()))->getCurrentState(), ((WLearningProcess*) (*this->localPolicies.begin()))->getMostRecentReward());
    return out; //TODO make this work for several locals
}

/**
 * place holder for writing out a comms update.  
 * @param dummy argument to differentiate as i am lazy
 * @return the previous winners stuff
 */
std::string DWLAgent::writeMessage(int)
{
    std::vector< std::pair< std::string, std::pair<std::string, double> > > ::iterator sugestedIterator = sugestedActions.begin();
    while (sugestedIterator != sugestedActions.end() && (*sugestedIterator).first != this->previousWinner)
    {//while not the right pair]
        sugestedIterator++;
    }
    if (sugestedIterator == sugestedActions.end())
    {
        return ""; //if no message send something
    }
    std::string out = "<";
    out += ((*sugestedIterator)).first;
    out += "><";
    //get the state to tell neighbour not the action
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();

    out += ((WLearningProcess*) (*localIterator))->getCurrentState();

    out += "><";
    std::stringstream strs;
    strs << (*sugestedIterator).second.second;
    out += strs.str();
    out += ">\n";
    //std::cerr<<"messege ="<<out<<"\n";
    return out; //TODO make this work for several locals
}

void DWLAgent::purgeCommsBuffer()
{
    //TODO posible delete them here   
    communications.clear();
}

void DWLAgent::purgeSugestedActionsBuffer()
{//TODO posible delete them here        
    sugestedActions.clear();
}

std::string DWLAgent::getName() const
{
    return name;
}

/** write a file for each policy that lives in this agent
@param name what prefix to look for*/
void DWLAgent::writePolicies(std::string name, std::string tag)
{
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {
        std::stringstream ss;
        Policy* policy = *localIterator;
        ss << name << "+Local+" << policy->getPolicyName();
        ((WLearningProcess*) policy)->printStateSpace(ss.str(), tag);
        localIterator++;
    }
    std::vector<Policy*>::iterator remoteIterator = remotePolicies.begin();
    while (remoteIterator != remotePolicies.end())
    {
        std::stringstream ss;
        Policy* policy = *remoteIterator;
        ss << name << "+Remote+" << policy->getPolicyName();
        ((WLearningProcess*) policy)->printStateSpace(ss.str(), tag);
        remoteIterator++;
    }
}

/** read a file for each policy that lives in this agent
@param name what to prefix the policies with*/
void DWLAgent::readPolicies(std::string name, std::string tag)
{
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {
        Policy* policy = *localIterator;
        ((WLearningProcess*) policy)->readStateSpace(name, tag);
        localIterator++;
    }
    std::vector<Policy*>::iterator remoteIterator = remotePolicies.begin();
    while (remoteIterator != remotePolicies.end())
    {
        Policy* policy = *remoteIterator;
        ((WLearningProcess*) policy)->readStateSpace(name, tag);
        remoteIterator++;
    }
}

/**
 *for agents with ability to work out reward from state
 * inform remote policies what happened in the simulator and update the comms vector to reflect reward.  
 * @param stateName the name of the state
 */
void DWLAgent::updateLocal(std::string stateName)
{
    rewardLog.push_back(historicReward);
    // 
    if (stateName.length() <= 0)
    {
        //do nothing

        std::cerr << this->getName() << "'s statename is too short in update local  " << "\n";
        std::cerr << stateName << "=statename " << remotePolicies.size() << "\n";
    }
    else
    {
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())
        {
            // charris - downcast to specific Policy
            // example downcast Programmer *pProg = (Programmer *)&employee; THIS IS C STYLE
            Policy* toCast = *localIterator;
            WLearningProcess* test = (WLearningProcess *) toCast;
            // TODO - safe C++ style dynamic cast not working for some reason - base needs to be virtual??
            // B* bp = dynamic_cast<B*>(arg); - example C++ style downcast - safe as involves a runtime type check
            //WLearningProcess* test = dynamic_cast<WLearningProcess*>(*localIterator);
            if (test->getPolicyName() == previousWinner)
            {//if this one won tell it to update and learn
                //std::cerr<<"about to call update 1 from "<<test->getPolicyName()<<"\n";


                test->update(stateName, 0, true); //doesnt matter what reward is passed as local policies can self-calculate
                if (sugestedActions.size() > 0)
                {//if there is a publiched action
                    vector<pair<string, std::pair<std::string, double> > >::iterator sugestedIterator = sugestedActions.begin();
                    while ((*sugestedIterator).first != this->previousWinner)
                    {//find the winner in comms vector
                        sugestedIterator++;
                    }
                    //now we have the pointer to its update put in the reward
                    (*sugestedIterator).second.second = test->getMostRecentReward();
                }
                historicReward += test->getMostRecentReward(); //as this won accumulate its reward
                if (usingTransferLearning)
                {
                    //std::cout << "about to make feedback\n";
                    if (test->getMostRecentReward() > 0)
                    {//if good reward feedback good
                        /*
                         //old update for if pos reward good was nieve if (usingTransferLearning)
                        {
                            std::stringstream ss;
                            ss << test->getPreviousState() << ":" << test->getCurrentAction();

                            test->provideTransferFeedback(ss.str(), true);
                        }*/
                    }
                    else
                    {//bad feeback
                        /*if (usingTransferLearning)
                        {
                            std::stringstream ss;
                            ss << test->getPreviousState() << ":" << test->getCurrentAction();

                            test->provideTransferFeedback(ss.str(), false);
                        }*/
                    }
                }
            }
            else
            {//it lost learn not nothing


                //	std::cerr<<"about to call update 2 from "<<test->getPolicyName()<<"\n";
                //set the winning action as current so that the loosing agent can learn from it
                test->update(stateName, this->winningAction, 0, false); //doesnt matter what reward is passed as local policies can self-calculate
                //	std::cerr << "1 " << std::endl;
                if (usingTransferLearning)
                {/*
                    //std::cout << "about to make feedback\n";
                    if (test->getMostRecentReward() > 0)
                    {//if good reward feedback good
                        std::stringstream ss;
                        ss << test->getPreviousState() << ":" << test->getCurrentAction();
                        test->provideTransferFeedback(ss.str(), true);
                    }
                    else
                    {//bad feeback
                        std::stringstream ss;
                        ss << test->getPreviousState() << ":" << test->getCurrentAction();
                        test->provideTransferFeedback(ss.str(), false);
                    }
                  * */
                }
            }
            localIterator++;
        }
    }

}

/**
 *for agents with ability to work out reward from state
 * inform remote policies what happened in the simulator and update the comms vector to reflect reward.  
 * @param stateName the name of the state
 *@param rewardValueIn an externally calculated reward
 */
void DWLAgent::updateLocal(std::string stateName, double rewardValueIn)
{
    std::cerr << "update local 0\n" << std::endl;
    rewardLog.push_back(historicReward);
    this->calculatedRewardForPreviousAction = rewardValueIn;
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {//std::cerr<<"update local 1\n";
        Policy* test = *localIterator;

        //std::cerr << this->getName() << " is local updating " << test->getPolicyName() << "  " << previousWinner;
        if (test->getPolicyName() == previousWinner)
        {//if this one won tell it to update and learn
            //std::cerr<<"update local 2\n";
            // std::cerr<<"about to call update 3 from "<<test->getPolicyName()<<"\n";
            ((WLearningProcess*) test)->update(stateName, rewardValueIn, true);
            //std::cerr << "local as winner with " << localPolicies.size() << " policies\n";
            std::vector< std::pair< std::string, std::pair<std::string, double> > >::iterator sugestedIterator = sugestedActions.begin();
            while ((*sugestedIterator).first != this->previousWinner)
            {//find the winner in comms vector
                sugestedIterator++;
                //std::cerr<<"update local 3\n";
            }
            //now we have the pointer to its update put in the reward
            (*sugestedIterator).second.second = ((WLearningProcess*) test)->getMostRecentReward();
            totalReward += ((WLearningProcess*) test)->getMostRecentReward();
            historicReward += ((WLearningProcess*) test)->getMostRecentReward(); //as this won accumulate its reward
        }
        else
        {//it lost learn nothing
            //std::cerr << " as loser with " << localPolicies.size() << " policies\n";
            //std::cerr<<"update local 4\n";
            //std::cerr<<"about to call update 4 from "<<test->getPolicyName()<<"\n";
            //set the winning action as current so that the loosing agent can learn from it
            ((WLearningProcess*) test)->update(stateName, this->winningAction, rewardValueIn, false); //give reward anyway as it keeps the tie code simple
        }
        localIterator++;
    }

}

/**
 * load the passed vector as comms.  allows integration with an external comms vector most notably the one i wrote for gridlab-d.
 */
void DWLAgent::readInComms(std::vector< std::pair< std::string, std::pair<std::string, double> > > input)
{
    while (communications.size() > 0)
    {
        communications.erase(communications.begin());
    }//out with the old

    //old non-neighbourly version //communications.insert(communications.begin(), input.begin(), input.end()); //in with the nucleus
    std::vector< std::pair< std::string, std::pair<std::string, double> > >::iterator communicationsIterator = input.begin();
    while (communicationsIterator != input.end())
    {
        std::pair< std::string, std::pair<std::string, double> > toTest = *communicationsIterator; //get the message
        if (neighbours.find(toTest.first) != neighbours.end())
        {//if was found as a neighbour
            communications.push_back(toTest); //in with the nucleus
        }
        communicationsIterator++;
    }
}

/**
 * inform remote policies what was communicated.  They update by finding the relivent update in the comms vector
 * 
 */
void DWLAgent::updateRemote()
{
    //std::cerr << "in remote update" << remotePolicies.size() << "\n";
    std::vector<Policy*>::iterator remoteIterator = remotePolicies.begin();
    while (remoteIterator != remotePolicies.end())
    {
        //std::cerr<<"policy "<<this->getName()<<"'s turn \n";
        std::string stateName;
        bool matchCheck = false;
        double rewardValue;
        Policy* test = *remoteIterator;
        std::pair< std::string, std::pair<std::string, double> > toSearch;
        // find update in comms vector 
        std::vector< std::pair< std::string, std::pair<std::string, double> > >::iterator communicationsIterator = communications.begin();
        while (!matchCheck && communicationsIterator != communications.end())
        {//std::cerr<<"1"<<"\n";
            toSearch = *communicationsIterator;
            //std::cerr<<"message name= "<<toSearch.first<<" policy name= "<<test->getPolicyName()<<"\n";
            if (toSearch.first == test->getPolicyName())
            {//if this is the update we are looking for this policy
                //std::cerr<<"2"<<"\n";
                stateName = toSearch.second.first; //save the name of the state
                rewardValue = toSearch.second.second; //the reward
                matchCheck = true;
            }
            else
            {//keep looking
                //std::cerr<<"3"<<"\n";
                communicationsIterator++;
            }
        }
        if (communicationsIterator == communications.end())
        {//if we went through all of the updates and didnt find the right one
            //not nessicarily wrong that there is no update for a given remote policy agent may not have done anything out of coms range etc
            //std::cerr << "DWLAgent updateRemote() failed to find update" << std::endl;
            //std::cerr<<"4"<<"\n";

        }
        else
        {//if the was an update appliy it
            //std::cerr << this->getName() << " is remote updating " << test->getPolicyName() << "  winner= " << previousWinner;
            if (test->getPolicyName() == previousWinner)
            {//if this one won tell it to update and learn
                //std::cerr <<this->getName()<< " remote as winner with " << remotePolicies.size() << " policies and " << communications.size() << " messages\n";
                ((WLearningProcess*) test)->update(stateName, rewardValue, true);
                std::vector< std::pair< std::string, std::pair<std::string, double> > >::iterator sugestedIterator = sugestedActions.begin();
                while ((*sugestedIterator).first != this->previousWinner)
                {//find the winner in comms vector
                    sugestedIterator++;
                }
                //now we have the pointer to its update put in the reward
                (*sugestedIterator).second.second = ((WLearningProcess*) test)->getMostRecentReward();
                totalReward += toSearch.second.second;
                historicReward += toSearch.second.second; //as this won accumulate its reward
            }
            else
            {//it lost learn nothing but move to new state
                ((WLearningProcess*) test)->update(stateName, this->winningAction, rewardValue, false);
            }
        }
        remoteIterator++;
    }
    //this->colaberationPolicy->update(colaberationPolicy->getDOP()->getCurrentState(),this->totalReward,true);
    //        this->colaberationPolicy->changeCValue(); //basicly an update only works on a policy set up a colab
    totalReward = 0; //clear this so it can accumlate again
}

/**
 * make a new colaberation policy 
 * @param numberOfvalues how the range should be split
 * @param maxValue range is 0 to this
 */
void DWLAgent::createColaberationPolicy(std::string name, int numberOfValues, double maxValue)
{
    collaborationPolicy = new CollaborationProcess(name, numberOfValues, maxValue);
    collaborationPolicy->setReward(new NeighbourReward());
}

/**
 * convinience function that is very important
 */
void DWLAgent::finishRun()
{
    this->purgeCommsBuffer();
    if (collaborationPolicy != NULL)
    {
        this->collaborationPolicy->update(collaborationPolicy->getCurrentState(), this->totalReward, true); //state for colaberation// give the total reward and make it learn
    }
    //TODO Make stats here
    totalReward = 0;
    this->purgeSugestedActionsBuffer();
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {//put locals into sugestion vector and see best
        Policy* test = *localIterator;
        //std::cout<<((WLearningProcess*) (test))->getPolicyName()<<"  finish run\n";
        if (usingTransferLearning)
        {//no extra cleaning needed
        }
        else
        {
            ((WLearningProcess*) test)->clearTLFeadback();
        }
        localIterator++;
    }
    std::vector<Policy*>::iterator remoteIterator = remotePolicies.begin();
    while (remoteIterator != remotePolicies.end())
    {
        Policy* test = *remoteIterator;
        if (usingTransferLearning)
        {//no extra cleaning needed
        }
        else
        {
            ((WLearningProcess*) test)->clearTLFeadback();
        }

        remoteIterator++;
    }
#if __cplusplus <= 199711L
#warning This library code will leak memory in very long runs. Compile for at least a C++11 to include shrink to fit, or ocasionally reinitalise agents
#else
    remotePolicies.shrink_to_fit();
    localPolicies.shrink_to_fit();
    communications.shrink_to_fit();
    sugestedActions.shrink_to_fit();
    mappings.shrink_to_fit();
    wMappings.shrink_to_fit();
    rewardLog.shrink_to_fit();
#endif

}

/**
 * ask all policies what to do and select the best
 * @return what to do
 */
std::string DWLAgent::nominate()
{ //std::cerr<<"in dwlagent Nominate 1\n";//TODO make it so no colaberation policy is required to work
    //std::cerr<<"local size = "<<localPolicies.size()<<"\n";
    //this->purgeSugestedActionsBuffer();
    double colaberationCoefficent = 1; //todo make learn c= atof(collaborationPolicy->getCurrentState().c_str()); //get the state of the colab dop and make that a double 
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    //std::cerr<<"in dwlagent Nominate 1.25\n";
    //cerr << (*localIterator)->getPolicyName() << "  ";
    std::pair<std::string, std::pair<std::string, double> > bestAction = ((WLearningProcess*) (*localIterator))->getSuggestion(); //initalise the winner to first action
    Policy* test = *localIterator; //put the inial winner into vector
    //std::cerr<<"in dwlagent Nominate 1.5\n";
    sugestedActions.push_back(bestAction);
    std::pair<std::string, std::pair<std::string, double> > sugestion;
    //std::cerr<<"local Policy "<<test->getPolicyName()<<" gave "<<bestAction.second.first<<" with w of "<<bestAction.second.second<<"\n";
    //std::cerr<<"in dwlagent Nominate 1.75\n";
    localIterator++; //point to next one
    //std::cerr<<"in dwlagent Nominate 2\n";
    while (localIterator != localPolicies.end())
    {//put locals into sugestion vector and see best
        test = *localIterator;
        //cerr << test->getPolicyName() << "  ";
        sugestion = ((WLearningProcess*) (test))->getSuggestion();
        //std::cerr<<"local Policy "<<test->getPolicyName()<<" gave "<<sugestion.second.first<<" with w of "<<sugestion.second.second<<"\n";
        if (bestAction.second.second < sugestion.second.second)
        {//if this one is better
            //this->purgeSugestedActionsBuffer();//hack as fails with two?
            bestAction = sugestion;
            sugestedActions.push_back(sugestion);
        }
        localIterator++;
    }
    //std::cerr<<"in dwlagent Nominate 3\n";
    std::vector<Policy*>::iterator remoteIterator = remotePolicies.begin();
    while (remoteIterator != remotePolicies.end())
    {//put remotes into sugestion vector and see best
        test = *remoteIterator;
        //std::cerr << test->getPolicyName() << "  \n";
        sugestion = ((WLearningProcess*) (test))->getSuggestion();
        sugestion.second.second = sugestion.second.second*colaberationCoefficent; //w for remote = w*c

        //std::cerr<<"remote Policy "<<test->getPolicyName()<<" gave "<<sugestion.second.first<<" with w of "<<sugestion.second.second<<"\n";
        if (bestAction.second.second < sugestion.second.second)
        {
            //this->purgeSugestedActionsBuffer();//hack as fails with two?
            bestAction = sugestion;
            sugestedActions.push_back(sugestion);
            //std::cerr<<this->getName()<<" best Action is from remote "<<bestAction.first<<"\n";
        }
        remoteIterator++;
    }
    previousWinner = bestAction.first;
    this->winningAction = bestAction.second.first;
    //std::cerr << "previous winner was " << previousWinner << "\n";
    //std::cerr << "At " << this->getName() << " Winner is " << bestAction.first << " with action " << bestAction.second.first << " C was " << colaberationCoefficent << std::endl;
    return bestAction.second.first; //send the winner back
}

/**return how many policies there ate*/
int DWLAgent::numberOfLocalPolicies()
{
    return this->localPolicies.size();
}

/**return how many policies there ate*/
int DWLAgent::numberOfRemotePolicies()
{
    return this->remotePolicies.size();
}

/**
 * take in the required information to make one end to end mapping
 * @param sourceNameIn agent name and policy name
 * @param tergetNameIn agent name and policy name
 * @param sourceIn the q table
 * @param targetIn the q table
 */
void DWLAgent::addMapping(std::string sourceNameIn, std::string targetNameIn, QTable* sourceIn, QTable* targetIn)
{

    if (this->usingTransferLearning)
    {
        bool isPresent = false;
        std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
        while (mappingIterator != mappings.end())
        {
            if (((*mappingIterator)->getSourceName() == sourceNameIn) && ((*mappingIterator)->getTargetName() == targetNameIn))
            {//is there
                std::cout << "attempting to re-add mapping\n";
                isPresent = true;
                mappingIterator = mappings.end();
            }
            else
            {
                mappingIterator++;
            }
        }
        if (!isPresent)
        {//if not there add it
            TransferMapping* map = new TransferMapping(); //make it and set the things
            map->setSource(sourceIn);
            map->setTarget(targetIn);
            map->setTargetName(targetNameIn);
            map->setSourceName(sourceNameIn);
            map->populateLearningMapping(); //TODO replace this with a populateMappingBySearch
            this->mappings.push_back(map);
        }

    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }
    //std::cerr<<"mappings = "<<mappings.size()<<"\n";
}

/**
 * take in the required information to make one end to end mapping
 * @param sourceNameIn agent name and policy name
 * @param tergetNameIn agent name and policy name
 * @param sourceIn the q table
 * @param targetIn the q table
 * @param source reward
 * @param target reward
 */
void DWLAgent::addMapping(std::string sourceNameIn, std::string targetNameIn, QTable* sourceIn, QTable* targetIn, Reward* sourceRewardIn, Reward* targetRewardIn)
{

    if (this->usingTransferLearning)
    {
        bool isPresent = false;
        std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
        while (mappingIterator != mappings.end())
        {
            if (((*mappingIterator)->getSourceName() == sourceNameIn) && ((*mappingIterator)->getTargetName() == targetNameIn))
            {//is there
                std::cout << "attempting to re-add mapping\n";
                isPresent = true;
                mappingIterator = mappings.end();
            }
            else
            {
                mappingIterator++;
            }
        }
        if (!isPresent)
        {//if not there add it
            TransferMapping* map = new TransferMapping(); //make it and set the things
            map->setSource(sourceIn);
            map->setTarget(targetIn);
            map->setTargetName(targetNameIn);
            map->setSourceName(sourceNameIn);
            map->setSourceReward(sourceRewardIn);
            map->setTargetReward(targetRewardIn);
            map->populateLearningMapping(); //TODO replace this with a populateMappingBySearch
            this->mappings.push_back(map);
        }

    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }
    //std::cerr<<"mappings = "<<mappings.size()<<"\n";
}

/**
 * take in the required information to make one end to end mapping
 * @param sourceNameIn agent name and policy name
 * @param tergetNameIn agent name and policy name
 * @param where to old mapping is
 */
void DWLAgent::loadMapping(std::string sourceNameIn, std::string targetNameIn, std::string address)
{
    if (this->usingTransferLearning)
    {
        bool isPresent = false;
        std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
        while (mappingIterator != mappings.end())
        {
            if (((*mappingIterator)->getSourceName() == sourceNameIn) && ((*mappingIterator)->getTargetName() == targetNameIn))
            {//is there so load
                isPresent = true;
                (*mappingIterator)->loadMapping(address);
                mappingIterator = mappings.end();
            }
            else
            {
                mappingIterator++;
            }
        }
        if (!isPresent)
        {//if not there add it
            std::cout << "dwl aganet loadMapping Warning mapping you are trying to load is not there, create it first with Add mapping\n";
        }


    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }
    //std::cerr<<"mappings = "<<mappings.size()<<"\n";
}

/**
 * take in the required information to make one end to end mapping
 * @param sourceNameIn agent name and policy name
 * @param tergetNameIn agent name and policy name
 * @param sourceIn the q table
 * @param targetIn the q table
 */
void DWLAgent::addMapping(std::string sourceNameIn, std::string targetNameIn, WTable* sourceIn, WTable* targetIn)
{//std::cerr<<"dwl agent adding a w mapping";
    if (this->usingTransferLearning)
    {
        bool isPresent = false;
        std::vector<TransferMapping*>::iterator mappingIterator = wMappings.begin();
        while (mappingIterator != wMappings.end())
        {
            if (((*mappingIterator)->getSourceName() == sourceNameIn) && ((*mappingIterator)->getTargetName() == targetNameIn))
            {//is there
                isPresent = true;
                mappingIterator = wMappings.end();
            }
            else
            {
                mappingIterator++;
            }
        }
        if (!isPresent)
        {//if not there add it
            TransferMapping* map = new TransferMapping(); //make it and set the things
            map->setWSource(sourceIn);
            map->setWTarget(targetIn);
            map->setTargetName(targetNameIn);
            map->setSourceName(sourceNameIn);
            map->populateWMappingBySearch(); //TODO replace this with a populateMappingNotRandomly
            this->wMappings.push_back(map);
        }
        else
        {
            std::cerr << "dwl agent add w mappping warning adding an already presnet mapping\n";
        }


    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }
    //std::cerr<<"  wmappings = "<<wMappings.size()<<"\n";
}

/**
 * get the transfers for all of the policies reciving from this one
 * @param policyName who to send from
 * @return what to send of form destination <state+action, value>
 */
std::vector<std::pair<std::string, std::pair<std::string, double> > > DWLAgent::transferToAllFromOne(std::string policyName)
{//UNOFFICIlly depricated

    std::vector<std::pair<std::string, std::pair<std::string, double> > > output;
    /*if (this->usingTransferLearning)
    {
    std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
    while (mappingIterator != mappings.end())
    {//for all
    TransferMapping* test = (*mappingIterator);
    if (test->getSourceName() == policyName)
    {//if there is a match
    std::pair<std::string, std::pair<std::string, double> > toAdd;
    toAdd.first = test->getTargetName();
    toAdd.second = TransferMapping::makePairForMapper(test->mapFromStateToTarget(TransferMapping::makePairForMapper(this->findInterestingPair(policyName)))); //set the pair to pass translated to the neighbours space
    output.push_back(toAdd);
    mappingIterator++;//mappingIterator=mappings.erase(mappingIterator);//does ++
    }
    else
    {
    mappingIterator++;
    }

    }

    }
    else
    {
    std::cerr << "not using transfer learning in DWLAgent so don't try to\n";
    exit(9867);
    }*/
    return output;
}

/**
 * get the transfers for all of the policies reciving from this one also get intresting info
 * @param policyName who to send from
 * @return what to send of form destination <state+action, value>
 */
std::string DWLAgent::transferToAllFromAll()
{
    //std::cerr << "DWLAgent transfer from and too all\n";
    std::vector < std::pair<std::string, std::pair<std::string, double> > >vectorVersion;
    std::vector < std::pair<std::string, std::pair<std::string, double> > >wVector;
    if (this->usingTransferLearning)
    {

        this->findAllInterestingPairs(vectorVersion);
        this->findAllInterestingWs(wVector);

    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n" << std::endl;
        exit(9867);
    }
    std::string output = "";
    //std::cout << "size= " << vectorVersion.size() << "\n";
    std::vector < std::pair<std::string, std::pair<std::string, double> > >::iterator outputIterator = vectorVersion.begin();
    while (outputIterator != vectorVersion.end())
    {
        std::pair<std::string, std::pair<std::string, double> > test = *outputIterator;
        std::stringstream ss;
        ss << test.second.second;
        output += "<transfer>";
        output += "<";
        output += test.first;
        output += ">";
        output += "<";
        output += test.second.first;
        output += "><";
        output += ss.str();
        output += ">\n";
        outputIterator++;
    }
    vectorVersion.clear();
    //now add the w transfers to the list
    std::vector < std::pair<std::string, std::pair<std::string, double> > >::iterator wIterator = wVector.begin();
    while (wIterator != wVector.end())
    {
        std::pair<std::string, std::pair<std::string, double> > test = *wIterator;
        std::stringstream ss;
        ss << test.second.second;
        output += "<wtransfer>";
        output += "<";
        output += test.first;
        output += ">";
        output += "<";
        output += test.second.first;
        output += "><";
        output += ss.str();
        output += ">\n";
        wIterator++;
    }
    wVector.clear();
    //std::cerr << "output t all to all " << output << std::endl;
    return output;
}

void DWLAgent::findAllInterestingPairs(std::vector < std::pair<std::string, std::pair<std::string, double> > >& input)
{
    if (this->usingTransferLearning)
    {
        //std::cerr << "a\n";
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())
        {//for all locals
            //std::cerr << "b\n";
            std::string policyName = (*localIterator)->getPolicyName();
            std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
            while (mappingIterator != mappings.end())
            {//for all
                // std::cerr << "c\n";
                TransferMapping* test = (*mappingIterator);
                //std::cout << "mapping's source name " << test->getSourceName() << " actual name= " << policyName << "\n";
                if (test->getSourceName() == policyName)
                {//if there is a match
                    //std::cerr << "d\n";
                    std::pair<std::string, std::pair<std::string, double> > toAdd;
                    toAdd.first = test->getTargetName();
                    std::vector<std::pair<std::string, double> > vect;
                    if (false && ((WLearningProcess*) (*localIterator))->getBoltzmannTemperature() <= 10)
                    {//if small botz be gready
                        vect = test->choosePairsBasedOnVotes(20, 1, ((WLearningProcess*) ((*localIterator)))->getQTable()); //TransferMapping::makePairForMapper(test->mapFromStateToTarget(TransferMapping::makePairForMapper(this->findInterestingPair(policyName)))); //set the pair to pass translated to the neighbours space
                    }
                    else
                    {//if big botz dont be gready
                        //std::cout << "this one\n";
                        vect = test->choosePairsBasedOnVotes(20, 0, ((WLearningProcess*) ((*localIterator)))->getQTable()); //TransferMapping::makePairForMapper(test->mapFromStateToTarget(TransferMapping::makePairForMapper(this->findInterestingPair(policyName)))); //set the pair to pass translated to the neighbours space
                    }
                    std::vector<std::pair<std::string, double> >::iterator vectIterator = vect.begin();
                    //std::cerr << "findAllInterestingPairs -policy name= " << policyName << "\n";
                    while (vect.end() != vectIterator)
                    {//add all the pairs
                        //std::cerr << "e\n";
                        if (((*vectIterator).first.compare("NO MATCH") == 0) || ((*vectIterator).first.compare("NOMATCH") == 0))
                        {//if no match do nothing
                            std::cerr << "in the no match thing\n";
                            exit(99);
                        }
                        else
                        {
                            //std::cerr << "adding an interesting pair\n";
                            toAdd.second = (*vectIterator);
                            input.push_back(toAdd);
                        }
                        vectIterator++;
                    }
                    vect.clear();
                    mappingIterator++; //mappingIterator=mappings.erase(mappingIterator);//does ++
                }
                else
                {
                    mappingIterator++;
                }

            }
            localIterator++;
        }
    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n" << std::endl;
        exit(9867);
    }

    //std::cerr << "find all intresting pairs end " << input.size() << "\r\n";
}

void DWLAgent::findAllInterestingWs(std::vector < std::pair<std::string, std::pair<std::string, double> > >& input)
{//std::cerr<<"enter findAllInterestingWs\n";
    if (this->usingTransferLearning)
    {//std::cerr<<"a +++++++++++\n";
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())
        {//for all locals
            //std::cerr<<"b ++++++++++++\n";
            std::string policyName = (*localIterator)->getPolicyName();
            std::vector<TransferMapping*>::iterator mappingIterator = wMappings.begin();
            //std::cerr<<wMappings.size()<<" size of mapping\n";
            while (mappingIterator != wMappings.end())
            {//for all
                //std::cerr<<"c ++++++++++++++\n";
                TransferMapping* test = (*mappingIterator);
                if (test->getSourceName() == policyName)
                {//if there is a match
                    //std::cerr<<"d +++++++++++++\n";
                    std::pair<std::string, std::pair<std::string, double> > toAdd;
                    toAdd.first = test->getTargetName();
                    std::vector<std::pair<std::string, double> > vect = test->mapWFromStateToTarget(this->findInterestingW(policyName)); //set the pair to pass translated to the neighbours space
                    std::vector<std::pair<std::string, double> >::iterator vectIterator = vect.begin();
                    //std::cerr<<"findAllInterestingWs -policy anme= "<<policyName<<"\n";
                    while (vect.end() != vectIterator)
                    {//add all the pairs
                        //   std::cerr<<"e +++++++++++++\n";
                        if ((*vectIterator).first.compare("NO MATCH") == 0)
                        {//if no match do nothing
                            //        std::cerr<<"f +++++++++++++++\n";
                        }
                        else
                        {
                            // std::cerr<<"adding an interesting pair\n";
                            toAdd.second = (*vectIterator);
                            input.push_back(toAdd);
                        }
                        vectIterator++;
                    }
                    vect.clear();
                    mappingIterator++; //mappingIterator=mappings.erase(mappingIterator);//does ++
                }
                else
                {
                    mappingIterator++;
                }

            }
            localIterator++;
        }
    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n" << std::endl;
        exit(9867);
    }
    //std::cerr << "find all intresting ws end "<<input.size()<<"\r\n";
}

/**
 * find something worth telling my neighbour about
 * @return a pair (state  value)
 */
std::vector<std::pair<std::string, double> > DWLAgent::findInterestingW(std::string policyName)
{//std::cerr<<"findInterestingw ++++++++"<<std::endl;
    std::vector<std::pair<std::string, double> >output;
    if (this->usingTransferLearning)
    {
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())
        {//for all
            //std::cerr<<"1++++++++++++++"<<std::endl;
            Policy* test = (*localIterator);
            //std::cerr<<"compare-="<<test->getPolicyName()<<"  "<<policyName<<"\n";
            if (test->getPolicyName() == policyName)//TODO check the name is agent+policy here not agent+agent+policy or just policy
            {//if there is a match		
                //	std::cerr<<"2++++++++++++++++"<<std::endl;
                WTable* table = ((WLearningProcess*) test)->getWTable();
                std::vector<std::string> states = table->getStatesOfHighest(); //table->getStateOfHighestW();//table->getStateOfSomething();////dont use chang in ivanas as qtable nneds tobe chabged betwen policy types//table->getStateOfGreatestChang(lastQTable); 
                std::vector<std::string>::iterator statesIterator = states.begin();
                //std::cerr<<"Size states= "<<states.size()<<std::endl;
                while (states.end() != statesIterator)
                {//go through all returned states and get the pairs
                    //	std::cerr<<"3++++++++++++++"<<std::endl;
                    if ((*statesIterator) == "")
                    {//if not a state do nothing
                    }
                    else
                    {
                        //	std::cerr<<"4++++++++++"<<std::endl;
                        //if not using state of highest turn on
                        output.push_back(table->getWTableEntry((*statesIterator))); //TODO this is probably not interesting enough   
                        //if trying to competitive transfer
                        //std::pair<std::string,double>temp;
                        //temp.first=(*statesIterator);
                        //temp.second=-999;
                        //output.push_back(temp);//this makes the state bad for other agent
                    }
                    statesIterator++;
                }

                states.clear();

            }
            localIterator++;
        }

    }
    else
    {
        std::cerr << "DWLAgent findInterestingW not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }
    if (output.size() == 0)
    {
        std::cerr << "DWLAgent findInterestingW didn't add anything as could't find " << policyName << " \n";
    }
    //std::cerr<<"found interesting w"<<output.begin()->first<<" "<<output.begin()->second<<" \n";
    return output;
}

/**
 * find something worth telling my neighbour about
 * @return a pair (state (action value))
 */
std::vector<std::pair<std::string, std::pair<std::string, double> > > DWLAgent::findInterestingPair(std::string policyName)
{//std::cerr<<"findInterestingPair ++++++++"<<std::endl;
    std::vector<std::pair<std::string, std::pair<std::string, double> > >output;
    if (this->usingTransferLearning)
    {
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())
        {//for all
            Policy* test = (*localIterator);
            if (test->getPolicyName() == policyName)//TODO check the name is agent+policy here not agent+agent+policy or just policy
            {//if there is a match		
                QTable* table = ((WLearningProcess*) test)->getQTable();
                if (lastQTable != NULL)//TODO what does last qtable do?  I think it is an occasionally shuffled copy now not used to allow for multiple policies????
                {
                    std::vector<std::string> states = table->getStateOfRandom(); //table->getStateOfGreatestChang(); //table->getStateOfManyVisit();//table->getStateOfMostVisit();//table->getBestStates();//table->getStateOfMostVisit();//table->getBestStates();//dont use chang in ivanas as qtable nneds tobe chabged betwen policy types//table->getStateOfGreatestChang(lastQTable); 
                    std::vector<std::string>::iterator statesIterator = states.begin();
                    while (states.end() != statesIterator)
                    {//go through all returned states and get the pairs
                        if ((*statesIterator) == "")
                        {//if not a state do nothing
                        }
                        else
                        {
                            output.push_back(table->getQTableEntry((*statesIterator), table->getBestAction((*statesIterator)).first)); //TODO this is probably not interesting enough                                
                        }
                        statesIterator++;
                    }

                    states.clear();
                }
                else
                {

                    output.push_back(table->getQTableEntry(table->getRandomState(), table->getBestAction(table->getRandomState()).first));
                    delete lastQTable;
                    lastQTable = new QTable();
                    lastQTable->addFromQTable(table->getQTable());
                }
                if (rand() % 10 == 0)
                {//shuffle q table?
                    //delete lastQTable;//TODO this might cause amemory leek as the pointer is being re allocated but deleting causes an infinate hang
                    lastQTable = new QTable();
                    lastQTable->addFromQTable(table->getQTable());
                }
                break;
            }
            else
            {
                localIterator++;
            }
        }
        if (localIterator == localPolicies.end())
        {
            std::cerr << "DWLAgent findInterestingPair didn't add anything as could't find " << policyName << " \n";
        }
    }
    else
    {
        std::cerr << "DWLAgent findInterestingPair not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }
    //std::cerr << "found interesting pair" << output.begin()->first << " " << output.begin()->second.first << "  " << output.begin()->second.second << " \n";
    return output;
}

/**
 * find something to tell my neighbour about wether it is worth it or not
 * @return a pair (state (action value))
 */
std::pair<std::string, std::pair<std::string, double> > DWLAgent::findUninterestingPair(std::string policyName)
{
    std::pair<std::string, std::pair<std::string, double> > output;
    if (this->usingTransferLearning)
    {
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())
        {//for all
            Policy* test = (*localIterator);
            if (test->getPolicyName() == policyName)//TODO check the name is agent+policy here not agent+agent+policy or just policy
            {//if there is a match						
                QTable* table = ((WLearningProcess*) test)->getQTable();
                if (lastQTable != NULL)
                {
                    table->changeRandomState();
                    output = table->getQTableEntry(table->getRandomState(), table->getBestAction(table->getRandomState()).first);

                    //delete lastQTable;
                }
                else
                {
                    table->changeRandomState();
                    output = table->getQTableEntry(table->getRandomState(), table->getBestAction(table->getRandomState()).first);
                    lastQTable = new QTable();
                    lastQTable->addFromQTable(table->getQTable());
                }
                if (rand() % 10 == 0)
                {
                    lastQTable = new QTable();
                    lastQTable->addFromQTable(table->getQTable());
                }
                break;
            }
            else
            {
                localIterator++;
            }
        }
        if (localIterator == localPolicies.end())
        {
            std::cerr << "DWLAgent findInterestingPair didn't add anything as could't find " << policyName << " \n";
        }
    }
    else
    {
        std::cerr << "DWLAgent findInterestingPair not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }

    //std::cerr<<"found interesting pair"<<output.first<<" "<<output.second.first<<"  "<<output.second.second<<" \n";
    return output;
}

/**
 *
 *return total reward ever
 */
double DWLAgent::getHistoricReward()
{
    return historicReward;
}

/**
 * add the string to the classes member
 * @param input
 */
void DWLAgent::readTransferedWInfoIn(std::vector<std::pair<std::string, std::pair<std::string, double> > > input)
{//std::cerr<<"readTransferedWInfoIn size "<<input.size()<<"\n";
    if (this->usingTransferLearning)
    {
        std::vector<std::pair<std::string, std::pair<std::string, double> > >::iterator inputIterator = input.begin();
        while (inputIterator != input.end())
        {//for all pairs
            std::pair<std::string, std::pair<std::string, double> > test = *inputIterator;
            std::vector<Policy*>::iterator localIterator = localPolicies.begin();
            while (localIterator != localPolicies.end())
            {
                Policy* local = *localIterator;
                //std::cerr<<"readTransferedWInfoIn Compare |"<<test.first <<"|  |"<< local->getPolicyName()<<"|\n";
                if (test.first == local->getPolicyName())//TODO as before check anme form
                {//this update belongs to this policy
                    //handle it
                    std::string state = test.second.first;
                    //std::cout<<"w state recived= "<<state<<" with value "<<test.second.second<<"\n";
                    double oldValue = ((WLearningProcess*) local)->getWTable()->getWValue(state);
                    double newValue = test.second.second;
                    double outputValue = oldValue / 2 + (newValue) / 2; //newValue/3+(2*oldValue)/3;//oldValue/3+(2*newValue)/3;//newValue;////wmerge

                    ((WLearningProcess*) local)->setWValue(state, outputValue);
                    localIterator = localPolicies.end(); //end loop
                }
                else
                {
                    localIterator++;
                }
            }
            inputIterator++;
        }
    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }
}

/**
 * add the string to the classes member
 * @param input
 */
void DWLAgent::readTransferedInfoIn(std::vector<std::pair<std::string, std::pair<std::string, double> > > input)
{
    //std::cerr << "readTransferedInfoIn size " << input.size() << "\n";
    if (this->usingTransferLearning)
    {
        std::vector<std::pair<std::string, std::pair<std::string, double> > >::iterator inputIterator = input.begin();
        while (inputIterator != input.end())
        {//for all pairs
            std::pair<std::string, std::pair<std::string, double> > test = *inputIterator;
            std::vector<Policy*>::iterator localIterator = localPolicies.begin();
            while (localIterator != localPolicies.end())
            {
                Policy* local = *localIterator;
                //std::cerr << "readTransferedInfoIn Compare |" << test.first << "|  |" << local->getPolicyName() << "|\n";
                if (test.first == local->getPolicyName())//TODO as before check anme form
                {//this update belongs to this policy
                    //handle it
                    //std::cout << test.second.first << " =state " << "\n";
                    std::string state = test.second.first;
                    std::string action = state.substr(state.find(":") + 1, (state.length() - (state.find(":") + 1)));
                    state = state.substr(0, state.find(":"));

                    double oldValue = ((WLearningProcess*) local)->getQTable()->getQValue(state, action);
                    double newValue = test.second.second;
                    double outputValue = newValue;
                    bool goodTransfer = false; //what to feed back
                    //std::cout << "values - old " << oldValue << " new " << newValue << "\n";
                    //origional used for exps

                    if (newValue > oldValue)
                    {//if new value is significantly different add it or if have no idea
                        outputValue = newValue;
                        goodTransfer = true;
                    }
                    else
                    {//merge them

                        outputValue = oldValue + ((newValue - oldValue) / 2);
                        goodTransfer = false;
                    }
                    //over 5
                    /*if (((WLearningProcess*) local)->getVisitCount(state, action) < 10)
                    {//if only seldom visited take what is offered if not redo the mapping (basicall assum ours better and ask for data elswhere))
                        outputValue = newValue;
                        goodTransfer = true;
                    }
                     */
                    /*if(((WLearningProcess*) local)->getVisitCount(state, action)<5)
                    {
                    outputValue=newValue;
                    }*/
                    /*if(((WLearningProcess*) local)->getVisitCount(state, action)<5)
                    {
                    outputValue=newValue;
                    }
                    else
                    {
                    outputValue=oldValue;
                    }*/
                    /*
                                        if (((WLearningProcess*) local)->getVisitCount(state, action) < 5)
                                        {
                                            int visitCount = ((WLearningProcess*) local)->getVisitCount(state, action);
                                            outputValue = (5 - visitCount) * newValue / 5 + visitCount * oldValue / 5;
                                            goodTransfer = true; //say was ok //todo turn on
                                        }///finish adaptive merge
                                        else
                                        {
                                            outputValue = oldValue;
                                        }*/
                    ((WLearningProcess*) local)->setQValue(state, action, outputValue);
                    //std::cout << "merging " << state << " " << action << " to " << outputValue << std::endl;
                    //now set feedback for the mapping
                    std::stringstream ss;
                    ss << state << ":" << action;
                    ((WLearningProcess*) local)->provideTransferFeedback(ss.str(), goodTransfer);
                    localIterator = localPolicies.end(); //end loop
                    break;
                }
                else
                {
                    localIterator++;
                }
            }

            inputIterator++;
        }
    }
    else
    {
        std::cerr << "not using transfer learning in DWLAgent so don't try to\n";
        exit(9867);
    }

}

/**
 * make the sting into a vector and give it to add
 * @param input
 */
void DWLAgent::readTransferedInfoIn(std::string input)
{
    //std::cerr << "about to readTransferedInfoIn\n";
    //std::cerr << input << "\n =input\n";
    std::string copy = input;
    input.erase(remove_if(input.begin(), input.end(), isspace), input.end());
    std::vector<std::pair<std::string, std::pair<std::string, double> > > output;
    std::vector<std::pair<std::string, std::pair<std::string, double> > > wOutput;
    std::string name, stateAction, value, readTransfer = "<transfer>", wTransfer = "<wtransfer>";
    //input=input.substr(15);//lenght of read transfer tag
    int count = 0;
    for (int a = 0; a < input.size(); a++)
    {
        if (input[a] == '<')
        {
            count++;
        }
    }
    //std::cout<<"number of < ="<<count<<"\n";
    while (count >= 3)//while enough <> for a thing
    {//while at least one tag left
        //input = input.substr((input.find(readTransfer) + readTransfer.length()), (input.length()-(input.find(readTransfer) + readTransfer.length())));
        int messageLenght = input.find(">", 0);
        messageLenght = input.find(">", messageLenght + 1);
        messageLenght = input.find(">", messageLenght + 1);
        messageLenght = input.find(">", messageLenght + 1);
        std::string message = input.substr(0, messageLenght);
        //std::cerr<<message<<"=message "<< messageLenght<<"\n";
        //std::cerr<<input<<"=input\n";
        std::string type = message.substr(0, (message.find(">", 0) + 1));
        //std::cerr<<type<<"=type\n";
        message = message.replace(0, type.size(), "");
        //std::cerr<<message<<"=message "<< messageLenght<<"\n";
        if (type == readTransfer)
        {
            name = message.substr(1, message.find(">") - 1);
            message = message.substr(message.find(">") + 1);
            stateAction = message.substr(1, message.find(">") - 1);
            message = message.substr(message.find(">") + 1);
            value = message.substr(1, message.find(">") - 1);

            if (message.length() > 1)
            {
                message = message.substr(message.find(">") + 1);
            }
            //std::cerr << "reg trans " << name << " " << stateAction << "  " << atof(value.c_str()) << "  " << "\n";
            if ((stateAction.find("NOMATCH") != std::string::npos) || (stateAction.find("NO MATCH") != std::string::npos))
            {//recived an non-translatable pair
                std::cout << "recived a non-transferable pair, so discarding\n";
            }
            else
            {//if it was a corect pair do stuff
                std::pair<std::string, std::pair<std::string, double> > toAdd;
                toAdd.first = name;
                toAdd.second.first = stateAction;
                toAdd.second.second = atof(value.c_str());
                output.push_back(toAdd);
                count = 0;
            }

        }
        else if (type == wTransfer)
        {
            name = message.substr(1, message.find(">") - 1);
            message = message.substr(message.find(">") + 1);
            stateAction = message.substr(1, message.find(">") - 1);
            message = message.substr(message.find(">") + 1);
            value = message.substr(1, message.find(">") - 1);

            if (message.length() > 1)
            {
                message = message.substr(message.find(">") + 1);
            }
            //std::cerr<<"w trans "<<name<<" "<<stateAction<<"  "<<value<< "  " <<"-w \n";
            std::pair<std::string, std::pair<std::string, double> > toAdd;
            toAdd.first = name;
            toAdd.second.first = stateAction;
            toAdd.second.second = atof(value.c_str());
            wOutput.push_back(toAdd);
            count = 0;

        }

        input = input.replace(0, messageLenght + 1, ""); //delet the one we just had plus>
        //td::cerr << input << "=input\n";
        count = 0;
        for (int a = 0; a < input.size(); a++)
        {
            if (input[a] == '<')
            {
                count++;
            }
        }

    }
    //std::cout << output.size() << " = size\n";
    if (output.size() > 0)
    {
        this->readTransferedInfoIn(output);
    }
    //output.clear();//shouldnt need

    if (wOutput.size() > 0)
    {
        this->readTransferedWInfoIn(wOutput);
    }
    //wOutput.clear();
    //std::cout << "got here " << std::endl;
}

int DWLAgent::getNumberOfMappings()
{
    return mappings.size();
}

/**
 *turn on or off lerning
 */
void DWLAgent::manageLearning(bool learnQ, bool learnW)
{
    learningQ = learnQ;
    learningW = learnW;
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {//put locals into sugestion vector and see best
        Policy* test = *localIterator;
        ((WLearningProcess*) test)->setLearningQ(learnQ);
        ((WLearningProcess*) test)->setLearningW(learnW);
        localIterator++;
    }
    std::vector<Policy*>::iterator remoteIterator = remotePolicies.begin();
    while (remoteIterator != remotePolicies.end())
    {//put locals into sugestion vector and see best
        Policy* test = *remoteIterator;
        ((WLearningProcess*) test)->setLearningQ(learnQ);
        ((WLearningProcess*) test)->setLearningW(learnW);
        remoteIterator++;
    }
}

void DWLAgent::changeActionSelectionTemperature(int input)
{
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {//put locals into sugestion vector and see best
        Policy* test = *localIterator;
        ((WLearningProcess*) test)->setBoltzmannTemperature(input);
        ((WLearningProcess*) test)->setTaylorTemperature(input);
        localIterator++;
    }//TODO add remotes too
}

void DWLAgent::changeAlphaGamma(double alpha, double gamma)
{
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {//put locals into sugestion vector and see best
        Policy* test = *localIterator;
        ((WLearningProcess*) test)->setQAlpha(alpha);
        ((WLearningProcess*) test)->setQGamma(gamma);
        localIterator++;
    }//TODO add remotes too
}

/**
 * egready must be turned on for this to have any effect it is off by default
 * @param input the e value 0-1
 */
void DWLAgent::changeEGreadyE(double input)
{
    if (input > 1 || input < 0)
    {//do nothing if wrong e
        std::cerr << "Wrong e in DWLAgent change EGready e\n";
    }
    else
    {
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())
        {//put locals into sugestion vector and see best
            Policy* test = *localIterator;
            ((WLearningProcess*) test)->setEGreadyE(input);
            localIterator++;
        }//TODO add remotes too
    }
}

/**
 * choose how to select actions, only one at a time please
 * @param boltzmann ture is wanted on
 * @param eGready true is wanted on
 */
void DWLAgent::chooseActionSelectionMethod(bool boltzmann, bool eGready, bool taylor)
{
    if ((boltzmann + eGready + taylor) > 1)
    {//do nothing if none or both action selections
        std::cout << "can t use more than 1 action selection\n";
    }
    else
    {
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())
        {//all locase need to be changed
            Policy* test = *localIterator;
            ((WLearningProcess*) test)->setUseBoltzmann(boltzmann);
            ((WLearningProcess*) test)->setUseEGready(eGready);
            ((WLearningProcess*) test)->setUseTaylor(taylor);
            localIterator++;
        }//TODO add remotes too
    }
}

std::vector<QTable*> DWLAgent::getLocalQTables()
{
    std::vector<QTable*> output;
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())
    {//all locase need to be changed
        Policy* test = *localIterator;
        output.push_back(((WLearningProcess*) test)->getQTable());
        localIterator++;
    }
    return output;
}

/**
 *write reward to file
 * input is the bit after name
 */
void DWLAgent::printReward(std::string input, std::string tag)
{
    //std::cout << "start print mappings" << std::endl;
    std::string filename = this->getName() + input + "-reward.txt." + tag + ".stats";
    //std::cerr << "writing " << filename << "\n";
    std::ofstream outputfile(filename.c_str());

    if (outputfile.is_open())
    {//std::cerr<<rewardLog.size() <<"= log size\n";
        std::vector<double>::iterator rewardIterator = rewardLog.begin();
        while (rewardIterator != rewardLog.end())
        {//std::cerr<<(*rewardIterator) <<"\n";
            outputfile << (*rewardIterator) << ",";
            rewardIterator++;
        }
        outputfile << "\r\n"; //eof
        outputfile.close();
    }
    else
    {
        std::cerr << "\nQtable->writeStateActionToFile Unable to open file\n";
        exit(89);
    }
    //std::cout << "Print reward done!\n";
}

/**
 print the mappings 
 mode =0 back and forwards, 1=source first -1 = target first*/
void DWLAgent::printMappings(std::string filename, int mode, std::string tag)
{
    std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
    while (mappingIterator != mappings.end())
    {//fort all mappings
        if (mode <= 0)
        {//target first
            (*mappingIterator)->printMappingTargetFirst(filename, tag);
        }
        if (mode >= 0)
        {//source first
            (*mappingIterator)->printMappingSourceFirst(filename, tag);
        }
        mappingIterator++;
    }
}

/**
 * go through all mappings and update them based on ants
 */
void DWLAgent::updateLearnedMappingFromAnts()
{
    std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
    while (mappingIterator != mappings.end())
    {//fort all mappings
        //get this mappings target
        (*mappingIterator)->runAnts(100, 10);
        mappingIterator++;
    }
}

/**
 * go through all mappings and update them based on feed back
 * @param sourceStateActionToUpdate
 * @param targetStateToUpdate
 * @param good
 */
void DWLAgent::updateLearnedMappingFromTarget()
{
    std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
    while (mappingIterator != mappings.end())
    {//fort all mappings
        //get this mappings target
        std::vector<Policy*>::iterator localIterator = localPolicies.begin();
        while (localIterator != localPolicies.end())//TODO check only local
        {//all locase need to be changed
            Policy* test = *localIterator;
            if (test->getPolicyName() == (*mappingIterator)->getTargetName())//TODO check correct name here
            {//found correct policy
                //get its state that it recieved and didnt/did like
                std::vector < std::pair < std::string, bool> > feedback = ((WLearningProcess*) test)->getTransferFeedback();
                //give it to the mapping to respond to
                std::vector < std::pair < std::string, bool> >::iterator feedbackIterator = feedback.begin();
                while (feedbackIterator != feedback.end())
                {
                    (*mappingIterator)->updateLearnedMappingFromTarget((*feedbackIterator).first, (*feedbackIterator).second);
                    feedbackIterator++;
                }
                break;

            }
            localIterator++;
        }

        //feed back to mapping


        mappingIterator++;
    }
}

std::string DWLAgent::sendFeedback()
{
    std::string output;
    std::vector<Policy*>::iterator localIterator = localPolicies.begin();
    while (localIterator != localPolicies.end())//TODO check only local
    {//for all local policies
        std::vector < std::pair < std::string, bool> > feedback = ((WLearningProcess*) (*localIterator))->getTransferFeedback();
        //std::cout << "sending " << feedback.size() << " items of feedback" << std::endl;
        std::vector < std::pair < std::string, bool> >::iterator feedbackIterator = feedback.begin();
        while (feedbackIterator != feedback.end())
        {//for all feedback make a string version
            output += "<feedback>";
            output += "<";
            output += (*localIterator)->getPolicyName();
            output += ">";
            output += "<";
            output += (*feedbackIterator).first;
            output += "><";
            std::stringstream ss;
            ss << (*feedbackIterator).second;
            output += ss.str();
            output += ">\n";
            feedbackIterator++;

        }
        localIterator++;
    }

    return output;
}

/**
 * turn the feedback into an effect on mapping
 * @param 
 */
void DWLAgent::recieveFeedback(std::string input)
{
    int count = 0;
    for (int a = 0; a < input.size(); a++)
    {
        if (input[a] == '<')
        {
            count++;
        }
    }

    while (count >= 3)//while enough <> for a thing
    {//while at least one tag left
        int messageLenght = input.find(">", 0);
        messageLenght = input.find(">", messageLenght + 1);
        messageLenght = input.find(">", messageLenght + 1);
        messageLenght = input.find(">", messageLenght + 1);
        std::string message = input.substr(0, messageLenght);
        //std::cerr << message << "=message " << messageLenght << "\n";
        //std::cerr << input << "=input\n";
        std::string type = message.substr(0, (message.find(">", 0) + 1));
        //std::cerr << type << "=type\n";
        message = message.replace(0, type.size(), "");
        // std::cerr << message << "=message " << messageLenght << "\n";

        if (type == "<feedback>")
        {
            std::string target = message.substr(1, message.find(">") - 1);
            message = message.substr(message.find(">") + 1);
            std::string stateAction = message.substr(1, message.find(">") - 1);
            message = message.substr(message.find(">") + 1);
            std::string value = message.substr(1, message.find(">") - 1);

            if (message.length() > 1)
            {
                message = message.substr(message.find(">") + 1);
            }
            //std::cerr << "recieved from transfer partner " << target << " " << stateAction << "  " << value << "  " << "\n";
            //now we have msg parse it and apply it
            std::vector<TransferMapping*>::iterator mappingIterator = mappings.begin();
            while (mappingIterator != mappings.end())
            {//fort all mappings
                //std::cout << "dwlagent recieveFeedback|" << (*mappingIterator)->getTargetName() << "| and the target= |" << target << "|" << std::endl;
                if ((*mappingIterator)->getTargetName() == target)
                {//see it the update is from this mappings target
                    //std::cerr << "found mapping" << std::endl;
                    //found our mapping
                    (*mappingIterator)->updateLearnedMappingFromTarget(stateAction, (bool)atoi(value.c_str()));
                    break;
                }
                mappingIterator++;
            }
        }
        input = input.replace(0, messageLenght + 1, ""); //delet the one we just had plus>
        //std::cerr<<input<<"=input\n";
        count = 0;
        for (int a = 0; a < input.size(); a++)
        {
            if (input[a] == '<')
            {
                count++;
            }
        }

    }


}