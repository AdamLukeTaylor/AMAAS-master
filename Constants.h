/* 
 * File:   Constants.h
 * Author: Adam
 *
 * Created on 17 June 2014, 14:30
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

class Constants {
public:
    //run stuff
    const static int NUMBER_OF_ALPHAS = 10; //one less than is actually wanted
    const static int NUMBER_OF_GAMMAS = 10; //11
    const static int NUMBER_OF_EXPS = 10; //10
    const static int NUMBER_OF_MT_CAR_EXPOLITE_RUNS = 1;
    const static int NUMBER_OF_CART_POLE_EXPLOITE_RUNS = 1;

    //mountain car
    //learn too fast ? ? ? ? ?
    const static int CAR_TRAINING = 10;
    const static int CAR_TRAINING_STEPS = (10000); //now used 
    const static int TEST_FREQ = 50000;
    const static int CAR_EXPLOITATION_TIME = 100000; //who cares how long results discarded
    const static int CAR_MAX = 100000;

    const static double LEFT_EDGE() {
        return -1.2;
    }

    const static double RIGHT_EDGE() {
        return 0.6;
    }

    const static double MAX_LEFT_X_DOT() {
        return -0.07;
    }

    const static double MAX_RIGHT_X_DOT() {
        return 0.07;
    }

    const static double TARGET_X() {
        return 0.6;
    }

    //CART POLE
    const static int POLL_TRAINING = 10;
    /*** Parameters for RL ***/
    const static int N_BOXES = 162; /* Number of disjoint boxes of state space. */

    const static float one_degree() {
        return 0.0174532;
    } /* 2pi/360 */

    const static float six_degrees() {
        return 0.1047192;
    }

    const static float twelve_degrees() {
        return 0.2094384;
    }

    const static float fifty_degrees() {
        return 0.87266;
    }

    /*** Parameters for simulation ***/
    const static float GRAVITY() {
        return 9.8;
    }

    const static float MASSCART() {
        return 1.0;
    }

    const static float MASSPOLE() {
        return 0.1;
    }

    const static float TOTAL_MASS() {
        return (Constants::MASSPOLE() + Constants::MASSCART());
    }

    const static float LENGTH() {
        return 0.5;
    } /* actually half the pole's length */

    const static float POLEMASS_LENGTH() {
        return (Constants::MASSPOLE() * Constants::LENGTH());
    }

    const static float FORCE_MAG() {
        return 10.0;
    }

    const static float TAU() {
        return 0.001;
        //return 0.001;
    } /* seconds between state updates */

    const static float FOURTHIRDS() {
        return 1.3333333333333;
    };

    const static float DEG_IN_RAD() {
        return 57.2957795;
    }
};
#endif	/* CONSTANTS_H */

