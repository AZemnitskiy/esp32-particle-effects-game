/*
 * Particle with an attractor
 */

#ifndef particle_attractor_h
#define particle_attractor_h

#include "Particle_Abstract.h"

class Particle_Attractor : public Particle_Abstract {
public:
    static byte atx; //horizontal attractor position
    static byte aty; //vertical attractor position
    static signed char atf; //attractor force

    Particle_Attractor();
    void update(void);
};

#endif /* particle_attractor_h */
