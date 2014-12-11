/* 
 * File:   Policy.h
 * Author: Adam
 *
 * Created on July 11, 2012, 9:24 AM
 */
#ifndef POLICY_H
#define	POLICY_H

#include <string>

/**
 This encapsulates a way of selecting actions.  Not necessarily containing a DWL process.  This should allow for agent type heterogeneity.

  charris - I think it should just be a generic container and we downcast to do specific stuff to an implementation?
  */

class Policy  {
public:
    Policy(std::string name);
    ~Policy();

    void setPolicyName(std::string policyName);
    std::string getPolicyName() const;

private:
    std::string policyName;

};

#endif	/* POLICY_H */

