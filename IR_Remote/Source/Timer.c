/*
 * Timer.c
 * AeVee Labs
 * 
 */ 

/****************************/
/*         INCLUDES			*/
/****************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "General.h"
#include "Timer.h"

/****************************/
/*	      DEFINES			*/
/****************************/

/****************************************/
/* STRUCTURE/UNION/ENUM Definitions		*/
/* (with or without Typedefs)			*/
/****************************************/

/****************************/
/*   PRIVATE VARIABLES		*/
/****************************/
PRIVATE uint16_t timer_array[timer_Number] = {0, 0, 0, WAKE_TIME_COUNT};	// may ultimately leave Sleep time at 0
	
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
 #if 0
Note: 
	1: Because IR_PWM_Controll() will maintain program flow control thru multiple "bit rate" timer counter interrupts, the timer count array must
	be updated in the Timer_Tick(ISR) itself.  If array counts weren't update in ISR, there wouldn't be a chance for a timer_control() to update timer count array, 
	since it would not get program flow control during these multiple interrupts.
#endif


PUBLIC void timer_Init(void)
{
#if 0	/* DISABLE PWM on TIMER 0 -- FOR NOW */

	/********************************/
	/*   PWM TIMER SETUP (Timer0)	*/
	/********************************/
	OCR0A = PWM_STOP;					// set the 38kHz PWM Timer to Stop Mode
	OCR0B = PWM_50_PERCENT_DUTY_CYCLE;	// set the duty cycle for the PWM timer output to 50%
	TCCR0A =
	(1 << bit_0)	|		// WGM00: Fast PWM mode, OCRA is TOP
	(1 << bit_1)	|		// WGM01: Fast PWM mode, OCRA is TOP
	(0 << bit_2)	|		// reserved bit
	(0 << bit_3)	|		// reserved bit
	(1 << bit_4)	|		// COM0B0: Set OC0B on Compare match (inverting mode)
	(1 << bit_5)	|		// COM0B1: Set OC0B on Compare match (inverting mode)
	(0 << bit_6)	|		// COM0A0: OCOA disconnected from pin
	(0 << bit_7);			// COM0A1: OCOA disconnected from pin
	
	TCCR0B =
	(1 << bit_0)	|		// CS00: timer clock select: clk_IO (no prescaling)
	(0 << bit_1)	|		// CS01: timer clock select: clk_IO (no prescaling)
	(0 << bit_2)	|		// CS02: timer clock select: clk_IO (no prescaling)
	(1 << bit_3)	|		// WGM02: Fast PWM mode, OCRA is TOP
	(0 << bit_4)	|		// reserved bit
	(0 << bit_5)	|		// reserved bit
	(0 << bit_6)	|		// FOC0B: No forced output
	(0 << bit_7);			// FOc0A: No forced output 
	
	TIMSK0 = 
	(0 << bit_0)	|		// TOIE0: Timer/Counter overflow interrupt Disabled
	(0 << bit_1)	|		// OCIE0A: Timer/Counter A output compare match interrupt Disabled
	(0 << bit_2)	|		// OCIE0B: Timer/Counter B output compare match interrupt Disabled
	(0 << bit_3)	|		// reserved bit
	(0 << bit_4)	|		// reserved bit
	(0 << bit_5)	|		// reserved bit
	(0 << bit_6)	|		// reserved bit
	(0 << bit_7);			// reserved bit	
#endif	
/********************************/
/* Set up faux PWM on Timer 0	*/
/********************************/
	OCR0A = PWM_RUN;		// set the 38kHz PWM Timer to Stop Mode -- Make PWM_RUN == 65 when using 20Meg crystal
	
	TCCR0A =
	(0 << bit_0)	|		// WGM00: CTC mode of operation
	(1 << bit_1)	|		// WGM01: CTC mode of operation
	(0 << bit_2)	|		// reserved bit
	(0 << bit_3)	|		// reserved bit
	(0 << bit_4)	|		// COM0B0: OCOB disconnected from pin
	(0 << bit_5)	|		// COM0B1: OCOB disconnected from pin
	(0 << bit_6)	|		// COM0A0: OCOA disconnected from pin
	(0 << bit_7);			// COM0A1: OCOA disconnected from pin
	
	TCCR0B =
	(1 << bit_0)	|		// CS00: timer clock select: clk_IO (no prescaling)-- Make prescaling x8 when using 20Meg crystal
	(0 << bit_1)	|		// CS01: timer clock select: clk_IO (no prescaling)-- Make prescaling x8 when using 20Meg crystal
	(0 << bit_2)	|		// CS02: timer clock select: clk_IO (no prescaling)-- Make prescaling x8 when using 20Meg crystal
	(0 << bit_3)	|		// WGM02: CTC mode of operation
	(0 << bit_4)	|		// reserved bit
	(0 << bit_5)	|		// reserved bit
	(0 << bit_6)	|		// FOC0B: No forced output
	(0 << bit_7);			// FOc0A: No forced output 
	
	TIMSK0 = 
	(0 << bit_0)	|		// TOIE0: Timer/Counter overflow interrupt Disabled
	(0 << bit_1)	|		// OCIE0A: Timer/Counter A output compare match interrupt Initially DISABLED
	(0 << bit_2)	|		// OCIE0B: Timer/Counter B output compare match interrupt Disabled
	(0 << bit_3)	|		// reserved bit
	(0 << bit_4)	|		// reserved bit
	(0 << bit_5)	|		// reserved bit
	(0 << bit_6)	|		// reserved bit
	(0 << bit_7);			// reserved bit
	
	/********************************/
	/*   TICK TIMER SETUP (Timer2)	*/
	/********************************/
	OCR2A = TICK_TIMER_RATE_COUNT;		// sets the terminal count for a TICK Timer running at 38KHz
	
	TCCR2A =
	(0 << bit_0)	|		// WGM20: CTC mode of operation
	(1 << bit_1)	|		// WGM21: CTC mode of operation
	(0 << bit_2)	|		// reserved bit
	(0 << bit_3)	|		// reserved bit
	(0 << bit_4)	|		// COM2B0: OCOb disconnected from pin
	(0 << bit_5)	|		// COM2B1: OCOB disconnected from pin
	(0 << bit_6)	|		// COM2A0: OCOA disconnected from pin
	(0 << bit_7);			// COM2A1: OCOA disconnected from pin
	
	TCCR2B =
	(0 << bit_0)	|		// CS20: timer clock select: clk_IO/64
	(0 << bit_1)	|		// CS21: timer clock select: clk_IO/64
	(1 << bit_2)	|		// CS22: timer clock select: clk_IO/64
	(0 << bit_3)	|		// WGM22: CTC mode of operation
	(0 << bit_4)	|		// reserved bit
	(0 << bit_5)	|		// reserved bit
	(0 << bit_6)	|		// FOC2B: No forced output
	(0 << bit_7);			// FOc2A: No forced output 
	
	TIMSK2 = 
	(0 << bit_0)	|		// TOIE2: Timer/Counter overflow interrupt Disabled
	(1 << bit_1)	|		// OCIE2A: Timer/Counter A output compare match interrupt Enabled
	(0 << bit_2)	|		// OCIE2B: Timer/Counter B output compare match interrupt Disabled
	(0 << bit_3)	|		// reserved bit
	(0 << bit_4)	|		// reserved bit
	(0 << bit_5)	|		// reserved bit
	(0 << bit_6)	|		// reserved bit
	(0 << bit_7);			// reserved bit
}

