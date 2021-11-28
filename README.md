# ICL_switch

***2 channel mains soft start with relay, Arduino controlled, with button + infra-red switching, and LED status indicators***

![build](https://user-images.githubusercontent.com/6553778/143658608-37163e32-2962-423f-8783-c5e60a1794e1.jpg)

## Firmware

The board/firmware can be configured in several ways:

### ICL to relay on delay of both channels

Jumpers on the header labelled `DEL 1:0` is used to adjust this delay without firmware changes, according to:

|DEL 1|DEL 0|delay units|
|:---:|:---:|:---:|
|open|open|1|
|open|short|2|
|short|open|3|
|short|short|4|

and the delay unit (in milliseconds) is set by the line:

```C
#define DEL_UNIT   250    // ms
```

### switch type

A jumper on the header labelled `MOM` is used select a momentary switch (jumper shorted) or a latching switch (jumper open).

### simulataneous or sequenced switching of the 2 channels

The channel-channel delay (in milliseconds) is set by the line:

```C
#define A2B_DELAY  0
```

A zero value will give simultaneous channel triggering; a non-zero value will determine the delay from the END of switching channel A to the BEGINNING of switching channel B.

### infra-red control

It is optional to use an IR receiver as an additional source of switching; the switch type must be set to momentary (MOM jumper shorted) to allow this to be used.

The relevant IR codes can be obtained via a search for your particular remote, or by using an arduino with an IR receiver and using the `IRreceiveDemo` sketch in the examples for the `IRremote` library.

The lines:

```C
if (IrReceiver.decodedIRData.decodedRawData == IR_ON) {
```

```C
if ((IrReceiver.decodedIRData.decodedRawData == IR_OFF1) || (IrReceiver.decodedIRData.decodedRawData == IR_OFF2)) {
```

give examples of how to use single or multiple codes for either the on or off action, and can be configured according to your needs, along with the definitions of the `IR_ONx` and `IR_OFFx` values.

### LEDs

These are controlled independantly of the TRIACs and relays, and their intensity can be set via the values of `INT_STBY` and `INT_ON`.

The default behaviour is:

|state|LED 1|LED 2|
|---|---|---|
|OFF|INT_STBY|OFF|
|ICL active|ON|OFF|
|relay active|OFF|INT_ON|

They can be reconfigured as desired within the function `onoff`.

## Parts and construction

**Microcontroller**: This uses an Arduino pro mini (5v); the firmware can be programmed via the Arduino IDE, using either a USB-to-serial converter, or an ICSP programmer, e.g. another Arduino running the `ArduinoISP` sketch in `Examples/01.Basics`.

**NTC**: The footprint will accept an Ametherm SL22 or MS32 NTC, chosen according to the desired rating.

**Heatsink**: The footprint is for an Aavid TV5G heatsink; any similar sized heatsink will do.

**IR receiver**: The board has been tested using a VS1838B IR receiver.

**R1/2/3/4**: Populate R1/2 for on-board LEDs; R3/4 for external LEDs via either the header pins or screw terminals. Choose value to set the maximum brightness desired.

**Q1/R13**: If using N-FETs as drawn, R13 can be replaced with a link; there is the option to use NPN bipolars, in which case a base resistor of ~10k should be adequate.

**R6/7**: These are 'safety' pull-downs for the TRIACs and relay drivers. I have found that they are not necessary for the TRIACs, but useful for the relay drivers if using FETs for Q1 (To help guard against ESD dischange, fit resistor before FET).

**PCB**: A 99x99mm PCB design is included here, with mounting holes on a 90x90mm spacing.
