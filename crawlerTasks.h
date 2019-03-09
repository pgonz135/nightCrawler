/*
 * crawlerTasks.h
 *
 *  Created on: Mar 3, 2019
 *      Author: pablo
 */

#ifndef CRAWLERTASKS_H_
#define CRAWLERTASKS_H_

#include <msp430.h>
#include <stdio.h>

/* SM Constants */
#define MAIN_STATE_PERIOD   ( 512 / 2 )             /* Period of the Main State */
#define TASK_PERIOD_GCD     ( 128 )                  /* GCD of All Tasks Period: Make Timer Tick as this Rate */
#define TIMER_TICK_RATE     ( 16 )                   /* Tick Rate of SM Timer */

/* State Machine */
extern const unsigned long tasksPeriodGCD;
extern const unsigned long mainStatePeriod;

/*Structure for task*/
typedef struct stateMachineTasks
{
  int state;                                        // Current state of the task
  unsigned long period;                             // Rate at which the task should tick
  unsigned long elapsedTime;                        // Time since task's previous tick
  int (*TickFct)(int);                              // Function to call for task's tick
} stateMachineTasks;

/*State Machine Functions*/
int StateMachine_MainState(int state);
void initStateMachine();
void processS1State( void );
void processS2State( void );

/* Hardcoded Number of Tasks */
stateMachineTasks tasks[1];                         // Number of tasks must be hard-coded
extern const unsigned char tasksNum;                // Number of Tasks

#endif /* CRAWLERTASKS_H_ */
