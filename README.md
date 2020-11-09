 # Meteorite Shooter
 Meteorite shooter game for ESP32 based on Adafruit_GFX library
 that uses particles physics for all animation effects.
   
 Tested to work with SSD1306 OLED display, but should work with most other monochrome and color displays.
 ESP32 is not strictly needed - this should work on any Arduino compatible platform what has enough RAM.
 Just keep in mind, that complex particle effects might require more compute capacity then a vanilla Arduino Uno can provide for smooth animation. 

 ### Build
 Use Visual Studio Code + platform.io plugin to build and upload the game to ESP32. 
   
 ### Particle Animations
 - Meteorites: side emmiter, draws polygons
 - Space dust: side emitter, draws pixels
 - Alien ships: side emmiter, draws sprites with mask
 - Rocker Exhaust: fire emitter, draws pixels
 - Explosions: hollow ring emitter, draws pixels
 - Bullets: fixed emmiter, draws little triangles
  
### Used libs
- [arduino-particle-sys](https://github.com/giladaya/arduino-particle-sys)
  Emitter library for Arduino by Gilad Dayagi
- [cute_c2](https://github.com/RandyGaul/cute_headers/blob/master/cute_c2.h)
  Header only library for collision detection
- [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)
  Drawing graphic primitives

### TODO
- finish conversion to cute_c2 for collision detection
- add score count and initial in-game menu
  
### Copyright
GNU Public License v2