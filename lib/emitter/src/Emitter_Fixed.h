/*
 * Emit a particle with the same parameters each time
 */

#ifndef emitter_fixed_h
#define emitter_fixed_h

#include "Emitter_Abstract.h"
#include "Arduino.h"

class Emitter_Fixed : public Emitter_Abstract {
public:
    unsigned int counter;
    byte x;
    byte y;
    signed char vx;
    signed char vy;
    byte ttl;

    Emitter_Fixed(byte x, byte y, signed char vx, signed char vy, byte ttl);
    void emit(Particle_Abstract * particle);
};

#endif /* emitter_fixed_h */
