/*
 * main.c
 *
 *  Created on: Aug 17, 2021
 *      Author: seif adel
 */
#include"timer.h"
#include"USART.h"
#define success 0x20
#define fail 0x30
#define motor_PORT PORTC
#define motor_PORT_DIR DDRC
#define IN1 PC0
#define IN2 PC1
#define Buzzer_PORT PORTB
#define Buzzer_PORT_DIR DDRB
#define Buzzer_PIN PB0
#define LED_PIN PB1
#define count_3_second 23438
#define count_6_second 46875

ISR(TIMER1_COMPA_vect)
{
	timer1_tick ++;
}

void fun_success(void)
{
	SET_BIT(motor_PORT,IN1);
	CLEAR_BIT(motor_PORT,IN2);
	//timer1_CTC_mode_init();
	/*while(timer1()<15)
	{
	}*/
	_delay_ms(1000);
	CLEAR_BIT(motor_PORT,IN1);
	CLEAR_BIT(motor_PORT,IN2);
	/*while(timer1()<18)
	{
	}*/
	_delay_ms(500);
	CLEAR_BIT(motor_PORT,IN1);
	SET_BIT(motor_PORT,IN2);
	/*while(timer1()<33)
	{
	}*/
	_delay_ms(1000);
	CLEAR_BIT(motor_PORT,IN1);
	CLEAR_BIT(motor_PORT,IN2);
	//timer1_stop();
}

void fun_fail(void)
{
	SET_BIT(Buzzer_PORT,Buzzer_PIN);
	SET_BIT(Buzzer_PORT,LED_PIN);
	//timer1_CTC_mode_init();
	/*while(timer1()<30)
	{
	}*/
	_delay_ms(3500);
	CLEAR_BIT(Buzzer_PORT,LED_PIN);
	CLEAR_BIT(Buzzer_PORT,Buzzer_PIN);
	//timer1_stop();
}
int main(void)
{
	uart_config_type uart_config = {POLLING,ASYNCH_DOUBLE,bit_8,RISING ,rate_9600};
	UART_init(&uart_config);
	//UART_setCallBack(uart_recieve_isr);
	motor_PORT_DIR = (1<<IN1) |(1<<IN2);
	Buzzer_PORT_DIR = (1<<Buzzer_PIN) | (1<<LED_PIN);
	CLEAR_BIT(Buzzer_PORT,LED_PIN);
	CLEAR_BIT(Buzzer_PORT,Buzzer_PIN);
	sei();
	while(1)
	{
		if(UART_recieveByte()==success)
		{
			fun_success();
		}
		else //if(UART_recieveByte()==fail)
		{
			fun_fail();
		}
	}
}
