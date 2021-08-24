/*
 * lcd.h
 *
 *  Created on: Aug 12, 2021
 *      Author: seif adel
 */

#ifndef LCD_H_
#define LCD_H_
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                      Types Declaration                                    *
 *******************************************************************************/
/* LCD Data bits mode configuration */

typedef enum
{
	BIT_4,BIT_8
}
LCD_Data_bits_mode;

typedef enum
{
	HIGHER,LOWER
}
LCD_Data_bits_location;

typedef struct
{
	LCD_Data_bits_mode Data_bits_mode;
	LCD_Data_bits_location Data_bits_location;
}
LCD_Configtype;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* LCD HW Pins */
#define RS PD4
#define RW PD5
#define E  PD6
#define LCD_CTRL_PORT PORTD
#define LCD_CTRL_PORT_DIR DDRD

#define LCD_DATA_PORT PORTB
#define LCD_DATA_PORT_DIR DDRB

/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const char *Str);
void LCD_init(const LCD_Configtype * ConfigPtr);
void LCD_clearScreen(void);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_intgerToString(int data);



#endif /* LCD_H_ */
