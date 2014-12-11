/* 
 * File:   WLearningProcess.cpp
 * Author: Adam
 * 
 * Created on July 11, 2012, 9:31 AM
 */

#include "WLearningProcess.h"
#include "Boltzmann.h"
#include "NeighbourReward.h"
#include "CollaborationProcess.h"
#include "TaylorSeriesSelection.h"
#include <sstream>
#include <map>

#include <vector>
using std::vector;

WLearningProcess::WLearningProcess(std::string name) : Policy(name)
{
    anyStatesAdded = false;
    this->setPolicyName(name);
    isFirst = true;

    qTable = new QTable(); //set up our tables
    wTable = new WTable(name);
    qTable->setAlpha(0.01);
    qTable->setGamma(.4);
    wTable->setAlpha(.1);
    wTable->setGamma(.4);

    boltzmann = new Boltzmann();
    boltzmann->setTemperature(500); //put in some values untill better are added        

    taylor = new TaylorSeriesSelection();
    taylor->setTemperature(500);
    eGready = new EGready();

    this->setUseBoltzmann(true); //make use botlz only


    learningQ = true;
    learningW = true;
    mostRecentReward = 0;
}

/**
 *put the feeb back recived into the vector 
 *
 */
void WLearningProcess::provideTransferFeedback(std::string stateName, bool feedback)
{
    std::pair < std::string, bool> toAdd;
    toAdd.first = stateName;
    toAdd.second = feedback;
    //std::cout << "Feedingback to " << stateName << " with " << feedback << std::endl;
    transferFeedback.push_back(toAdd);
    //std::cout << "Feedingback size in wlp " << transferFeedback.size() << " " << this->getPolicyName() << "\n";
}

void WLearningProcess::clearTLFeadback()
{

    transferFeedback.clear();
    std::vector < std::pair < std::string, bool> > emptyVectorSB;
    transferFeedback.swap(emptyVectorSB);
    //std::cout<<"cleared tl feed bk"<<transferFeedback.size()<<"\n";
}

WLearningProcess::~WLearningProcess()
{
    delete wTable;
    delete qTable;
    delete boltzmann;
    delete eGready;
    delete taylor;
    delete reward;
    transferFeedback.clear();
    std::vector < std::pair < std::string, bool> > emptyVectorSB;
    transferFeedback.swap(emptyVectorSB);
}

/**
 * this one is used to create a colaberation policey and should not be used for other reasons
 * @param numberOfStates
 * @param maxRange
 */
WLearningProcess::WLearningProcess(std::string name, int numberOfStates, double maxRange) : Policy(name)
{
    anyStatesAdded = false;
    isFirst = true;
    qTable = new QTable(); //set up our tables
    wTable = new WTable(name);
    qTable->setAlpha(.9);
    qTable->setGamma(.6);
    wTable->setAlpha(.9);
    wTable->setGamma(.6);

    boltzmann = new Boltzmann();
    boltzmann->setTemperature(2500); //put in some values untill better are added      

    eGready = new EGready();

    this->setPolicyName(name);

    //make the state space
    //put everything into the internal representation
    std::string actions[3] = {"+", "-", "0"};

    for (int a = 0; a <= numberOfStates; a++)
    {//for all the c values
        std::stringstream strs;
        strs << (a * maxRange / numberOfStates);
        std::string str = strs.str();
        for (int b = 0; b < 3; b++)
        {//for all the actions
            if (a == 2)
            {//set a valid state to curnet and next
                currentAction = actions[b]; //init the storage
                currentState = str;
                previousState = str;
            }
            //add the states to the q table
            qTable->addStateAction(str, actions[b], 0);
        }
        wTable->addState(str, 0);
    }
}

/**
 * adds the policies name to the suggestion
 * @return 
 */
std::pair<std::string, std::pair<std::string, double> > WLearningProcess::getSuggestion()
{
    std::pair<std::string, std::pair<std::string, double> > output;
    output.first = this->getPolicyName();
    output.second = this->generateActionSuggestion();
    return output;
}

/**
 * interface into dop's get available action
 * @return a vector of actions
 */
std::vector<std::pair<std::string, double> > WLearningProcess::getAvailableActions(std::string s)
{
    return qTable->getActionsFromState(s);
}

void WLearningProcess::setCurrentAction(std::string current_Action)
{
    currentAction = current_Action;
}

std::string WLearningProcess::getCurrentAction() const
{
    return currentAction;
}

void WLearningProcess::setPreviousState(std::string previous_State)
{
    previousState = previous_State;
}

std::string WLearningProcess::getPreviousState() const
{
    return previousState;
}

void WLearningProcess::setCurrentState(std::string current_State)
{
    currentState = current_State;
}

std::string WLearningProcess::getCurrentState() const
{
    return currentState;
}

