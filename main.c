/*
    Copyright © Pablo J. Gonzalez and individual contributors. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
        Neither the name of Django nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <msp430fr2111.h>
#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <global.h>
#include <crawlerTasks.h>

void main(void) {

  initStateMachine();
  initUCS();
  initPorts();                                      /* Initialize MSP430FR2111 Ports */
  timerSet( BUZZER_FREQUENCY, BUZZER_DUTY_CYCLE );

  // Configure ADC - Pulse sample mode; ADCSC trigger
  ADCCTL0 |= ADCSHT_8 | ADCON;                                  // ADC ON,temperature sample period>30us
  ADCCTL1 |= ADCSHP;                                            // s/w trig, single ch/conv, MODOSC
  ADCCTL2 |= ADCRES;                                            // 12-bit conversion results
  ADCMCTL0 |= ADCSREF_0 | ADCINCH_4;                            // ADC input ch A12 => temp sense
  ADCIE |=ADCIE0;                                               // Enable the Interrupt request for a completed ADC_B conversion

  /* Force timers to tick to zero */
  getTimersPtr -> taskManagerTimer = 0;
  getTimersPtr -> lowResTimer = 0;
  getTimersPtr -> highResTimer = 0;

  /* Turn LEDs and Buzzer Off */
  WHITE_LEDS_OFF();
  RED_LEDS_OFF();
  PIEZO_BUZZER_OFF();


  /* Infinite while loop */
  while (1) {
      __bis_SR_register( GIE );                     /* Interrupts Enabled */
      __no_operation();                             /* For debugger to work */
  }
}

void timerSet( uint16_t frequency, uint16_t dutyCycle ) {

  /* Set up PWM for Piezo */
  TB0CCR0 = CORE_FREQUENCY / frequency ;   // PWM Period
  TB0CCTL2 = OUTMOD_7;                     // CCR2 reset/set
  TB0CCR2 = dutyCycle;                     // CCR2 PWM duty cycle
  TB0CTL = TBSSEL_2 | MC_1 | TBCLR;        // ACLK, up mode, clear TBR
}

/* Timer Interrupt */
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B (void) {

}

/* Watchdog Timer Interrupt (16ms tick rate) */
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void){

    static uint8_t counter = 0;
    counter++;

    /* Heart of Task Manager */
    if (  counter >= ( TASK_PERIOD_GCD / TIMER_TICK_RATE ) ) {
      unsigned char i;
      for  (i = 0; i < tasksNum; ++i ) {
        if ( tasks[i].elapsedTime >= tasks[i].period )
        {
          tasks[i].state = tasks[i].TickFct(tasks[i].state);
          tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += tasksPeriodGCD;
        counter = 0;
      }
    }
}

/* ADC Interrupt */
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            roomBrightness =  ADCMEM0;
            roomBrightness = ( roomBrightness / ADC_BITS ) * ADC_DVCC
            break;
        default:
            break;
    }
}








