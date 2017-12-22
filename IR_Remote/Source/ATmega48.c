/*
 * ATmega48.c
 * AeVee Labs
 * 
 */ 

/****************************/
/*         INCLUDES			*/
/****************************/
#include <avr/io.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "General.h"
#include "ATmega48.h"

/****************************/
/*	      DEFINES			*/
/****************************/
#define CLKPS_MASK		0x0F
#define CLK_DIV(x)		CLKPR = (1 << CLKPCE);	\
						CLKPR = x & CLKPS_MASK
						
/****************************************/
/* STRUCTURE/UNION/ENUM Definitions		*/
/* (with or without Typedefs)			*/
/****************************************/

/****************************/
/*   PRIVATE VARIABLES		*/
/****************************/

/************************************/
/*   PRIVATE FUNCTION PROTOTYPES	*/
/************************************/

/********************************************************/
/* PUBLIC VARIABLES (Defining Declarations) (g_prefix)	*/
/********************************************************/

/************************************************/
/*   Source code								*/
/*	     Inline Function Definitions			*/
/*	     Public Function Definitions			*/
/*	     Private Function Definitions			*/
/************************************************/

/************************/
/* configure IO Ports	*/
/************************/
PRIVATE void configure_Ports(void)
{
	// see data sheet page 78
	
	/********************/
	/* configure Port B	*/
	/********************/
	
	// pin direction
	DDRB =
	(0 << bit_0)	|		// PB0 is configured as input	--> NC
	(0 << bit_1)	|		// PB1 is configured as input	--> NC
	(0 << bit_2)	|		// PB2 is configured as input	--> NC
	(0 << bit_3)	|		// PB3: will be Alt funct. (input)	--> UC_MOSI	// SLAVE Data input (no pull-up)
	(1 << bit_4)	|		// PB4: will be Alt funct. (output)	--> UC_MISO	// SLAVE Data output
	(0 << bit_5)	|		// PB5: will be Alt funct. (input)	--> UC_SCK	// SLAVE Clock input (no pull-up)
	(0 << bit_6)	|		// PB6: will be Alt funct. (input)	--> XTAL1	// input to the inverting oscillator amp (no pull-up)
	(1 << bit_7);			// PB7: will be Alt funct. (output)	--> XTAL2	// output from the inverting oscillator amp	
	
	// pull-up resistors	
	PORTB =
	(1 << bit_0)	|		// PB0, pull-up ACTIVE
	(1 << bit_1)	|		// PB1, pull-up ACTIVE
	(1 << bit_2)	|		// PB2, pull-up ACTIVE
	(0 << bit_3)	|		// PB3, pull-up INACTIVE
	(0 << bit_4)	|		// PB4, Output is Initialized to Zero level (UC_MISO)
	(0 << bit_5)	|		// PB5, pull-up INACTIVE
	(0 << bit_6)	|		// PB6, pull-up INACTIVE
	(0 << bit_7);			// PB7, Output is Initialized to Zero level (XTAL2)
	
	/********************/
	/* configure Port C	*/
	/********************/

	// pin direction
	DDRC =
	(0 << bit_0)	|		// PC0 is configured as input		--> SW_DOWN		--(no internal pull-up)
	(0 << bit_1)	|		// PC1 is configured as input		--> SW_UP		--(no internal pull-up)	
	(0 << bit_2)	|		// PC2 is configured as input		--> SW_LEVEL	--(no internal pull-up)
	(0 << bit_3)	|		// PC3 is configured as input		--> SW_STATUS	--(no internal pull-up)
	(0 << bit_4)	|		// PC4 is configured as input		--> SW_FILTER	--(no internal pull-up)
	(0 << bit_5)	|		// PC5 is configured as input		--> SW_IPUT		--(no internal pull-up)
	(0 << bit_6);			// PC6: will be Alt funct. (input)	--> UC_RESET	--(no internal pull-up)
		
	// pull-up resistors	
	PORTC =
	(0 << bit_0)	|		// PC0, pull-up INACTIVE
	(0 << bit_1)	|		// PC1, pull-up INACTIVE
	(0 << bit_2)	|		// PC2, pull-up INACTIVE
	(0 << bit_3)	|		// PC3, pull-up INACTIVE
	(0 << bit_4)	|		// PC4, pull-up INACTIVE
	(0 << bit_5)	|		// PC5, pull-up INACTIVE
	(0 << bit_6);			// PC6, pull-up INACTIVE

	
	/********************/
	/* configure Port D	*/
	/********************/

	// pin direction
	DDRD =
	(0 << bit_0)	|		// PD0 is configured as input	--> NC
	(0 << bit_1)	|		// PD1 is configured as input	--> NC
	(0 << bit_2)	|		// PD2 is configured as input	--> NC
	(1 << bit_3)	|		// PD3 is configured as output	--> "blinking development LED"
	(0 << bit_4)	|		// PD4 is configured as input	--> NC
	(1 << bit_5)	|		// PD5: Alt function (output)	--> OC0B / IR_XMIT
	(0 << bit_6)	|		// PD6 is configured as input	--> NC
	(0 << bit_7);			// PD7 is configured as input	--> NC
		
	// pull-up resistors	
	PORTD =
	(1 << bit_0)	|		// PD0, pull-up ACTIVE
	(1 << bit_1)	|		// PD1, pull-up ACTIVE
	(1 << bit_2)	|		// PD2, pull-up ACTIVE
	(0 << bit_3)	|		// PD3, Output is Initialized to Zero level (LED OFF)
	(1 << bit_4)	|		// PD4, pull-up ACTIVE
	(0 << bit_5)	|		// PD5, Output is Initialized to Zero level (IR XMIT OFF)
	(1 << bit_6)	|		// PD6, pull-up ACTIVE
	(1 << bit_7);			// PD7, pull-up ACTIVE
	
}

PRIVATE void configure_Clocks()
{
	// full Swing crystal oscillator fuse cksel3...1 -> 011
	// start up time, slowly rising power: CKSEL0 SUT1...0   -> 1 11 
	
	/*  Set clock division	*/
	//CLK_DIV(0);						// set clock divide to 1, using private macro
	clock_prescale_set(clock_div_1);	// set clock divide to 1, using supplied function
}

PRIVATE void configure_WDT()
{
	wdt_disable();
	// Do this later
}

PUBLIC void configure_Micro(void)
{
	configure_Ports();
	configure_Clocks();
	//configure_Interrupts();
	//configure_Power();
	configure_WDT();
	//configure_misc_SystemRegs();
}

#if 0
PRIVATE void configure_Interrupts(void)
{
	// Handled in the individual drivers
}

PRIVATE void configure_Power()
{
	// not required
}

PRIVATE void configure_misc_SystemRegs()
{
	
}

/* Not Sure about this one yet */
ISR(WDT_vect)		//Watchdog Timeout Interrupt
{
	
}
#endif