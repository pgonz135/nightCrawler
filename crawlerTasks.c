/*
   crawlerTasks.c

    Created on: Mar 3, 2019
        Author: pablo
*/

#include <crawlerTasks.h>
#include <global.h>
#include <stdlib.h>
#include <string.h>

/* State Machine */
const unsigned long tasksPeriodGCD  = TASK_PERIOD_GCD;
const unsigned long mainStatePeriod = MAIN_STATE_PERIOD;
const unsigned char tasksNum = 1;                  // Number of Tasks

enum BL_States {
  S1 = 0,                                           // All states of the SM
  S2,
};

void initStateMachine() {
  /* Definition of Tasks*/
  unsigned char i = 0;                             // Counter for State Machine
  tasks[i].state = -1;                             // Initial state for as SMs
  tasks[i].period = mainStatePeriod;               // Period of this particular SM
  tasks[i].elapsedTime = tasks[i].period;          // Make time elapsed equal period of SM
  tasks[i].TickFct = &StateMachine_MainState;
  ++i;                                             // Move on to next SM
}

int StateMachine_MainState(int state)              // SM function definition
{
  /* State Transitions */
  switch (state)
  {
    case -1:
      state = S1;
      break;

    case S1:
      state = S2;
      break;

    case S2:
      state = S1;
      break;

    default:
      state = -1;
  }

  /* State Actions */
  switch (state) {

    case S1:
      processS1State();
      break;

    case S2:
      processS2State();
      break;

    default:
      break;
  }
  return state;
}

/* Process S1 State (512ms Period) */
void processS1State() {
  static uint8_t motionSensorStatus = 0;
  static BOOLEAN motionStatus = bFalse;
  static BOOLEAN triggeredStatus = bFalse;
  static int lightsTimer = 0;
  static int buzzerTimer = 0;

  GET_ADC_VALUES();                                    /* Get ADC Conversion Values */
  motionSensorStatus = GET_PIR_MOTION_STATUS();        /* Get Status of Motion Sensor */

  if ( motionSensorStatus != 0 ) {
    motionStatus = bTrue;                              /* Motion Detected, Raise Flag */
    PIEZO_BUZZER_ON();                                 /* Start the Buzzer */
  }

  if ( motionStatus == bTrue ) {                       /* Motion Detected, Keep Lights On */

    /* Adjust brightness when motion is detected */
    if ( triggeredStatus == bFalse ) {
      triggeredStatus = bTrue;

      /* Total Darkness */
      if ( roomBrightness <= TOTAL_DARK_VOLTAGE ) {
        WHITE_LEDS_ON();
        RED_LEDS_ON();
      }
      /* Semi-Darkness */
      else if ( roomBrightness > TOTAL_DARK_VOLTAGE && roomBrightness <= SEMI_DARK_VOLTAGE ) {
        WHITE_LEDS_ON();
        RED_LEDS_OFF();
      }
      /* No Darkness */
      else if ( roomBrightness > SEMI_DARK_VOLTAGE ) {
        WHITE_LEDS_OFF();
        RED_LEDS_OFF();
      }
      /* Unknown Case */
      else {
        WHITE_LEDS_ON();
        RED_LEDS_ON();
      }
    }

    /* Increase Timers */
    lightsTimer++;
    buzzerTimer++;

    if ( lightsTimer >= ( 2 * TIME_ALARM_ON ) ) {      /* When Buzzer Timer Expires, Turn it Off */
      PIEZO_BUZZER_OFF();
      buzzerTimer = 0;
    }

    if ( lightsTimer >= ( 2 * TIME_LIGHTS_ON ) ) {      /* When Lights Timer Expires, Turn it Off */
      WHITE_LEDS_OFF();
      RED_LEDS_OFF();
      motionStatus = bFalse;
      triggeredStatus = bFalse;
      lightsTimer = 0;
    }
  }
}


void processS2State() {

}




