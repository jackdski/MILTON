/*
 *
 * ECEN 2270
 * controller.h
 *  controller device's functions for MILTON
 *
 * Jack Danielski
 *
 */

/*
 * Mode 0: Clapper Mode
 *  - angles front towards clap sounds
 *  - will approach clap sounds until it hits something
 *
 * Mode 1: "Mario-Cart" Mode
 *  - resets "zero" position with a button press on the MSP
 *  Example Controls
 *  - tilt forward - go forward
 *  - tilt backward - go backward
 *  - tilt forward-right -> go forward right
 *
 *  BUTTON A -> Shoot toast
 *  BUTTON B -> Change Mode
 *  INDICATE MODE WITH LEDS ON BOARD
 *
 */


// CLAPPER FUNCTIONS

/*
 *  -use interrupts for mic inputs and for
 *      bumper inputs
 *  -ultrasonic input followed unless mic and ultrasonic
 *      in same direction are flagged
 */

void changeAngle(uint8_t angle) {

}

void clapperMode() {

}

// TILT CONTROL FUNCTIONS

