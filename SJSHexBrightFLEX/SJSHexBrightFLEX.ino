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
#include <strobe.h>

// These next two lines must come after all other library #includes
#define BUILD_HACK
#include <hexbright.h>

// Settings
#define OVERTEMP 300 // When over this, we turn off or drop light level
#define BUTTONMINPRESS 300 // min time for a button press to matter
#define BUTTONMAXPRESS 650 // max time for a button press to matter
#define DAZZLETIME 1000 // max time for a button press to matter and go to dazzle
#define CYCLETRANSITION 75 // time for light level transistion while in CYCLE_MODE

static int brightness_level = 4;

#define OFF_MODE 0
#define DAZZLE_MODE 1
#define CYCLE_MODE 2

int mode = 0;


hexbright hb;

void setup() {
  hb.init_hardware();
}

void loop() {
  hb.update();

  //// Button actions to recognize, one-time actions to take as a result
  if (hb.button_just_released()) {
    if (hb.button_pressed_time() < BUTTONMINPRESS) {
      mode = CYCLE_MODE;
      int levels[] = {1, 250, 500, 750, 1000};
      brightness_level = (brightness_level + 1) % 5;
      hb.set_light(CURRENT_LEVEL, levels[brightness_level], CYCLETRANSITION);
      // while cycling, if held for less than dazzle time but more than BUTTONMINPRESS
    } else if (hb.button_pressed_time() < DAZZLETIME) {
      mode = DAZZLE_MODE;
    }

  }

  // strobe if off and held for > DAZZLETIME
  if (hb.button_pressed()) {
    if (hb.button_pressed_time() > DAZZLETIME && mode == OFF_MODE) {
      mode = DAZZLE_MODE;
    }
  }

  // turn off
  if (hb.button_pressed_time() > BUTTONMAXPRESS ) {
    mode = OFF_MODE;
    hb.set_light(CURRENT_LEVEL, OFF_LEVEL, NOW);
    // in case we are under usb power, reset state
    brightness_level = 4;
  }

  //// Actions over time for a given mode
  if (mode == DAZZLE_MODE) { // just
  //set_strobe_fpm(1000);
        static int i = 0;
        if (!i) {
          // fade from max to 0 over a random time btwn 30 and 350 milliseconds (length flash "on")
          hb.set_light(MAX_LEVEL, 0, random(30, 250));
          // only light up every random number of times btwn 6 and 60 through
          // which should equate to 50 - 500 ms (length flash "off")
          i = random(50, 500) / 8.3333;
        }
        i--;
  }

  // Print power status on green LED
  print_power();
}


