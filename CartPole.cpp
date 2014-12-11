
/* ----------------------------------------------------------------------
    This file contains a simulation of the cart and pole dynamic system and 
 a procedure for learning to balance the pole.  Both are described in 
 Barto, Sutton, and Anderson, "Neuronlike Adaptive Elements That Can Solve
 Difficult Learning Control Problems," IEEE Trans. Syst., Man, Cybern.,
 Vol. SMC-13, pp. 834--846, Sept.--Oct. 1983, and in Sutton, "Temporal
 Aspects of Credit Assignment in Reinforcement Learning", PhD
 Dissertation, Department of Computer and Information Science, University
 of Massachusetts, Amherst, 1984.  The following routines are included:

       main:              controls simulation interations and implements 
                          the learning system.

       cart_and_pole:     the cart and pole dynamics; given action and
                          current state, estimates next state

       get_box:           The cart-pole's state space is divided into 162
                          boxes.  get_box returns the index of the box into
                          which the current state appears.

 These routines were written by Rich Sutton and Chuck Anderson.  Claude Sammut 
 translated parts from Fortran to C.  Please address correspondence to
 Rich at

                GTE Laboratories Incorporated
                40 Sylvan Road
                Waltham, MA  02254

 or send email to     sutton@gte.com   or   anderson@cs.colostate.edu
---------------------------------------
Changes:
  1/93: A bug was found and fixed in the state -> box mapping which resulted 
        in array addressing outside the range of the array.  It's amazing this
        program worked at all before this bug was fixed.  -RSS

  9/97  Move defines to a header file, to make it easier to modify
        simulation parameters.  - Lynn Ryan

---------------------------------------------------------------------- */
#include <iostream>
#include <math.h>
#include "CartPole.h"
#include "Constants.h"
#include <sstream>
#include <stdlib.h>
using namespace std;

CartPole::CartPole()
{
    x = x_dot = theta = theta_dot = 0.0; //start at 0
    currentState = getBox();
}

void CartPole::randReset()
{
    x = ((double) rand() / RAND_MAX)*4.8 - 2.4;
    x_dot = ((double) rand() / RAND_MAX)*1.12 - .56;
    theta = ((double) rand() / RAND_MAX)*2 * Constants::six_degrees() - Constants::six_degrees();
    theta_dot = ((double) rand() / RAND_MAX)*2 * Constants::fifty_degrees() - Constants::fifty_degrees();
    currentState = getBox();
}

void CartPole::reset()
{
    x = x_dot = theta = theta_dot = 0.0; //start at 0
    currentState = getBox();
}

void CartPole::setCurrentState(std::string currentState)
{
    this->currentState = currentState;
}

std::string CartPole::getCurrentState()
{
    //box based
    std::stringstream ss;
    //ss << this->getBox();
    //currentState = ss.str();
    //state based
    currentState = this->getState();
    return currentState;
}

CartPole::~CartPole()
{
}

//typedef float vector[N_BOXES];

/** 
 * ----------------------------------------------------------------------
 *   cart_pole:  Takes an action (0 or 1) and the current values of the
 * four state variables and updates their values by estimating the state
 * Constants::TAU() seconds later.
 * ----------------------------------------------------------------------
 **/
void CartPole::executeAction(int action)
{
    //std::cout << "executing " << action << "  \n";
    //this->printState();
    if (action != 0 && action != 1)
    {
        std::cerr << "action outside space\n";
        exit(19);
    }
    float xacc, thetaacc, force, costheta, sintheta, temp;

    force = (action > 0) ? Constants::FORCE_MAG() : -Constants::FORCE_MAG();
    costheta = cos(theta);
    sintheta = sin(theta);

    temp = (force + Constants::POLEMASS_LENGTH() * theta_dot * theta_dot * sintheta) / Constants::TOTAL_MASS();

    thetaacc = (Constants::GRAVITY() * sintheta - costheta * temp)
            / (Constants::LENGTH() * (Constants::FOURTHIRDS() - Constants::MASSPOLE() * costheta * costheta / Constants::TOTAL_MASS()));

    xacc = temp - Constants::POLEMASS_LENGTH() * thetaacc * costheta / Constants::TOTAL_MASS();

    /*** Update the four state variables, using Euler's method. ***/

    x += Constants::TAU() * x_dot;
    x_dot += Constants::TAU() * xacc;
    theta += Constants::TAU() * theta_dot;
    theta_dot += Constants::TAU() * thetaacc;

    //this->printState();
    //this->getBox();
}

