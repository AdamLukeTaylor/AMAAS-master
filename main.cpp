/* 
 * File:   main.cpp
 * Author: Adam
 *
 * Created on 17 June 2014, 11:13
 */

#include <cstdlib>
#include <errno.h>
#include <sys/stat.h>
#include <iostream>
#include "CartPole.h"
#include "DWLAgent.h"
#include "CartPoleAgent.h"
#include "MtCarAgent.h"
#include <sstream>
#include "Constants.h"
#include "MtCar.h"
#include "limits.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
//#include <vld.h>

using namespace std;

double exploiteCartPole();
int poleRun(int number);
double exploiteMtCar();
int carRun(int number);
void duelRun(int number);
void setCarAgentForRun(double alpha, double gamma);
void setPoleAgentForRun(double alpha, double gamma);
void finaliseData();
void initData();
void writeData(std::string fileName, bool end, std::string tag);
void runJava(std::string location, std::string tag);
void cleanUp();

CartPoleAgent *poleAgent;
CartPole *pole;
int poleCurrentAction = 0;
double poleAveFallTime = 0;
int poleNumberOfFalls = 0;
int poleLoopCounter = 0;
int poleStepCount = 0;
int poleCurrentMax = 0;

int carLoopCounter = 0;
MtCar* car;
MtCarAgent* carAgent;
int carFinishCount = 0;
int carThisRunTime = 0;
int carAverageTime = 0;
int carCurrentMin = INT_MAX;

std::vector<int> poleSteps;
std::vector<int> carSteps;
double poleResults[Constants::NUMBER_OF_ALPHAS][Constants::NUMBER_OF_GAMMAS];
double poleFalls[Constants::NUMBER_OF_ALPHAS][Constants::NUMBER_OF_GAMMAS]; //dosent really add anytghing as pole alwasys fals
double carResults[Constants::NUMBER_OF_ALPHAS][Constants::NUMBER_OF_GAMMAS];
double carFinishes[Constants::NUMBER_OF_ALPHAS][Constants::NUMBER_OF_GAMMAS];
double alphas [Constants::NUMBER_OF_ALPHAS + 1];
double gammas [Constants::NUMBER_OF_GAMMAS + 1];

int currentRun = 0;
int currentAlpha = 0;
int currentGamma = 0;

//thes3 keep track of what we are doing
std::string expName[] = {
    "eval",
    "f9"//f 10, .001
}; //e =reward 1 fg2= reward 2 g= 1 tst not 3
int expNumber = 1;

/*
 * 
 */
