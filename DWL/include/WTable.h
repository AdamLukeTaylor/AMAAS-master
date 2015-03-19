/* 
 * File:   WTable.h
 * Author: Adam
 *
 * Created on October 8, 2012, 10:11 AM
 */
#include <vector>
#include <string>
#include <map>

#ifndef WTABLE_H
#define	WTABLE_H

class WTable {
public:
    WTable(std::string);
    WTable();
    virtual ~WTable();

    void addState(std::string stateName, double value);
    double getWValue(std::string stateName);
    void setWValue(std::string stateName, double value);
    void writeStateActionToFile(std::string filenameIn, std::string tag);
    bool readStateActionFromFile(std::string filenameIn, std::string tag);
    void wLearningUpdate(std::string previousStateName, double reward, double maxQForCurrentState, double qForCurrentStateLoosingAction);
    void setGamma(double gamma);
    double getGamma() const;
    void setAlpha(double alpha);
    double getAlpha() const;
    void setID(std::string ID);
    std::string getID() const;
    std::map<std::string, double> getWTable() const;
    std::pair<std::string, double> getWTableEntry(std::string input);
    std::vector<std::string> getStateOfSomething();
    std::vector<std::string> getStateOfHighestW();
    std::vector<std::string> getStatesOfHighest();
private:
    double alpha;
    std::string ID;
    double gamma;
    std::map<std::string, double > wTable; //map of statename as key to an action and value
};

#endif	/* WTABLE_H */

