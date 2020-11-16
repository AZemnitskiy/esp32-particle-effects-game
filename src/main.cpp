 /*
  * Meteorite shooter game for Arduino/ESP32 based on Adafruit_GFX library
  * that uses particles physics for all animation effects.
  * 
  * Tested to work with SSD1306 OLED display, but should work with most other monochrome and color displays.
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
#include <Fonts/Org_01.h>

// particle & system
#include <ParticleSys.h>
#include <Particle_Std.h>
#include <Particle_Attractor.h>

//emmiters
#include <Emitter_Fire.h>
#include <Emitter_Side.h>
#include <Emitter_Fixed.h>
#include <Emitter_HollowCircle.h>

// Sprites & Gfx utils
#include <Sprites.h>
#include <GfxParticleSys.h>

// Audio effects
#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#define CUTE_C2_IMPLEMENTATION

#include <cute_c2.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI  18 // SCL
#define OLED_CLK   23 // SDA
#define OLED_DC    21
#define OLED_CS    5
#define OLED_RESET 17

#define BUTUP 14
#define BUTDOWN 27
#define BUTRIGHT 26
#define BUTLEFT 32

// display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int positionX = 20, positionY = 30, stepX = 0, stepY = 0, trispeed = 3;
bool hasSpaceshipCollision = false;
int score = 0, lives = 3;

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
Particle_Attractor alienShips[numAlienShips];
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

// sounds
TaskHandle_t Task1;
AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

void detectBulletWithMeteoriteCollisions(){
  for (int j = 0; j < numMeteoriteParticles; j++){
    if( meteoriteParticles[j].isAlive) {
      
      // meteorite - "circle"
      float circleRadius = (float) round((int)(8 /abs(max(meteoriteParticles[j].vx, -2))));
      c2v center = { (float) meteoriteParticles[j].x, (float)meteoriteParticles[j].y};
      c2Circle meteoriteCircle = {center, circleRadius};

      // check bullet collisions
      for (int i = 0; i < numBullets; i++){
        if( bullets[i].isAlive) {
            // bullet trace - "capsule"
            c2v start = {(float) (bullets[i].x - bulletSpeedOn), (float) bullets[i].y};
            c2v end = {(float) bullets[i].x, (float)bullets[i].y};
            float capsuleRadius = 1.0;
            c2Capsule bulletCapsule = {start, end, capsuleRadius};
            
            if(c2CircletoCapsule(meteoriteCircle, bulletCapsule)) {
              // update explosion emitter position
              explosionCenter.x = meteoriteParticles[j].x;
              explosionCenter.y = meteoriteParticles[j].y;
              meteoriteParticles[j].isAlive = false;
              explosionSystem.cycles_remaining = numExplosionParticles;
              score++;
            }
         }
      }

     // check spaceship collisions
     // positionY, positionX
      // int count = 6;
      // c2v verts[C2_MAX_POLYGON_VERTS] = {{}, {}, {}, {}, {}};
      // c2v norms[C2_MAX_POLYGON_VERTS];
      // c2Poly spaceshipPolygon = {count, *verts, *norms};
      // c2MakePoly(&spaceshipPolygon);
      // if(c2CircletoPoly(meteoriteCircle, &spaceshipPolygon, NULL) && !hasSpaceshipCollision){
      //     hasSpaceshipCollision = true;
      //     // substract one life
      // }
      // else {
      //   hasSpaceshipCollision = false;
      // }

    }
  }
}


// void detectSpaceshipWithMeteoriteCollisions(){
//   for (int j = 0; j < numMeteoriteParticles; j++)
//     {
//         if( meteoriteParticles[j].isAlive) {

//             // meteorite - "circle"
//             float circleRadius = (float) round((int)(8 /abs(max(meteoriteParticles[j].vx, -2))));
//             c2v center = { (float) meteoriteParticles[j].x, (float)meteoriteParticles[j].y};
//             c2Circle meteoriteCircle = {center, circleRadius};

//             // spaceship - "polygon"
//             c2v start = {(float) (bullets[i].x - bulletSpeedOn), (float) bullets[i].y};
//             c2v end = {(float) bullets[i].x, (float)bullets[i].y};
//             float capsuleRadius = 1.0;
//             c2Capsule bulletCapsule = {start, end, capsuleRadius};
            
//             if(c2CircletoCapsule(meteoriteCircle, bulletCapsule)) {
//               // update explosion emitter position
//               explosionCenter.x = meteoriteParticles[j].x;
//               explosionCenter.y = meteoriteParticles[j].y;
//               meteoriteParticles[j].isAlive = false;
//               explosionSystem.cycles_remaining = numExplosionParticles;
//             }
//          }
//       }
// }

void drawLives() {
  display.fillRect(-1, -1, 20,10, BLACK);
  display.drawBitmap(1, 1, life, 7, 6, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 5);
  display.print("x");
  display.print(lives);
}

void drawScore() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(1, SCREEN_HEIGHT - 1);
  display.print(score);
}

void drawSpaceship()
{
  display.drawBitmap(positionX, positionY - 3, userShipMask, USER_SHIP_WIDTH, USER_SHIP_HIGHT, BLACK);
  display.drawBitmap(positionX+1, positionY - 3, userShip, USER_SHIP_WIDTH, USER_SHIP_HIGHT, WHITE);
}

void playBackgroundMusic( void * pvParameters ){
  file = new AudioFileSourceSPIFFS("/background_3.mp3");
  id3 = new AudioFileSourceID3(file);
  out = new AudioOutputI2S(0, 1);
  out->SetGain(((float)60)/100.0);
  out->SetOutputModeMono(true);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out, true);

  for(;;){
     if (mp3->isRunning()) {
        if (!mp3->loop()) 
        {
          // id3 = new AudioFileSourceID3(file);
          // mp3->begin(id3, out);
        }
      }
  }
}

void setup() {
  Serial.begin(9600);
  SPIFFS.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  
  // configure display
  display.clearDisplay();
  display.setFont(&Org_01);

  // set various particle system settings
  Emitter_Fire::maxTtl = 6;
  Particle_Attractor::atf = 2;

  // explosion is always trggered on-demand
  explosionSystem.is_forever = false;
  explosionSystem.cycles_remaining = 0;

  // enable controls
  pinMode(BUTUP, INPUT_PULLUP);
  pinMode(BUTDOWN, INPUT_PULLUP);
  pinMode(BUTRIGHT, INPUT_PULLUP);
  pinMode(BUTLEFT, INPUT_PULLUP);


//create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    playBackgroundMusic,   /* Task function. */
                    "backgroundMusic",     /* name of task. */
                    10000,                 /* Stack size of task */
                    NULL,                  /* parameter of the task */
                    1,                     /* priority of the task */
                    &Task1,                /* Task handle to keep track of created task */
                    1);                    /* pin task to core 0 */ 
  

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

    Particle_Attractor::atx = meteoriteParticles[0].x;
    Particle_Attractor::aty = meteoriteParticles[0].y;

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
  drawScore();

  // refreshed screen
  display.display();
  delay(40);
}


