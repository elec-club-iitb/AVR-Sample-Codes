/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Created By: Riddhish Bhalodia

Refrences: Patrick Hood-Daniel

*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
int main(void){	

	LCD_CONTROL_DDR |= 1<<EN | 1<<RW | 1<<RS;
	_delay_ms(15);
//	DDRB  |= 1<<PINB1;	
//	DDRC |= 1<<PINC0 | 1<<PINC1 ;

	Send_A_Command(0x01); //Clear Screen
	_delay_ms(2);
	Send_A_Command(0x38);
	_delay_us(50);
	Send_A_Command(0b00001110);
	_delay_us(50);	
	Send_A_Command(0x83);
	        Send_A_String("35");

	while(1){ 
//	PORTB ^= 1<<PORTB1;
//	Send_A_String("Riddhish");
//	_delay_ms(10000);
//	Send_A_Command(0x01);
//	_delay_ms(2);	
	}
	return 0;
}
void Check_Busy()
{
	LCD_DDR = 0;
	LCD_CONTROL_PORT |= 1<<RW;
	LCD_CONTROL_PORT &= ~1<<RS;

	while (LCD_PORT >= 0x80){
		EN_Toggle();
	}
	LCD_DDR = 0xFF; 
}
void EN_Toggle()
{
	LCD_CONTROL_PORT |= 1<<EN;
	asm volatile ("nop");
	asm volatile ("nop");
	LCD_CONTROL_PORT &= ~1<<EN;
}

void Send_A_Command(unsigned char command)
{
	Check_Busy();
	LCD_PORT = command;
	LCD_CONTROL_PORT &= ~ ((1<<RW)|(1<<RS));
	EN_Toggle();
	LCD_PORT = 0;
}

void Send_A_Character(unsigned char character)
{
	Check_Busy();
	LCD_PORT = character;
	LCD_CONTROL_PORT &= ~ (1<<RW);
	LCD_CONTROL_PORT |= 1<<RS;
	EN_Toggle();
	LCD_PORT = 0;
}

void Send_A_String(char *StringOfCharacters)
{
	while(*StringOfCharacters > 0){
		Send_A_Character(*StringOfCharacters++);
	}
}

void Send_An_Integer(int IntegerToDisplay, char NumberOfDigits)
{
	char StringToDisplay[NumberOfDigits];
	itoa(IntegerToDisplay, StringToDisplay, 10);
	Send_A_String(StringToDisplay); Send_A_String(" ");
}
