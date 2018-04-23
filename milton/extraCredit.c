/*
 *
 * ECEN 2270
 * extraCredit.c
 *  extra credit command functions for MILTON
 *
 * Jack Danielski
 *
 */

#include "msp.h"
#include "command.h"
#include "extraCredit.h"
#include <stdint.h>

void extraCredit() {
    delay(1.0);
    forward(1800, 70); // move 1m 'north'
    delay(0.5);
    turnLeft();
    delay(0.5);
    forward(3700, 70); //move 2m 'west'
    delay(0.5);
    turnRight();
    delay(0.5);
    forward(1800, 70); // move 1m 'north'
    delay(0.5);
    turnRight();
    delay(0.5);
    forward(3700, 70); // move 1m 'north'
    delay(0.5);
    turnRight();
    delay(0.5);
    forward(3700, 70); //move 2m 'west'
    delay(0.5);
    turnLeft();
    delay(0.5);
    forward(1800, 70); // move 1m 'north'
    delay(0.5);
    turnLeft();
}
