/* 
 * File:   MtCar.h
 * Author: Adam
 *
 * Created on 24 June 2014, 08:55
 */
#include <string>
#ifndef MTCAR_H
#define	MTCAR_H

class MtCar {
public:
    MtCar();
    MtCar(const MtCar& orig);
    std::string getState();

    int getSign(double x);

    void printState();
    void reset(bool random);
    bool executeAction(int action);
    virtual ~MtCar();
private:

    double x; //+ is right -left
    double xDot;

};

#endif	/* MTCAR_H */

