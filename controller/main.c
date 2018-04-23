/*
 * main.c
 * MILTON Controller
 *
 */

#include "msp.h"
#include "adc.h"
#include "Bluetooth.h"
#include "configure.h"
#include "circbuf.h"

// Mode
volatile uint8_t MODE = 0;

// JoyStick Position stuff
volatile uint16_t X = 0;
volatile uint16_t Y = 0;

// Buffers for storing Bluetooth transmissions
volatile CircBuf_t * RXBuf;
volatile CircBuf_t * TXBuf;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	//configure_clocks();
	configureJoystick();
	configureBluetooth();
	configureButtons();

	RXBuf = createCircBuf(10);
	TXBuf = createCircBuf(10);

	P1->SEL0 = 0;
	P1->SEL1 = 0;
	P1->DIR |= BIT0;
	P1->OUT &= ~BIT0;

    P2->SEL0 = 0;
    P2->SEL0 = 0;
    P2->DIR |= (BIT0 | BIT1 | BIT2);
    P2->OUT &= ~(BIT0 | BIT1 | BIT2);

	while(1) {
	    if(X > 7700 || X < 7300) {
	        P2->OUT |= BIT0;
	    }
        else {
            P2->OUT &= ~BIT0;
        }
	    if(Y < 7900 || Y > 8250) {
	        P2->OUT |= BIT1;
	    }
	    else {
	        P2->OUT &= ~BIT1;
	    }

	    // msg Example:
	    //      <0%100:1%050>
	    if(MODE == 0) { // Manual Mode
	        volatile uint8_t xPercent;
            volatile uint8_t yPercent;
            volatile uint8_t xDir; // 1 = +; 0 = -
            volatile uint8_t yDir; // 1 = +; 0 = -
	        P2->OUT &= ~BIT2;
	        if(X > 7700 && Y > 8200) { // forward-right
	            xPercent = (X-7700)/7000;
	            yPercent = (16400-Y)/ 8200;
	            xDir = 1;
	            yDir = 1;
	        }
	        else if(X < 7300 && Y > 8250) { // forward-left
	            xPercent = 100 + ((X-7300)/6000); // 1300 < X < 7300
                yPercent = (16400-Y) / 8200;
                xDir = 0;
                yDir = 1;
	        }
	        else if(X < 7300 && Y < 7900) { // backwards-left
                xPercent = 100 + ((X-7300)/6000); // 1300 < X < 7300
                yPercent = Y / 7900;
                xDir = 0;
                yDir = 0;
	        }
	        else if(X > 7700 && Y < 7900) { // backwards-right
                xPercent = ((X-7700)/7000);
	            //xPercent = (14700-X)/6500;
                yPercent = Y / 7900;
                xDir = 1;
                yDir = 0;
	        }
	        else if(X > 7700 && (Y > 7900 && Y < 8250)) { // turn right
                xPercent = ((X-7700)/7000);
	            //xPercent = (14700-X) / 6500;
	            yPercent = 0;
	            xDir = 1;
	            yDir = 0;
	        }
            else if(X < 7500 && (Y > 7900 && Y < 8250)) { // turn left
                xPercent = 100 + ((X-7300)/6000); // 1300 < X < 7300
                yPercent = 0;
                xDir = 0;
                yDir = 0;
            }
            else if((X > 7300 && X < 7700) && Y > 8250) { // go forward
                xPercent = 0;
                yPercent = (16400-Y) / 8200;
                xDir = 0;
                yDir = 1;
            }
            else if((X > 7300 && X < 7700) && Y < 7900) { // go backward
                xPercent = 0;
                yPercent = Y / 7900;
                xDir = 0;
                yDir = 0;
            }
	        else {  // stay still
	            //both transmit 0's
	            xPercent = 0;
	            yPercent = 0;
	            xDir = 0;
	            yDir = 0;
	        }
            sendCommand(xPercent, yPercent, xDir, yDir);
	        P2->OUT = BIT1;

	    }
	    if(MODE == 1) { // Clapper Mode
	        P2->OUT = BIT2;
	    }

	}
}