void WLearningProcess::setLearningQ(bool in)
{
    learningQ = in;
}

bool WLearningProcess::getLearningQ()
{
    return learningQ;
}

void WLearningProcess::setLearningW(bool in)
{
    learningW = in;
}

bool WLearningProcess::getLearningW()
{
    return learningW;
}

void WLearningProcess::setUseEGready(bool useEGready)
{
    this->useEGready = useEGready;
    this->useBoltzmann = this->useTaylor = false; //make sure only one used
}

bool WLearningProcess::getUseEGready() const
{
    return useEGready;
}

void WLearningProcess::setUseBoltzmann(bool useBoltzmann)
{
    this->useBoltzmann = useBoltzmann;
    this->useEGready = this->useTaylor = false; //make sure only one used
}

void WLearningProcess::setUseTaylor(bool useTaylor)
{
    this->useTaylor = useTaylor;
    this->useEGready = this->useBoltzmann = false; //make sure only one used
}

bool WLearningProcess::getUseBoltzmann() const
{
    return useBoltzmann;
}

bool WLearningProcess::getUseTaylor() const
{
    return useTaylor;
}

void WLearningProcess::setEGreadyE(double input)
{
    this->eGready->setE(input);

}

/*
 *set the current stuff in read from table states only
 *needs to be done so first actionn can be found
 */
void WLearningProcess::initalise()
{
    currentState = qTable->getRandomState();
    previousState = qTable->getRandomState();
}

/**
 *  this performs a time step and then puts an action state and w value together for the policy to then propergate
 * @return the action state and w
 */
std::pair<std::string, double> WLearningProcess::generateActionSuggestion()
{
    //get the action from the next state

    if (currentState.length() == 0 || currentState == "")
    {
        //std::cout<<"about to use initlaize in wlearn\n";
        this->initalise();
    }
    if (useBoltzmann)
    { //std::cout<<"about to use boltz in wlearn\n";
        currentAction = boltzmann->calculateNextAction(qTable->getActionsFromState(currentState));
    }
    else if (useEGready)
    {
        //std::cout<<"about to use eG in wlearn\n";
        currentAction = eGready->calculateNextAction(qTable->getActionsFromState(currentState));
    }
    else if (useTaylor)
    {
        //std::cout<<"about to use eG in wlearn\n";
        currentAction = taylor->calculateNextAction(qTable->getActionsFromState(currentState));
    }
    return std::pair<std::string, double>(currentAction, wTable->getWValue(currentState));
}

/**
 * teach the process based on how the world is now
 * @param worldState new state
 * @param rewardValueIn how good it was if calc'ed externally
 * @param winningAction if this process won in nomination
 */
void WLearningProcess::update(std::string worldState, double rewardValueIn, bool winningAction)
{
    //std::cerr<<worldState<<" =world";
    //std::cout << boltzmann->getTemperature() << " = temprature\n";
    boltzmann->setTemperature(boltzmann->getTemperature() - 2);
    previousState = currentState; //as getting an update we now have a new state to learn about
    //action that won must be set to learn correctly
    // reward->setReward(rewardValueIn); //has to this way round to prevent lost updates from calc;
    //reward->calcReward(worldState); //will do nothing in the neighbour reward //at this point reward will reflect the transition that acctually happend
    // reward->calcReward(previousState, worldState);
    currentState = worldState;
    if (winningAction)
    {//if this should learn from where the wold is now		

        reward->setReward(rewardValueIn); //has to this way round to prevent lost updates from calc;
        reward->calcReward(previousState, worldState); //will do nothing in the neighbour reward 
        mostRecentReward = reward->getReward();
        qTable->setAlpha(this->getQAlpha()); //TODO rest of these and follow back
        qTable->setGamma(this->getQGamma());
        wTable->setAlpha(this->getWAlpha());
        wTable->setGamma(this->getWGamma());
        //std::cout<<this->getQAlpha()<<"= q alpha\n";
        if (learningQ)
        {
            qTable->qLearningUpdate(previousState, currentAction, currentState, reward->getReward()); //update the q value
        }
        //std::cerr << "WLearningProcess.Update someone won with no action " << worldState <<" "<< rewardValueIn<<"  "<<reward->getReward()<< "\n";

    }
    else
    {//should never update loosing action as dwlagent calls the action version when updating looser
        if (!isFirst)
        {
            isFirst = false;
            //the should have got reward this->reward->calcReward(worldState);//the reward i would have got
            //the actually got reward
            reward->setReward(rewardValueIn); //has to this way round to prevent lost updates from calc
            // reward->calcReward(worldState); //will do nothing in the neighbour reward 
            reward->calcReward(previousState, worldState);
            if (learningQ)
            {
                qTable->qLearningUpdate(previousState, currentAction, currentState, reward->getReward()); //update the q value
            }
            if (learningW)
            {
                //std::cout<<"Learningw 1\n";
                //w learn based on previous state, actual reward, best possible q, q from sugested action

                //			std::cout <<"wwwwwwww learning update with reward " << this->reward->getReward()<< std::endl;
                wTable->wLearningUpdate(previousState, this->reward->getReward(), qTable->getBestAction(currentState).second, qTable->getQValue(currentState, currentAction)); //update w value	//TODO what q value to use		

                // wTable->wLearningUpdate(previousState, this->reward->getReward(), qTable->getBestAction(currentState).second, qTable->getQValue(currentState, currentAction)); //update w value	//TODO what q value to use		
            }
        }
        //std::cerr << "WLearningProcess.Update "<<this->getPolicyName()<<" didn't win with no action " << worldState <<" "<< rewardValueIn <<"  "<<mostRecentReward<< "\n";
        mostRecentReward = rewardValueIn; //set this so that it can be communicated
    }
    //std::cerr<<" end\n";
    isFirst = false;
}

