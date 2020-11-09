/*
 * Emit particles to simulate fire
 */

#ifndef emitter_fire_h
#define emitter_fire_h

#include <PsConstants.h>
#include <Emitter_Abstract.h>

class Emitter_Fire : public Emitter_Abstract {
public:
    static byte baseHue;
    static byte maxTtl;
    unsigned int counter;
    boolean cycleHue;
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;
    boolean isHorizontal;

    Emitter_Fire(bool isHorizontal, int xStart, int xEnd, int yStart, int yEnd);
    void emit(Particle_Abstract *particle);
    void update();

private:
    int rng(int startValue, int endValue);
};

#endif /* emitter_fire_h */
