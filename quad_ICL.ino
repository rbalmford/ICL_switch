//////////////////////////////////////
////  dual ICL switch + IR  v1.1  ////
////  (c) 2021  R. Balmford       ////
//////////////////////////////////////

#define A2B_DELAY  0
#define DEL_UNIT   250    // ms
#define INT_STBY   16
#define INT_ON     255

#define IR_ON    0x7C03857A
#define IR_OFF1  0x7F00857A
#define IR_OFF2  0xD52A817E

#define IR_RECEIVE_PIN  8
#define PIN_SW          7
#define PIN_DEL1        2
#define PIN_DEL0        3
#define PIN_MOM         4
#define PIN_ICL_A       A3
#define PIN_RLY_A       11
#define PIN_ICL_B       A2
#define PIN_RLY_B       10
#define PIN_LED1        6
#define PIN_LED2        5

#define DEBOUNCE_DELAY  50    // ms

#include <IRremote.h>

////////////////////////////////////////

void onoff(byte stateON) {
    if (stateON) {
        uint16_t delay_units = !digitalRead(PIN_DEL1)*2 + !digitalRead(PIN_DEL0) + 1;
        if (A2B_DELAY == 0) {
            digitalWrite(PIN_ICL_A, HIGH);
            digitalWrite(PIN_ICL_B, HIGH);
            digitalWrite(PIN_LED1, HIGH);
            delay(delay_units*DEL_UNIT);
            digitalWrite(PIN_RLY_A, HIGH);
            digitalWrite(PIN_RLY_B, HIGH);
            analogWrite(PIN_LED2, INT_ON);
            digitalWrite(PIN_LED1, LOW);
        }
        else {
            digitalWrite(PIN_ICL_A, HIGH);
            delay(delay_units*DEL_UNIT);
            digitalWrite(PIN_RLY_A, HIGH);
            delay(A2B_DELAY);
            digitalWrite(PIN_ICL_B, HIGH);
            delay(delay_units*DEL_UNIT);
            digitalWrite(PIN_RLY_B, HIGH);
        }
    }
    else {
        if (A2B_DELAY == 0) {
            digitalWrite(PIN_RLY_A, LOW);
            digitalWrite(PIN_RLY_B, LOW);
            digitalWrite(PIN_LED2, LOW);
            delay(100);
            digitalWrite(PIN_ICL_A, LOW);
            digitalWrite(PIN_ICL_B, LOW);
            analogWrite(PIN_LED1, INT_STBY);
        }
        else {
            digitalWrite(PIN_RLY_B, LOW);
            delay(100);
            digitalWrite(PIN_ICL_B, LOW);
            delay(A2B_DELAY);
            digitalWrite(PIN_RLY_A, LOW);
            delay(100);
            digitalWrite(PIN_ICL_A, LOW);
        }
    }
}

////////////////////////////////////////

void setup() {

    digitalWrite(PIN_ICL_A, LOW);
    digitalWrite(PIN_ICL_B, LOW);
    digitalWrite(PIN_RLY_A, LOW);
    digitalWrite(PIN_RLY_B, LOW);
    pinMode(PIN_ICL_A, OUTPUT);
    pinMode(PIN_ICL_B, OUTPUT);
    pinMode(PIN_RLY_A, OUTPUT);
    pinMode(PIN_RLY_B, OUTPUT);

    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    analogWrite(PIN_LED1, INT_STBY);
    digitalWrite(PIN_LED2, LOW);

    pinMode(PIN_SW, INPUT_PULLUP);
    pinMode(PIN_DEL1, INPUT_PULLUP);
    pinMode(PIN_DEL0, INPUT_PULLUP);
    pinMode(PIN_MOM, INPUT_PULLUP);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

}    // setup

////////////////////////////////////////

void loop() {

    static byte stateON  = false;
    byte        stateMOM = !digitalRead(PIN_MOM);

    unsigned long        currentTime = millis();
    static unsigned long SWstartTime = currentTime;

    byte        SWstate     = !digitalRead(PIN_SW);
    static byte lastSWstate = SWstate;
    static byte SWactivity  = false;
    if (SWstate != lastSWstate) {
        SWactivity = true;
        lastSWstate = SWstate;
        SWstartTime = currentTime;
    }
    if (SWactivity) {
        if (currentTime - SWstartTime > DEBOUNCE_DELAY) {
            SWactivity = false;
            if (stateMOM) {                // momentary switch
                if (SWstate) {             // if SW has been pressed
                    stateON = !stateON;    // toggle stateON
                    onoff(stateON);
                }
            }
            else {                         // not momentary switch
                onoff(SWstate);
            }
        }
    }

    if (IrReceiver.decode()) {
        if (stateMOM) {                // IR only active if switch type is momentary
            if (IrReceiver.decodedIRData.decodedRawData == IR_ON) {
                if (!stateON) {
                    stateON = true;
                    onoff(stateON);
                }
            }
            if ((IrReceiver.decodedIRData.decodedRawData == IR_OFF1) || (IrReceiver.decodedIRData.decodedRawData == IR_OFF2)) {
                if (stateON) {
                    stateON = false;
                    onoff(stateON);
                }
            }
        }
        IrReceiver.resume();    // Receive the next value
    }

    delay(10);

}    // loop
