/*
 * timer.c
 *
 *  Created on: Aug 13, 2021
 *      Author: seif adel
 */
#include "timer.h"

/************************************************************************************************************/
/*                                                Timer0                                                    */
/************************************************************************************************************/
#ifdef ENABLE_TIMER0

/***************************************** Timer0 normal mood *****************************************/
#ifdef TIMER0_NORMAL_MODE

void timer0_normal_mode_init(){

	TCNT0 = TIMER0_INITIAL_VALUE;    //Set Timer initial value

	SET_BIT(TIMSK, TOIE0); // Enable Timer0 Overflow Interrupt

	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0
	 * 3. Normal Mode COM00=0 & COM01=0
	 */
	TCCR0 = (1<<FOC0);

	#if(TIMER0_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS02=0 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS00);

	#elif (TIMER0_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=0  */
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=1  */
		SET_BIT(TCCR0, CS00);
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=0  */
		SET_BIT(TCCR0, CS02);

	#elif (TIMER0_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS02);
		SET_BIT(TCCR0, CS00);

	#else  /* No Clock Source (timer counter stops). */
	/*CS00=0 CS01=0 CS02=0 */
	#endif

}

void timer0(uint8 max_ticks, void(*f_ptr)())
{
	if( timer0_tick == max_ticks)
	{
		timer0_tick =0;
		(*f_ptr)();
	}
}

ISR(TIMER0_OVF_vect)
{
	timer0_tick++;
	TCNT0 = TIMER0_INITIAL_VALUE
}

#endif


/********************************************** Timer0 compare mood **************************************************/
#ifdef TIMER0_CTC_MODE
void timer0_CTC_mode_init(uint8 top)
{
	TCNT0 = TIMER0_INITIAL_VALUE;     //Set Timer initial value

	OCR0  = top; // Set Compare Value

	#ifdef TIMER0_NORMAL_CTC        /* In case of normal compare mood */
	SET_BIT(TIMSK, OCIE0); // Enable Timer0 compare Interrupt
	#endif

	#ifndef TIMER0_NORMAL_CTC       /* In all cases except normal compare mood */
		SET_BIT(DDRB,PB3); // Configure PB3/OC0 as output pin --> pin where the PWM signal is generated from MC
	#endif

	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. CTC Mode WGM01=1 & WGM00=0 */
	TCCR0 = (1<<FOC0) | (1<<WGM01);

	/* port operation, OC0 disconnected. */
	#ifdef TIMER0_NORMAL_CTC
		/* COM01, COM00  are cleared */
	#endif
	/* Toggle OC0 on compare match */
	#ifdef TIMER0_TOGGLE_CTC
		/* COM01 = 0, COM00 = 1*/
		SET_BIT(TCCR0, COM00);
	#endif
	/* Clear OC0 on compare match */
	#ifdef TIMER0_CLEAR_CTC
		/* COM01 = 1, COM00 = 0*/
		SET_BIT(TCCR0, COM01);
	#endif
	/* Set OC0 on compare match */
	#ifdef TIMER0_SET_CTC
		/* COM01 = 1, COM00 = 1*/
		SET_BIT(TCCR0, COM01);
		SET_BIT(TCCR0, COM00);
	#endif

	/* choosing of Pre_scaler */
	#if(TIMER0_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS02=0 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS00);

	#elif (TIMER0_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=0  */
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=1  */
		SET_BIT(TCCR0, CS00);
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=0  */
		SET_BIT(TCCR0, CS02);

	#elif (TIMER0_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS02);
		SET_BIT(TCCR0, CS00);

	#else  /* No Clock Source (timer counter stops). */
	/* CS00=0 CS01=0 CS02=0 */
	#endif
}

#ifdef TIMER0_NORMAL_CTC

void timer0(uint8 max_ticks, void(*f_ptr)())
{
	if( timer0_tick == max_ticks)
	{
		timer0_tick =0;
		(*f_ptr)();
	}
}

