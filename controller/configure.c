/*
 * configure.c
 * MILTON Controller
 *
 */

#include "msp.h"
#include <stdint.h>

extern uint8_t MODE;

void configureButtons() {
    P5->DIR &= ~BIT1;
    P5->REN |= BIT1;
    P5->IFG &= ~BIT1;
    P5->IES |= BIT1;

    P3->DIR &= ~BIT5;
    P3->REN |= BIT5;
    P3->IFG &= ~BIT5;
    P3->IES |= BIT5;

    P5->IFG =  0;
    P3->IFG =  0;

    P3->IE |= BIT5;
    P5->IE |= BIT1;

    NVIC_EnableIRQ(PORT5_IRQn);
    NVIC_EnableIRQ(PORT3_IRQn);
}

void PORT5_IRQHandler() {
    if(P5->IFG & BIT1) {
        if(MODE == 1) {
            MODE = 0;
        }
        else {
            MODE = 1;
        }
        P5->IFG &= ~BIT1;
    }
}

void PORT3_IRQHandler() {
    if(P3->IFG & BIT5) {
        if(MODE == 1) {
            MODE = 0;
        }
        else {
            MODE = 1;
        }
        P3->IFG &= ~BIT5;
    }
}

