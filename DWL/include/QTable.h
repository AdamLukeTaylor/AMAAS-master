/* 
 * File:   QTable.h
 * Author: Adam
 *
 * Created on October 8, 2012, 10:11 AM
 */
#include <vector>
#include <string>
#include <map>
#include <iostream>

#ifndef QTABLE_H
#define	QTABLE_H

struct compareClass {

    bool operator()(const std::string, std::pair<std::string, double> &left, const std::string, std::pair<std::string, double> &right) const {
        std::cerr << "comparing" << "\n";
        if (left.first.compare(right.first) == 0) {
            return true;
        } else {
            return false;
        }
    }
};

class QTable {
public:
    QTable();
    virtual ~QTable();

    void addStateAction(std::string stateName, std::string actionName, double value);
    double getQValue(std::string stateName, std::string actionName);
    void setQValue(std::pair<std::string, std::pair<std::string, double> > input);
    void setQValue(std::string stateName, std::string actionName, double value);
    std::vector<std::pair<std::string, double> > getActionsFromState(std::string stateName);
    void writeStateActionToFile(std::string filenameIn, std::string tag);
    void readStateActionFromFile(std::string filenameIn, std::string tag);
    void qLearningUpdate(std::string previousStateName, std::string actionName, std::string currentStateName, double reward);
    std::pair<std::string, double> getBestAction(std::string stateName);
    void setGamma(double gamma);
    double getGamma() const;
    void setAlpha(double alpha);
    double getAlpha() const;
    std::multimap<std::string, std::pair<std::string, double> > getQTable() const;
    std::pair<std::string, std::pair<std::string, double> > getQTableEntry(std::string state, std::string action);
    std::string getRandomState();
    std::string randomState; //hold a state to hack something to work
    std::vector<std::string> getStateOfGreatestChang(); //community???//yes
    void addFromQTable(std::multimap<std::string, std::pair<std::string, double> > in);
    void changeRandomState();
    std::vector<std::string> getStateOfManyVisit();
    std::vector<std::string> getBestStates();
    //ADDED FOR EXP
    std::vector<std::string> getStateOfMostVisit();
    int getVisitCount(std::string stateName, std::string actionName);
    std::vector<std::string> getStateOfRandom();
    std::string getRandomAction();
    int numberOfStates();
    //end
private:
    double alpha;
    double gamma;

    std::multimap<std::string, std::pair<std::string, double> > qTable; //map of statename as key to an action and value
    //ADDED FOR EXP
    std::multimap<std::string, std::pair<std::string, int> > visitCount; //map of statename as key to an action and value
    std::multimap<std::string, std::pair<std::string, bool> > converged; //map of statename as key to an action and value
    std::multimap<std::string, std::pair<std::string, double> > qTablePrevious; //map of statename as key to an action and value
    //END
};

#endif	/* QTABLE_H */