ISR(TIMER0_COMP_vect)
{
	timer0_tick++;
	TCNT0 = TIMER0_INITIAL_VALUE
}

#endif

#endif


/********************************************* Timer0 PWM mood *****************************************************/
#ifdef TIMER0_PWM_MODE
void timer0_PWM_init(uint8 duty_cycle_value){

	TCNT0 = TIMER0_INITIAL_VALUE; // Set Timer Initial Value to 0

	OCR0  = duty_cycle_value; // Set Compare Value

	SET_BIT(DDRB,PB3); // Configure PB3/OC0 as output pin --> pin where the PWM signal is generated from MC

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1 */
	SET_BIT(TCCR0, WGM00);
	SET_BIT(TCCR0, WGM01);

	#ifdef TIMER0_NON_INVERTED_PWM
		/* Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1 */
		SET_BIT(TCCR0, COM01);
	#endif

	#ifdef TIMER0_INVERTED_PWM
		/* Clear OC0 when match occurs (inverted mode) COM00=1 & COM01=1 */
		SET_BIT(TCCR0, COM01);
		SET_BIT(TCCR0, COM00);
	#endif

	#if(TIMER0_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS02=0 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS00);

	#elif (TIMER0_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=0  */
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=1  */
		SET_BIT(TCCR0, CS00);
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=0  */
		SET_BIT(TCCR0, CS02);

	#elif (TIMER0_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS02);
		SET_BIT(TCCR0, CS00);

	#else  /* No Clock Source (timer counter stops). */
	/* CS00=0 CS01=0 CS02=0 */
	#endif

}
#endif

void timer0_pause()
{
	/* No Clock Source (timer counter stops)
	* clock = F_CPU/1024 CS00=0 CS01=0 CS02=0 */
	CLEAR_BIT(TCCR0, CS00);
	CLEAR_BIT(TCCR0, CS01);
	CLEAR_BIT(TCCR0, CS02);
}

void timer0_resume(){

	#if(TIMER0_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS02=0 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS00);

	#elif (TIMER0_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=0  */
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=1  */
		SET_BIT(TCCR0, CS00);
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=0  */
		SET_BIT(TCCR0, CS02);

	#elif (TIMER0_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS02);
		SET_BIT(TCCR0, CS00);

	#else  /* No Clock Source (timer counter stops). */
	/* CS00=0 CS01=0 CS02=0 */
	#endif
}

void timer0_stop()
{
	/* No Clock Source (timer counter stops)
	* clock = F_CPU/1024 CS00=0 CS01=0 CS02=0 */
	CLEAR_BIT(TCCR0, CS00);
	CLEAR_BIT(TCCR0, CS01);
	CLEAR_BIT(TCCR0, CS02);

	#ifdef TIMER0_NORMAL_MODE
		/* Reset the timer ticks interrupt counter */
		timer0_tick = 0;
	#endif

	#ifdef TIMER0_CTC_MODE
		#ifdef TIMER0_NORMAL_CTC
			timer0_tick = 0;                    /* Reset the timer ticks interrupt counter */
		#endif
	#endif
}


#endif


/************************************************************************************************************/
/*                                                Timer1                                                    */
/************************************************************************************************************/
#ifdef ENABLE_TIMER1

