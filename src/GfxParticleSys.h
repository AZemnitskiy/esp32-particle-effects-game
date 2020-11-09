/*
*  Star dust animation
*/

#ifndef gfx_particle_sys_h
#define gfx_particle_sys_h

#include "Arduino.h"
#include <Particle_Abstract.h>
#include <Emitter_Abstract.h>
#include <ParticleSys.h>
#include <Adafruit_GFX.h>

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

class GfxParticleSys : public ParticleSys {
public:
  
    Adafruit_GFX* display;
    int color;

    GfxParticleSys(Adafruit_GFX *display, int color, int num, Particle_Abstract particles[], Emitter_Abstract* emitter);
    void drawAsPixels();
    void drawAsPolygons();
    void drawAsSprites(const uint8_t sprite[], const uint8_t mask[], int width, int height);
    void drawAsSmallTriangles();
    void displace(int stepX, int stepY);

private:
    void drawPolygon(int16_t cx, int16_t cy, uint8_t sides, int16_t diameter, float rot);
};

#endif /* gfx_particle_sys_h */