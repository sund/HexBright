/* 
 2014 Version based on https://github.com/dhiltonp/hexbright
 
 BETA!
 
 */
#include <hexbright.h>
#include <Wire.h>

hexbright hb();

void setup() {
  hb.init_hardware(); 
}

void loop() {
  hb.update();
  
  # turn on
  if(hb.button_released() && hb.button_held() < 300/MS) {
    hb.set_light(CURRENTLEVEL, MAX_LOW_LEVEL, 120/MS)
  }
  
  # turn off
  if(hb.button_held() > 300/MS) {
    hb.shutdown();
  }
  
  
}

