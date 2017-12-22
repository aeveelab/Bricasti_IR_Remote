/*
 * Main.c
 * AeVee Labs
 * 
 */ 

/****************************/
/*         INCLUDES			*/
/****************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "General.h"
#include "ATmega48.h"
#include "IR_Transmit.h"
#include "Sleep_State.h"
#include "Switch_Scan.h"
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

/************************************/
/* PRIVATE FUNCTION PROTOTYPES		*/
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

/************************************/
/*  LED blink (development only)	*/
/************************************/
PRIVATE void led_blink_Control(void)
{
	if(timer_Get(led_blink_timer) == 0 )
	{
		timer_Set(led_blink_timer, BLINK_RATE_COUNT);
		PIND = DEVELOPMENT_LED;						// writing a 1 to PINxn toggles the value of PORTxn, independent of DDRxn.	
	}
}

/****************************************************/
/* init_System(): Call init routine for each module	*/
/****************************************************/
PRIVATE void init_System(void)
{
	switch_scan_Init();
	IR_transmit_Init();
	sleep_state_Init();
	timer_Init();
}

/****************************************/
/* main():	Program entry point			*/
/****************************************/

int main(void)
{
    configure_Micro();					// configure: IO Ports, Clock, Power, Interrupts, etc
	init_System();
	ENABLE_INTERRUPTS();

	while(1)
    {
        switch_scan_Control();			// scan for valid single key presses and load command buffer with valid key
		IR_transmit_Control();			// Sends any valid command to PWM machine at the RESET/SEND rate, and then on to IR driver PIN.
		sleep_state_Control();			// check if sleep timer has expired.  If so, initiates Sleep mode
		led_blink_Control();			// blinking LED, for development only
		//WDT();
    }
}