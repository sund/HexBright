/* 
 My version of HexBrightFLEX based on hexbright_bjh
 v1.3.2 (change the 'Powered Up! text below to match)
 __________________________________________________
 
 See sund/HexBright/HexBright_Mine/SJSHexBrightFLEX/
 README.md
 __________________________________________________
 TO DO:
 Short term
 .Flash red LED when within 10 or so of OVERTEMP and
 flashes 2x within 5
 
 Long Term
 .Calc temp from wire
 .change dazzle mode to pressing button and shaking light.
 and enter SOS after holding for 5 sec
 .beacon mode and 'stabbing motion' down
 .momentary press - enter with ?
 .static mode - press while pointing down and set on surface and stay on for 5 more so minutes
 __________________________________________________
 See the sund/HexBright/README.md for info on sources.
 
 */

#include <Wire.h>
#include <hexbright.h>
//needed with hexbright.h
hexbright hb;

// Settings
#define OVERTEMP                300

// Pin assignments not declared in hexbright.h
#define DPIN_RLED_SW            2
#define DPIN_PWR                8
#define DPIN_DRV_MODE           9
#define DPIN_DRV_EN             10

// Modes
#define MODE_OFF                0
#define MODE_LOW                1
#define MODE_MED                2
#define MODE_HIGH               3
#define MODE_DAZZLE             4
#define MODE_DAZZLE_PREVIEW     5
#define MODE_SOS                6
#define MODE_SOS_PREVIEW        7

// Variables
const unsigned int SerialPrintIntrvl = 1000;
const unsigned long ActTimeOut = 600;

// State; initialize
byte mode = 0;
unsigned long btnTime = 0;
boolean btnDown = false;
boolean dazzle_on = true;
long dazzle_period = 100;

void setup()
{
  //needed with hexbright.h
  hb.init_hardware();
  // We just powered on!  That means either we got plugged 
  // into USB, or the user is pressing the power button.

  // Initialize serial busses
  Serial.begin(9600);
  Wire.begin();

  // Configure accelerometer
  btnTime = millis();
  btnDown = hb.button_pressed();
  mode = MODE_OFF;

  Serial.println("Powered up! v1.3.2");
  randomSeed(analogRead(1));
}

