/*
 * timer.h
 *
 *  Created on: Aug 13, 2021
 *      Author: seif adel
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/**********************************************************************************************************/
/*                                        Preprocessor Defines                                            */
/**********************************************************************************************************/

/********************************  Enable a specific timer ********************************/
//#define ENABLE_TIMER0
#define ENABLE_TIMER1
//#define ENABLE_TIMER2
//#define ENABLE_WATCHDOG

/************************* Timer0 Preprocessor defines  *****************************/
#ifdef ENABLE_TIMER0
	/****************************  Select timer0 moods **************************/
	#define TIMER0_NORMAL_MODE
	//#define TIMER0_CTC_MODE
//	#define TIMER0_PWM_MODE

	/****************************  Select CTC moods ****************************/
	#ifdef TIMER0_CTC_MODE
		#define TIMER0_NORMAL_CTC
		//#define TIMER0_TOGGLE_CTC
		//#define TIMER0_SET_CTC
		//#define TIMER0_CLEAR_CTC
	#endif

	/****************************  Select PWM moods ****************************/
	#ifdef TIMER0_PWM_MODE
		//#define TIMER0_INVERTED_PWM
		#define TIMER0_NON_INVERTED_PWM
	#endif

	 /**************************** Timer0 Pre_scaler ***************************/
	#define TIMER0_PRESCALER 8

	/* The initial value of start counting */
	#define TIMER0_INITIAL_VALUE 0

#endif

/************************* Timer1 Preprocessor defines  *****************************/
#ifdef ENABLE_TIMER1
//#define TIMER1_NORMAL_MODE
#define TIMER1_CTC_MODE

#endif

/************************* Timer2 Preprocessor defines  *****************************/
#ifdef ENABLE_TIMER2
	/****************************  Select timer2 moods **************************/
//	#define TIMER2_NORMAL_MODE
//	#define TIMER2_CTC_MODE
	#define TIMER2_PWM_MODE

	/****************************  Select CTC moods ****************************/
	#ifdef TIMER2_CTC_MODE
		#define TIMER2_NORMAL_CTC
//		#define TIMER2_TOGGLE_CTC
		//#define TIMER2_SET_CTC
		//#define TIMER2_CLEAR_CTC
	#endif

	/****************************  Select PWM moods ****************************/
	#ifdef TIMER2_PWM_MODE
		//#define TIMER2_INVERTED_PWM
		#define TIMER2_NON_INVERTED_PWM
	#endif

	 /**************************** Timer2 Pre_scaler ***************************/
	#define TIMER2_PRESCALER 8

	/* The initial value of start counting */
	#define TIMER2_INITIAL_VALUE 0

#endif

/************************** Watch dog Timer Preprocessors  ******************************/
#ifdef ENABLE_WATCHDOG
	#define WATCHDOG_PRESCALER 21
#endif



/*****************************************************************************************************/
/*                                          Global Variables                                         */
/*****************************************************************************************************/

/************************* Timer0 Global Variables *********************************/
#ifdef ENABLE_TIMER0
	/* Timer0_tick is used in normal mood and normal_CTC mood to count the interrupts of the timer */
	#ifdef TIMER0_NORMAL_MODE
		static volatile uint8 timer0_tick = 0;  /* Timer0 interrupt counter */
	#endif
	#ifdef TIMER0_CTC_MODE
		#ifdef NORMAL_CTC
			static volatile uint8 timer0_tick = 0;  /* Timer0 interrupt counter */
		#endif
	#endif

#endif

/************************* Timer1 Global Variables *********************************/
#ifdef ENABLE_TIMER1
	/* Timer0_tick is used in normal mood and normal_CTC mood to count the interrupts of the timer */
	#ifdef TIMER1_NORMAL_MODE
		static volatile uint8 timer1_tick = 0;  /* Timer1 interrupt counter */
	#endif
	#ifdef TIMER1_CTC_MODE
		 volatile extern uint8 timer1_tick = 0;  /* Timer1 interrupt counter */
	#endif

#endif


/************************* Timer2 Global Variables *********************************/
#ifdef ENABLE_TIMER2
	/* Timer2_tick is used in normal mood and normal_CTC mood to count the interrupts of the timer */
	#ifdef TIMER2_NORMAL_MODE
		static volatile uint8 timer2_tick = 0;  /* Timer2 interrupt counter */
	#endif

	#ifdef TIMER2_CTC_MODE
		#ifdef TIMER2_NORMAL_CTC
			static volatile uint8 timer2_tick = 0;  /* Timer2 interrupt counter */
		#endif
	#endif
#endif



/****************************************************************************************************/
/*                                      Functions Prototypes                                        */
/****************************************************************************************************/

/********************** Timer0 Functions Prototypes  ***************************/
#ifdef ENABLE_TIMER0

