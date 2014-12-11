/* 
 * File:   CollaborationProcess.h
 * Author: Adam
 *
 * Created on July 12, 2012, 11:46 AM
 */

#include "WLearningProcess.h"

#ifndef CollaborationProcess_H
#define	CollaborationProcess_H

class CollaborationProcess : public WLearningProcess {
public:
    CollaborationProcess(std::string name, int numberOfStates, double maxRange);
    ~CollaborationProcess();
    void update(std::string worldState, double rewardValueIn, bool winningAction);
private:
    double changeBy;
    double maxValue;
    int numberOfRuns;
};

#endif	/* CollaborationProcess_H */

