/* 
 2014 Version based on https://github.com/dhiltonp/hexbright
 
 BETA!
 
 */
#include <print_power.h>

#define BUILD_HACK
#include <hexbright.h>


#define MS 5
hexbright hb;

#define HOLD_TIME 250 // milliseconds before going to strobe
#define OFF_TIME 650 // milliseconds before going off on the next normal button press

#define BRIGHTNESS_COUNT 4
#define BRIGHTNESS_OFF BRIGHTNESS_COUNT-1
int brightness[BRIGHTNESS_COUNT] = {1000, 600, 300, OFF_LEVEL};
int current_brightness = BRIGHTNESS_OFF; // start on the last mode (off)

unsigned long short_press_time = 0;

void setup() {
  hb.init_hardware(); 
}

void loop() {
  hb.update();
  
  if(hb.button_just_released()) {
    
    if(hb.button_pressed_time()<HOLD_TIME) {
      // we just had a normal duration press
      if(short_press_time+OFF_TIME<millis() && current_brightness!=BRIGHTNESS_OFF) {
        // it's been a while since our last button press, turn off
        current_brightness = BRIGHTNESS_OFF;
      } else {
        short_press_time = millis();
        current_brightness = (current_brightness+1)%BRIGHTNESS_COUNT;
      }
    } else {
      // we have been doing strobe, set light at the previous light level (do nothing to current_brightness)
    }
    // actually change the brightness
    hb.set_light(CURRENT_LEVEL, brightness[current_brightness], 50);
  } else if (hb.button_pressed() && hb.button_pressed_time()>HOLD_TIME) {
    // held for over HOLD_TIME ms, go to strobe
    static unsigned long flash_time = millis();
    if(flash_time+70<millis()) { // flash every 70 milliseconds
      flash_time = millis(); // reset flash_time
      hb.set_light(MAX_LEVEL, 0, 20); // and pulse (going from max to min over 20 milliseconds)
      // actually, because of the refresh rate, it's more like 'go from max brightness on high
      //  to max brightness on low to off.
    }
  } 
  print_power();
  
  
}

