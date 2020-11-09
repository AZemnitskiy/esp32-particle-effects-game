#include "Emitter_HollowCircle.h"

void Emitter_HollowCircle::update(){

}

Emitter_HollowCircle::Emitter_HollowCircle(float inner_size,
									 byte velocity_max_range, byte radius,
									 byte ttl, Particle_Abstract *source)
{
    this->inner_size = inner_size;
    this->velocity_max_range = velocity_max_range;
	  this->ttl = ttl;
    this->radius = radius;
    this->source = source;
    this->counter = 0;

}


void Emitter_HollowCircle::emit(Particle_Abstract * particle)
{
    this->counter++;

    // create two random angles alpha and beta
    // with these two angles, we are able to select any point on an
    // ellipsoid's surface
    float alpha  = random(0, (int)(PI * 2 * 1000.0))/ 1000.0;

    // create two random radius values that are bigger than the inner
    // size, but smaller/equal than/to the outer size 1.0 (inner size is
    // between 0 and 1)
    float a = random((int)(this->inner_size * 1000.0),1000)/1000.0;

    // with a & b we have defined a random ellipsoid between the inner
    // ellipsoid and the outer sphere (radius 1.0)
    // with alpha and beta we select on point on this random ellipsoid
    // and calculate the 2D coordinates of this point
    particle->x =  round(source->x + a * cos(alpha) * radius) ;
    particle->y =  round(source->y + a * sin(alpha) * radius);

	  float velocity_mult = random(1,velocity_max_range);
	  particle->vx = round(a * cos(alpha) * velocity_mult) ;
    particle->vy = round(a * sin(alpha) * velocity_mult);
    particle->ttl = random(1, ttl);
    particle->hue = counter%255;
    particle->isAlive = true;

}