int main(int argc, char** argv)
{
    for (int a = 0; a <= Constants::NUMBER_OF_ALPHAS; a++)
    {
        alphas[a] = ((double) a) / (Constants::NUMBER_OF_ALPHAS);
    }
    for (int a = 0; a <= Constants::NUMBER_OF_GAMMAS; a++)
    {
        gammas[a] = ((double) a) / (Constants::NUMBER_OF_GAMMAS);
    }
    //test transfer
    /* setCarAgentForRun(.5, .5);
     setPoleAgentForRun(.5, .5);
     poleAgent->manageLearning(false, false); //cant learn on its own
     poleAgent->setUsingTransferLearning(true);
     carAgent->setUsingTransferLearning(true);
     carAgent->addMapping("MtCar+MtCar", "CartPole+CartPole", carAgent->getLocalQTables().front(), poleAgent->getLocalQTables().front());
     carAgent->printMappings("one", 0);
     for (int a = 0; a < 1000; a++)
     {
         std::string transfer = carAgent->transferToAllFromAll(); //trnafer from car
         //std::cout << transfer << " =transfer\n";
         if (transfer.find("<0>") == std::string::npos)
         {
             //std::cout << transfer << " =======================================================================\n";
         }
         poleAgent->readTransferedInfoIn(transfer); //to pole
         int action = atoi(poleAgent->nominate().c_str());
         pole->executeAction(action);
         std::stringstream ss;
         ss << pole->getBox();
         //std::cout << "got here in amin " << std::endl;
         poleAgent->updateLocal(ss.str()); //generate evaluation of mapping
         std::string feedback = poleAgent->sendFeedback(); //send eval
         // std::cout << feedback << " =feedback" << std::endl;
         if (a % 1000 == 0)
         {
             // pole->reset();
             //  car->reset(false);
         }
         if (feedback.find("<0>") != std::string::npos)
         {//yote keep
             std::cout << transfer << " =transfer\n";
             std::cout << feedback << " =feedback\n";
         }
         carAgent->recieveFeedback(feedback); //apply eval
         //std::cout << "got here in amin " << std::endl;
         //carAgent->updateLearnedMappingFromTarget();
         int carAction = atoi(carAgent->nominate().c_str()); //move car round 
         bool carActionResult = car->executeAction(carAction);
         //cout << "action = " << carAction << " execute: " << carActionResult << "\n";
         //car->printState();
         carAgent->updateLocal(car->getState());
         carAgent->finishRun();

         // std::cout << "--------------------\n";
     }
     carAgent->printMappings("two", 0);
     */
    //test pole
    /* std::string filename = "test";
     //for (currentAlpha = 0; currentAlpha < Constants::NUMBER_OF_ALPHAS; currentAlpha++)
     {
         currentAlpha = 1;
         // cout << "--------------- run " << currentAlpha << "-----------------------\n";
         // for (currentGamma = 0; currentGamma <= Constants::NUMBER_OF_GAMMAS; currentGamma++)
         {
             int numberOfTimes = 20;
             currentGamma = 9;
             for (currentRun = 0; currentRun < Constants::NUMBER_OF_EXPS * numberOfTimes; currentRun++)
             {

                 setPoleAgentForRun(alphas[currentAlpha], gammas[currentGamma]);

                 poleSteps.push_back(poleRun((currentRun / numberOfTimes + 1)* 100)); //currentRun * 100);
                 std::stringstream ss;
                 ss << "CartPole+CartPole " << currentRun;
                 cout << "currentRun= " << (currentRun + 1) << "\n";
                 cleanUp();
                 ss.str("");
                 ss << "run " << (currentRun + 1);
                 writeData(ss.str(), false, expName[expNumber]); //write out polefall times
             }
         }
     }
     std::stringstream ss;
     ss << filename.c_str();
     ss.str("");
     ss << "final";
     writeData(ss.str(), true, expName[expNumber]); //flush all stats
     ss.str("");
     ss << "C://Users//Adam//Documents//NetBeansProjects//AMAAS-master//results//" << expName[expNumber] << "//overall " << expName[expNumber];
     runJava(ss.str(), expName[expNumber]);*/
    //duel run    
    initData();
    //std::cout << "1" << std::endl;

    std::string filename = "";

    //for (currentAlpha = 0; currentAlpha < Constants::NUMBER_OF_ALPHAS; currentAlpha++)
    {
        //currentAlpha = 8;
        cout << "--------------- run " << currentAlpha << "-----------------------\n";
        //for (currentGamma = 0; currentGamma < Constants::NUMBER_OF_GAMMAS; currentGamma++)
        //currentGamma = 8;
        {//set results
            int numberOfTimes = 90; //currentRun < Constants::NUMBER_OF_EXPS * numberOfTimes
            for (currentRun = 0; currentRun < 2; currentRun++)
            {
                setCarAgentForRun(alphas[9], gammas[1]); //alphas[currentAlpha], gammas[currentGamma]);
                setPoleAgentForRun(alphas[9], gammas[1]); //alphas[currentAlpha], gammas[currentGamma]);
                carAgent->addMapping("MtCar+MtCar", "CartPole+CartPole", carAgent->getLocalQTables().front(), poleAgent->getLocalQTables().front());
                if (currentRun > 0)
                {//if we've run befor load mapping
                    std::stringstream ss;
                    ss << "before";
                    carAgent->printMappings(ss.str(), 1, expName[expNumber]);
                    carAgent->loadMapping("MtCar+MtCar", "CartPole+CartPole", "C:\\Users\\Adam\\Documents\\NetBeansProjects\\AMAAS-master\\MtCar+MtCar to CartPole+CartPole reuse-mapping.txt.f9.stats");
                }
                int toRun = (currentRun / numberOfTimes) + 1;
                std::cout << toRun << "=torun\n";
                duelRun(toRun * Constants::CAR_TRAINING_STEPS); //(currentRun + 3) / 3);                

                //carSteps.push_back(carRun(toRun));
                //poleSteps.push_back(poleRun(toRun));
                std::stringstream ss;
                ss << "s=MtCar+MtCar " << "t= CartPole+CartPole " << currentRun;
                carAgent->printMappings(ss.str(), 1, expName[expNumber]);
                ss.str("");
                ss << "reuse";
                carAgent->printMappings(ss.str(), 1, expName[expNumber]);
                cout << "currentRun= " << (currentRun + 1) << "\n";
                cleanUp();
                std::stringstream ss1;
                ss1 << "run " << (currentRun + 1);
                writeData(ss1.str(), false, expName[expNumber]); //write out polefall times
            }



            //carAgent->writePolicies("end");
            //poleAgent->writePolicies("end");
            //put out stats
            //std::stringstream ss;
            //ss << filename.c_str();
            //ss.str("");
            //ss << "C://Users//Adam//Documents//NetBeansProjects//AMAAS-master//results//" << expName[expNumber] << "//a=" << currentAlpha << "b=" << currentGamma;
            ///runJava(ss.str());
        }
    }

    std::stringstream ss;
    ss << filename.c_str();
    // ss.str("");
    ss << "final";
    writeData(ss.str(), true, expName[expNumber]); //flush all stats
    std::stringstream ss1;
    ss1 << "C://Users//Adam//Documents//NetBeansProjects//aamasResults//" << expName[expNumber] << "//overall";
    runJava(ss1.str(), expName[expNumber]);
    return 0;
}
//1 is inital test 2 is retest 3 is plus main swap vector//55 19 53

