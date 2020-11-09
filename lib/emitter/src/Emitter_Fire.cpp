#include "Emitter_Fire.h"

byte Emitter_Fire::baseHue = 128; //blues
byte Emitter_Fire::maxTtl = 128;

Emitter_Fire::Emitter_Fire(bool isHorizontal, int xStart, int xEnd, int yStart, int yEnd)
{
    this->counter = 0;
    this->cycleHue = false;
    this->isHorizontal = isHorizontal;
    this->xStart = xStart;
    this->xEnd = xEnd;
    this->yStart = yStart;
    this->yEnd = yEnd;
}

void Emitter_Fire::update()
{
}

void Emitter_Fire::emit(Particle_Abstract *particle)
{
    counter++;
    if (cycleHue) baseHue = (counter>>2)%240;

    if(this->isHorizontal) {
      particle->x = random(xStart, xEnd);
      particle->y = yStart;
      int dist = abs(particle->x - (xStart + xEnd) / 2);
      particle->ttl = random(1, max(maxTtl - dist, 1)) ;
      particle->vy = random(1, max(4 - dist, 1)) ;
      particle->vx = 0;
    }
    else {
      particle->y = random(yStart, yEnd);
      particle->x = xStart;
      int dist = abs(particle->y - (yStart + yEnd) / 2);
      particle->ttl = random(1, max(maxTtl - dist, 1)) ;
      particle->vx = - random(1, max(4 - dist, 1)) ;
      particle->vy = 0;
    }
    particle->hue = baseHue;


    particle->isAlive = true;
}
