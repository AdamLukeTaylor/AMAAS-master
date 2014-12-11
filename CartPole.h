/* 
 * File:   CartPole.h
 * Author: Adam
 *
 * Created on 17 June 2014, 11:14
 */

#ifndef CARTPOLE_H
#define	CARTPOLE_H

class CartPole {
public:
    CartPole();
    virtual ~CartPole();
    int getBox();
    std::string getState();
    void executeAction(int action);
    void setCurrentState(std::string currentState);
    std::string getCurrentState();
    void printState();
    float getTheta_dot() const;
    float getTheta() const;
    float getX_dot() const;
    float getX() const;
    void reset();
    void randReset();
private:
    float x;
    float x_dot;
    float theta;
    float theta_dot;
    std::string currentState;

};

#endif	/* CARTPOLE_H */

