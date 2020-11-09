#include "ParticleSys.h"

ParticleSys::ParticleSys(int num, Particle_Abstract particles[], Emitter_Abstract *emitter)
{
    this->num = num;
    this->particles = particles;
    this->emitter = emitter;
    this->cycles_remaining = 1;
    this->is_forever = true;
}

void ParticleSys::update()
{

    emitter->update();
    for(int i = 0; i<num; i++) {
        if (!particles[i].isAlive && cycles_remaining > 0) {
            emitter->emit(&particles[i]);
            cycles_remaining--;
        }
        else if (!particles[i].isAlive && this->is_forever) {
           emitter->emit(&particles[i]);
        }

        if (particles[i].isAlive){
            particles[i].update();
        }
    }
}
