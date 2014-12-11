/* 
 * File:   WLearningProcess.h
 * Author: Adam
 *
 * Created on July 11, 2012, 9:31 AM
 */
#ifndef WLearningProcess_H
#define	WLearningProcess_H

#include "WTable.h"
#include "QTable.h"
#include "Reward.h"
#include "Boltzmann.h"
#include "EGready.h"
#include "Policy.h"
#include "TaylorSeriesSelection.h"

/***
 * This has everything that is needed for one policy in a DWL agent without any of the collaboration stuff.
 *
 * charris - I think it should extend Policy as it is a type of Policy - so we can easily plug in other
 * 			 types of Policy
 *
 */
class WLearningProcess : public Policy {
public:
    WLearningProcess(std::string name);
    WLearningProcess(std::string name, int numberOfStates, double maxRange);
    ~WLearningProcess();

    //interface methods
    std::pair<std::string, double> generateActionSuggestion();
    std::pair<std::string, std::pair<std::string, double> > getSuggestion();
    void update(std::string worldState, double rewardValueIn, bool winningAction);
    void update(std::string worldState, std::string action, double rewardValueIn, bool winningAction);
    bool addStateAction(std::string state, std::string action);

    //get and set
    void setBoltzmannTemperature(double boltzmannTemperature);
    double getBoltzmannTemperature() const;
    void setTaylorTemperature(double TaylorTemperature);
    double getTaylorTemperature() const;
    void setWGamma(double wGamma);
    double getWGamma() const;
    void setWAlpha(double wAlpha);
    double getWAlpha() const;
    void setQGamma(double qGamma);
    double getQGamma() const;
    void setQAlpha(double qAlpha);
    double getQAlpha() const;
    void setReward(Reward* reward);
    double getMostRecentReward() const;
    void printStateSpace(std::string name, std::string tag);
    void readStateSpace(std::string name);
    std::vector<std::pair<std::string, double> > getAvailableActions(std::string s);
    void setCurrentAction(std::string current_Action);
    std::string getCurrentAction() const;
    void setPreviousState(std::string next_State);
    std::string getPreviousState() const;
    void setCurrentState(std::string current_State);
    std::string getCurrentState() const;
    double getQValue(std::string stateName, std::string actionName);
    void setQValue(std::string stateName, std::string actionName, double value);
    double getWValue(std::string stateName);
    void setWValue(std::string stateName, double value);
    WTable* getWTable();
    QTable* getQTable();
    void addRemoteFromWTable(WTable* w, QTable* q);
    std::vector<std::string> getActions();
    void initalise();
    void setLearningQ(bool in);
    bool getLearningQ();
    void setLearningW(bool in);
    bool getLearningW();
    void setUseEGready(bool useEGready);
    bool getUseEGready() const;
    void setUseBoltzmann(bool useBoltzmann);
    bool getUseBoltzmann() const;
    void setUseTaylor(bool useTaylor);
    bool getUseTaylor() const;
    void setEGreadyE(double input);
    int getVisitCount(std::string stateName, std::string actionName);
    std::vector<std::pair<std::string, bool> >getTransferFeedback();
    void provideTransferFeedback(std::string stateName, bool feedback);
    void clearTLFeadback();
private:
    bool useBoltzmann;
    bool useEGready;
    bool useTaylor;
    bool anyStatesAdded; //used to ensure curent state action etc are initalisd to useful values
    WTable* wTable;
    QTable* qTable;
    std::string currentState, previousState, currentAction; //this is the state now in the one that we were in before executing the action to get here
    Boltzmann* boltzmann;
    EGready* eGready;
    TaylorSeriesSelection* taylor;
    Reward* reward;
    double mostRecentReward;
    bool isFirst;
    bool learningQ;
    bool learningW;
    std::vector < std::pair < std::string, bool> > transferFeedback;


};

#endif	/* WLearningProcess_H */

