/*
 * A fixed particle that doen't move, just fades
 */

#ifndef particle_fixed_h
#define particle_fixed_h

#include "Particle_Abstract.h"

class Particle_Fixed : public Particle_Abstract {
public:
    static byte decayFactor;
    Particle_Fixed();
    void update(void);
};

#endif /* particle_fixed_h */
