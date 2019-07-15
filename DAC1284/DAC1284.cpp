#include "DAC1284.h"  // Class declaration

/*

  ARCHIDUINO by Seletronica s.a.s. - Via Degli Alpini 7 - 12020 VALGRANA (CN) - ITALY
  -----------------------------------------------------------------------------------
  Website and e-shop:         http://www.seletronica.com
  Documentation repository:   http://www.archiduino.com 
  -----------------------------------------------------------------------------------
  Written by: BisoTronic   -> http://www.bisotronic.it
  -----------------------------------------------------------------------------------
  This software is published "as is", without technical support, and with no 
  warranty, express or implied, as to its usefulness for any purpose. Anyway, if you 
  have any question or suggestion, please feel free to contact: info@bisotronic.it
  -----------------------------------------------------------------------------------
  
This library is intended to be used with Analog Devices AD5663 dual 16 bit DAC.

					   *      COMMAND   *     ADDRESS  	*  		  HIGHER DATA BYTE        ||  LOWER DATA BYTE            ||	
					   * 	C2  C1  C0  *	A2  A1  A0 	* 
				----------------------------------------------------------------------------------------------------------
				23	22		21	20	19		18	17	16		15	14	13	12	11	10	09	08	07	06	05	04	03	02	01	00
				----------------------------------------------------------------------------------------------------------
PWR DOWN MODE 	DC	DC		1	0	0		0	0	0		0	0	0	0	0	0	0	0	0	0	1	0	0	0	1	1
RESET           DC	DC		1	0	1		1	1	1		0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
WRITE DAC A		DC	DC		0	1	1		0	0	0		n	n	n	n	n	n	n	n	n	n	n	n	n	n	n	n
WRITE DAC B		DC	DC		0	0	0		0	0	1		n	n	n	n	n	n	n	n	n	n	n	n	n	n	n	n
WRITE ALL DACs	DC	DC		0	0	0		1	1	1		n	n	n	n	n	n	n	n	n	n	n	n	n	n	n	n

DC = DON'T CARE
n  = BINARY VALUE TO BE SET IN DAC REGISTER

Datasheet: http://www.analog.com/media/en/technical-documentation/data-sheets/AD5663.pdf

*/


//-------------------------------------------------------- INIT
void DAC1284::init ()
{
	/*
	SCLK = SCK;
	SYNC = SS;
	DATA = MOSI;
	*/
	
	DDRB  |= B10110000; 		/*	pinMode(SCLK, OUTPUT);
									pinMode(SYNC, OUTPUT);
									pinMode(DATA, OUTPUT);  */                            
								
	PORTB |= (1<<7);			// CLOCK HIGH
	PORTB &= ~(1<<4);			// SYNC  LOW
	
	write(B00100000, 35);	   	// set PWR DOWN MODE to ZERO
	
	delay(50);
	
}

//-------------------------------------------------------- RESET
void DAC1284::reset (byte DAC_CMD)
{

/* 
This is not a real reset (if related to single channel). It simply
sets the channel value to ZERO. If you reset both channels, it 
writes the actual code for software reset (not addressable, I guess).

RESET_ALL   = B00101111;

*/  

	write(DAC_CMD, 0);                              
	delay(5);
	
}

//-------------------------------------------------------- WRITE
void DAC1284::write(byte DAC_CMD, uint16_t DACDataOut)
{

/*
DAC_A  	= B00011000;
DAC_B  	= B00011001;
DAC_ALL = B00011111;
*/
	
	int i = 0;
	int pinState;


	PORTB &= B01111111; 				// digitalWrite(SCLK, LOW);
	PORTB &= B11101111; 				// digitalWrite(SYNC, LOW);
	
	for (i=7; i>=0; i--)  {

		PORTB |= B10000000; 			// digitalWrite(SCLK, HIGH);          
		if ( DAC_CMD & (1 << i)  ) {
		  PORTB |=  B00100000; 			// pinState= 1;
		} else {  
		  PORTB &=  B11011111;			// pinState= 0;
		}
		PORTB &= B01111111;				// digitalWrite(SCLK, LOW);            

	}

	for (i=15; i>=0; i--)  {

		PORTB |= B10000000; 			// digitalWrite(SCLK, HIGH);          
		if ( DACDataOut & (1 << i)  ) {
		  PORTB |=  B00100000; 			// pinState= 1;
		} else {  
		  PORTB &=  B11011111;			// pinState= 0;
		}
		PORTB &= B01111111;				// digitalWrite(SCLK, LOW);    
	}

	PORTB |= B10000000; 				// digitalWrite(SCLK, HIGH);
	PORTB |= B00010000; 				// digitalWrite(SYNC, LOW);
  
}