/**
 * ----------------------------------------------------------------------
 *  get_box:  Given the current state, returns a number from 1 to 162
 * designating the region of the state space encompassing the current state.
 * Returns a value of -1 if a failure state is encountered.
 * ----------------------------------------------------------------------
 **/
int CartPole::getBox()
{
    int box = -2;

    if (x < -2.4 ||
            x > 2.4 ||
            theta < -Constants::twelve_degrees() ||
            theta > Constants::twelve_degrees())
    {
        // cout << "+++++++++++++++++\nThe pole fell over\n+++++++++++++++++\n";
        box = -1;
        return (-1); /* to signal failure */
    }

    if (x < -0.8) box = 0;
    else if (x < 0.8) box = 1;
    else box = 2;

    if (x_dot < -0.5);
    else if (x_dot < 0.5) box += 3;
    else box += 6;

    if (theta < -Constants::six_degrees());
    else if (theta < -Constants::one_degree()) box += 9;
    else if (theta < 0) box += 18;
    else if (theta < Constants::one_degree()) box += 27;
    else if (theta < Constants::six_degrees()) box += 36;
    else box += 45;

    if (theta_dot < -Constants::fifty_degrees());
    else if (theta_dot < Constants::fifty_degrees()) box += 54;
    else box += 108;
    if (box == -2)
    {
        cout << "failed to find state\n";
        this->printState();
    }

    return (box);
}

/**
 * ----------------------------------------------------------------------
 *  get my states
 * ----------------------------------------------------------------------
 **/
std::string CartPole::getState()
{
    std::string output = "";
    std::string cartPos = "";
    std::string cartSpeed = "";
    std::string polePos = "";
    std::string poleSpeed = "";

    if (x < -2.4 ||
            x > 2.4 ||
            theta < -Constants::twelve_degrees() ||
            theta > Constants::twelve_degrees())
    {
        // cout << "+++++++++++++++++\nThe pole fell over\n+++++++++++++++++\n";
        return ("-1"); /* to signal failure */
    }
    else
    {
        //cart pos
        if (x < -0.8) cartPos = "L";
        else if (x < 0.8) cartPos = "C";
        else cartPos = "R";
        //cart speed
        if (x_dot < -0.5)
        {
            cartSpeed = "FL";
        }
        else if (x_dot < 0)
        {
            cartSpeed = "SL";
        }
        else if (x_dot > 0.5)
        {
            cartSpeed = "FR";
        }
        else if (x_dot >= 0)
        {
            cartSpeed = "SR";
        }
        else
        {
            std::cout << "state problem 1\n";
            exit(45245);
        }
        //pole pos
        if (theta < -Constants::six_degrees())
        {
            polePos = "LL";
        }
        else if (theta < -Constants::one_degree())
        {
            polePos = "ML";
        }
        else if (theta < 0)
        {
            polePos = "SL";
        }
        else if (theta < Constants::one_degree())
        {
            polePos = "SR";
        }
        else if (theta < Constants::six_degrees())
        {
            polePos = "MR";
        }
        else
        {
            polePos = "LR";
        }
        //pole speed
        if (theta_dot < -Constants::fifty_degrees())
        {
            poleSpeed = "FL";
        }
        else if (theta_dot < 0)
        {
            poleSpeed = "SL";
        }
        else if (theta_dot < Constants::fifty_degrees())
        {
            poleSpeed = "SR";
        }
        else
        {
            poleSpeed = "FR";
        }
        output = cartPos + "&" + cartSpeed + "&" + polePos + "&" + poleSpeed;
        //std::cout << "State= " << output << "\n";
    }
    return (output);
}

void CartPole::printState()
{
    cout << "---------------------------------\nState is:\n";
    //cout << "\tx (position of cart)= " << x << "\n";
    //cout << "\tx_dot (cart speed)= " << x_dot << "\n";
    //cout << "\ttheta (angle)= " << theta * Constants::DEG_IN_RAD() << "\n";
    //cout << "\ttheta_dot (rotational speed)= " << theta_dot * Constants::DEG_IN_RAD() << "\n";
    //cout << "\tBox= " << this->getBox() << "\n";
    cout << "\tState= " << this->getState() << "\n";
}

float CartPole::getTheta_dot() const
{
    return theta_dot;
}

float CartPole::getTheta() const
{
    return theta;
}

float CartPole::getX_dot() const
{
    return x_dot;
}

float CartPole::getX() const
{
    return x;
}