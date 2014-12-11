/* 
 * File:   TransferMapping.h
 * Author: Adam
 *
 * Created on November 5, 2012, 3:42 PM
 */
#include "QTable.h"
#include "WTable.h"
#include <map>
#include <string>

#ifndef TRANSFERMAPPING_H
#define	TRANSFERMAPPING_H

class TransferMapping {
public:
    TransferMapping();
    virtual ~TransferMapping();
    void setTarget(QTable* target);
    QTable* getTarget() const;
    void setSource(QTable* source);
    QTable* getSource() const;
    void setWTarget(WTable* target);
    WTable* getWTarget() const;
    void setWSource(WTable* source);
    WTable* getWSource() const;
    void populateMappingRandomly();
    void populateMappingBySearch();
    void populateLearningMapping();
    void populateWMappingBySearch();
    std::pair<std::string, std::pair<std::string, double> > mapFromStateToTarget(std::pair<std::string, double> input);
    std::vector<std::pair<std::string, std::pair<std::string, double> > > mapFromStateToTarget(std::vector<std::pair<std::string, double> >input);
    std::pair<std::string, double> mapWFromStateToTarget(std::pair<std::string, double> input);
    std::vector<std::pair<std::string, double> > mapWFromStateToTarget(std::vector<std::pair<std::string, double> >input);
    static std::pair<std::string, double> makePairForMapper(std::pair<std::string, std::pair<std::string, double> > input);
    static std::vector<std::pair<std::string, double> > makePairForMapper(std::vector<std::pair<std::string, std::pair<std::string, double> > >input);
    bool getSourceSet();
    bool getTargetSet();
    bool getWSourceSet();
    bool getWTargetSet();
    void setTargetName(std::string TargetName);
    std::string getTargetName() const;
    void setSourceName(std::string sourceName);
    std::string getSourceName() const;
    void printMappingSourceFirst(std::string input, std::string tag);
    void printMappingTargetFirst(std::string input, std::string tag);
    void updateLearnedMappingFromTarget(std::string targetStateToUpdate, bool good);
    void updateLearnedMappingFromSource(std::string stateToUpdate);
    std::vector<std::pair<std::string, double> > choosePairsBasedOnVotes(int numberOfPairs, double greadiness, QTable* qIn);
    void loadMapping(std::string filename);
    std::vector<std::string> split(std::string s, char delim);
private:
    std::string sourceName;
    std::string TargetName;
    bool sourceSet;
    bool targetSet;
    bool wSourceSet;
    bool wTargetSet;
    QTable* source;
    QTable* target;
    WTable* wSource;
    WTable* wTarget;
    std::multimap<std::string, std::pair<std::string, double> > mappings; //this is source state+action, target state+action, scaling factor for new info
    int deletedCount; //
    std::map<std::pair<std::string, std::string>, int> votes; //if it should change a mapping
    std::vector<std::string> sourceUnallocatedPool;
    std::vector<std::string> matchless;
    std::multimap<std::string, std::string> deleted;
    std::vector<std::string> sourceAllocatedPool;
    //std::vector<std::pair<std::string, std::string > > outBuffer; //memory of what has been sent from what
};

#endif	/* TRANSFERMAPPING_H */

