/*
 * A hollow ring emmiting particles.
 * Suitable for explision animations.
 */

#ifndef emitter_hollow_circle_h
#define emitter_hollow_circle_h

#include "Emitter_Abstract.h"
#include "Arduino.h"

class Emitter_HollowCircle : public Emitter_Abstract {
public:
    Particle_Abstract *source;
    unsigned int counter;

    Emitter_HollowCircle(float inner_size,
					  byte velocity_max_range, byte radius,
					  byte ttl, Particle_Abstract *source);
    void emit(Particle_Abstract * particle);
    void update();
private:
    byte ttl;
    byte velocity_max_range;
	  float inner_size;
    byte radius;
};

#endif /* Emitter_HollowCircle_h */
