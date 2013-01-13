_A better place for version history rather than bloating the .ino file._

--- 
HexBright Basic Code - High/Med/Low/Dazzle
_with the hexbright.h library_
  
I am adding more functions found in the hexbright.h to replace the code as possible while keeping the same features and functions.

Noticed that the only thing broke is the inactivity timeout.
  
Based on:
Factory firmware for HexBright FLEX v2.4 Dec 6, 2012
https://github.com/hexbright/samples

Cherry picked dazzle improvements from https://github.com/digitalmisery/HexBrightFLEX:
- Changed the way dazzle works and set flicker rate to match the known frequencies for vertigo (about 5 to 20Hz): http://en.wikipedia.org/wiki/Flicker_vertigo
- Cherry pick SOS from https://github.com/jaebird/samples.git.
- Added SOS to dazzle mode. To get there press and hold pwr button to get dazzle, then 2 second long press again for SOS

---
UPDATE LOG:
1-11-13
- More cleaning of redundant code with hexbright.h and some of the light levels

1-10-13
- Cleaned it up. mainly temp report and chargestate.