void cleanUp()
{
    //cout << "a" << std::endl;

    delete car;
    //cout << "b" << std::endl;
    delete carAgent;
    //cout << "c" << std::endl;
    delete pole;
    //cout << "d" << std::endl;
    delete poleAgent;
    //cout << "e" << std::endl;
}

/*
initalise the stats array*/
void initData()
{
    for (int a = 0; a < Constants::NUMBER_OF_ALPHAS; a++)
    {//set alphas
        alphas[a] = .1 + a * .1;
    }
    for (int a = 0; a < Constants::NUMBER_OF_GAMMAS; a++)
    {//set gammas
        gammas[a] = .1 + a * .1;
    }
    for (int a = 0; a < Constants::NUMBER_OF_ALPHAS; a++)
    {
        for (int b = 0; b < Constants::NUMBER_OF_GAMMAS; b++)
        {//set results

            carResults[a][b] = 0;
            carFinishes[a][b] = 0;
            poleResults[a][b] = 0;
            poleFalls[a][b] = 0;
        }
    }
}

/*
average the stats*/
void finaliseData()
{
    for (int a = 0; a < Constants::NUMBER_OF_ALPHAS; a++)
    {
        for (int b = 0; b < Constants::NUMBER_OF_GAMMAS; b++)
        {//set results
            //cout << "for " << alphas[a] << "," << gammas[b] << " results were " << poleResults[a][b];

            carResults[a][b] = carResults[a][b] / Constants::NUMBER_OF_EXPS;
            carFinishes[a][b] = carFinishes[a][b] / Constants::NUMBER_OF_EXPS;
            poleResults[a][b] = poleResults[a][b] / Constants::NUMBER_OF_EXPS;
            poleFalls[a][b] = poleFalls[a][b] / Constants::NUMBER_OF_EXPS;
            //cout << " now " << poleResults[a][b] << "\n";
        }
    }
}

/*
reset the car stats*/
void setCarAgentForRun(double alpha, double gamma)
{

    car = new MtCar();
    carAgent = new MtCarAgent("MtCar");
    carAgent->changeAlphaGamma(alpha, gamma);
    carLoopCounter = 0;
    carFinishCount = 0;
    carThisRunTime = 0;
    carAverageTime = 0;
    carCurrentMin = INT_MAX;
}

/*
reset the car stats*/
void setPoleAgentForRun(double alpha, double gamma)
{

    pole = new CartPole();
    poleAgent = new CartPoleAgent("CartPole");
    poleAgent->changeAlphaGamma(alpha, gamma);
    //std::cout << "setting poleAgent to a= " << alpha << " and g= " << gamma << "\n";
    poleCurrentAction = 0;
    poleAveFallTime = 0;
    poleNumberOfFalls = 0;
    poleLoopCounter = 0;
    poleStepCount = 0;
    poleCurrentMax = 0;
}

/*
run the car until it has finished 200 times*/
int carRun(int number)
{
    while (carFinishCount < (number + 1) * Constants::CAR_TRAINING)
    {
        carAgent->manageLearning(true, true);
        carAgent->changeActionSelectionTemperature(400);
        carAgent->chooseActionSelectionMethod(false, false, !false);
        for (;; carLoopCounter++)
        {
            carThisRunTime++;
            int carAction = atoi(carAgent->nominate().c_str());
            bool carActionResult = car->executeAction(carAction);
            // cout << "action = " << carAction << " execute: " << carActionResult << "\n";
            //car->printState();
            carAgent->updateLocal(car->getState());
            carAgent->finishRun();
            if (carActionResult == true)
            {
                carAgent->updateLocal(car->getState());
                //cout << "Finished it took " << carThisRunTime << " timesteps\n";

                carAverageTime += carThisRunTime;
                carThisRunTime = 0;
                carFinishCount++;
                //exit(0);
                car->reset(false); //reset to not random pos
                break;

            }
        }
    }
    carAverageTime = carAverageTime / carFinishCount;
    std::cout << "MtCar Finished " << carFinishCount << " times averageing " << carAverageTime << " this was " << carLoopCounter << " steps total\n";
    std::stringstream ss;
    ss << "MtCar - End of explore + a= " << currentAlpha << " g= " << currentGamma << " run= " << currentRun;
    carAgent->writePolicies(ss.str(), expName[expNumber]);
    std::stringstream ss1;
    ss1 << " - a = " << currentAlpha << " g = " << currentGamma << " run = " << currentRun;
    carAgent->printReward(ss1.str(), expName[expNumber]);

    //carAgent->writePolicies("End");

    return exploiteMtCar();
}