void loop()
{
  //needed with hexbright.h
  hb.update();

  static unsigned long lastDazzleTime, lastTempTime, lastChrgTime, lastModeTime, lastAccTime, lastModeSOSTime;
  static unsigned long ditdah;
  static int ledState = LOW;

  unsigned long time = millis(); // time on in millsecs

    // Check the state of the charge controller
  int chargeState = hb.get_charge_state();

  // Print the charge every so often
  if (time-lastChrgTime > SerialPrintIntrvl)
  {
    lastChrgTime = time;
    if (chargeState = 3)
      Serial.println("Charged!");
    else
      if (chargeState = 1)
        Serial.println("Charging!");
      else
        Serial.println("On Battery!");
  }

  //  printing charge state
  hb.print_charge(GLED);

  // Check the temperature sensor
  if (time-lastTempTime > SerialPrintIntrvl)
  {
    lastTempTime = time;
    int temperature = hb.get_thermal_sensor();

    if (temperature > OVERTEMP && mode != MODE_OFF)
    {
      Serial.println("** Overheating!");

      for (int i = 0; i < 6; i++)
      {
        hb.set_light(MAX_LOW_LEVEL, MAX_LOW_LEVEL, 100);
        hb.set_light(MAX_LEVEL, MAX_LEVEL, 100);
      }
      hb.set_light(MAX_LOW_LEVEL, MAX_LOW_LEVEL, 0);

      mode = MODE_LOW;
    }
  }

  // Check if the accel has anything to say
  // at a resonable interval
  byte tapped = 0, shaked = 0;
  if (!digitalRead(DPIN_ACC_INT))
  {
    Wire.beginTransmission(ACC_ADDRESS);
    Wire.write(ACC_REG_TILT);
    Wire.endTransmission(false);       // End, but do not stop!
    Wire.requestFrom(ACC_ADDRESS, 1);  // This one stops.
    byte tilt = Wire.read();

    if (time-lastAccTime > 50)
    {
      lastAccTime = time;

      if (hb.tapped()) Serial.println("** Tap!");
      if (hb.shaked()) Serial.println("** Shake!");
    }
  }

  // If we are in the special modes (DAZZLE or SOS)
  // then flip the switch on and off as needed.
  switch (mode)
  {
  case MODE_DAZZLE:
  case MODE_DAZZLE_PREVIEW:
    if (time - lastDazzleTime > dazzle_period)
    {
      digitalWrite(DPIN_DRV_EN, dazzle_on);
      dazzle_on = !dazzle_on;
      lastDazzleTime = time;
      dazzle_period = random(25,100);
    }    
    break;
  case MODE_SOS:
  case MODE_SOS_PREVIEW:
    digitalWrite(DPIN_DRV_EN, morseCodeSOS(time - lastModeSOSTime));
    break;
  }

  // Periodically pull down the button's pin, since
  // in certain hardware revisions it can float.
  // Do we have to do this on the shipping version of hexbright?
  pinMode(DPIN_RLED_SW, OUTPUT);
  pinMode(DPIN_RLED_SW, INPUT);

  // Check for mode changes
  byte newMode = mode;
  byte newBtnDown = hb.button_pressed();
  switch (mode)
  {
  case MODE_OFF:
    if (btnDown && !newBtnDown && (time-btnTime)>20)
      newMode = MODE_LOW;
    if (btnDown && newBtnDown && (time-btnTime)>500)
      newMode = MODE_DAZZLE_PREVIEW;
    break;
  case MODE_LOW:
    if (btnDown && !newBtnDown && (time-btnTime)>50)
      if (time-lastModeTime > 1000)
        newMode = MODE_OFF;
      else newMode = MODE_MED;
    break;
  case MODE_MED:
    if (btnDown && !newBtnDown && (time-btnTime)>50)
      if (time-lastModeTime > 1000)
        newMode = MODE_OFF;
      else newMode = MODE_HIGH;
    break;
  case MODE_HIGH:
    if (btnDown && !newBtnDown && (time-btnTime)>50)
      newMode = MODE_OFF;
    break;
  case MODE_DAZZLE_PREVIEW:
    // This mode exists just to ignore this button release.
    if (btnDown && !newBtnDown)
      newMode = MODE_DAZZLE;
    break;
  case MODE_DAZZLE:
    if (btnDown && !newBtnDown && (time-btnTime)>50)
      newMode = MODE_OFF;
    if (btnDown && newBtnDown && (time-btnTime)>2000) //full 2 second press
      newMode = MODE_SOS_PREVIEW;
    break;
  case MODE_SOS_PREVIEW:
    // This mode exists just to ignore this button release.
    if (btnDown && !newBtnDown)
      newMode = MODE_SOS;
    break;
  case MODE_SOS:    
    if (btnDown && !newBtnDown && (time-btnTime)>50)
      newMode = MODE_OFF;
    break;
  }

  //activity power down EXCLUDES SOS MODE!
  if ((time - max(lastAccTime,lastModeTime) > ActTimeOut ) && newMode != MODE_SOS) {
    newMode = MODE_OFF;
    Serial.println("** Inactivity shutdown!");
  }

  // Do the mode transitions
  if (newMode != mode)
  {
    lastModeTime = millis();

    // Enable or Disable accelerometer
    byte disable[] = {
      ACC_REG_MODE, 0x00        };  // Mode: standby!
    byte enable[] = {
      ACC_REG_MODE, 0x01        };  // Mode: active!
    Wire.beginTransmission(ACC_ADDRESS);
    if (newMode == MODE_OFF) {
      Wire.write(disable, sizeof(disable));
    } 
    else Wire.write(enable, sizeof(enable));
    Wire.endTransmission();

    switch (newMode)
    {
    case MODE_OFF:
      Serial.println("Mode = off");
      hb.shutdown();
      break;
    case MODE_LOW:
      Serial.println("Mode = low");
      hb.set_light(MAX_LOW_LEVEL, 250, 0);
      break;
    case MODE_MED:
      Serial.println("Mode = medium");
      hb.set_light(MAX_LOW_LEVEL, MAX_LOW_LEVEL, 0);
      break;
    case MODE_HIGH:
      Serial.println("Mode = high");
      hb.set_light(MAX_LEVEL, MAX_LEVEL, 0);
      break;
    case MODE_DAZZLE:
    case MODE_DAZZLE_PREVIEW:
      Serial.println("Mode = dazzle");
      pinMode(DPIN_PWR, OUTPUT);
      digitalWrite(DPIN_PWR, HIGH);
      digitalWrite(DPIN_DRV_MODE, HIGH);
      break;
    case MODE_SOS:
    case MODE_SOS_PREVIEW:
      Serial.println("Mode = SOS");
      pinMode(DPIN_PWR, OUTPUT);
      digitalWrite(DPIN_PWR, HIGH);
      digitalWrite(DPIN_DRV_MODE, HIGH);
      lastModeSOSTime = time;
      break;
    }

    mode = newMode;
  }

  // Remember button state so we can detect transitions
  if (newBtnDown != btnDown)
  {
    btnTime = time;
    btnDown = newBtnDown;
    delay(50);
  }
}

bool morseCodeSOS(unsigned long time){
  const unsigned long dit = 180; 
  // 180 ms is the frame for each dit "on", the larger this number the slower the SOS

  // Morse Code:  (thanks jaebird!)
  // S = ...  O = ---
  // SOS word = ...---...

  // word space = 7 dits duration
  // S = 5 dits duration
  // char space = 3 dits duration
  // O = 11 dits duration
  // char space = 3 dits duration
  // S = 5 dits duration
  // total duration = 34

  byte step = (time / dit) % 34; //dit number modulo the length of the sequence;
  // Start with word space
  if (step < 7) return false;
  step -= 7;
  // First S
  if (step < 5) return (step % 2) == 0; // every second dit is off
  step -= 5;
  // Char space
  if (step < 3) return false;
  step -= 3;
  // O
  if (step < 11) return (step % 4) != 3; // every fourth dit is off
  step -= 11;
  // Char space
  if (step < 3) return false;
  step -= 3;
  // Last S
  if (step < 5) return (step % 2) == 0; // every second dit is off
  // Should never get here
  Serial.println("Morse SOS overrun error");  
  return false;
}


