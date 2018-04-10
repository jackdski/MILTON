/*
 *
 * ECEN 2270
 * command.c
 *  command functions for MILTON
 *
 * Jack Danielski
 *
 */

#include "msp.h"
#include "command.h"
#include <stdint.h>

#define TWOFEET 580 // in mm

extern volatile uint32_t encoderRightCount;
extern volatile uint32_t encoderLeftCount;
extern volatile uint8_t onOff;

void createPWM(uint8_t percent) {
    percent = (percent / 100) * 255;
}

void delay(float seconds) {
    uint16_t i;
    for(i = 0; i < seconds * 12000; i++);
}

uint32_t distanceInEnc(uint32_t distance) {
    return (distance / 0.53); // in mm
}

void forward(uint32_t distance, uint8_t percent) {
    //percent = (percent / 100) * 255;
    uint32_t dist = distanceInEnc(TWOFEET);
    encoderRightCount = 0;
    encoderLeftCount = 0;
    P5->OUT |= (BIT0 & BIT4);
    P5->OUT &= ~(BIT2 & BIT5);
    uint16_t i;
    while(encoderRightCount < 800) {
        P3->OUT |= (BIT2 & BIT3);
        for(i = 0; i < percent*1000; i++); // delay on
        P3->OUT &= ~(BIT2 & BIT3);
        for(i = 0; i < (100-percent) * 1000; i++); // delay off
    }
    P5->OUT &= ~(BIT0 & BIT2 & BIT4 & BIT5);
}

void turnAroundCCW() {
    uint8_t percent = 25; // run at 25%
    encoderRightCount = 0;
    encoderLeftCount = 0;
    P5->OUT |= (BIT0 & BIT5);
    P5->OUT &= ~(BIT2 & BIT4);
    uint16_t i;
    while(encoderRightCount < 800) {
        P3->OUT |= (BIT2 & BIT3);
        for(i = 0; i < percent*1000; i++); // delay on
        P3->OUT &= ~(BIT2 & BIT3);
        for(i = 0; i < (100-percent) * 1000; i++); // delay off
    }
    P5->OUT &= ~(BIT0 & BIT2 & BIT4 & BIT5);
}

void turnAroundCW() {
    uint8_t percent = 25; // run at 25%
    encoderRightCount = 0;
    encoderLeftCount = 0;
    P5->OUT |= (BIT2 & BIT4);
    P5->OUT &= ~(BIT0 & BIT5);
    uint16_t i;
    while(encoderRightCount < 800) {
        P3->OUT |= (BIT2 & BIT3);
        for(i = 0; i < percent*1000; i++); // delay on
        P3->OUT &= ~(BIT2 & BIT3);
        for(i = 0; i < (100-percent) * 1000; i++); // delay off
    }
    P5->OUT &= ~(BIT0 & BIT2 & BIT4 & BIT5);
}

void turnLeft() {
    uint8_t percent = 25; // run at 25%
    encoderRightCount = 0;
    encoderLeftCount = 0;
    P5->OUT |= (BIT0 & BIT5);
    P5->OUT &= ~(BIT2 & BIT4);
    uint16_t i;
    while(encoderRightCount < 400) {
        P3->OUT |= (BIT2 & BIT3);
        for(i = 0; i < percent*1000; i++); // delay on
        P3->OUT &= ~(BIT2 & BIT3);
        for(i = 0; i < (100-percent) * 1000; i++); // delay off
    }
    P5->OUT &= ~(BIT0 & BIT2 & BIT4 & BIT5);
}

void turnRight() {
    uint8_t percent = 25; // run at 25%
    encoderRightCount = 0;
    encoderLeftCount = 0;
    P5->OUT |= (BIT2 & BIT4);
    P5->OUT &= ~(BIT0 & BIT5);
    uint16_t i;
    while(encoderRightCount < 400) {
        P3->OUT |= (BIT2 & BIT3);
        for(i = 0; i < percent*1000; i++); // delay on
        P3->OUT &= ~(BIT2 & BIT3);
        for(i = 0; i < (100-percent) * 1000; i++); // delay off
    }
    P5->OUT &= ~(BIT0 & BIT2 & BIT4 & BIT5);
}

void PORT3_IRQHandler(){
    if(P3->IFG & BIT6) {        // left encoder -> P3.6
        encoderLeftCount++;
    }
    if(P3->IFG & BIT7) {
        encoderRightCount++;    // right encoder -> P3.7
    }
    if(P3->IFG & BIT5) {        // switch -> P3.5
        if(onOff != 0x00) {
            onOff = 0x00;
        }
        if (onOff == 0x00) {
            onOff = 0xFF;
        }
    }
}

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
void configurePins() {
    // encoder inputs (port 3.6 and port 3.7)
    // on/off switch input (port 3.5)
    // PWM outputs (port 3.2 and port 3.3)
    P3->SEL0 &= ~(BIT2 & BIT3 & BIT5 & BIT6 & BIT7);// set to General IO Mode
    P3->SEL1 &= ~(BIT2 & BIT3 & BIT5 & BIT6 & BIT7);// Make sure not to be in tertiary function
    P3->DIR &= ~(BIT5 & BIT6 & BIT7);               // set direction to input
    P3->DIR |= (BIT2 & BIT3);                       // set direction to output
    P3->REN |= (BIT2 & BIT3 & BIT5 & BIT6 & BIT7);  // enable pullup
    P3->OUT |= (BIT2 & BIT3 & BIT5 & BIT6 & BIT7);  // clear interrupts
    P3->IES = (BIT2 & BIT3 & BIT5 & BIT6 & BIT7);   // set IFT flag to high to low transition

    P3->IFG = 0;  // zero all interrupt flags
    P3->IE =  (BIT2 & BIT3 & BIT5 & BIT6 & BIT7);       // Enable port interrupt

    // CW/CCW Control Outputs
    P5->SEL0 &= ~(BIT0 & BIT2 & BIT4 & BIT5);
    P5->SEL1 &= ~(BIT0 & BIT2 & BIT4 & BIT5);
    P5->DIR |= (BIT0 & BIT2 & BIT4 & BIT5); // set to outputs
    P5->REN |= (BIT0 & BIT2 & BIT4 & BIT5);
    P5->OUT |= (BIT0 & BIT2 & BIT4 & BIT5);
    P5->IES = (BIT0 & BIT2 & BIT4 & BIT5);

    P5->IFG = 0;
    P5->IE = (BIT0 & BIT2 & BIT4 & BIT5);

    NVIC_EnableIRQ(PORT3_IRQn);
    NVIC_EnableIRQ(PORT5_IRQn);
}