/*
run the car and pole until it has finished 200 times*/
void duelRun(int number)
{
    int breakRun;
    if (number < Constants::CAR_MAX)
    {

        breakRun = Constants::CAR_MAX;
    }
    else
    {
        breakRun = number;
    }
    bool useTL = !false;
    bool poleLearn = !false;
    bool interTest = !true;
    bool limitTraining = true;
    poleAgent->setUsingTransferLearning(true);
    carAgent->setUsingTransferLearning(true);
    carAgent->manageLearning(true, true);
    carAgent->changeActionSelectionTemperature(100);
    carAgent->chooseActionSelectionMethod(false, false, !false);
    poleAgent->changeActionSelectionTemperature(100);
    poleAgent->manageLearning(poleLearn, true);
    poleAgent->chooseActionSelectionMethod(false, false, !false);
    std::cout << "training until car finishes " << number << " steps\n";
    while (carLoopCounter < number)//Constants::CAR_TRAINING_STEPS)// * number + 10000) //while (carFinishCount < Constants::CAR_TRAINING)//Constants::CAR_TRAINING * (currentRun + 1)*10)//
    { //init agents
        for (;; carLoopCounter++)
        {
            if (limitTraining && carLoopCounter > number)
            {
                break;
            }
            //for intermediate testing
            if (interTest)
            {
                if (carLoopCounter < Constants::TEST_FREQ && carLoopCounter % (Constants::TEST_FREQ / 10) == 0)
                {//more frequent init testing
                    int loopBK = carLoopCounter;
                    std::cout << "Count " << carLoopCounter << "\n";
                    int exploitePole = exploiteCartPole();
                    int exploiteCar = exploiteMtCar();
                    poleSteps.push_back(exploitePole);
                    carSteps.push_back(exploiteCar);
                    poleAgent->setUsingTransferLearning(true);
                    carAgent->setUsingTransferLearning(true);
                    carAgent->manageLearning(true, true);
                    carAgent->changeActionSelectionTemperature(100);
                    carAgent->chooseActionSelectionMethod(false, false, !false);
                    poleAgent->changeActionSelectionTemperature(100);
                    poleAgent->manageLearning(poleLearn, true);
                    poleAgent->chooseActionSelectionMethod(false, false, !false);
                    carLoopCounter = loopBK + 1;
                    stringstream ss;
                    ss << "CartPole - End of explore + a= " << currentAlpha << " g= " << currentGamma << " loop= " << carLoopCounter;
                    poleAgent->writePolicies(ss.str(), expName[expNumber]);
                }
                else if (carLoopCounter > 0 && carLoopCounter % Constants::TEST_FREQ == 0)
                {
                    int loopBK = carLoopCounter;
                    std::cout << "2 Count " << carLoopCounter << "\n";
                    int exploitePole = exploiteCartPole();
                    int exploiteCar = exploiteMtCar();
                    poleSteps.push_back(exploitePole);
                    carSteps.push_back(exploiteCar);
                    poleAgent->setUsingTransferLearning(true);
                    carAgent->setUsingTransferLearning(true);
                    carAgent->manageLearning(true, true);
                    carAgent->changeActionSelectionTemperature(100);
                    carAgent->chooseActionSelectionMethod(false, false, !false);
                    poleAgent->changeActionSelectionTemperature(100);
                    poleAgent->manageLearning(poleLearn, true);
                    poleAgent->chooseActionSelectionMethod(false, false, !false);
                    carLoopCounter = loopBK + 1;
                    stringstream ss;
                    ss << "CartPole - End of explore + a= " << currentAlpha << " g= " << currentGamma << " loop= " << carLoopCounter;
                    poleAgent->writePolicies(ss.str(), expName[expNumber]);

                }
            }


            carThisRunTime++;
            poleStepCount++;
            //run a step for the car
            int carAction = atoi(carAgent->nominate().c_str());
            bool carActionResult = car->executeAction(carAction);
            //cout << "action = " << carAction << " execute: " << carActionResult << "\n";
            carAgent->updateLocal(car->getState());
            carAgent->finishRun();
            //run a step for thee pole
            poleCurrentAction = atoi(poleAgent->nominate().c_str());
            //std::cout << "pre execute\n";
            //pole->printState();
            //std::cout << "action " << poleCurrentAction << "\n";
            //std::cout << poleCurrentAction;
            //see what happened with that action
            pole->executeAction(poleCurrentAction);
            // std::cout << "post execute\n";pole->printState();
            //learn from that
            std::string poleState = pole->getCurrentState();
            poleAgent->updateLocal(poleState);
            poleAgent->finishRun();

            if (useTL)
            {
                //finished runs now transfer
                std::string transfer = carAgent->transferToAllFromAll();
                //std::cout << transfer << " =transfer\n";
                poleAgent->readTransferedInfoIn(transfer);
                //now feedback
                //std::cout << " about to feedback from pole\n";
                std::string feedback = poleAgent->sendFeedback();
                carAgent->sendFeedback(); //just to clear vector
                //std::cout << "|" << feedback << " =feedback\n";
                carAgent->recieveFeedback(feedback);
                //lern from feedback
                carAgent->updateLearnedMappingFromTarget();
            }
            if (pole->getCurrentState() == "-1")
            {//see if fell over
                //cout << "Trial " << numberOfFalls << " lasted " << stepCount << " steps\n";
                //cart->printState();
                poleAveFallTime += poleStepCount;
                poleStepCount = 0;
                poleNumberOfFalls++;
                pole->randReset();
                //std::cout << "poll finished\n";

            }
            if (carActionResult == true || carLoopCounter > breakRun)
            {
                carAgent->updateLocal(car->getState());
                //cout << "Finished it took " << carThisRunTime << " timesteps\n";

                carAverageTime += carThisRunTime;
                carThisRunTime = 0;
                carFinishCount++;
                //exit(0);
                car->reset(!false); //reset to not random pos

                break;

            }
        }
    }
    cout << "Finished here " << carFinishCount << "\n";
    if (carFinishCount == 0)
    {
        std::cout << "C'est possible l'error ici, voitureFinniNumbre est zero\n";
        carFinishCount++;
    }
    carAverageTime = carAverageTime / carFinishCount;
    std::cout << "MtCar Finished " << carFinishCount << " times averageing " << carAverageTime << " this was " << carLoopCounter << " steps total\n";
    std::stringstream ss;
    ss << "MtCar - End of explore + a= " << currentAlpha << " g= " << currentGamma << " run= " << currentRun;
    carAgent->writePolicies(ss.str(), expName[expNumber]);
    std::stringstream ss1;
    ss1 << " - a = " << currentAlpha << " g = " << currentGamma << " run = " << currentRun;
    carAgent->printReward(ss1.str(), expName[expNumber]);
    //carAgent->writePolicies("End");
    cout << "2Finished\n";

    poleAveFallTime = (poleAveFallTime / poleNumberOfFalls);
    cout << "Fell " << poleNumberOfFalls << " times averaging " << poleAveFallTime << " steps per fall\n";
    std::stringstream ss2;
    ss2 << "CartPole - End of explore + a= " << currentAlpha << " g= " << currentGamma << " run= " << currentRun;
    poleAgent->writePolicies(ss2.str(), expName[expNumber]);
    //std::cout << "returned" << std::endl;
    std::stringstream ss3;
    ss3 << " - a = " << currentAlpha << " g = " << currentGamma << " run = " << currentRun;
    poleAgent->printReward(ss3.str(), expName[expNumber]);
    //std::cout << "about to exploit" << std::endl;
    if (false == interTest)
    {
        int exploitePole = exploiteCartPole();
        std::cout << "end cartpole explot\n";
        int exploiteCar = exploiteMtCar();
        std::cout << "end mt car explot\n";
        poleResults[currentAlpha][currentGamma] += exploitePole;
        poleSteps.push_back(exploitePole); //save the record
        carSteps.push_back(exploiteCar);
        carResults[currentAlpha][currentGamma] += exploiteCar;
        poleFalls[currentAlpha][currentGamma] = poleNumberOfFalls;
    }


    //return exploiteMtCar();
    // std::cout << "end duel run\n";
}

