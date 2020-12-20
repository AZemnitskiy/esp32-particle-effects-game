 # Meteorite Shooter
 Meteorite shooter game for ESP32 based on Adafruit_GFX library that uses particles physics for all animation effects.

 Tested to work with SSD1306 OLED display, but should work with most other monochrome and color displays.
 ESP32 is not strictly needed - this should work on any Arduino compatible platform, just keep in mind that complex particle effects might require more compute capacity then a vanilla Arduino Uno can provide for smooth animation effects. 

![screenshot](https://raw.githubusercontent.com/AZemnitskiy/esp32-particle-effects-game/main/img/screenshot.PNG)

# Gameplay Video:
[https://youtu.be/A6KwtKLPqg0](https://youtu.be/A6KwtKLPqg0)

 ### OLED Display and Button Wiring with Esp32 NodeMCU
OLED 1306 display supports SPI communication protocol. You can use the following table as a reference.

| OLED 1306 Pin | Esp32 NodeMCU Pin |
|---------------|-------------------|
| VIN           | 3.3V              |
| GND           | GND               |
| SCL (MOSI)    | GPIO 23           |
| SDA (CLK)     | GPIO 18           |
| DC            | GPIO 21           |
| CS            | GPIO 5            |
| RESET         | GPIO 4            |

In-game push buttons were wired as follows:

| BUTTON | Esp32 NodeMCU Pin |
|--------|-------------------|
| UP     | GPIO 19           |
| DOWN   | GPIO 16           |
| RIGHT  | GPIO 22           |
| LEFT   | GPIO 17           |

 ### Building Project
 Use Visual Studio Code + [platform.io](https://platform.io) plugin to build and upload the game to ESP32. 

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
- [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)
  Drawing graphic primitives

### TODO
- finish conversion to cute_c2 for collision detection
- add score count and initial in-game menu
  
### Copyright
GNU Public License v.2