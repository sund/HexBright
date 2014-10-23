//
// My version of HexBrightFLEX based on hexbright
// __________________________________________________
//
// See README.md
// __________________________________________________
//
// __________________________________________________
// See the README.md for info on sources.
//
//

#include <print_power.h>
#include <print_number.h>

// These next two lines must come after all other library #includes
#define BUILD_HACK
#include <hexbright.h>

hexbright hb;

// Settings
#define OVERTEMP 300 // When over this, we turn off or drop light level
#define BUTTONMINPRESS 300 // min time for a button press to matter
#define BUTTONMAXPRESS 650 // max time for a button press to matter
#define DAZZLETIME 1000 // max time for a button press to matter and go to dazzle



void setup()
{
  //needed with hexbright.h
  hb.init_hardware();
}

void loop()
{
  hb.update();
  static int brightness_level = 4;
  
  // get current state
  int state = hb.get_light_level();
  
  // list of power levels {1st, 2nd, 3rd, 4th}
  int levels[] = {250,500,750,1000};
  
  // Pressing buttons
  if(hb.button_just_released()) {
  // if we are off and the button is pressed for < BUTTONMINPRESS ... power on to 1st level
    if(state=0 && hb.button_pressed_time()<BUTTONMINPRESS) {
      brightness_level = (brightness_level+1)%5;
      hb.set_light(CURRENT_LEVEL, levels[brightness_level], 125);
    
   // else if being pressed for > DAZZLETIME... go to dazzle
     
    
   // if in dazzle and button is pressed < BUTTONMINPRESS... turn off
   
     // else if in dazzle and button held for > BUTTONMAXPRESS then go to 1st level
   
   // if we are on any powerlevel and a button is pressed for > BUTTONMINPRESS and < BUTTONMAXPRESS... turn off
   
     // else go to next level
     
   // if we are in any power level and button pressed > BUTTONMAXPRESS... go to dazzle
  
    }
  }
  // Print power status on green LED
  print_power();
}

