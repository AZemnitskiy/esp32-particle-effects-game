/*
 * A spinning emitter.
 * Works well with 36 particles
 */

#ifndef emitter_spin_h
#define emitter_spin_h

#include "Emitter_Abstract.h"
#include "Arduino.h"

class Emitter_Spin : public Emitter_Abstract {
public:
    static byte maxTtl;
    byte x;   //left
    byte y;   //bottom
    byte r;   //radius
    signed char rv;  //radial velocity
    boolean oscilate; //whether to oscilate radial velocity
    unsigned int counter;
    Emitter_Spin(byte x, byte y, byte r, signed char rv);
    void emit(Particle_Abstract * particle);
    void update();
private:
    signed char vx;
    signed char vy;
    signed char tempRv;
};

#endif /* emitter_spin_h */
