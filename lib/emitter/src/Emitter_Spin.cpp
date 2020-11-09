#include "Emitter_Spin.h"

byte Emitter_Spin::maxTtl = 100;

Emitter_Spin::Emitter_Spin(byte x, byte y, byte r, signed char rv)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->rv = tempRv = rv;
    oscilate = false;
    counter = 0;
}

void Emitter_Spin::update()
{
    static signed char direction = -1;
    float radAngle;
    counter++;

    //calculate velocity vector
    if (oscilate && (counter%20 == 0)){
        tempRv += direction;
        if (abs(tempRv) > rv){
            direction = -direction;
        }
    }

    // Conver from Degree -> Rad
    if (counter%2 == 0) {
        radAngle = -counter*tempRv*(PI/180) ;
    } else {
        radAngle = 180-counter*tempRv*(PI/180) ;
    }
    // Convert Polar -> Cartesian
    vx = (signed char)(r * cos(radAngle));
    vy = (signed char)(r * sin(radAngle));
}

void Emitter_Spin::emit(Particle_Abstract * particle)
{
    particle->x = this->x;
    particle->y = this->y;

    particle->vx = vx;
    particle->vy = vy;

    particle->ttl = random(20, maxTtl);
    particle->hue = (counter>>1)%255;
    particle->isAlive = true;
}
