/* 
 * File:   TaylorSeriesSelection.h
 * Author: Adam
 *
 * Created on 21 October 2014, 11:32
 */
#include "ActionSelection.h"

#include <math.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iostream>
#ifndef TAYLORSERIESSELECTION_H
#define	TAYLORSERIESSELECTION_H

class TaylorSeriesSelection : public ActionSelection {
public:

    TaylorSeriesSelection();

    virtual ~TaylorSeriesSelection();

    std::string calculateNextAction(std::vector<std::pair<std::string, double> >);

    void setTemperature(double tmp);
    double getTemperature();


private:



    double temperature;

};

#endif	/* TAYLORSERIESSELECTION_H */

