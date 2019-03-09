/*
 * global.h
 *
 *  Created on: Mar 3, 2019
 *      Author: pablo
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>
#include <stdlib.h>

/* Global Variables */
volatile extern float roomBrightness;

/* Macros and Constants */
#define TIME_LIGHTS_ON       ( 20 )          /* Time in seconds for lights to stay on */
#define TIME_ALARM_ON        ( 10 )          /* Time in seconds for alarm to stay on */
#define BUZZER_FREQUENCY     ( 4200 )        /* Buzzer frequency in Hertz */
#define BUZZER_DUTY_CYCLE    ( 50 )          /* Buzzer duty cycle in % */
#define BRIGHTNESS_SAMPLES   ( 20 )          /* Samples to grab for room brightness averaging */

#define TOTAL_DARK_VOLTAGE  ( 0.50f )        /* Voltage where room is considered to be dark */
#define SEMI_DARK_VOLTAGE   ( 1.20f )        /* Voltage where room is considered to be dark */
#define NO_DARK_VOLTAGE     ( 2.20f )


#define CORE_FREQUENCY      ( 16000000U )    /* Clock Rate */
#define ADC_DVCC            ( 3.30f )        /* 3.3V External Reference */
#define ADC_BITS            ( 1024.0f )      /* 2^10 */


/* Timers Structure */
struct timers{
    volatile uint16_t  taskManagerTimer;
    volatile uint16_t lowResTimer;
    volatile uint32_t highResTimer;
};

/* Boolean Data Structure */

typedef enum BOOLEAN{
    bFalse = 0,
    bTrue  = 1,
}BOOLEAN;

/* First Instance of Timers Struct */
extern struct timers globalTimers;

/* Pointer to access Timers Struct */
struct timers *getTimersPtr;

/* White LED Macros */
#define WHITE_LEDS_ON()             P2OUT |=  BIT7;
#define WHITE_LEDS_OFF()            P2OUT &= ~BIT7;
#define WHITE_LEDS_TOGGLE()         P2OUT ^=  BIT7;

/* Red LED Macros */
#define RED_LEDS_ON()               P2OUT |=  BIT1;
#define RED_LEDS_OFF()              P2OUT &= ~BIT1;
#define RED_LEDS_TOGGLE()           P2OUT ^=  BIT1;

/* Get Motion Sensor Status */
#define GET_PIR_MOTION_STATUS()     P1IN &= BIT1;

/* Piezo Macro */
#define PIEZO_BUZZER_ON()           timerSet( BUZZER_FREQUENCY, BUZZER_DUTY_CYCLE );
#define PIEZO_BUZZER_OFF()          timerSet( BUZZER_FREQUENCY, 0 );
#define GET_ADC_VALUES()            ADCCTL0 |= ADCENC | ADCSC;

/* Global Function Prototypes */
void initPorts( void );
void initUCS( void );
void timerSet( uint16_t, uint16_t );

#endif /* GLOBAL_H_ */