/**
 * teach the process based on how the world is now
 * @param worldState new state
 * @param action the action taken to get from there to wold state
 * @param rewardValueIn how good it was if calc'ed externally
 * @param winningAction if this process won in nomination
 */
void WLearningProcess::update(std::string worldState, std::string action, double rewardValueIn, bool winningAction)
{
    //std::cerr << worldState << "= 2 world ";
    boltzmann->setTemperature(boltzmann->getTemperature() - 2);
    previousState = currentState; //as getting an update we now have a new state to learn about
    currentState = worldState;
    //if won current action== action used
    if (winningAction)
    {//if this should learn from where the wold is now
        reward->setReward(rewardValueIn); //has to this way round to prevent lost updates from calc
        //reward->calcReward(worldState); //will do nothing in the neighbour reward 
        reward->calcReward(previousState, worldState);
        mostRecentReward = reward->getReward();
        qTable->setAlpha(this->getQAlpha()); //TODO rest of these and follow back
        qTable->setGamma(this->getQGamma());
        wTable->setAlpha(this->getWAlpha());
        wTable->setGamma(this->getWGamma());
        if (learningQ)
        {
            qTable->qLearningUpdate(previousState, currentAction, currentState, reward->getReward()); //update the q value
        }
        //std::cerr << "WLearningProcess.Update with action someone won " << worldState <<" "<<action<<" "<< rewardValueIn <<"  "<<reward->getReward()<< "\n";

    }
    else
    {//if lost action== my selected action
        if (!isFirst)
        {
            isFirst = false;
            reward->setReward(rewardValueIn); //has to this way round to prevent lost updates from calc
            reward->calcReward(previousState, worldState);

            //this->reward->calcReward(worldState); //the reward i would have got
            if (learningQ)
            {//learn from what actualy happend the action used
                qTable->qLearningUpdate(previousState, action, currentState, reward->getReward()); //update the q value
            }
            if (learningW)
            {//the action that i selected is in current action
                //reward->setReward(rewardValueIn); //has to this way round to prevent lost updates from calc
                //reward->calcReward(worldState); //will do nothing in the neighbour reward 
                //reward->calcReward(previousState, worldState);
                //now reward->get reward gives what was actually recive by executing winning action
                //w learn based on previous state, actual reward, best possible q, q from sugested action
                //std::cout<<"Learningw 2\n";

                //			std::cout <<"wwwwwwww learning update with reward " << this->reward->getReward()<< std::endl;
                wTable->wLearningUpdate(previousState, this->reward->getReward(), qTable->getBestAction(currentState).second, qTable->getQValue(currentState, currentAction)); //update w value	//TODO what q value to use		

            }
        }
        //std::cerr << "WLearningProcess.Update  with action no one won " << worldState <<" "<<action<<" "<< rewardValueIn <<"  "<<mostRecentReward<< "\n";
        mostRecentReward = rewardValueIn; //set this so that it can be communicated
    }
    //std::cerr<<" end 2\n";
    isFirst = false;
}

/**  charris
 *
 * adds a State Action pair to the WLearningProcess relevant data structures
 * returns true if added, false if State, Action already in data structure (optional??)
 * currently Model, WModel, state_space
 */
bool WLearningProcess::addStateAction(std::string state, std::string action)
{ //add to the transition map

    if (anyStatesAdded == false)
    {//if no states yet

        currentState = state;
        previousState = state;
        currentAction = action;
        anyStatesAdded = true;
    }

    qTable->addStateAction(state, action, 0);
    wTable->addState(state, 0);
    return true;
}

/*void WLearningProcess::setDop(DOP* dop)
{
this->dop = dop;
}*/

void WLearningProcess::setBoltzmannTemperature(double boltzmannTemperature)
{
    boltzmann->setTemperature(boltzmannTemperature);
}

double WLearningProcess::getTaylorTemperature() const
{
    return taylor->getTemperature();
}

