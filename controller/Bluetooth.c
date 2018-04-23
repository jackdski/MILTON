/*
 * ECEN 2270
 * Bluetooth.c
 *  Bluetooth UART communication for MILTON
 *
 * Jack Danielski
 *
 */

#include "msp.h"
#include <stdint.h>
#include "Bluetooth.h"
#include "circbuf.h"

extern CircBuf_t * TXBuf;
extern CircBuf_t * RXBuf;

//Connect 9.7 to the RX
void configureBluetooth(){
    //Configure UART pins, set 2-UART pins to UART mode
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A0->BRW = 78;                         //Set Baud Rate
    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A0->IFG &= ~(BIT1 | BIT0);
    UCA0IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA0_IRQn);
}

// msg Example:
//      <0%100:1%050>
void sendCommand(uint8_t xPer, uint8_t yPer, uint8_t xDir, uint8_t yDir) {
    uint8_t xD = xDir+48;   // convert to ASCII
    uint8_t yD = yDir+48;   // convert to ASCII
    uint8_t xPercent[3];
    uint8_t yPercent[3];

    if(xPer < 100) {
        xPercent[0] = '0';
        xPercent[1] = xPer / 10 + 48;
        xPercent[2] = xPer % 10 + 48;
    }
    else {
        //xPercent = "100";
        xPercent[0] = '1';
        xPercent[1] = '0';
        xPercent[2] = '0';
    }

    if(yPer < 100) {
        yPercent[0] = '0';
        yPercent[1] = yPer / 10 + 48;
        yPercent[2] = yPer % 10 + 48;
    }
    else {
        //yPercent = "100";
        yPercent[0] = '1';
        yPercent[1] = '0';
        yPercent[2] = '0';
    }

    addItemCircBuf(TXBuf, '<');
    addItemCircBuf(TXBuf, xD);
    addItemCircBuf(TXBuf, '%');
//    addItemCircBuf(TXBuf, xPercent);
    loadToBuf(TXBuf, xPercent, 3);
    addItemCircBuf(TXBuf, ':');
    addItemCircBuf(TXBuf, yD);
    addItemCircBuf(TXBuf, '%');
//    addItemCircBuf(TXBuf, yPercent);
    loadToBuf(TXBuf, yPercent, 3);

    EUSCI_A3->IFG |= BIT1;
    while(!isEmpty(TXBuf));
}

void EUSCIA3_IRQHandler(){
    if (EUSCI_A3->IFG & BIT0){
        addItemCircBuf(RXBuf, EUSCI_A3->RXBUF);
    }
    if (EUSCI_A3->IFG & BIT1){
        //Transmit Stuff
        if(isEmpty(TXBuf)) {
            EUSCI_A3->IFG &= ~BIT1;
            return;
        }
        EUSCI_A3->TXBUF = removeItem(TXBuf);
    }
}
