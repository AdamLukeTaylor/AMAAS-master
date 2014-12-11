/* 
 * File:   main.cpp
 * Author: Adam
 *
 * Created on July 11, 2012, 9:03 AM
 */
#include "vld.h"
#include <cstdlib>
#include "TestReward.h"
#include "TestAgent.h"
#include "QTable.h"
#include "WTable.h"
#include "TransferMapping.h"
#include <iostream>
#include <sstream>

using namespace std;

/*this is a test main showing how a system should* look
 * *I take no responability if this is, in fact, not how a system should look
 */
int main(int argc, char** argv)
{

    //***************************************simple system sample     

    TestAgent* d1 = new TestAgent("dwl1"); //create some agents with states
    TestAgent* d2 = new TestAgent("dwl2"); //these also create local policies
    cout << "created agents\n";
    d1->addNeighbours(d2->getName()); //add as neighbours so they communicate
    d2->addNeighbours(d1->getName());
    cout << "added neighbours\n";

    d1->addRemotePolicy(d2->publishLocalPolicies("d2Policies")); //this is emulated communicatuions
    d2->addRemotePolicy(d1->publishLocalPolicies("d1Policies")); //each agent 'broadcasts' its statespace and the other agent maps it to its actions
    cout << "added remote policies\n";

    d1->createColaberationPolicy("colab1", 4, 1); //these allow for the learning of the colab coefficent
    d2->createColaberationPolicy("colab 2", 5, 1);


    for (int a = 0; a < 10; a++)
    {
        cout << "----------start--------------\n" << endl;
        cout << "about to nominate\n" << endl;
        cout << d1->getName() << " nominated " << d1->nominate() << endl; //nominate an action from all policies
        cout << d2->getName() << " nominated " << d2->nominate() << endl;
        //instead of printing above they would go into gridlab-d and then what it returns would be passed into updateLocal
        cout << "about to update locals\n" << endl;
        d1->updateLocal("b", 2); //pass back what state the environment is in optionally pass back reward if calculated externally
        d2->updateLocal("b", 5); //for this test reward is just a dummy so need to a pass a reward for stuff to happen
        cout << "about to pass updates\n" << endl;
        d1->addMessage(d2->writeMessage()); //have the policies say how good neighbours actions were
        d2->addMessage(d1->writeMessage()); //and read what was said         
        cout << "about to update remote\n" << endl;
        d1->updateRemote(); //learn based on the messages we just got
        d2->updateRemote();
        cout << "about to clean up\n" << endl;
        d1->finishRun(); //adgust c value and clean up some stuff
        d2->finishRun();
        cout << "------------end--------------\n" << endl;
        std::stringstream ss;
        ss << "d1 test " << a;
        d1->writePolicies(ss.str()); //write the state space to look at and see how fantastic the code is now
        std::stringstream ss1;
        ss1 << "d2 test " << a;
        d2->writePolicies(ss1.str());
    }

	    //***************************************Transfer learning sample
    
            //set up transfer
            TransferMapping* t=new TransferMapping();
    QTable* qs=new QTable();
    QTable* qt=new QTable();
    qs->addStateAction("a","+",10);
    qs->addStateAction("a","-",9);
    qs->addStateAction("b","+",8);
    qs->addStateAction("b","-",7);
    qs->addStateAction("c","+",6);
    qs->addStateAction("c","-",5);
    qs->addStateAction("d","+",4);
    qs->addStateAction("d","-",3);
        
    qt->addStateAction("z","+",0);
    qt->addStateAction("z","-",0);
    qt->addStateAction("x","+",0);
    qt->addStateAction("x","-",0);
    qt->addStateAction("y","+",0);
    qt->addStateAction("y","-",0);
    qt->addStateAction("w","+",0);
    qt->addStateAction("w","-",0);
        
    t->setSource(qs);
    t->setTarget(qt);
    t->populateMappingRandomly();
    std::cerr<<"populated\n";
    qs->writeStateActionToFile("source");
    qt->writeStateActionToFile("target");
     qt->setQValue(t->mapFromStateToTarget(TransferMapping::makePairForMapper(qs->getQTableEntry("a","+"))));
        
            //make a transfer
    qs->writeStateActionToFile("source1");
    qt->writeStateActionToFile("target1");std::cerr<<"mapped 1\n";
    qt->setQValue(t->mapFromStateToTarget(TransferMapping::makePairForMapper(qs->getQTableEntry("b","+"))));
    std::cerr<<"mapped 2\n";
    qs->writeStateActionToFile("source2");
    qt->writeStateActionToFile("target2");
    std::cerr<<"end\n";
		
            //in actuall code the set up can be abstracted to the following
           d1->addMapping(d1->getName(),d2->getName(),*d1->getLocalQTables().begin(),*d2->getLocalQTables().begin());//from 1 to 2
    d2->addMapping(d2->getName(),d1->getName(),*d2->getLocalQTables().begin(),*d1->getLocalQTables().begin());//from 2 to 1
       
       //the transfer can be absfracted as the following
            d2->addMessage(d1->transferToAllFromAll());//give agent 2 all the messages form 1
            d1->addMessage(d2->transferToAllFromAll());//give agent 1 all the messages from 2//for multiple this will need better management like  amessage server ofr simmilar
	   
     

    return 0; //return 0 you know, just because....
}

