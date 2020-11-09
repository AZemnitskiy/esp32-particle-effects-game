/*
 * Emit particles from one side of the box
 */

#ifndef emitter_side_h
#define emitter_side_h

#include "Emitter_Abstract.h"
#include "Arduino.h"

class Emitter_Side : public Emitter_Abstract {
public:
    static byte baseHue;
    static byte maxTtl;
    unsigned int counter;
    char side;

    Emitter_Side(char side);
    void emit(Particle_Abstract * particle);
};

#endif /* emitter_side_h */