/*
use what has been learned on the car
returns what is the min seen if multiple calls*/
double exploiteMtCar()
{
    double totalExpolit = 0;
    for (int a = 0; a < Constants::NUMBER_OF_MT_CAR_EXPOLITE_RUNS; a++)
    {
        //set to exploit and reset the stats 
        carAgent->changeActionSelectionTemperature(0);
        carAgent->manageLearning(!false, !false);
        //carAgent->chooseActionSelectionMethod(false, true);
        carAgent->changeEGreadyE(0.0);
        carLoopCounter = 0;
        int stepCount = 0;

        {//if not finished running
            car->reset(false); //reset the simulator to normal
            //carAgent->writePolicies("MtCar - start exploit");
            for (; carLoopCounter < Constants::CAR_MAX; carLoopCounter++)
            {//run for a time

                //choose action
                int carAction = atoi(carAgent->nominate().c_str());
                //cout << "Agent nominated " << carAction << "\n";
                //car->printState();
                //see what happened with that action
                car->executeAction(carAction);
                //learn from that
                carAgent->updateLocal(car->getState());
                carAgent->finishRun();
                if (car->getState() == "0")
                {//see if done
                    //cout << "It took " << stepCount << " steps\n";
                    //car->printState();
                    carLoopCounter = Constants::CAR_EXPLOITATION_TIME;
                    stepCount++;

                }
                else
                {
                    // car->printState();
                    stepCount++;
                }

            }
            //learn once more incase it finished
            carAgent->updateLocal(car->getState());
        }
        //if (stepCount < carCurrentMin)//only use if car about max
        {
            //carAgent->publishLocalPolicies("output");
            carCurrentMin = stepCount;
            cout << "car Test lasted " << stepCount << " steps\n";
        }
        if (stepCount < Constants::CAR_EXPLOITATION_TIME)
        {//if finished in this run
            carFinishes[currentAlpha][currentGamma]++;
        }

        totalExpolit += carCurrentMin;
    }
    std::stringstream ss;
    ss << "MtCar - End of exploit + a= " << currentAlpha << " g= " << currentGamma << " run= " << currentRun;
    //carAgent->writePolicies(ss.str(), expName[expNumber]);
    totalExpolit = totalExpolit / Constants::NUMBER_OF_MT_CAR_EXPOLITE_RUNS;

    return totalExpolit;
}

