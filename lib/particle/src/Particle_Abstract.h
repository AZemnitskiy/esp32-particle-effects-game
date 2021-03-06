/*
 * Abstract particle class
 */

#ifndef particle_abstract_h
#define particle_abstract_h

#include "PsConstants.h"
#include "Arduino.h"

class Particle_Abstract {
public:
    byte x;   //left
    byte y;   //bottom
    signed char vx;  //horizontal velocity
    signed char vy;  //vertical velocity
    byte ttl; //time to live
    byte hue; //hue
    boolean isAlive; //is alive?

    virtual void update(void);
};

#endif /* particle_abstract_h */
