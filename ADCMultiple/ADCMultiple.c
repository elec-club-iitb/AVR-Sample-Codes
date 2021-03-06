#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <util/delay.h>
#include "lcd.c"

uint16_t compare = 0;
int count;
uint16_t final = 0;
int res = 0;
int res_prev = 0;
int res_prev_prev = 0;
int main(void)
{
LCD_CONTROL_DDR |= 1<<EN | 1<<RW | 1<<RS;
	_delay_ms(15);
	//DDRC |= 1<<PINC0 | 1<<PINC1 ;
	DDRC = 0xFF;
	Send_A_Command(0x01); //Clear Screen
	_delay_ms(2);
	Send_A_Command(0x38);
	_delay_us(50);
	Send_A_Command(0b00001110);
	_delay_us(50);	


ADCSRA |= 1<<ADPS2;
//ADMUX |= (1<<REFS0)|(1<<REFS1) ;
ADMUX  = 0xC3;
ADCSRA |= 1<<ADIE;
ADCSRA |= 1<<ADEN;

sei();

ADCSRA |= 1<<ADSC;
count = 0;
while (1)
{	

}
}

ISR(ADC_vect)
{
   // PORTC = 0b00001000;
uint8_t theLow  ;
uint16_t theTenBitResult;
//ADMUX = 0xC1;
//while(count<101){
   theLow = ADCL;
   theTenBitResult =ADCH<<8 | theLow;
  // final = theTenBitResult + final;
  // count = count + 1;
//}
//final = final/100;
//final = 0;
switch (ADMUX)
{
case 0xC3:
      if(count<80){
        final = final + theTenBitResult;
        count++;
      }
      else{
        final = final/81;
        compare = final;
        Send_A_Command(0x01);
        _delay_ms(2);
	res = (47000/final);    // Gain of 2.2
	if(res == 231){
		Send_A_String("Conducting");
		Send_A_Command(0xC0);
		Send_A_String("Completely Wet");
		//break;
	}
	else{
	Send_An_Integer(final,4);
	Send_A_String(", ");
        Send_An_Integer(res,4);
        Send_A_String("M ,");
	Send_A_Command(0xC0);
	if(res>300 && res_prev>300 && res_prev_prev >300){
                Send_A_String("Safe Zone!");
	}
	if((res>200 && res<300) && (res_prev>200 && res_prev<300) && (res_prev_prev>200 && res_prev_prev<300)){
		Send_A_String("Warning Zone!");
	}
	if(res<200 && res_prev<200 && res_prev_prev<200){
                Send_A_String("Danger Zone!");
	}
	}
	_delay_ms(250);
        count = 0;
	res_prev_prev = res_prev;
	res_prev = res;
//	final = 0;
  }

//ADMUX = 0xC2;
break;
case 0xC2:

	if(count<100){
        	final = final + theTenBitResult;
        	count++;
      	}
      	else{
        	final = final/101;
        	compare = final;
     //   	Send_A_Command(0x01);
     //   	_delay_ms(2);
//		res = 47188/final; // gain of 4.7
        	Send_An_Integer(final,4);
        	Send_A_String("M ,");
        	_delay_ms(300);
        	count = 0;
//		final = 0;
  }

ADMUX = 0xC1;
break;

//case 0xC2:
//Send_A_Command(0xC0);
  //     _delay_ms(2);
        
        //Send_An_Integer(theTenBitResult,4);
       // Send_A_String(",");

//	_delay_ms(300);
//ADMUX = 0xC3;
//break;

//case 0xC3:

  //      _delay_ms(2);
        
       // Send_An_Integer(theTenBitResult,4);
       // Send_A_String(",");

	//_delay_ms(300);
//ADMUX = 0xC0;
//break;
default:
//Default code
break;
}

//Send_A_Command(0xC0);
  //  _delay_ms(2);
    //Send_An_Integer(compare,4);
    //_delay_ms(200);
    // Simple Processing of the compare value
    

 ADCSRA |= 1<<ADSC;
}
