/*
 * global.c
 *
 *  Created on: Mar 3, 2019
 *      Author: pablo
 */

#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <global.h>

/* Initializer Timers */
struct timers globalTimers = { 0, 0, 0};
volatile extern float roomBrightness = 0;

/* Initialize Universal Clock System (UCS) */
void initUCS( void ){

    WDTCTL = WDT_ADLY_16;                              // Watchdog timer set to 16 milliseconds

    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    __bis_SR_register(SCG0);                           // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                         // Set REFO as FLL reference source
    CSCTL0 = 0;                                        // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                            // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;                               // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;                             // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                           // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));         // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;         // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                                       // default DCOCLKDIV as MCLK and SMCLK source
    PM5CTL0 &= ~LOCKLPM5;                              // Disable the GPIO power-on default high-impedance mode
                                                       // to activate previously configured port settings

    SFRIE1 |= WDTIE;                                   // Enable WDT interrupt
}

/* Initialize MSP430FR2111 Ports used in NightCrawler */
void initPorts( void ){

    P2DIR |= BIT7;             /* Pin 2.7 "White LEDs" Output */
    P2DIR |= BIT1;             /* Pin 2.7 "Red LEDs" Output */
    P2DIR |= BIT0;             /* Pin 2.0 Piezo Buzzer Output */

    /* Set Up Interrupts/Pull Up on Motion Sensor (P1.1) */
    P1DIR &=  ~BIT1;
    P1OUT |=   BIT1;
    P1REN |=   BIT1;
    P1IES &=  ~BIT1;
    P1IE  &=  ~BIT1;
    P1IFG &=  ~BIT1;

    /* Set Up Interrupts/Pull Up on Off PB (P2.6) */
    P2DIR &=  ~BIT6;
    P2OUT |=   BIT6;
    P2REN |=   BIT6;
    P2IES &=  ~BIT6;
    P2IE  &=  ~BIT6;
    P2IFG &=  ~BIT6;

    /* Route PWM to Piezo ON P1.7 */
    P1DIR |= BIT7;
    P1SEL1 |= BIT7;
}