/*
run the cart poll until it has fallen 2000 times*/
int poleRun(int number)
{
    std::cout << ((number + 1) * Constants::POLL_TRAINING) << " =the number of training steps\n";
    poleAgent->changeActionSelectionTemperature(499);
    poleAgent->manageLearning(true, true);
    poleAgent->setUsingTransferLearning(false);
    poleAgent->chooseActionSelectionMethod(false, false, !false);
    //while (poleLoopCounter < number)//Constants::CAR_TRAINING_STEPS)
    while (poleNumberOfFalls < ((number + 1) * Constants::POLL_TRAINING))
    {//if not finished running
        //std::cout << "ping\n";
        pole->reset();
        poleStepCount = 0;
        for (;; poleLoopCounter++)
        {//run for a time
            //choose action
            poleCurrentAction = atoi(poleAgent->nominate().c_str());
            std::string state = pole->getCurrentState();
            //see what happened with that action
            pole->executeAction(poleCurrentAction);
            std::cout << poleCurrentAction;
            //learn from that
            std::stringstream ss;
            ss << pole->getBox();
            poleAgent->updateLocal(pole->getCurrentState());
            //std::cout << "Nominated " << poleCurrentAction << " in " << state << " which lead to " << pole->getCurrentState() << "\n";
            poleAgent->finishRun();
            if (pole->getCurrentState() == "-1")
            {//see if fell over
                // cout << "Trial " << numberOfFalls << " lasted " << poleStepCount << " steps\n";
                //std::cout << "ping\n";
                //cart->printState();
                poleAveFallTime += poleStepCount;
                poleNumberOfFalls++;
                break;

            }
            else
            {
                //cart->printState();
            }

            poleStepCount++;
        }
        //learn once morein case it fell
        std::stringstream ss;
        ss << pole->getBox();
        poleAgent->updateLocal(pole->getCurrentState());
    }
    poleAveFallTime = (poleAveFallTime / poleNumberOfFalls);
    cout << "Fell " << poleNumberOfFalls << " times averaging " << poleAveFallTime << " steps per fall. Took " << poleLoopCounter << " steps.\n";
    std::stringstream ss;
    ss << "CartPole - End of explore + a= " << currentAlpha << " g= " << currentGamma << " run= " << currentRun;
    poleAgent->writePolicies(ss.str(), expName[expNumber]);
    std::stringstream ss1;
    ss1 << " - a = " << currentAlpha << " g = " << currentGamma << " run = " << currentRun;
    poleAgent->printReward(ss1.str(), expName[expNumber]);

    return exploiteCartPole();

}

