/* 
 * File:   DWLAgent.h
 * Author: Adam
 *
 * Created on July 11, 2012, 9:10 AM
 */
#ifndef DWLAGENT_H
#define	DWLAGENT_H

#include "Policy.h"
#include "Reward.h"
#include "WLearningProcess.h"
#include "CollaborationProcess.h"
#include <string>
#include "set"
#include "TransferMapping.h"
#include "WTable.h"

class Compare {
public:

    bool operator()(std::string s1, std::string s2) {
        if (s1.compare(s2) < 0)
            return true;
        else
            return false;
    }
};

/**
 * This class will extend agent and provide what is needed for a domain free DWL agent.  It will have to be extended to a particular appliction.
 */
class DWLAgent {
public:
    DWLAgent(std::string ID);
    ~DWLAgent();
    std::string nominate();
    std::string getName();
    void updateLocal(std::string stateName);
    void updateLocal(std::string stateName, double rewardValueIn);
    void updateRemote();
    Policy* addLocalPolicy(std::string name, Reward* rewardIn);
    std::vector<WTable*> publishLocalPolicies();
    std::string publishLocalPolicies(std::string path, std::string tag);
    // TODO - change Remote to look like Local?
    void addRemotePolicy(std::string path); //place holder
    void addRemotePolicy(std::string name, WTable dopIn); //place holder
    void addRemotePolicy(std::string name, std::vector<WTable*> dopIn);
    void addRemotePolicy(std::vector<std::pair<std::string, WTable*> > policies);
    void addMessage(std::string name, std::pair<std::string, double> messageIn); //place holder 
    void addMessage(std::pair< std::string, std::pair<std::string, double> >); //place holder 
    void addMessage(std::string messageIn); //place holder 
    std::pair< std::string, std::pair<std::string, double> > writeMessage();
    std::string writeMessage(int);
    void purgeCommsBuffer(); //clear the comms
    void finishRun(); //convinience method learn from c etc
    void purgeSugestedActionsBuffer(); //clear the comms
    void createColaberationPolicy(std::string name, int numberOfvalues, double maxValue);
    std::string getName() const;
    void writePolicies(std::string name, std::string tag); //put out file for the policies
    int numberOfLocalPolicies();
    int numberOfRemotePolicies();
    QTable* replaceActionsInRemoteStateSpace(WTable* wIn);
    void readInComms(std::vector< std::pair< std::string, std::pair<std::string, double> > > input);
    void readPolicies(std::string name);
    void addNeighbours(std::string name); //place holder change parameter when i no what gridlab does
    int numberOfNeighbours();
    void setUsingTransferLearning(bool usingTransferLearning);
    bool getUsingTransferLearning() const;
    void addMapping(std::string sourceNameIn, std::string targetNameIn, QTable* sourceIn, QTable* targetIn);
    std::vector<std::pair<std::string, std::pair<std::string, double> > > transferToAllFromOne(std::string policyName);
    std::string transferToAllFromAll();
    std::vector<std::pair<std::string, std::pair<std::string, double> > > findInterestingPair(std::string policyName);
    std::vector<std::pair<std::string, double> > findInterestingW(std::string policyName);
    void findAllInterestingPairs(std::vector < std::pair<std::string, std::pair<std::string, double> > >& input);
    void findAllInterestingWs(std::vector < std::pair<std::string, std::pair<std::string, double> > >& input);
    std::pair<std::string, std::pair<std::string, double> > findUninterestingPair(std::string policyName);
    void readTransferedInfoIn(std::vector<std::pair<std::string, std::pair<std::string, double> > > input);
    void readTransferedInfoIn(std::string input);
    void readTransferedWInfoIn(std::vector<std::pair<std::string, std::pair<std::string, double> > > input);
    int getNumberOfMappings();
    double getHistoricReward();
    void manageLearning(bool learnQ, bool learnW);
    void changeActionSelectionTemperature(int input);
    void changeAlphaGamma(double alpha, double gamma);
    void changeEGreadyE(double input);
    void chooseActionSelectionMethod(bool boltzmann, bool eGready, bool taylor);
    std::vector<QTable*> getLocalQTables();
    void printReward(std::string input, std::string tag);
    void addMapping(std::string sourceNameIn, std::string targetNameIn, WTable* sourceIn, WTable* targetIn);
    void printMappings(std::string filename, int mode, std::string tag);
    void updateLearnedMappingFromTarget();
    void updateLearnedMappingFromSource(std::string stateToUpdate);
    std::string sendFeedback();
    void recieveFeedback(std::string input);
    void loadMapping(std::string sourceNameIn, std::string targetNameIn, std::string address);
protected:
    std::vector<Policy*> localPolicies; //this is the policies this agent has regardless of neighbours
    std::vector<Policy*> remotePolicies; //these are the policies gathered from neighbours
    std::set<std::string, Compare> neighbours;
    CollaborationProcess* collaborationPolicy; //this is used to learn the colaberation coefficent
    std::vector< std::pair< std::string, std::pair<std::string, double> > > sugestedActions; //this holds the action proposed by each policy and the policy, prior to a winner being picked.  The KeyValue pair is currentState, sugestedAction and the W value for them.
    std::string previousWinner; //used to update the learning values
    std::vector< std::pair< std::string, std::pair<std::string, double> > > communications; //this will store the information recieved from neigbours
    std::string name; //shouldnt have spaces as this causes file name problems
    double totalReward; //total this time used for colaberation
    double historicReward; //total ever
    double calculatedRewardForPreviousAction; //store the reward that was got for an action so it can be communicated to neighbours
    //transfer stuff
    bool usingTransferLearning;
    std::vector<TransferMapping*> mappings;
    std::vector<TransferMapping*> wMappings;
    QTable* lastQTable; //TODO will need to be in policy or vector for multi policy tl
    bool learningQ;
    bool learningW;
    std::string winningAction; //the action that was used for internal use only
    std::vector<double> rewardLog;

}; //TODO change what the updates actually mean ie drop actions make general

#endif	/* DWLAGENT_H */

