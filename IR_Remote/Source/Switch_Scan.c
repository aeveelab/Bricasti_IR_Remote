/*
 * Switch_Scan.c
 * AeVee Labs
 * 
 */ 

/****************************/
/*         INCLUDES			*/
/****************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "General.h"
#include "Switch_Scan.h"
#include "Timer.h"

/****************************/
/*	      DEFINES			*/
/****************************/
#define READ_KEYS			(switch_bits = PINC)
#define SAME_KEY			(saved_switch_bits == switch_bits)
#define VALID_KEY_PRESSED	(switch_bits == 	(switch_bits & -switch_bits) && switch_bits != 0)

#define NUM_SWITCH			(6)
#define SET_TOGGLE_BIT		g_IR_command_code |= TOGGLE_BIT_MASK 
#define CLEAR_TOGGLE_BIT	g_IR_command_code &= ~TOGGLE_BIT_MASK
#define TOGGLE_BIT_MASK		(1 << 2)
#define SWITCH_MASK			0x3F

/****************************************/
/* STRUCTURE/UNION/ENUM Definitions		*/
/* (with or without Typedefs)			*/
/****************************************/

/****************************/
/*   PRIVATE VARIABLES		*/
/****************************/
static uint16_t command_table[NUM_SWITCH] =
{
			// 00/c0c1c2c3c4c5/a0a1a2a3a4/T/s2s1
	0x20DB,	//   00/100000/11011/0/11  command = 0x01
	0x10DB,	//   00/010000/11011/0/11  command = 0x02
	0x30DB,	//   00/110000/11011/0/11  command = 0x03
	0x08DB,	//   00/001000/11011/0/11  command = 0x04 
	0x28DB,	//   00/101000/11011/0/11  command = 0x05 
	0x18DB	//   00/011000/11011/0/11  command = 0x06 
};
/************************************/
/*   PRIVATE FUNCTION PROTOTYPES	*/
/************************************/


/********************************************************/
/* PUBLIC VARIABLES (Defining Declarations) (g_prefix)	*/
/********************************************************/
uint16_t g_IR_command_code = 0;

/************************************************/
/*   Source code								*/
/*	     Inline Function Definitions			*/
/*	     Public Function Definitions			*/
/*	     Private Function Definitions			*/
/************************************************/
PUBLIC void switch_scan_Init(void)
{
	/* Setup Pin Change interrupt on PORTC, for remote Switch Scan (6 switches */
	PCICR =					// Pin Change Interrupt Control Register
	(0 << bit_0)	|		// PCIE0: Disabled
	(1 << bit_1)	|		// PCIE1: Enabled (PORTC: interrupt on pin-change)
	(0 << bit_2)	|		// PCIE2: Disabled
	(0 << bit_3)	|		// reserved bit
	(0 << bit_4)	|		// reserved bit
	(0 << bit_5)	|		// reserved bit
	(0 << bit_6)	|		// reserved bit
	(0 << bit_7);			// reserved bit
	
	PCMSK1 =				// Pin change Mask register 1 (PORTC pins)
	(1 << bit_0)	|		// PCINT8:  Enabled (SW_DOWN)
	(1 << bit_1)	|		// PCINT9:  Enabled (SW_UP)
	(1 << bit_2)	|		// PCINT10: Enabled (SW_LEVEL)
	(1 << bit_3)	|		// PCINT11: Enabled (SW_STATUS)
	(1 << bit_4)	|		// PCINT12: Enabled (SW_FILTER)
	(1 << bit_5)	|		// PCINT13: Enabled (SW_INPUT)
	(0 << bit_6)	|		// PCINT14: Disabled (used for RESET function)
	(0 << bit_7);			// reserved: NA
}

/********************************************************************************/
/* switch_scan_Control: Handle the scanning of the switches.  load the single	*/
/*						byte command buffer with ONE valid command, each IR		*/
/*						Transmit Repeat cycle.									*/
/********************************************************************************/
PUBLIC void switch_scan_Control(void)	// This function would go away with Q
{
	static uint8_t toggle_bit = 0;
	static uint8_t saved_switch_bits;
	static uint16_t saved_IR_command_code;
	uint8_t switch_bits = 0;
	uint8_t count;
	
	// DEBOUNCE SWITCHES note ISR on pin change, kinda does this automatically based on latency to change
	
	READ_KEYS;									// assign switch status to switch_bits
	switch_bits = ~switch_bits;					// correct for switch polarity
	switch_bits &= SWITCH_MASK;					// isolate switch inputs
	/********************************************/
	/*		Handle InValid key Presses			*/
	/********************************************/
	if(!VALID_KEY_PRESSED)						// NO Key pressed OR MULTIPLE Keys pressed
	{
		saved_switch_bits = 0;					// Update saved switch status and prepare for next scan
		return;
		//g_IR_command_code = 0;				// clear buffer in iR_Transmit, once the command is about to be sent/  This will
	}											// insure a button press and fast release before sending, will still be sent 
	
	timer_Set(sleep_timer, WAKE_TIME_COUNT);	// reset sleep timer
	/********************************************/
	/*		Handle Valid Single Key Presses		*/
	/********************************************/
	if(!SAME_KEY)								// Was this SINGLE KEY just pressed for the 1st time
	{											// RISK: Same key pressed repeatedly TOO FAST! (ie missing the release state)
		saved_switch_bits = switch_bits;		// Yes
		toggle_bit = !toggle_bit;				// adjust toggle bit
		
		count=0;
		while(switch_bits)								// find index into command table for new key
		{
			switch_bits >>= 1;							// This LOOP will CLEAR switch_bits (ie the Switch Status buffer)		
			count++;
		}
		g_IR_command_code = command_table[count-1];		// get appropriate FULL COMMAND_CODE from table
		
		if(toggle_bit)									// Insert the Toggle bit
		{
			SET_TOGGLE_BIT;  
		}
		else
		{
			CLEAR_TOGGLE_BIT;
		}
		saved_IR_command_code = g_IR_command_code;	// keep track for Keys that are HELD DOWN
	}		
	else
	{
		g_IR_command_code = saved_IR_command_code;	// This is a HELD DOWN KEY
	}	

}	