/*use what has been learned on the cart poll*/
double exploiteCartPole()
{
    double totalStepCount = 0;
    try
    {

        for (int a = 0; a < Constants::NUMBER_OF_CART_POLE_EXPLOITE_RUNS; a++)
        {
            std::cout << "new exploit run" << std::endl;
            poleAgent->changeActionSelectionTemperature(1);
            poleAgent->manageLearning(!true, !true);
            poleAgent->chooseActionSelectionMethod(false, false, !false);
            //exploit
            //  std::cout << "after set" << std::endl;
            poleLoopCounter = 0;
            int stepCount = 0;
            {//if not finished running
                pole->reset();
                //   std::cout << "1" << std::endl;
                for (;; poleLoopCounter++)
                {//run for a time

                    if (poleLoopCounter % Constants::CAR_MAX == 0 && poleLoopCounter != 0)
                    {//if still ballencing heratbeat
                        cout << "It took " << stepCount << " steps and didnt fall\n";
                        // pole->printState();
                        break;
                    }
                    //choose action
                    poleCurrentAction = atoi(poleAgent->nominate().c_str());
                    //std::cout << poleCurrentAction << ",";
                    //pole->printState();
                    // std::cout << "2" << std::endl;
                    //see what happened with that action
                    pole->executeAction(poleCurrentAction);
                    //std::cout << poleCurrentAction;
                    //std::cout << "3" << std::endl;
                    //learn from that
                    poleAgent->updateLocal(pole->getCurrentState());
                    poleAgent->finishRun();
                    //cout << "Agent nominated " << poleCurrentAction << " in " << pole->getCurrentState() << "\n";
                    //std::cout << "4" << std::endl;
                    if (pole->getCurrentState() == "-1")
                    {//see if fell over
                        cout << "It took " << stepCount << " steps\n";
                        // pole->printState();
                        break;

                    }
                    else
                    {
                        //cart->printState();
                    }
                    stepCount++;
                }
                //learn once more incase it fell
                std::stringstream ss;
                //std::cout << "5" << std::endl;
                ss << pole->getBox();
                // std::cout << "6" << std::endl;
                std::string poleState = pole->getCurrentState();
                //std::cout << "7" << std::endl;
                if (poleState.size() <= 0)
                {
                    std::cout << "pole error 2 " << carLoopCounter << " actioon= " << poleCurrentAction << "\n";
                    pole->printState();
                }
                poleAgent->updateLocal(poleState);
                //std::cout << "8" << std::endl;
            }
            //if (stepCount > poleCurrentMax)//only use if we car baout max
            {


                poleCurrentMax = stepCount;
                //cout << "Pole test lasted: " << stepCount << " steps\n";
            }
            totalStepCount += stepCount;
        }
        std::cout << "done loop" << std::endl;
        std::stringstream ss;
        //std::cout << "10" << std::endl;
        ss << "CartPole - End of exploit + a= " << currentAlpha << " g= " << currentGamma << " run= " << currentRun;
        //std::cout << "11" << std::endl;
        totalStepCount = totalStepCount / Constants::NUMBER_OF_CART_POLE_EXPLOITE_RUNS;
        pole->reset();
        //poleSteps.push_back(totalStepCount); //save the record
        //std::cout << "12" << std::endl;
    }
    catch (std::bad_alloc& ba)
    {
        std::cout << "bad_alloc caught: " << ba.what() << " trying again\n";
        exit(54234563);
        /// totalStepCount = exploiteCartPole();
    }
    return totalStepCount;
}

/*
 * parseValues is to write data at the beginning of file
 */
