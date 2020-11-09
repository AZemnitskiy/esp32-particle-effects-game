 /*
  * Meteorite shooter game for Arduino/ESP32 based on Adafruit_GFX library
  * that uses particles physics for all animation effects.
  * 
  * Tested to work with SSD1306 OLED display, but should work with any supporting display.
  * 
  * Particle Animations:
  * - Meteorites: side emmiter, draws polygons
  * - Space dust: side emitter, draws pixels
  *   Alien ships: side emmiter, draws sprites with mask
  * - Rocker Exhaust: fire emitter, draws pixels
  * - Explosions: hollow ring emitter, draws pixels
  * - Bullets: fixed emmiter, draws little triangles
  */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// particle & system
#include <ParticleSys.h>
#include <Particle_Std.h>

//emmiters
#include <Emitter_Fire.h>
#include <Emitter_Side.h>
#include <Emitter_Fixed.h>
#include <Emitter_HollowCircle.h>

// Sprites & Gfx utils
#include <Sprites.h>
#include <GfxParticleSys.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI  23 // SCL
#define OLED_CLK   18 // SDA
#define OLED_DC    21
#define OLED_CS    5
#define OLED_RESET 4

#define BUTUP 19
#define BUTDOWN 16
#define BUTRIGHT 22
#define BUTLEFT 17

// display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int positionX = 20, positionY = 30, stepX = 0, stepY = 0, trispeed = 3;

// particle settings
int fireWidth = 5;
const byte numFireParticles = 20;
Particle_Std fireParticles[numFireParticles];
Emitter_Fire fireEmitter(false, positionX, positionX, positionY, positionY + fireWidth);
GfxParticleSys fireSystem(&display, WHITE, numFireParticles, fireParticles, &fireEmitter);

// star dust settings
const int numDustParticles = 15;
Particle_Std dustParticles[numDustParticles];
Emitter_Side dustEmmiter ('r');
GfxParticleSys dustSystem(&display, WHITE, numDustParticles, dustParticles, &dustEmmiter);

// meteorite settings
const byte numMeteoriteParticles = 2;
Particle_Std meteoriteParticles[numMeteoriteParticles];
Emitter_Side meteoriteEmitter('r');
GfxParticleSys meteoriteSystem(&display, WHITE, numMeteoriteParticles, meteoriteParticles, &meteoriteEmitter);

// alien ship settings
const byte numAlienShips = 1;
Particle_Std alienShips[numAlienShips];
Emitter_Side alienEmitter('r');
GfxParticleSys alienSystem(&display, WHITE, numAlienShips, alienShips, &alienEmitter);

// explosion settings
Particle_Std explosionCenter;
const byte numExplosionParticles = 100;
byte explosionVelocity = 3;
byte explosionRadiusStart = 5;
byte explosionTtl = 6;
Particle_Std explosionParticles[numExplosionParticles];
Emitter_HollowCircle explosionEmitter(0.9, explosionVelocity, explosionRadiusStart, explosionTtl, &explosionCenter);
GfxParticleSys explosionSystem(&display, WHITE, numExplosionParticles, explosionParticles, &explosionEmitter);

// bullet settings
int bulletSpeedOn = 6, bulletSpeedOff = 0;
int bulletTtlOn = 60, bulletTtlOff = 1;
const byte numBullets = 1;
Particle_Std bullets[numBullets];
Emitter_Fixed bulletEmitter(positionX + 15, positionY + 3, bulletSpeedOff, 0, bulletTtlOff);
GfxParticleSys bulletSystem(&display, WHITE, numBullets, bullets, &bulletEmitter);


void detectBulletWithMeteoriteCollisions(){
  for (int i = 0; i < numBullets; i++)
  {
   if( bullets[i].isAlive) {
      for (int j = 0; j < numMeteoriteParticles; j++)
      {
         if( meteoriteParticles[j].isAlive) {
            int radius =  round((int)(8 /meteoriteParticles[j].vx));
            if(pow(bullets[i].x  - meteoriteParticles[j].x, 2) + pow(bullets[i].y - meteoriteParticles[j].y, 2) <= pow(radius, 2)) {
              // point is inside the circle, we have a collision
              // update explosion emitter position
              explosionCenter.x = meteoriteParticles[j].x;
              explosionCenter.y = meteoriteParticles[j].y;
              meteoriteParticles[j].isAlive = false;
              explosionSystem.cycles_remaining = numExplosionParticles;
            }
         }
      }
    }
  }
}

