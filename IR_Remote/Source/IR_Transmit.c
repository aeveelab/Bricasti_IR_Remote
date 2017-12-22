/*
 * IR_Transmit.c
 * AeVee Labs
 * 
 */ 

/****************************/
/*         INCLUDES			*/
/****************************/
#include <avr/io.h>
#include "General.h"
#include "IR_Transmit.h"
#include "ATmega48.h"
#include "Timer.h"
#include "Switch_Scan.h"

/****************************/
/*	      DEFINES			*/
/****************************/
#define READY_TO_XMIT_STREAM	(timer_Get(repeat_rate_timer) == 0)
#define	READY_TO_XMIT_BIT		(timer_Get(bit_rate_timer) == 0)
#define NUMBER_OF_BITS			14
#define INITIAL_STATE			0

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

// NON-FAST PWM MODE macro version
PRIVATE inline void IR_XMIT_OFF(void)
{					
	TIMSK0 &= ~(1 << TIMER0_MATCHA_INT_EN_MASK );		// Disable Timer0 MatchA Interrupt
	PORTD  &= ~(1 << IR_XMIT_PIN_MASK );				// Clear IR xmit pin
}

//NOTE:The fastest that commands will be sent is 114ms, the resend rate (whether same key press or new key)
PUBLIC void IR_transmit_Init(void)
{
	// nothing here yet
}

PUBLIC void IR_transmit_Control(void)
{
	uint8_t i;
	uint16_t IR_data;										// make this file scope???
	
	if(!READY_TO_XMIT_STREAM)								// Has repeat rate timer expired?
	{
		return;
	}
	
	timer_Set(repeat_rate_timer, REPEAT_RATE_COUNT);		// Reset repeat rate timer for next full command transmission
	timer_Set(bit_rate_timer, INITIAL_STATE);				// Synchronize repeat_rate AND bit_rate timers
	/*  ONLY READ COMMAND at the REPEAT RATE	*/
	//IR_data = IR_queue_Read(void);						// For Q ONLY: READ Q
	IR_data = g_IR_command_code;							// For NO Q ONLY: READ Global
	g_IR_command_code = 0;									// For NO Q ONLY: READ Global:  Clear command buffer
	if(IR_data != 0)										// ANY command to Send?
	{

		for(i=0; i < NUMBER_OF_BITS; i++)					// xmit all bits for one RC-5 IR transmit packet
		{
			if(IR_data & 1<<i)								// Test if next bit to send is a 1 or 0
			{												// xmit a Manchester encoded 1 (Call manchester_Encode(ONE) )
				while(!READY_TO_XMIT_BIT);					// wait for bit clk before transmitting 1st 1/2 bit
				IR_XMIT_OFF();								// xmit low 1/2 of bit
				timer_Set(bit_rate_timer, BIT_RATE_COUNT);	// Reset bit rate timer for next bit transmission
				
				while(!READY_TO_XMIT_BIT);					// wait for bit clk before transmitting 2nd 1/2 bit
				IR_XMIT_ON;									// xmit high 1/2 of bit
				timer_Set(bit_rate_timer, BIT_RATE_COUNT);	// Reset bit rate timer for next bit transmission
				//TCNT0 = 0;								// Restart timer before starting TURNING ON PWM -- FAST PWM MODE ONLY
				
			}
			else
			{												// xmit a Manchester encoded 0 (Call manchester_Encode(ZERO) )
				while(!READY_TO_XMIT_BIT);					// wait for bit clk before transmitting 1st 1/2 bit
				IR_XMIT_ON;									// xmit high 1/2 of bit
				timer_Set(bit_rate_timer, BIT_RATE_COUNT);	// Reset bit rate timer for next bit transmission
				//TCNT0 = 0;									// Restart timer before starting TURNING ON PWM -- FAST PWM MODE ONLY
				
				while(!READY_TO_XMIT_BIT);					// wait for bit clk before transmitting 2nd 1/2 bit
				IR_XMIT_OFF();								// xmit low 1/2 of bit
				timer_Set(bit_rate_timer, BIT_RATE_COUNT);	// Reset bit rate timer for next bit transmission
			}
		}
		while(!READY_TO_XMIT_BIT);							// wait for last 1/2 of last bit to finish transmitting
		IR_XMIT_OFF();										// Turn of IR xmit, during IDLE time
	}	
}
