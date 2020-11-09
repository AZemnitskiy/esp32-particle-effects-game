#include "Particle_Fixed.h"

byte Particle_Fixed::decayFactor = 10;

Particle_Fixed::Particle_Fixed()
{
    isAlive = false;
}

void Particle_Fixed::update(void)
{
    //age
    if (ttl < decayFactor) {
        isAlive = false;
        return;
    }
    ttl -= decayFactor;
}