void drawLives() {
  display.fillRect(-1, -1, 30,10, BLACK);
 // display.drawRect(-1, -1, 30,10, WHITE);
  for (int i = 0; i < 3; i++)
  {
     display.drawBitmap(1  + i * 9, 1, life, 7, 6, WHITE);
  }
}

void drawSpaceship()
{
  display.drawBitmap(positionX, positionY - 3, userShipMask, USER_SHIP_WIDTH, USER_SHIP_HIGHT, BLACK);
  display.drawBitmap(positionX+1, positionY - 3, userShip, USER_SHIP_WIDTH, USER_SHIP_HIGHT, WHITE);
}


// void drawAlienShips()
// {
//   int i;
//   for (i = 0; i < numAlienShips; i++)
//   {
//    if( alienShips[i].isAlive) {
//       // draw mask
//       display.drawBitmap(alienShips[i].x, alienShips[i].y  + cos((alienShips[i].x / 90.0) * 2*PI)*5, spaceship2_mask, 16, 11, BLACK);
//       // draw image
//       display.drawBitmap(alienShips[i].x, alienShips[i].y  + cos((alienShips[i].x / 90.0) * 2*PI)*5, spaceship2, 16, 11, WHITE);
//     }
//   }
// }

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();

  Emitter_Fire::maxTtl = 6;

  // explosion is always trggered on-demand
  explosionSystem.is_forever = false;
  explosionSystem.cycles_remaining = 0;

  pinMode(BUTUP, INPUT_PULLUP);
  pinMode(BUTDOWN, INPUT_PULLUP);
  pinMode(BUTRIGHT, INPUT_PULLUP);
  pinMode(BUTLEFT, INPUT_PULLUP);
}

void processInputs() {

    if (positionY < -3) {
      positionY = 56;
    }
    else if (positionY >= 60) {
      positionY = 0;
    }

    //down button
    if (digitalRead(BUTDOWN) == LOW) {
      stepY = trispeed;
    }

    //up button
    if (digitalRead(BUTUP) == LOW) {
      stepY = -trispeed;
    }

    // right button (controls flight speed)
    if (digitalRead(BUTRIGHT) == LOW) {
      Emitter_Fire::maxTtl = 8;
      int stepXmin = min(-trispeed, 0);
      dustSystem.displace(stepXmin, 0);
      meteoriteSystem.displace(stepXmin, 0);
      alienSystem.displace(stepXmin, 0);
    }
    else {
      Emitter_Fire::maxTtl = 6;
    }

    // left button (shoots bullets)
    if (digitalRead(BUTLEFT) == LOW) {
      bulletEmitter.vx = bulletSpeedOn;
      bulletEmitter.ttl = bulletTtlOn;
    }
    else {
       bulletEmitter.vx = bulletSpeedOff;
       bulletEmitter.ttl = bulletTtlOff;
    }

    positionY += stepY;

    // update fire emmiter postion
    fireEmitter.yStart = positionY + 1;
    fireEmitter.yEnd   = positionY  + fireWidth;

    // update bullet emmiter postion
    bulletEmitter.y = positionY + 3;

    stepY = 0;
}


void loop() {
  display.clearDisplay();

  // read button inputs
  processInputs();

  // update particle systems
  fireSystem.update();
  meteoriteSystem.update();
  dustSystem.update();
  alienSystem.update();
  bulletSystem.update();
  explosionSystem.update();

  // detect collisions
  detectBulletWithMeteoriteCollisions();

  // draw particle-based objects
  fireSystem.drawAsPixels();
  dustSystem.drawAsPixels();
  meteoriteSystem.drawAsPolygons();
  alienSystem.drawAsSprites(alienShip, alienShipMask, ALIEN_SHIP_WIDTH, ALIEN_SHIP_HIGHT);
  explosionSystem.drawAsPixels();
  bulletSystem.drawAsSmallTriangles();

  // draw other objects
  drawSpaceship();
  drawLives();

  // refreshed screen
  display.display();
  delay(40);
}

