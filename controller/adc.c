/*
 *
 * ECEN 2270
 * adc.h
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

#include "msp.h"
#include "adc.h"
#include <stdint.h>

extern uint16_t X;
extern uint16_t Y;

void configureJoystick() {
    // Initialize the shared reference module
    // By default, REFMSTR=1 = REFCTL is used to configure the internal reference
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);        //If ref generator busy, WAIT
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;//Enable internal 1.2V ref

    // Configure ADC - Pulse sample mode; ADC14SC trigger
    // ADC ON, temperature sample period > 30us
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // allow to make changes
    //Add sequence of channels
    ADC14->CTL0 |= ADC14_CTL0_SHT0_5 | ADC14_CTL0_SHT1_5 | ADC14_CTL0_MSC | ADC14_CTL0_ON | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_3;
    ADC14->CTL1 = (ADC14_CTL1_RES_3);

    //Joystick Configuration
    P4->SEL0 |=  BIT4;
    P4->SEL1 |=  BIT4;
    P6->SEL0 |=  BIT0;
    P6->SEL1 |=  BIT0;
    ADC14->MCTL[0] |= (ADC14_MCTLN_INCH_15  | ADC14_MCTLN_VRSEL_0); // Y data - A15 (P4.4)
    ADC14->MCTL[1] |= (ADC14_MCTLN_INCH_9   | ADC14_MCTLN_VRSEL_0 | ADC14_MCTLN_EOS);   // X data - A9 (P6.0)
    ADC14->IER0 |= (ADC14_IER0_IE0 | ADC14_IER0_IE1); //Enable MCTL0/MEM0(BIT0) and MCTL1/MEM1(BIT1) Interrupts
    //ADC14->IFGR0 &= (ADC14_IER0_IE0 | ADC14_IER0_IE1);

    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY)); // Wait for ref generator to settle
    ADC14->CTL0 |= (ADC14_CTL0_ENC); // Enable Conversions
    ADC14->CTL0 |= ADC14_CTL0_SC;
    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC interrupt in NVIC module

}

// X and Y are flipped but whatever
void ADC14_IRQHandler() {
    if(ADC14_IFGR0_IFG0) {
        X = ADC14->MEM[0];
    }
    if(ADC14_IFGR0_IFG1) {
        Y = ADC14->MEM[1];
    }

}

void configure_clocks(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.

}
