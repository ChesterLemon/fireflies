/*
Firefly Light Controller Version 10.0
by Justin Slater for the Fleet Science Center, October 2023
============================================================
This project simulates synchronous firefly (Photinus Carolinus) behavioral patterns, 
using 16 5mm yellow LEDs.  The LEDs are intended to be hung amongst faux-trees to simulate 
a natural environment.  The "fireflies" initiate a series of blinks in semi-unison, before 
entering an extended "dark phase".  This dark phase is used to elicit a call-and-response
interaction from the female fireflies.  The female firefly is represented by a single LED, 
chosen randomly at the start of each dark phase, which continues blinking for the duration. 
The blink patterns are procedurally generated using a FSM so no two patterns are the same.
*/

int LED[] = {53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23}; //pin values for each LED
unsigned long prevBlink[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //track time of most recent state change
int blinkDelay[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //initial delay values
byte state[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW}; //set initial pin states to off (low)
int delayTime = 0; //amount of time to delay for each synchronized dark period
unsigned long stopTime = 0; //used in DARK state to delay without pausing execution
int blinkCount = 0; //track number of blinks for synchronized dark period
int starterLEDs = 2; //use for slower cycle start
int endLEDs[8] = {3, 9, 5, 13, 17, 17, 17, 17}; //use for slower cycle "fizzle-out"; 17's act as placeholders
int endLength = 4; //number of LEDs to continue flashing during "fizzle-out"
int imposter = 15; //inital LED for imposter
unsigned long timeNow = millis(); //track amount of time passed since start of current state

#define BLINK 1
#define FIZZLE 2
#define DARK 3
#define START 4

byte programState = START;

void setup() { //initialize all LED pins
  for (int i = 0; i < 16; i++) {
    pinMode(LED[i], OUTPUT);
  }
}

bool checkStates() { //check if states of all LEDs are LOW or not
  for (int i = 0; i < 16; i++){
    if (state[i] != LOW && i != imposter) { //don't check imposter LED
      return false;
    }
    else {
      return true;
    }
  }
}

bool checkEndArray(int x) { //check if x is in endLEDs[]
  for (int i = 0; i < 8; i++) {
    if (x == endLEDs[i]) {
      return true;
    }
  }
  return false;
}

void loop() {
  switch(programState) {
    case BLINK:
      timeNow = millis();
      if (timeNow - prevBlink[imposter] >= blinkDelay[imposter]) { //constantly blink imposter
        prevBlink[imposter] += blinkDelay[imposter]; //set new previous toggle time
        if (state[imposter] == LOW) { //change LED state
          state[imposter] = HIGH;
        }
        else {
          state[imposter] = LOW;
        }
        digitalWrite(LED[imposter], state[imposter]); //toggle LED
        blinkDelay[imposter] = random(250, 1000); //set new random blink delay between .25 and 1 seconds
      }
      if (blinkCount <= 240) {
        for (int i = 0; i < 16; i++) {
          //if time passed since last toggle > blink delay, toggle given LED
          if (timeNow - prevBlink[i] >= blinkDelay[i] && i != imposter) {
            prevBlink[i] += blinkDelay[i]; //set new previous toggle time
            if (state[i] == LOW) { //change LED state
              state[i] = HIGH;
            }
            else {
              state[i] = LOW;
            }
            digitalWrite(LED[i], state[i]); //toggle LED
            blinkDelay[i] = random(250, 1000); //set new random blink delay between .25 and 1 second
            blinkCount++;
          }
        }
      }
      else {
        endLength = random(4, 8); //set 4-8 LEDs to continue blinking for "fizzle-out"
        for (int i = 0; i < 8; i++) { //reset endLEDs[] to all placeholder 17's
          endLEDs[i] = 17;
        }
        for (int i = 0; i < endLength; i++) { //assign new endLEDs[] values
          endLEDs[i] = random(0, 15);
        }
        imposter = random(0, 15); //set new imposter LED
        programState = FIZZLE;
      }
      break;

    case FIZZLE:
      timeNow = millis(); //get current time
      if (timeNow - prevBlink[imposter] >= blinkDelay[imposter]) { //constantly blink imposter
        prevBlink[imposter] += blinkDelay[imposter]; //set new previous toggle time
        if (state[imposter] == LOW) { //change LED state
          state[imposter] = HIGH;
        }
        else {
          state[imposter] = LOW;
        }
        digitalWrite(LED[imposter], state[imposter]); //toggle LED
        blinkDelay[imposter] = random(250, 1000); //set new random blink delay between .25 and 1 seconds
      }
      for (int i = 0; i < 16; i++) {
        //turn off each LED after blink delay completes
        if (timeNow - prevBlink[i] >= blinkDelay[i] && i != imposter) {
          if (state[i] == HIGH) {
            state[i] = LOW;
            digitalWrite(LED[i], LOW); //turn off LED
          }
          else if (state[i] == LOW && checkEndArray(i) == true) { //if state == LOW and i is in endLEDs[]
            state[i] = HIGH;
            digitalWrite(LED[i], HIGH);
            blinkCount++;
          }
          prevBlink[i] += blinkDelay[i]; //set new previous toggle time
          blinkDelay[i] = random(250, 1000); //set new random blink delay between .25 and 1 second
        }
      }
      if (blinkCount > 250) {
        delayTime = random(4000, 6000);
        stopTime = millis();
        programState = DARK;
      }
      break;

    case DARK:
      timeNow = millis(); //get current time
      if (timeNow - prevBlink[imposter] >= blinkDelay[imposter]) { //constantly blink imposter LED
        prevBlink[imposter] += blinkDelay[imposter]; //set new previous toggle time
        if (state[imposter] == LOW) { //change LED state
          state[imposter] = HIGH;
        }
        else {
          state[imposter] = LOW;
        }
        digitalWrite(LED[imposter], state[imposter]); //toggle LED
        blinkDelay[imposter] = random(250, 1000); //set new random blink delay between .25 and 1 seconds
      }
      for (int i = 0; i < 16; i++) {
        //turn off each LED after blink delay completes
        if (timeNow - prevBlink[i] >= blinkDelay[i] && state[i] == HIGH && i != imposter) { 
          state[i] = LOW;
          digitalWrite(LED[i], LOW); //turn off LED
        }
      }
      if (checkStates() == true) { //check if all LEDs are off
        if (timeNow - delayTime < stopTime) { //delay without pausing execution
          timeNow = millis();
        }
        else {
          blinkCount = 0; //reset blinkCount before switching to SYNC_BLINK state
          starterLEDs = random(1,3); //set 1-3 LEDs to initially blink before the rest
          for (int i = 0; i < 16; i++) {
            prevBlink[i] = timeNow;
            blinkDelay[i] = random(2000, 4000); //set longer initial delay for non-starter LEDs
          }
          for (int i = 0; i < starterLEDs + 1; i++) {
            blinkDelay[random(0, 15)] = random(250, 750); //set shorter initial delay for 1-3 LEDs for gradual start of cycle
          }
          blinkDelay[imposter] = random(250, 1000); //make sure imposter keeps blinking during start-up
          programState = BLINK;
        }
      }
      break;

    case START: //initial state when controller receives power
      for (int i = 0; i < 16; i++) {
        timeNow = millis(); //get current time
        prevBlink[i] = timeNow; //set initial values for prevBlink
        blinkDelay[i] = random(500, 2000); //set initial values for blinkDelay
      }
      programState = BLINK; //start constant random blink pattern
      break;
  }
}