void WLearningProcess::setTaylorTemperature(double taylorTemperature)
{
    taylor->setTemperature(taylorTemperature);
}

double WLearningProcess::getBoltzmannTemperature() const
{
    return boltzmann->getTemperature();
}

void WLearningProcess::setWGamma(double wGamma)
{
    if (wGamma > 1 || wGamma < 0)
    {
        std::cerr << "WLearningProcess - setWGamma  Passed value out of range " << wGamma << std::endl;
        exit(23);
    }
    wTable->setGamma(wGamma);
}

double WLearningProcess::getWGamma() const
{
    return wTable->getGamma();
}

void WLearningProcess::setWAlpha(double wAlpha)
{
    if (wAlpha > 1 || wAlpha < 0)
    {
        std::cerr << "WLearningProcess - setWAlpha  Passed value out of range " << wAlpha << std::endl;
        exit(23);
    }
    wTable->setAlpha(wAlpha);
}

double WLearningProcess::getWAlpha() const
{
    return wTable->getAlpha();
}

void WLearningProcess::setQGamma(double qGamma)
{
    if (qGamma > 1 || qGamma < 0)
    {
        std::cerr << "WLearningProcess - setQGamma  Passed value out of range " << qGamma << std::endl;
        exit(23);
    }
    qTable->setGamma(qGamma);
}

double WLearningProcess::getQGamma() const
{
    return qTable->getGamma();
}

void WLearningProcess::setQAlpha(double qAlpha)
{
    if (qAlpha > 1 || qAlpha < 0)
    {
        std::cerr << "WLearningProcess - setQAlpha  Passed value out of range " << qAlpha << std::endl;
        exit(23);
    }
    qTable->setAlpha(qAlpha);
}

double WLearningProcess::getQAlpha() const
{
    return qTable->getAlpha();
}

void WLearningProcess::setReward(Reward* reward)
{
    this->reward = reward;
}

double WLearningProcess::getMostRecentReward() const
{
    return mostRecentReward;
}

/**print the policy's state space*/
void WLearningProcess::printStateSpace(std::string name, std::string tag)
{

    qTable->writeStateActionToFile(name, tag);
    wTable->writeStateActionToFile(name, tag);
}

/**read the policy's state space. with out -q or -w as this adds them*/
void WLearningProcess::readStateSpace(std::string name)
{
    std::stringstream s1;
    s1 << name; //isolate the correct file
    s1 << "-q";
    std::stringstream s2;
    s2 << name;
    s2 << "-w";
    qTable->readStateActionFromFile(s1.str());
    wTable->readStateActionFromFile(s2.str());
}

/**
 * get a q value only for colabprocess as private table
 * @param stateName
 * @param actionName
 * @return 
 */
double WLearningProcess::getQValue(std::string stateName, std::string actionName)
{
    return qTable->getQValue(stateName, actionName);
}

/**
 * get a w value only for colabprocess as private table
 * @param stateName
 * @return 
 */
double WLearningProcess::getWValue(std::string stateName)
{
    return wTable->getWValue(stateName);
}

WTable* WLearningProcess::getWTable()
{
    return wTable;
}

QTable* WLearningProcess::getQTable()
{
    return qTable;
}

/**
 * this takes in the passed table and sets them.  intended to take a w table and the q with my actions version
 * @param w
 * @param q
 */
void WLearningProcess::addRemoteFromWTable(WTable* w, QTable* q)
{
    this->qTable = q;
    this->wTable = w;
}

std::vector<std::string> WLearningProcess::getActions()
{
    std::vector<std::string> output;
    std::vector<std::pair<std::string, double> > actionValues = qTable->getActionsFromState(currentState);
    std::vector<std::pair<std::string, double> >::iterator actionValuesIterator = actionValues.begin();
    while (actionValuesIterator != actionValues.end())
    {
        output.push_back((*actionValuesIterator).first);
        actionValuesIterator++;
    }
    actionValues.clear();
    return output;
}

void WLearningProcess::setQValue(std::string stateName, std::string actionName, double value)
{
    qTable->setQValue(stateName, actionName, value);
}

void WLearningProcess::setWValue(std::string stateName, double value)
{
    wTable->setWValue(stateName, value);
}

int WLearningProcess::getVisitCount(std::string stateName, std::string actionName)
{
    return qTable->getVisitCount(stateName, actionName);
}

std::vector<std::pair<std::string, bool> > WLearningProcess::getTransferFeedback()
{
    std::vector < std::pair < std::string, bool> >temp = transferFeedback;
    //std::cout << transferFeedback.size() << "= feed back size in wLP\n";
    //std::cout << temp.size() << " " << this->getPolicyName() << "= feed back size temp in wLP\n";
    transferFeedback.clear(); //we're done with it
    return temp;
}