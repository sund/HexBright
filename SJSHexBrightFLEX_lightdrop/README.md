SJSHexBrightFLEX lightdrop

_This is the same as SJSHexBrightFLEX, but with one 'secret' feature. In the Serial Monitor enter a password and hit enter while activating dazzle mode and it will output a secret message to the Serial Monitor._

--- 
HexBright Basic Code - High/Med/Low/Dazzle/SOS
  
Based on:
Factory firmware for HexBright FLEX v2.4  Dec 6, 2012
https://github.com/hexbright/samples
  
Cherry picked dazzle improvements from https://github.com/digitalmisery/HexBrightFLEX:
- Changed the way dazzle works and set flicker rate to match the known frequencies for vertigo (about 5 to 20Hz): http://en.wikipedia.org/wiki/Flicker_vertigo
- Cherry pick SOS from https://github.com/jaebird/samples.git.
- Added SOS to dazzle mode. To get there press and hold pwr button to get dazzle, then 2 second long press again for SOS.

---
UPDATE LOG:

1-26-13
- Commented out temp report and Accel actions. At about line 340 is the code to change to print out your secret message. 

1-17-13
- Made each version be more uniform in comments.

1-13-13
- removed the chargeFade and made sure the inactivity timeout doesn't kick in if off. Think either or both of these resulted in some instances where the light won't come if charging for a while.

12-29-12
- some code from sharph (https://github.com/sharph/sharp-hexbright) dealing with fading the green led during charge
Added more code to choose blink or fade

12/24/12
- Added a bit to print the charge state along with Temp.
- and added a constant for printing things to serial.

12/23/12
- added some comments and a TODO: derp moment with bitwise AND and time for green led and flash.

12/21/12
- Updated with fixed SOS code : fixed SOS code & timeout and some readability things
