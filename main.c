/*
 * ECEN 2270 - Electronic Design Lab
 * The MILTON Project
 * Jack Danielski
 *
 */

#include <stdint.h>

#include "msp.h"
#include "command.h"
#include "extraCredit.h"

#define TWOFEET 580 // in mm
//#define EXTRACREDIT

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

volatile uint32_t encoderRightCount = 0;
volatile uint32_t encoderLeftCount = 0;
volatile uint8_t onOff = 0;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	configurePins();
    __enable_interrupt();

    uint16_t twoFeet = distanceInEnc(TWOFEET);

    //uint16_t i;
	while(1) {
	    // two feet stuff
//	    if(P3->IN & BIT5) {
//#ifdef EXTRACREDIT
//	        extraCredit();
//#endif
	       delay(1.0); // 1s delay
	       forward(twoFeet, 50); // forward 2ft at 25% speed
	       delay(0.5); // 0.5s delay
	       turnAroundCW();
	       delay(0.5); // 0.5s delay
	       forward(twoFeet, 50);
           delay(0.5); // 0.5s delay
           turnAroundCCW();
	    //}
	}

}
