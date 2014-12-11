/* 
 * File:   Policy.cpp
 * Author: Adam
 * 
 * Created on July 11, 2012, 9:24 AM
 */

#include "Policy.h"

Policy::Policy(std::string name)
{
        this->policyName=name;
}

void Policy::setPolicyName(std::string policyName)
{
        this->policyName = policyName;
}

std::string Policy::getPolicyName() const
{
        return policyName;
}



Policy::~Policy()
{

}



