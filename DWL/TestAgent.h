/*
 * WaterHeaterAgent.h
 *
 *  Created on: 16 Jul 2012
 *      Author: charris
 */

#ifndef TESTAGENT_H_
#define TESTAGENT_H_

#include "DWLAgent.h"
#include <string>



class TestAgent: public DWLAgent {
public:
	TestAgent(std::string ID);
	virtual ~TestAgent();
};


#endif /* WATERHEATERAGENT_H_ */
