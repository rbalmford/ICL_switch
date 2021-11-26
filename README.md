# ICL_switch

2 channel mains soft start with relay, Arduino controlled, with button + infra-red control

![build](https://user-images.githubusercontent.com/6553778/143658608-37163e32-2962-423f-8783-c5e60a1794e1.jpg)

## Firmware

The firmware can be configured in several ways:

### simulataneous or sequenced switching of the 2 channels

The channel-channel delay (in milliseconds) is set by the line:
```C
#define A2B_DELAY  0
```
A zero value will give simultaneous channel triggering; a non-zero value will the determine the delay from the END of switching channel A to the BEGINNING of switching channel B.

### ICL to relay on delay of both channels

External jumpers on the header labelled 'DEL 1:0' can be used to adjust this delay without firmware changes, where:
DEL 1|DEL 0|delay units
---|---|---
open|open|1
open|short|2
short|open|3
short|short|4

and the delay unit (in milliseconds) is set by the line:
```C
#define DEL_UNIT   250    // ms
```

### infra-red

It is optional to use an IR receiver as an additional source of switching; to use this you must determine the relevant codes. This can be done via looking up the codes for your particular remote, or by using an arduino with an IR receiver and using the 'IRreceiveDemo' sketch in the examples for the IRremote library.

The lines:
```C
if (IRresults.value == IR_ON) {
```
and
```C
if ((IRresults.value == IR_OFF1) || (IRresults.value == IR_OFF2)) {
```
give examples of how to use single or multiple codes for either the on or off action, and can be configured according to your needs.

Note: The firmware uses version 2.8.0 of the IRremote library; versions 3.x onwards cannot be used as the decode functions were changed.
