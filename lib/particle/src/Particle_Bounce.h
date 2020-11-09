/*
 * A particle that bounces off the "box" walls
 */

#ifndef particle_bounce_h
#define particle_bounce_h

#include "Particle_Abstract.h"

class Particle_Bounce : public Particle_Abstract {
public:
    static signed char ax; //horizontal acceleration
    static signed char ay; //vertical acceleration

    Particle_Bounce();
    void update(void);
};

#endif /* particle_bounce_h */
