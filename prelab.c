//********************************************************************************
//Universidad del Valle de Guatemala	
//IE2023: Programacion de Microcontroladores
//Autor: Fernando Gabriel Caballeros Cu
//Proyecto: Laboratorio 04
//Archivo: main.c
//Hardware: ATMega328p
//Created: 07/04/2023
//********************************************************************************
#include <avr/io.h>
#include <util/delay.h>

#define inc PC0
#define dec PC1
#define ledPins {PB4, PB3, BP2, PB1, BP0, PD7, PD6, PD5}

volatile uint8_t counter = 0;

void setup();
void upLeds();


int main(void)
{
    setup();
	//LOOP
    while (1) 
    {
	    uint8_t incrementar = (PINC & (1 << inc)) == 0;
	    uint8_t decrementar = (PINC & (1 << dec)) == 0;

	    if (incrementar && counter < 255) {
		    counter++;
		    upLeds();  
		    _delay_ms(1000);
	    }

	    if (decrementar && counter > 0) {
		    counter--;
		    upLeds();  
		    _delay_ms(1000); 
	    }
    }
}


void setup(void){
	DDRC &= ~(1 << inc);
	PORTC |= (1 << inc);
	DDRC &= ~(1 << dec);
	PORTC |= (1 << dec);

	DDRB |= (1 << DDB4) | (1 << DDB3) | (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
	DDRD |= (1 << DDD7) | (1 << DDD6) | (1 << DDD5);
}

void upLeds() {
	for (int i = 0; i < 8; i++) {
		uint8_t bit = (counter >> i) & 1;  
		if (i < 5) {
			if (bit) PORTB |= (1 << (PORTB4 - i));  
			else PORTB &= ~(1 << (PORTB4 - i));  
			} else {
			if (bit) PORTD |= (1 << (PORTD5 + (i -5))); 
			else PORTD &= ~(1 << (PORTD5 + (i - 5)));  
		}
	}
}