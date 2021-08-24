/*
 * main.c
 *
 *  Created on: Aug 16, 2021
 *      Author: seif adel
 */
#include"USART.h"
#include"lcd.h"
#include"keypad.h"
//#include"timer.h"
#include"external_eeprom.h"
#define done 0x10
#define success 0x20
#define fail 0x30
#define address_of_flag 0x0313
#define address_of_password 0x0211
#define address_of_digits 0x0011
#define count_6_second 46875
uint8 glob_timer1_tick=0;
uint8 nuber_of_digits_of_pass=4;
uint8 i;
void start_of_locker(void)
{
	while(1)
	{
		LCD_goToRowColumn(0,0);
		LCD_displayString("enter the number of digits");
		LCD_goToRowColumn(1,0);
		LCD_displayString("of password");
		uint8 digits = KeyPad_getPressedKey();
		_delay_ms(40);
		if(digits ==4)
			nuber_of_digits_of_pass =4;
		else if(digits ==5)
			nuber_of_digits_of_pass =5;
		else if(digits ==6)
			nuber_of_digits_of_pass =6;
		else
		{
			LCD_clearScreen();
			LCD_goToRowColumn(0,0);
			LCD_displayString("only available 4 or 5 or 6 digits");
		}
		EEPROM_writeByte((address_of_digits),(digits));
		_delay_ms(10);
		uint8 key[nuber_of_digits_of_pass];
		bool correctnes_of_repeatition = TRUE;
		LCD_clearScreen();
		LCD_goToRowColumn(0,0);
		LCD_displayString("Please enter the new password");
		LCD_goToRowColumn(1,0);
		for(i=0;i<nuber_of_digits_of_pass;i++)
		{
			key[i] = KeyPad_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(40);
		}
		LCD_clearScreen();
		LCD_goToRowColumn(0,0);
		LCD_displayString("please repeat your password");
		LCD_goToRowColumn(1,0);
		uint8 key2;
		for(i=0;i<nuber_of_digits_of_pass;i++)
		{
			key2 = KeyPad_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(40);
			if(key2 != key[i])
			{
				correctnes_of_repeatition = FALSE;
				break;
			}
		}
		if(correctnes_of_repeatition==TRUE)
		{
			LCD_clearScreen();
			LCD_goToRowColumn(0,0);
			LCD_displayString("***successfully***");
			EEPROM_writeByte(address_of_flag,1);
			_delay_ms(10);
			for(int i=0;i<nuber_of_digits_of_pass;i++)
			{
				EEPROM_writeByte((address_of_password + i),(key[i]));
				_delay_ms(10);
			}
			break;
		}
	}
}
int main(void)
{
	LCD_Configtype LCD_Config = {BIT_8,HIGHER};
	uart_config_type uart_config = {POLLING,ASYNCH_DOUBLE,bit_8,RISING,rate_9600};
	LCD_init(&LCD_Config);
	EEPROM_init();
	UART_init(&uart_config);
	uint8 flag = 0;
	EEPROM_readByte(address_of_flag,&flag);
	_delay_ms(10);
	EEPROM_readByte(address_of_digits,&nuber_of_digits_of_pass);
	_delay_ms(10);
	if(flag!=1)
		start_of_locker();
	while(1)
	{
		LCD_clearScreen();
		LCD_goToRowColumn(0,0);
		LCD_displayString("enter # to open the door");
		LCD_goToRowColumn(1,0);
		LCD_displayString("enter * to change the password");
		uint8 key = KeyPad_getPressedKey();
		_delay_ms(40);
		if(key!='*' && key !='#')
		{
			continue;
		}
		uint8 reptition_of_wrong_pass = 0;
		uint8 saved_pass[nuber_of_digits_of_pass];
		for(i=0;i<nuber_of_digits_of_pass;i++)
		{
			EEPROM_readByte((address_of_password +i),(saved_pass +i));
		}
		uint8 key2[nuber_of_digits_of_pass];
		while(1)
		{
			if(reptition_of_wrong_pass==3) // case of enter the password wrong 3 times
			{
				LCD_clearScreen();
				LCD_goToRowColumn(0,0);
				LCD_displayString("***system paused for 1 min***");
				UART_sendByte(fail,POLLING);
				//timer1_CTC_mode_init();
				 //Loop executed for 60 seconds to print the count down time.
				uint8 time=10;
				/*while(timer1() < 60)
				{
					LCD_goToRowColumn(1,0);
					time = 60-timer1();       // Calculate the current (count down) value.
					LCD_intgerToString(time / 10);  // Print the tens digit of the current value.
					LCD_intgerToString(time % 10);  // Print the unit digit of the current value.
				}*/
				while(time>0)
				{
					LCD_goToRowColumn(1,0);
					LCD_intgerToString(time / 10);  // Print the tens digit of the current value.
					LCD_intgerToString(time % 10);  // Print the unit digit of the current value.
					_delay_ms(300);
					time--;
				}
				//timer1_stop();
				break;
			}
			bool indication_of_true_pass = TRUE;
			LCD_clearScreen();
			LCD_goToRowColumn(0,0);
			if(reptition_of_wrong_pass==0)
				LCD_displayString("enter the password");
			else
				LCD_displayString("wrong, reenter the password");
			LCD_goToRowColumn(1,0);
			for(i=0;i<nuber_of_digits_of_pass;i++) // taking the password from the user
			{
				key2[i] = KeyPad_getPressedKey();
				LCD_displayCharacter('*');
				_delay_ms(50);
				if(key2[i]!=saved_pass[i])
				{
					indication_of_true_pass = FALSE;
					reptition_of_wrong_pass++;
					break;
				}
			}
			if(reptition_of_wrong_pass<3 &&indication_of_true_pass==TRUE ) // case of entering the password correctly
			{
				reptition_of_wrong_pass =0;
				if(key =='#') // case of open the door
				{
					UART_sendByte(success, POLLING);
					LCD_clearScreen();
					LCD_goToRowColumn(0,0);
					LCD_displayString("door open successfully");
					_delay_ms(1000);
					break;
				}
				else // case of change the password
				{
					uint8 new_password[nuber_of_digits_of_pass];
					LCD_clearScreen();
					LCD_goToRowColumn(0,0);
					LCD_displayString("enter the new password");
					LCD_goToRowColumn(1,0);
					for(i=0;i<nuber_of_digits_of_pass;i++)
					{
						new_password[i]= KeyPad_getPressedKey();
						LCD_displayCharacter('*');
						_delay_ms(30);
					}
					for(i=0;i<nuber_of_digits_of_pass;i++)
					{
						EEPROM_writeByte((address_of_password + i),(new_password[i]));
						_delay_ms(10);
					}
					LCD_clearScreen();
					LCD_goToRowColumn(0,0);
					LCD_displayString("***saving new password*****");
					_delay_ms(600);
					break;
				}
			}
		}
	}
}
