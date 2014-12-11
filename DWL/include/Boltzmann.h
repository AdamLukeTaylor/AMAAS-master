#ifndef _Boltzmann_H_
#define _Boltzmann_H_

#include "ActionSelection.h"

#include <math.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iostream>

class Boltzmann : public ActionSelection {
public:

    Boltzmann();

    virtual ~Boltzmann();

    std::string calculateNextAction(std::vector<std::pair<std::string, double> >);

    void setTemperature(double tmp);
    double getTemperature();


private:



    double temperature;

};

#endif
