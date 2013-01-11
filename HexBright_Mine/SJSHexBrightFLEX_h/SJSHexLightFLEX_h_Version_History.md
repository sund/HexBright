A better place for version history rather than bloating the .ino file.

__________________________________________________ 
HexBright Basic Code with the hexbright.h library
- High/Med/Low/Dazzle
  
  I am adding more functions found in the hexbright.h
  to replace the code as possible while keeping the
  same features and functions.
  
  Based on:
  Factory firmware for HexBright FLEX 
  v2.4  Dec 6, 2012
  https://github.com/hexbright/samples
  
  Cherry picked dazzle improvements from 
  https://github.com/digitalmisery/HexBrightFLEX:
  - Changed the way dazzle works and set flicker rate to match
  the known frequencies for vertigo (about 5 to 20Hz):
  http://en.wikipedia.org/wiki/Flicker_vertigo
  Cherry pick SOS from https://github.com/jaebird/samples.git
  * Added SOS to dazzle mode. To get there: press and hold pwr button to get dazzle, then 2 second long press again for SOS
__________________________________________________

__________________________________________________
UPDATE LOG:
1-10-13
Cleaned it up. mainly temp report and chargestate.