/********************************************************/
/* Interrupt Handler: Pin change Interrupt Request 1	*/
/********************************************************/
ISR(PCINT1_vect)		// Pin Change Interrupt Request 1 FOR NO Q
{
	// This routine is entered any time one of the (6) remote switches is pressed or released.
	// For now it will just be used to Wake from Sleep or to Update Sleep timer.
	// note: PCIF1 is auto cleared
	// 1: may eventually write to a command Q, if Polling switches doesn't cut it  
	//		a: Valid Key: write command from table into command Q (circular buffer)
	//		b: key Release or Invalid Key: write 0 into command Q (circular buffer)
	//		c: in switch_scan_Control(), when I read from Q, I'll refill Q with same code as done for Q at init 0xFF
	//		d: also in switch_scan_Control(), I may double check PORT to make sure we didn't miss a switch release and are stuck xmitting when we shouldn't be
	
	// 2: PCIFR-- Pin change interrupt flag register, bit PCIF2 is set when IR_RECOUT pin toggles
	//     bit PCIF2 is cleared by hardware when this ISR executes, does not have to be manually cleared.
	//     see data sheet page 81.  
	
	// 3: executing this interrupt will somehow Wake up micro from Sleep on any pin change: If we ARE SLEEPING
	
	/********************/
	/* Wake up Micro	*/
	/********************/
	// Not sure yet what to do, if anything
	
	/********************/
	/*Reset Sleep timer */
	/********************/
	timer_Set(sleep_timer, WAKE_TIME_COUNT);		// may want to set Flag and do in switch_scan_Control(void)
}
#if 0
/************************************/
/* PUT ALL of this in Switch_Scan.h */
/************************************/	
PUBLIC uint16_t IR_queue_Read(void);

/************************************/
/* PUT ALL of this in Switch_Scan.c */
/************************************/
static uint16_t command_Q[Q_SIZE] = {0};
static uint8_t toggle_bit = 0;
static uint8_t read_ptr = 0;
static uint8_t write_ptr = 0;

#define Q_SIZE				10					// MAKE THI BIG ENOUGH SO that it CANT GET FILLED
#define EMPTY_QUEUe			(read_ptr == write_ptr)
#define BASE_COMMAND		0b0000000011011011	// 0b/00/C0C1C2C3C4C5/11011/T/11  0b/00/c0c1c2c3c4c5/a0a1a2a3a4/T/s2s1		// place at the top of this file	
#define COMMAND_BIT_MASK	9


/* Pin Change Interrupt Request 1 */
ISR(PCINT1_vect)								
{
	uint8_t switch_bits = 0;
	
	if(READ_KEYS == 0)							// Was this a button released interrupt?
	{
		toggle_bit = !toggle_bit;				// Yes.  Toggle the toggle bit.
	}
	else if(VALID_KEY_PRESSED)					// Was only one key pressed, Not multiple keys?
	{
		IR_queue_Write(switch_bits);			// Yes, write command into command Queue  
	}
}
PRIVATE inline void IR_queue_Write(uint8_t switch_bits)	// only called from within PC1 ISR, so no Critical sections
{
	/* Q is big enough that it can't be Filled ???? */
	command_Q[write_ptr] = BASE_COMMAND | (toggle_bit << TOGGLE_BIT_MASK) | (switch_bits << COMMAND_BIT_MASK);
	write_ptr = ++write_ptr % Q_SIZE;			// inc write ptr, consider wrap
}

PUBLIC uint16_t IR_queue_Read(void)
{
	uint16_t IR_data = 0;					// Clear IR_data for return, if Q is empty;
	
	if(!EMPTY_QUEUe )
	{
		cli();
			IR_data = command_Q[read_ptr];	// Read Q
		sei();
		read_ptr = ++read_ptr % Q_SIZE;		// inc rd ptr, consider wrap (read_prt not accessed in ISR)
	}	
	return (IR_data);
}
#endif