#ifdef TIMER1_NORMAL_MODE
void timer1_normal_mode_init()
{
	TCNT1 = 0;    //Set Timer initial value

	SET_BIT(TIMSK, TOIE1); // Enable Timer1 Overflow Interrupt

	/* Configure the timer control register
	 * 1. Non PWM mode FOC1A=1 FOC1B=1
	 * 2. COM1A1=0 COM1A0=0 COM1B1=0 COM1B0=0
	 * 3. Normal Mode WGM13=0  WGM12=0  WGM11=0 WGM10=0
	 */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	#if(TIMER0_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS02=0 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS00);

	#elif (TIMER0_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=0  */
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS02=0 CS01=1 CS00=1  */
		SET_BIT(TCCR0, CS00);
		SET_BIT(TCCR0, CS01);

	#elif (TIMER0_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=0  */
		SET_BIT(TCCR0, CS02);

	#elif (TIMER0_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS02=1 CS01=0 CS00=1  */
		SET_BIT(TCCR0, CS02);
		SET_BIT(TCCR0, CS00);

	#else  /* No Clock Source (timer counter stops). */
	/*CS00=0 CS01=0 CS02=0 */
	#endif
}
#endif

#ifdef TIMER1_CTC_MODE
void timer1_CTC_mode_init()
{
	TCNT1 = 0;     //Set Timer initial value
	TCCR1A = (1<<FOC1A)| (1<<FOC1B);
	TCCR1B = (1<<WGM12) | (1<<CS12) |(1<<CS10);
	OCR1A = 350;
	TIMSK = (1<<OCIE1A);
}
#endif


uint8 timer1()
{
	return timer1_tick;
}

void timer1_stop()
{
	TCCR1B =0;
	timer1_tick =0;
}

#endif

/************************************************************************************************************/
/*                                                Timer2                                                    */
/************************************************************************************************************/
#ifdef ENABLE_TIMER2

/***************************************** Timer2 normal mood *****************************************/
#ifdef TIMER2_NORMAL_MODE
void timer2_normal_mode_init(){

	TCNT2 = TIMER2_INITIAL_VALUE; //Set Timer initial value

	SET_BIT(TIMSK, TOIE2); // Enable Timer0 Overflow Interrupt

	/* Configure the timer control register
	 * 1. Non PWM mode FOC2=1
	 * 2. Normal Mode WGM21=0 & WGM20=0
	 * 3. Normal Mode COM20=0 & COM21=0
	 */
	TCCR2 = (1<<FOC2);

	#if(TIMER2_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS22=0 CS21=0 CS20=1  */
		SET_BIT(TCCR2, CS20);

	#elif (TIMER2_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS22=0 CS21=1 CS20=0  */
		SET_BIT(TCCR2, CS21);

	#elif (TIMER2_PRESCALER == 32) /* CPU Clock/32. */
		/* Clock = F_CPU CS22=0 CS21=1 CS20=1  */
		SET_BIT(TCCR2, CS21);
		SET_BIT(TCCR2, CS20);

	#elif (TIMER2_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS22=1 CS21=0 CS20=0  */
		SET_BIT(TCCR2, CS22);

	#elif (TIMER2_PRESCALER == 128) /* CPU Clock/128. */
		/* Clock = F_CPU CS22=1 CS21=0 CS20=1  */
		SET_BIT(TCCR2, CS20);
		SET_BIT(TCCR2, CS22);

	#elif (TIMER2_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS22=1 CS21=1 CS20=0  */
		SET_BIT(TCCR2, CS22);
		SET_BIT(TCCR2, CS21);

	#elif (TIMER2_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS22=1 CS21=1 CS20=1  */
		SET_BIT(TCCR2, CS22);
		SET_BIT(TCCR2, CS21);
		SET_BIT(TCCR2, CS20);
	#else  /* No Clock Source (timer counter stops). */
	/*CS20=0 CS21=0 CS22=0 */
	#endif

}

void timer2(uint8 max_ticks, void(*f_ptr)())
{
	if( timer2_tick == max_ticks)
	{
		timer2_tick =0;
		(*f_ptr)();
	}
}

ISR(TIMER2_OVF_vect)
{
	timer2_tick++;
	TCNT2 = TIMER2_INITIAL_VALUE;   /* Set the initial value of timer2 */
}

#endif


