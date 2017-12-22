/*
 * Sleep_State.c
 * AeVee Labs
 * 
 */ 

/****************************/
/*         INCLUDES			*/
/****************************/
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "General.h"
#include "Sleep_State.h"
#include "IR_Transmit.h"
#include "ATmega48.h"
#include "Timer.h"

/****************************/
/*	      DEFINES			*/
/****************************/
#define SLEEP_ENABLED	SMCR = 0b00000101
#define SLEEP_DISABLED	SMCR = 0b00000100
						
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

PUBLIC void sleep_state_Init(void)
{
	SMCR =
	(0 << bit_0)	|		// SE:	Sleep Enable (initially disabled)
	(0 << bit_1)	|		// SM0:	Power-Down Sleep mode
	(1 << bit_2)	|		// SM1:	Power-Down Sleep mode
	(0 << bit_3)	|		// SM2:	Power-Down Sleep mode
	(0 << bit_4)	|		// reserved
	(0 << bit_5)	|		// reserved
	(0 << bit_6)	|		// reserved
	(0 << bit_7);			// reserved
	
	PRR =					// No Power Reduction for now
	(0 << bit_0)	|		// PRADC:	Power Reduction ADC
	(0 << bit_1)	|		// PRUSART0:Power Reduction UART0
	(0 << bit_2)	|		// PRSPI:	Power Reduction SPI
	(0 << bit_3)	|		// PRTIM1:	Power Reduction Timer/counter1
	(0 << bit_4)	|		// reserved
	(0 << bit_5)	|		// PRTIM0:	Power Reduction Timer/Counter0
	(0 << bit_6)	|		// PRTIM2:	Power Reduction Timer/counter2
	(0 << bit_7);			// PRTWI:	Power Reduction TWI
}

PUBLIC void sleep_state_Control(void)	// Must look at fuses CKSEL, and "Start-Up time see page 28
{
		cli();									
		if(timer_Get(sleep_timer) == 0)					// CLI() prevents ISR from running after this test and causing problems:
		{												// if INT occurs HERE, micro will wake immediately after exectuing sleep_cpu()
			PORTD &= ~(DEVELOPMENT_LED);
			timer_Set(sleep_timer, WAKE_TIME_COUNT);	// reset sleep timer
			SLEEP_ENABLED;								// Enabled to allow entry into Sleep mode
			sei();										// keeps interrupts enabled while Sleeping, so micro can be waken
			sleep_cpu();								// 1st instruction after SEI is guaranteed to be executed before an INT could trigger, so device will be put to sleep.
			SLEEP_DISABLED;								// Execute here when awaken, then return to main( ) loop.  (won't allow entry into sleep mode)
		}		
		sei();			
}
#if 0
	if expired, 
		a: Clear critcal things, maybe call Init_System(), make sure everything that should be "is RESET"  (Except toggle_bit) 
		b: Toggle  toggle_bit
		c: put micro in sleep state
#endif