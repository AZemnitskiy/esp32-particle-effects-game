/*
 * ParticleSys.h - generic particle system class
 */

#ifndef p_system_h
#define p_system_h

#include "PsConstants.h"
#include <Particle_Abstract.h>
#include <Emitter_Abstract.h>


//#include "Arduino.h"

class ParticleSys {
public:
    int num;
    Particle_Abstract *particles;
    Emitter_Abstract *emitter;
    byte cycles_remaining;
    boolean is_forever;

    ParticleSys(int num, Particle_Abstract particles[], Emitter_Abstract *emitter);
    void update();

};

#endif /* p_system_h */
