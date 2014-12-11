/* 
 * File:   MtCar.cpp
 * Author: Adam
 * 
 * Created on 24 June 2014, 08:55
 */

#include "MtCar.h"
#include "Constants.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

MtCar::MtCar()
{
    x = -0.5;
    xDot = 0;
}

MtCar::MtCar(const MtCar& orig)
{
}

MtCar::~MtCar()
{
}

void MtCar::printState()
{
    std::cout << "---------------\nState:\n";
    std::cout << "\tx= " << x << "\n";
    std::cout << "\txDot= " << xDot << "\n";
    std::cout << "\tstate= " << getState() << "\n";
}

/**
 * move on one timestep changing x and xDot appropriatly
 * @param action -1 for left 0 for nothing 1 rot right
 * @return true if finished at goal else false
 */
bool MtCar::executeAction(int action)
{
    //check actions
    if (action != 0 && action != 1 && action != -1)
    {
        std::cerr << "action outside space\n";
        exit(19);
    }

    //update speed
    xDot += action * .5 * cos(3 * x)*-.0025;


    if (xDot < Constants::MAX_LEFT_X_DOT())
    {//too fast left
        xDot = Constants::MAX_LEFT_X_DOT();
    }
    else if (xDot > Constants::MAX_RIGHT_X_DOT())
    {//too fast RIGHT
        xDot = Constants::MAX_RIGHT_X_DOT();
    }

    //update position
    x += xDot;
    if (x < Constants::LEFT_EDGE())
    {//if too far left
        x = Constants::LEFT_EDGE();
    }
    if (x >= Constants::RIGHT_EDGE())
    {//if too far right its done don't reset yet
        return true;
    }
    else
    {
        return false;
    }
}

int MtCar::getSign(double x)
{
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

/**
 * put things back to the start
 */
void MtCar::reset(bool random)
{

    if (random == false)
    {
        x = -0.5;
        xDot = 0;
    }
    else
    {
        double f = (double) rand() / RAND_MAX;
        x = f * 1.8 - 1.2;
        xDot = f * .14 - 0.07;
    }
}

std::string MtCar::getState()
{
    std::string output = "";
    if (x > Constants::RIGHT_EDGE())
    {
        output += "0"; //goal
        return output;
    }
    else if (x > Constants::RIGHT_EDGE() / 2)
    {
        output += "4"; //fast right
    }
    else if (x > 0)
    {
        output += "3"; //slow right
    }

    else if (x < Constants::LEFT_EDGE() / 2)
    {
        output += "1"; //fast left
    }
    else
    {
        output += "2"; //slow left
    }
    output += "&"; //makes it more human readable
    if (xDot > Constants::MAX_RIGHT_X_DOT() / 3)
    {
        output += "FR"; //fast right
    }
    else if (xDot > 0)
    {
        output += "SR"; //slow right
    }
    else if (xDot < Constants::MAX_LEFT_X_DOT() / 3)
    {
        output += "FL"; //fast left
    }
    else
    {
        output += "SL"; //slow left
    }


    return output;
}