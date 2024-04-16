//********************************************************************************
//Universidad del Valle de Guatemala	
//IE2023: Programacion de Microcontroladores
//Autor: Fernando Gabriel Caballeros Cu
//Proyecto: Laboratorio 04
//Archivo: main.c
//Hardware: ATMega328p
//Created: 07/04/2023
//********************************************************************************
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define inc PC0
#define dec PC1

volatile uint8_t counter = 0;
volatile uint8_t counter_d = 0;

void setup_pins();
void upLeds();
void setup_ADC();


void setup_pins(void){
	DDRC &= ~(1 << inc);
	PORTC |= (1 << inc);
	DDRC &= ~(1 << dec);
	PORTC |= (1 << dec);

	DDRB |= (1 << DDB5) | (1 << DDB4) | (1 << DDB3) | (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
	DDRD |= (1 << DDD7) | (1 << DDD6) | (1 << DDD5) | (1 << DDD4) | (1 << DDD3) | (1 << DDD2) | (1 << DDD0) | (1 << DDD1);
	DDRC |= (1 << DDC5) | (1 << DDC4) | (1 << DDC3) | (1 << DDC2);
	
	sei();
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

void setup_ADC(void){
	ADMUX=0;
	ADCSRA=0;
	ADMUX |= (1 << REFS0);
	ADMUX |= (1 << ADLAR);
	ADMUX |= (1 << MUX1) | (1 << MUX2);
	
	ADCSRA |= (1 << ADIE);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (ADPS0);
	
	ADCSRA |= (1 << ADEN);
	
}

ISR(ADC_vect){
	
	counter_d=ADCH;
	
	ADCSRA |= (1 << ADIF);
	
}
int main(void)
{
    setup_pins();
	setup_ADC();
	uint8_t tabla_seg[] = {0b1000000, 0b1111001, 0b0100100, 0b011000, 0b0011001, 0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0010000, 0b0001000, 0b0000011, 0b1000110, 0b0100001, 0b0000110, 0b0001110};
	int cont  = 0;
	uint8_t disp1 = 0;
	uint8_t disp2 = 0;
	//LOOP
    while (1) 
    {
	    uint8_t incrementar = (PINC & (1 << inc)) == 0;
	    uint8_t decrementar = (PINC & (1 << dec)) == 0;

	    if (incrementar && counter <= 256) {
		    counter++;
		    upLeds();  
		    _delay_ms(100);
	    }

	    if (decrementar && counter > 0) {
		    counter--;
		    upLeds();  
		    _delay_ms(100); 
	    }
		disp1= counter_d & 0b11110000;
		disp2= counter_d >> 4;
		
		if (cont==0){
			
			PORTC &= ~(1<<PINC3);
			PORTC |= (1<<PINC2);
			PORTD= tabla_seg[disp1];
			cont=1;
			}else if (cont==1){
			PORTC &= ~(1<<PINC2);
			PORTC |= (1<<PINC3);
			PORTD= tabla_seg[disp2];
			cont=0;
		}
		
			
		ADCSRA |= (1<<ADSC);
		_delay_ms(10);
    }
	return 0;
}
