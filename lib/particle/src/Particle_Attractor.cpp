#include "Particle_Attractor.h"

byte Particle_Attractor::atx = 112;
byte Particle_Attractor::aty = 112;
signed char Particle_Attractor::atf = 4;

Particle_Attractor::Particle_Attractor()
{
    isAlive = false;
}

void Particle_Attractor::update(void)
{
    int dx, dy, tempX, tempY, tempVx, tempVy;
    signed char acx, acy;
    float mult;
    //age
    //ttl--;
    if (ttl == 0) {
        isAlive = false;
    }

    dx = (int)atx - x;
    dy = (int)aty - y;
    mult = (float)atf/sqrt(dx*dx+dy*dy);
    acx = (signed char)(mult*dx);
    acy = (signed char)(mult*dy);

    //apply acceleration
    tempVx = vx+acx;
    tempVy = vy+acy;

    tempX = x + tempVx;
    tempY = y + tempVy;

    if (tempX < 0 || tempX > PS_MAX_X){
        tempVx = 0;//-tempVx;
    }
    if (tempY < 0 || tempY > PS_MAX_Y){
        tempVy = 0;//-tempVy;
    }

    if (tempVx > 4 || tempVx < -4) vx = 4* tempVx/abs(tempVx);
    else vx = tempVx;

    if (tempVy > 4 || tempVy < -4) vy = 4* tempVy/abs(tempVy);
    else vy = tempVy;

    if (tempX > PS_MAX_X) x = PS_MAX_X;
    else if (tempX < 0) x = 0;
    else x = tempX;

    if (tempY > PS_MAX_Y) y = PS_MAX_Y;
    else if (tempY < 0) y = 0;
    else y = tempY;
}