PUBLIC uint16_t timer_Get(timer_t timer)
{
	// critical section: probably wouldn't be needed with uint8_t, 8 bit access is guaranteed atomic
	uint16_t temp_time;
	
	cli();
	temp_time = timer_array[timer];		// access time remaining on requested timer
	sei();
		
	return(temp_time); 
}

PUBLIC void timer_Set(timer_t timer, uint16_t time)
{
	// critical section: probably wouldn't be needed with uint8_t, 8 bit access is guaranteed atomic
	cli();
	timer_array[timer] = time;			// setup timer for requested time
	sei();
}

/********************************************/
/*				TICK TIMER ISR				*/
/* Timer2 Compare Match A Interrupt Handler	*/
/********************************************/
ISR(TIMER2_COMPA_vect)
{
	uint8_t i;
	
	for(i=0; i < timer_Number; i++)	
	{
		if(timer_array[i])
		{
			timer_array[i]--;
		}
	}	
}

/************************************************/
/*	PWM TIMER ISR's								*/
/************************************************/

/************************************************/
/*  IR XMIT:  When ENABLED IR XMIT is ACTIVE	*/
/************************************************/
//Timer0 Compare Match A Interrupt Handler
ISR(TIMER0_COMPA_vect)
{
	PORTD  ^= 1 << IR_XMIT_PIN_MASK;				// TOGGLe IR xmit pin
}
// Timer0 Compare Match B Interrupt Handler	-- DISABLED
ISR(TIMER0_COMPB_vect)
{
	
}
// Timer0 Overflow Interrupt Handler	-- DISABLED
ISR(TIMER0_OVF_vect)
{
	
}