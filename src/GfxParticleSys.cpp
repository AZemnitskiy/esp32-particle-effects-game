#include "GfxParticleSys.h"

GfxParticleSys::GfxParticleSys(Adafruit_GFX *display, int color, 
                int count, Particle_Abstract particles[], 
                Emitter_Abstract* emitter): ParticleSys(count, particles, emitter)
{
    this->display = display;
    this->color = color;
}

void GfxParticleSys::drawAsPixels()
{
  for (int i = 0; i < num; i++)
  {
   if( particles[i].isAlive) {
      display->drawPixel(particles[i].x, particles[i].y, color);
    }
  }
}

void GfxParticleSys::drawAsPolygons()
{
  int i;
  for (i = 0; i < num; i++)
  {
   if( particles[i].isAlive) {

      int scale = max(particles[i].vx, -2);
      drawPolygon(particles[i].x, particles[i].y, 9,  abs(8/scale), 0.0);
      drawPolygon(particles[i].x, particles[i].y, 16, abs(8/scale), 0.0);
      drawPolygon(particles[i].x, particles[i].y, 16, abs(4/scale), 0.0);
    }
  }
}


void GfxParticleSys::displace(int stepX, int stepY) {
  for (int i = 0; i < num; i++)
  {
   if(particles[i].isAlive) {
      particles[i].x += stepX;
      particles[i].y += stepY;
    }
  }
}

void GfxParticleSys::drawAsSmallTriangles() {
   for (int i = 0; i < num; i++)
   {
    if(particles[i].isAlive) {
      display->drawPixel(particles[i].x, particles[i].y, color);
      display->drawPixel(particles[i].x, particles[i].y-1, color);
      display->drawPixel(particles[i].x, particles[i].y+1, color);
      display->drawPixel(particles[i].x+1, particles[i].y, color);
    }
  }
}

void GfxParticleSys::drawAsSprites(const uint8_t sprite[], const uint8_t mask[], int width, int height) {
  for (int i = 0; i < num; i++)
  {
   if( particles[i].isAlive) {
      // draw mask
      display->drawBitmap(particles[i].x, particles[i].y, mask, width, height, 0);
      // draw image
      display->drawBitmap(particles[i].x, particles[i].y, sprite, width, height, color);
    }
  }
}

//from triangle to whatever...
void GfxParticleSys::drawPolygon(int16_t cx, int16_t cy, uint8_t sides, int16_t diameter, float rot)
{
  sides = (sides > 2? sides : 3);
  float dtr = (PI/180.0) + PI;
  float rads = 360.0 / sides;//points spacd equally
  uint8_t i;
  for (i = 0; i < sides; i++) {
    display->drawLine(
      cx + (sin((i*rads + rot) * dtr) * diameter),
      cy + (cos((i*rads + rot) * dtr) * diameter),
      cx + (sin(((i+1)*rads + rot) * dtr) * diameter),
      cy + (cos(((i+1)*rads + rot) * dtr) * diameter),
      color);
  }
}