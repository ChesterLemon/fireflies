# fireflies
Firefly exhibit for the Fleet Science Center
This project simulates synchronous firefly (Photinus Carolinus) behavioral patterns, 
using 16 5mm yellow LEDs.  The LEDs are intended to be hung amongst faux-trees to simulate 
a natural environment.  

The "fireflies" initiate a series of blinks in semi-unison, before 
entering an extended "dark phase".  This dark phase is used to elicit a call-and-response
interaction from the female fireflies.  The female firefly is represented by a single LED, 
chosen randomly at the start of each dark phase, which continues blinking for the duration. 
The blink patterns are procedurally generated using a FSM so no two patterns are the same.