/********************************************** Timer2 compare mood **************************************************/
#ifdef TIMER2_CTC_MODE
void timer2_CTC_mode_init(uint8 top)
{
	TCNT2 = TIMER2_INITIAL_VALUE; //Set Timer initial value

	OCR2  = top; // Set Compare Value

	#ifdef TIMER2_NORMAL_CTC        /* In case of normal compare mood */
	SET_BIT(TIMSK, OCIE2); // Enable Timer0 compare Interrupt
	#endif

	#ifndef TIMER2_NORMAL_CTC       /* In all cases except normal compare mood */
		SET_BIT(DDRD,PD7);          // Configure PD7/OC2 as output pin --> pin where the PWM signal is generated from MC
	#endif

	/* Configure the timer control register
	 * 1. Non PWM mode FOC2=1
	 * 2. CTC Mode WGM21=1 & WGM20=0 */
	TCCR2 = (1<<FOC2) | (1<<WGM21);

	/* port operation, OC0 disconnected. */
	#ifdef TIMER2_NORMAL_CTC
		/* COM21, COM20  are cleared */
	#endif
	/* Toggle OC2 on compare match */
	#ifdef TIMER2_TOGGLE_CTC
		/* COM21 = 0, COM20 = 1*/
		SET_BIT(TCCR2, COM20);
	#endif
	/* Clear OC2 on compare match */
	#ifdef TIMER2_CLEAR_CTC
		/* COM21 = 1, COM20 = 0*/
		SET_BIT(TCCR2, COM21);
	#endif
	/* Set OC2 on compare match */
	#ifdef TIMER2_SET_CTC
		/* COM01 = 1, COM00 = 1*/
		SET_BIT(TCCR2, COM21);
		SET_BIT(TCCR2, COM20);
	#endif

	/* choosing of Pre_scaler */
	#if(TIMER2_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS22=0 CS21=0 CS20=1  */
		SET_BIT(TCCR2, CS20);

	#elif (TIMER2_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS22=0 CS21=1 CS20=0  */
		SET_BIT(TCCR2, CS21);

	#elif (TIMER2_PRESCALER == 32) /* CPU Clock/32. */
		/* Clock = F_CPU CS22=0 CS21=1 CS20=1  */
		SET_BIT(TCCR2, CS21);
		SET_BIT(TCCR2, CS20);

	#elif (TIMER2_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS22=1 CS21=0 CS20=0  */
		SET_BIT(TCCR2, CS22);

	#elif (TIMER2_PRESCALER == 128) /* CPU Clock/128. */
		/* Clock = F_CPU CS22=1 CS21=0 CS20=1  */
		SET_BIT(TCCR2, CS20);
		SET_BIT(TCCR2, CS22);

	#elif (TIMER2_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS22=1 CS21=1 CS20=0  */
		SET_BIT(TCCR2, CS22);
		SET_BIT(TCCR2, CS21);

	#elif (TIMER2_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS22=1 CS21=1 CS20=1  */
		SET_BIT(TCCR2, CS22);
		SET_BIT(TCCR2, CS21);
		SET_BIT(TCCR2, CS20);
	#else  /* No Clock Source (timer counter stops). */
	/*CS20=0 CS21=0 CS22=0 */
	#endif
}

#ifdef TIMER2_NORMAL_CTC

void timer2(uint8 max_ticks, void(*f_ptr)())
{
	if( timer2_tick == max_ticks)
	{
		timer2_tick =0;
		(*f_ptr)();
	}
}

ISR(TIMER2_COMP_vect)
{
	timer2_tick++;
	TCNT2 = TIMER2_INITIAL_VALUE;
}

#endif

#endif


