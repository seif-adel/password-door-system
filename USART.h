/*
 * UART.h
 *
 *  Created on: Jan 22, 2021
 *      Author: seif adel
 */

#ifndef USART_H_
#define USART_H_
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
ASYNCH , SYNCH , ASYNCH_DOUBLE
}uart_mode;

typedef enum
{
POLLING, INTERRUPT
}uart_way_of_work;

typedef enum
{
bit_8, bit_9
}uart_number_of_bits;

typedef enum
{
RISING, FALLING
}edge_trigger_transmit;

typedef enum
{
	rate_9600 = 9600, rate_10600=10600
}
USART_BAUDRATE;

typedef struct
{
	uart_way_of_work WayOfWork;
	uart_mode mode;
	uart_number_of_bits NumberOfData;
	edge_trigger_transmit edge_transmit ;
	USART_BAUDRATE USART_BAUD ;
}uart_config_type;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const uart_config_type * config_ptr);

void UART_sendByte(const uint8 data,uart_way_of_work x);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str ,uart_way_of_work x);

void UART_receiveString(uint8 *Str); // Receive until #

void UART_setCallBack(void(*a_ptr)(void));


#endif /* USART_H_ */