/************************************************************************/
/* Function [Name]:            timer0_normal_mode_init()
 *
 * Function [Description]:     Initializing the overflow mode of timer 0
 *
 * Function [configuration]:   -Set Timer initial value to 0
 * 							   -Enable Timer0 Overflow Interrupt
 * 							   -Configure the timer control register:
 * 							      1-Non PWM mode FOC0=1
 * 							      2-Normal Mode WGM01=0 & WGM00=0
 * 							      3-Normal Mode COM00=0 & COM01=0
 * 							      4-CLK Pre_scaler
 ************************************************************************/
void timer0_normal_mode_init();



/************************************************************************/
/* Function [Name]:            timer0_CTC_mode_init()
 *
 * Function [Description]:     Initializing the Compare mode of timer 0
 *
 * Function [configuration]:   -Set Timer initial value to 0
 * 							   -Set Compare Value
 * 							   -Enable Timer0 CTC Interrupt
 * 							   -Configure the timer control register:
 * 							      1-Non PWM mode FOC0=1
 * 							      2-CTC Mode WGM01=1 & WGM00=0
 * 							      3-Choosing Mode on COM00 & COM01
 * 							      4-CLK Pre_scaler
 ************************************************************************/
void timer0_CTC_mode_init(uint8 top);



/************************************************************************/
/* Function [Name]:            timer0_PWM_init()
 *
 * Function [Description]:     Initializing the PWM mode of timer 0
 *
 * Function [configuration]:   -Set Timer initial value to 0
 * 							   -Set duty cycle Value
 * 							   -Configure PB3/OC0 as output pin
 * 							   -Configure the timer control register:
 * 							      1-Fast PWM mode  FOC0=0
 * 							      2-Fast PWM Mode WGM01=1 & WGM00=1
 * 							      3-Choosing Mode on COM00 & COM01
 * 							      4-CLK Pre_scaler
 ************************************************************************/
void timer0_PWM_init(uint8 duty_cycle_value);


/************************************************************************/
/* Function [Name]:            timer0()
 *
 * Function [Description]:      Calling another function when the number
 * 							    of interrupts equals max_ticks
 *
 *Function [configuration]:    -If number of interrupts equal max_ticks
 *								do the following:
 *								 1-Reset the global interrupt flag (tick)
 *								 2-Call the passed function
 ************************************************************************/
void timer0(uint8 max_ticks, void(*f_ptr)());


/************************************************************************/
/* Function [Name]:            timer0_pause()
 *
 * Function [Description]:      pause the timer from counting and keep
 * 								the current timer0_tick value.
 *
 *Function [configuration]:    -Reset all the Pre_scaler pits.
 *								by clearing bits In TCCR0 Register
 *								( CS00, CS01, CS02 )
 ************************************************************************/
void timer0_pause();


/************************************************************************/
/* Function [Name]:            timer0_resume()
 *
 * Function [Description]:      Resume the timer0 to continue counting
 *
 * Function [configuration]:    -Put the Pre_scaler bits with their values
 ************************************************************************/
void timer0_resume();


/************************************************************************/
/* Function [Name]:            timer0_stop()
 *
 * Function [Description]:      Stop the timer0 from counting
 *
 * Function [configuration]:    -Reset all the Pre_scaler pits.
 *								  by clearing bits In TCCR0 Register
 *								  ( CS00, CS01, CS02 )
 *								-Reset timer0_tick
 ************************************************************************/
void timer0_stop();

#endif


/********************** Timer1 Functions Prototypes  ***************************/
#ifdef ENABLE_TIMER1

/************************************************************************/
/* Function [Name]:            timer1_normal_mode_init()
 *
 * Function [Description]:     Initializing the overflow mode of timer 1
 *
 * Function [configuration]:   -Set Timer initial value to 0
 * 							   -Enable Timer1 Overflow Interrupt
 * 							   -Configure the timer control register:
 * 							      1-Non PWM mode F1C0=1
 * 							      2-Normal Mode WGM11=0 & WGM10=0
 * 							      3-Normal Mode COM10=0 & COM11=0
 * 							      4-CLK Pre_scaler
 ************************************************************************/
void timer1_normal_mode_init();


void timer1_CTC_mode_init();

/************************************************************************/
/* Function [Name]:            timer1()
 *
 * Function [Description]:      Calling another function when the number
 * 							    of interrupts equals max_ticks
 *
 *Function [configuration]:    -If number of interrupts equal max_ticks
 *								do the following:
 *								 1-Reset the global interrupt flag (tick)
 *								 2-Call the passed function
 ************************************************************************/
uint8 timer1();