void writeData(std::string fileName, bool end, std::string tag)
{

    if (end)
    {
        finaliseData();
        std::stringstream ss;
        ss << fileName << "+carResults.csv." << tag << ".stats";
        ofstream fout(ss.str().c_str());
        fout << "\t,";
        for (int a = 0; a < Constants::NUMBER_OF_GAMMAS; a++)
        {//top row
            fout << gammas[a] << "\t";
            if (a != (Constants::NUMBER_OF_GAMMAS - 1))
            {
                fout << ",";
            }
            else
            {
                fout << "\r\n";
            }
        }
        for (int a = 0; a < Constants::NUMBER_OF_ALPHAS; a++)
        {
            fout << alphas[a] << "\t,";
            for (int b = 0; b < Constants::NUMBER_OF_GAMMAS; b++)
            {//set results
                fout << carResults[a][b] << "\t";
                if (b != (Constants::NUMBER_OF_GAMMAS - 1))
                {
                    fout << ",";
                }
                else
                {
                    fout << "\r\n";
                }
            }
        }
        fout.close();
        fout.clear();

        //print finshes
        std::stringstream ss1;
        ss1 << fileName << "+carFinishes.csv." << tag << ".stats";
        ofstream fout0(ss1.str().c_str());
        fout0 << "\t,";
        for (int a = 0; a < Constants::NUMBER_OF_GAMMAS; a++)
        {//top row
            fout0 << gammas[a] << "\t";
            if (a != (Constants::NUMBER_OF_GAMMAS - 1))
            {
                fout0 << ",";
            }
            else
            {
                fout0 << "\r\n";
            }
        }
        for (int a = 0; a < Constants::NUMBER_OF_ALPHAS; a++)
        {
            fout0 << alphas[a] << "\t,";
            for (int b = 0; b < Constants::NUMBER_OF_GAMMAS; b++)
            {//set results
                fout0 << carFinishes[a][b] << "\t";
                if (b != (Constants::NUMBER_OF_GAMMAS - 1))
                {
                    fout0 << ",";
                }
                else
                {

                    fout0 << "\r\n";
                }
            }
        }
        fout0.close();

        std::stringstream ss2;
        ss2 << fileName << "+poleResults.csv." << tag << ".stats";
        ofstream fout1(ss2.str().c_str());
        fout1 << "\t,";
        for (int a = 0; a < Constants::NUMBER_OF_GAMMAS; a++)
        {//top row
            fout1 << gammas[a] << "\t";
            if (a != (Constants::NUMBER_OF_GAMMAS - 1))
            {
                fout1 << ",";
            }
            else
            {
                fout1 << "\r\n";
            }
        }
        for (int a = 0; a < Constants::NUMBER_OF_ALPHAS; a++)
        {
            fout1 << alphas[a] << "\t,";
            for (int b = 0; b < Constants::NUMBER_OF_GAMMAS; b++)
            {//set results
                fout1 << poleResults[a][b] << "\t";
                if (b != (Constants::NUMBER_OF_GAMMAS - 1))
                {
                    fout1 << ",";
                }
                else
                {
                    fout1 << "\r\n";
                }
            }
        }
        fout1.close();
        fout1.clear();

        //print finshes
        std::stringstream ss3;
        ss3 << fileName << "+poleFalls.csv." << tag << ".stats";
        ofstream fout2(ss3.str().c_str());
        fout2 << "\t,";
        for (int a = 0; a < Constants::NUMBER_OF_GAMMAS; a++)
        {//top row
            fout2 << gammas[a] << "\t";
            if (a != (Constants::NUMBER_OF_GAMMAS - 1))
            {
                fout2 << ",";
            }
            else
            {
                fout2 << "\r\n";
            }
        }
        for (int a = 0; a < Constants::NUMBER_OF_ALPHAS; a++)
        {
            fout2 << alphas[a] << "\t,";
            for (int b = 0; b < Constants::NUMBER_OF_GAMMAS; b++)
            {//set results
                fout2 << poleFalls[a][b] << "\t";
                if (b != (Constants::NUMBER_OF_GAMMAS - 1))
                {
                    fout2 << ",";
                }
                else
                {

                    fout2 << "\r\n";
                }
            }
        }
        fout2.close();
    }
    //print finshes
    std::stringstream ss;
    ss.str("");
    ss << fileName << "+poleSteps.csv." << tag << ".stats";
    ofstream fout3(ss.str().c_str());

    std::vector<int>::iterator stepsIterator = poleSteps.begin();
    while (stepsIterator != poleSteps.end())
    {//for all steps

        fout3 << (*stepsIterator++) << ",";
    }
    fout3 << "\r\n";
    fout3.close();

    //print finshes

    std::stringstream ss1;
    ss1 << fileName << "+carSteps.csv." << tag << ".stats";
    ofstream fout4(ss1.str().c_str());

    stepsIterator = carSteps.begin();
    while (stepsIterator != carSteps.end())
    {//for all steps

        fout4 << (*stepsIterator++) << ",";
    }
    fout4 << "\r\n";
    fout4.close();

    /*poleSteps.clear();
    carSteps.clear();
    vector<int> emptyVectorI;
    poleSteps.swap(emptyVectorI);
    carSteps.swap(emptyVectorI);*/
}

/**
 * run a java prog to clean up
 */
void runJava(std::string location, std::string tag)
{
    cout << "~~before java" << endl;
    std::stringstream ss;
    ss << "java -jar C://Users//Adam//Documents//NetBeansProjects//AMAAS-master//AAMASClean.jar ";
    ss << location;
    ss << " ";
    ss << tag;
    system(ss.str().c_str());

    cout << "~~done java" << endl;




}