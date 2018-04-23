/*
 *
 * ECEN 2270
 * command.h
 *  command functions for MILTON
 *
 * Jack Danielski
 *
 */

#include "msp.h"
#include <stdint.h>

#define TWOFEET 580 // in mm

// create a PWM with percent duty cycle
void createPWM(uint8_t percent);

// delay
void delay(float seconds);

// find the amount of pulses needed to move
// a distance in mm
uint32_t distanceInEnc(uint32_t distance);

// move forward a distance at speed "percent" (0-100)
void forward(uint32_t distance, uint8_t percent);

// turns 180 degrees counterclockwise
void turnAroundCCW();

// turns 180 degrees clockwise
void turnAroundCW();

// turns 90 degrees counterclockwise (left)
void turnLeft();

// turns 90 degrees clockwise (right)
void turnRight();

// handles Port 3 interrupts
//void PORT3_IRQHandler();

/*
 * Encoder Left - 3.6 (input)
 * Encoder Right - 3.7 (input)
 * On/Off Switch - 3.5 (input)
 * CW Left Control - 5.0 (output)
 * CCW Left Control - 5.2 (output)
 * REFleft - 3.2 (output)
 * REFright - 3.3 (output)
 * CW Right Control - 5.4 (output)
 * CCW Right Control - 5.5 (output)
 */
// configures the pins needed for I/O with MILTON
void configurePins();