/********************************************* Timer2 PWM mood *****************************************************/
#ifdef TIMER2_PWM_MODE
void timer2_PWM_init(uint8 duty_cycle_value){

	TCNT2 = TIMER2_INITIAL_VALUE;         // Set Timer Initial Value

	OCR2  = duty_cycle_value; // Set Compare Value

	SET_BIT(DDRD,PD7); // Configure PD7/OC2 as output pin --> pin where the PWM signal is generated from MC

	/* Configure timer control register
	 * 1. Fast PWM mode FOC2=0
	 * 2. Fast PWM Mode WGM21=1 & WGM20=1 */
	SET_BIT(TCCR2, WGM20);
	SET_BIT(TCCR2, WGM21);

	#ifdef TIMER2_NON_INVERTED_PWM
		/* Clear OC2 when match occurs (non inverted mode) COM20=0 & COM21=1 */
		SET_BIT(TCCR2, COM21);
	#endif

	#ifdef TIMER2_INVERTED_PWM
		/* Clear OC2 when match occurs (inverted mode) COM20=1 & COM21=1 */
		SET_BIT(TCCR2, COM21);
		SET_BIT(TCCR2, COM20);
	#endif

	#if(TIMER2_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS22=0 CS21=0 CS20=1  */
		SET_BIT(TCCR2, CS20);

	#elif (TIMER2_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS22=0 CS21=1 CS20=0  */
		SET_BIT(TCCR2, CS21);

	#elif (TIMER2_PRESCALER == 32) /* CPU Clock/32. */
		/* Clock = F_CPU CS22=0 CS21=1 CS20=1  */
		SET_BIT(TCCR2, CS21);
		SET_BIT(TCCR2, CS20);

	#elif (TIMER2_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS22=1 CS21=0 CS20=0  */
		SET_BIT(TCCR2, CS22);

	#elif (TIMER2_PRESCALER == 128) /* CPU Clock/128. */
		/* Clock = F_CPU CS22=1 CS21=0 CS20=1  */
		SET_BIT(TCCR2, CS20);
		SET_BIT(TCCR2, CS22);

	#elif (TIMER2_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS22=1 CS21=1 CS20=0  */
		SET_BIT(TCCR2, CS22);
		SET_BIT(TCCR2, CS21);

	#elif (TIMER2_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS22=1 CS21=1 CS20=1  */
		SET_BIT(TCCR2, CS22);
		SET_BIT(TCCR2, CS21);
		SET_BIT(TCCR2, CS20);
	#else  /* No Clock Source (timer counter stops). */
	/*CS20=0 CS21=0 CS22=0 */
	#endif
}
#endif

void timer2_pause()
{
	/* No Clock Source (timer counter stops)
	* clock = F_CPU/1024 CS20=0 CS21=0 CS22=0 */
	CLEAR_BIT(TCCR2, CS20);
	CLEAR_BIT(TCCR2, CS21);
	CLEAR_BIT(TCCR2, CS22);
}

void timer2_resume(){

	#if(TIMER2_PRESCALER == 1)   /* CPU Clock (No pre_scaling). */
		/* Clock = F_CPU CS22=0 CS21=0 CS20=1  */
		SET_BIT(TCCR2, CS20);

	#elif (TIMER2_PRESCALER == 8) /* CPU Clock/8. */
		/* Clock = F_CPU CS22=0 CS21=1 CS20=0  */
		SET_BIT(TCCR2, CS21);

	#elif (TIMER2_PRESCALER == 32) /* CPU Clock/32. */
		/* Clock = F_CPU CS22=0 CS21=1 CS20=1  */
		SET_BIT(TCCR2, CS21);
		SET_BIT(TCCR2, CS20);

	#elif (TIMER2_PRESCALER == 64) /* CPU Clock/64. */
		/* Clock = F_CPU CS22=1 CS21=0 CS20=0  */
		SET_BIT(TCCR2, CS22);

	#elif (TIMER2_PRESCALER == 128) /* CPU Clock/128. */
		/* Clock = F_CPU CS22=1 CS21=0 CS20=1  */
		SET_BIT(TCCR2, CS20);
		SET_BIT(TCCR2, CS22);

	#elif (TIMER2_PRESCALER == 256) /* CPU Clock/256. */
		/* Clock = F_CPU CS22=1 CS21=1 CS20=0  */
		SET_BIT(TCCR2, CS22);
		SET_BIT(TCCR2, CS21);

	#elif (TIMER2_PRESCALER == 1024) /* CPU Clock/1024. */
		/* Clock = F_CPU CS22=1 CS21=1 CS20=1  */
		SET_BIT(TCCR2, CS22);
		SET_BIT(TCCR2, CS21);
		SET_BIT(TCCR2, CS20);
	#else  /* No Clock Source (timer counter stops). */
	/*CS20=0 CS21=0 CS22=0 */
	#endif
}