/************************************************************************/
/* Function [Name]:            timer1_stop()
 *
 * Function [Description]:      Stop the timer1 from counting
 *
 * Function [configuration]:    -Reset all the Pre_scaler pits.
 *								  by clearing bits In TCCR0 Register
 *								  ( CS10, CS11, CS12 )
 *								-Reset timer0_tick
 ************************************************************************/
void timer1_stop();

#endif


/********************** Timer2 Functions Prototypes  ***************************/
#ifdef ENABLE_TIMER2

/************************************************************************/
/* Function [Name]:            timer2_normal_mode_init()
 *
 * Function [Description]:     Initializing the overflow mode of timer 2
 *
 * Function [configuration]:   -Set Timer initial value to 0
 * 							   -Enable Timer2 Overflow Interrupt
 * 							   -Configure the timer control register:
 * 							      1-Non PWM mode FOC2=1
 * 							      2-Normal Mode WGM21=0 & WGM20=0
 * 							      3-Normal Mode COM20=0 & COM21=0
 * 							      4-CLK Pre_scaler
 ************************************************************************/
void timer2_normal_mode_init();



/************************************************************************/
/* Function [Name]:            timer2_CTC_mode_init()
 *
 * Function [Description]:     Initializing the Compare mode of timer 2
 *
 * Function [configuration]:   -Set Timer initial value to 0
 * 							   -Set Compare Value
 * 							   -Enable Timer2 CTC Interrupt
 * 							   -Configure the timer control register:
 * 							      1-Non PWM mode FOC2=1
 * 							      2-CTC Mode WGM21=1 & WGM20=0
 * 							      3-Choosing Mode on COM20 & COM21
 * 							      4-CLK Pre_scaler
 ************************************************************************/
void timer2_CTC_mode_init(uint8 top);



/************************************************************************/
/* Function [Name]:            timer2_PWM_init()
 *
 * Function [Description]:     Initializing the PWM mode of timer 0
 *
 * Function [configuration]:   -Set Timer initial value to 2
 * 							   -Set duty cycle Value
 * 							   -Configure PD7/OC2 as output pin
 * 							   -Configure the timer control register:
 * 							      1-Fast PWM mode  FOC2=0
 * 							      2-Fast PWM Mode WGM21=1 & WGM20=1
 * 							      3-Choosing Mode on COM20 & COM21
 * 							      4-CLK Pre_scaler
 ************************************************************************/
void timer2_PWM_init(uint8 duty_cycle_value);



/************************************************************************/
/* Function [Name]:            timer2()
 *
 * Function [Description]:      Calling another function when the number
 * 							    of interrupts equals max_ticks
 *
 *Function [configuration]:    -If number of interrupts equal max_ticks
 *								do the following:
 *								 1-Reset the global interrupt flag (tick)
 *								 2-Call the passed function
 ************************************************************************/
void timer2(uint8 max_ticks, void(*f_ptr)());


/************************************************************************/
/* Function [Name]:            timer2_pause()
 *
 * Function [Description]:      pause the timer from counting and keep
 * 								the current timer2_tick value.
 *
 *Function [configuration]:    -Reset all the Pre_scaler pits.
 *								by clearing bits In TCCR0 Register
 *								( CS20, CS21, CS22 )
 ************************************************************************/
void timer2_pause();


/************************************************************************/
/* Function [Name]:            timer2_resume()
 *
 * Function [Description]:      Resume the timer2 to continue counting
 *
 * Function [configuration]:    -Put the Pre_scaler bits with their values
 ************************************************************************/
void timer2_resume();



/************************************************************************/
/* Function [Name]:            timer2_stop()
 *
 * Function [Description]:      Stop the timer0 from counting
 *
 * Function [configuration]:    -Reset all the Pre_scaler pits.
 *								  by clearing bits In TCCR2 Register
 *								  ( CS20, CS21, CS22 )
 *								-Reset timer2_tick.
 ************************************************************************/
void timer2_stop();

#endif

/********************** Watch dog Functions Prototypes  ***************************/
#ifdef ENABLE_WATCHDOG

/************************************************************************/
/* Function [Name]:             watchDogON()
 *
 * Function [Description]:      Enable the watch dog timer to start
 *
 * Function [configuration]:    -Enable watch_dog by SET WDE bit
 * 								-Choose Watch_dog Pre_scaler bits
 * 								 ( WDP2, WDP1, WDP0 )
 ************************************************************************/
void watchDogON(void);


/************************************************************************/
/* Function [Name]:             watchDogOFF()
 *
 * Function [Description]:      Disable the watch dog timer to stop
 *
 * Function [configuration]:    -Disable watch_dog by:
 * 								  1-SET WDTOE, WDE in one instruction
 * 								  2-Delay 4 clock cycles
 * 								  3-Reset WDE
 ************************************************************************/
void watchDogOFF(void);

#endif

#endif