void timer2_stop()
{
	/* No Clock Source (timer counter stops)
	* clock = F_CPU/1024 CS20=0 CS21=0 CS22=0 */
	CLEAR_BIT(TCCR2, CS20);
	CLEAR_BIT(TCCR2, CS21);
	CLEAR_BIT(TCCR2, CS22);

	#ifdef TIMER2_NORMAL_MODE
		/* Reset the timer ticks interrupt counter */
		timer2_tick = 0;
	#endif

	#ifdef TIMER2_NORMAL_CTC
		/* Reset the timer ticks interrupt counter */
		timer2_tick = 0;
	#endif
}
#endif

/************************************************************************************************************/
/*                                             Watch dog timer                                              */
/************************************************************************************************************/
#ifdef ENABLE_WATCHDOG

void watchDogON(void)
{
	SET_BIT(WDTCR, WDE);                       /* Enable the watch_dog timer */

	/* Watch_dog timer Pre_scaler */
	#if (WATCHDOG_PRESCALER == 21)             /* 2.1 seconds */
		/* SET WDP2, WDP1, WDP0 */
		SET_BIT(WDTCR, WDP0);
		SET_BIT(WDTCR, WDP1);
		SET_BIT(WDTCR, WDP2);

	#elif (WATCHDOG_PRESCALER == 1)            /* 1 seconds */
		/* WDP2 = 1, WDP1 = 1, WDP0 = 0 */
		SET_BIT(WDTCR, WDP1);
		SET_BIT(WDTCR, WDP2);

	#elif (WATCHDOG_PRESCALER == 52)          /* 0.52 seconds */
		/* WDP2 = 1, WDP1 = 0, WDP0 = 1 */
		SET_BIT(WDTCR, WDP0);
		SET_BIT(WDTCR, WDP2);

	#elif (WATCHDOG_PRESCALER == 26)          /* 0.26 seconds */
		/* WDP2 = 1, WDP1 = 0, WDP0 = 0 */
		SET_BIT(WDTCR, WDP2);

	#elif (WATCHDOG_PRESCALER == 13)          /* 0.13 seconds */
		/* WDP2 = 0, WDP1 = 1, WDP0 = 1 */
		SET_BIT(WDTCR, WDP1);
		SET_BIT(WDTCR, WDP0);

	#elif (WATCHDOG_PRESCALER == 65)          /* 65 m_seconds */
		/* WDP2 = 0, WDP1 = 1, WDP0 = 0 */
		SET_BIT(WDTCR, WDP1);

	#elif (WATCHDOG_PRESCALER == 325)          /* 32.5 m_seconds */
		/* WDP2 = 0, WDP1 = 0, WDP0 = 1 */
		SET_BIT(WDTCR, WDP0);

	#else                                      /* 16.3 m_sec */
		/* Clear WDP2, WDP1, WDP0 */
	#endif
}

void watchDogOFF(void)
{
	uint8 cycles;
	WDTCR = (1<<WDTOE) | (1<<WDE);                     /* Write Logic 1 to WDTOE and WDE */
	for(cycles=0; cycles<4; cycles++);      /* Estimation to wait 4 clock cycles */
	CLEAR_BIT(WDTCR, WDE);                  /* Clear WDE bit */
}

#endif
