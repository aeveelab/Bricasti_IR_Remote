/*
 * Timer.h
 * AeVee Labs
 *
 */ 


#ifndef TIMER_H_
#define TIMER_H_

/****************************/
/*         INCLUDES			*/
/****************************/

/****************************/
/*	      DEFINES			*/
/****************************/
#define PWM_RUN				106						// compare value for PWM timer to run at 38KHz -- Make PWM_RUN == 65 when using 20Meg crystal
#define PWM_STOP			0xFF					// compare value for PWM timer to stop running (stay low)

//#define IR_XMIT_OFF()			OCR0A = PWM_STOP		// These macros will be used with FAST PWM MODE
//#define IR_XMIT_ON			OCR0A = PWM_RUN			// These macros will be used with FAST PWM MODE
// NON-FAST PWM MODE macro version
#define IR_XMIT_ON				TIMSK0 |= (1 << TIMER0_MATCHA_INT_EN_MASK );		// Enable Timer0 MatchA Interrupt

#define TIMER0_MATCHA_INT_EN_MASK	1
#define IR_XMIT_PIN_MASK			5
#define PWM_50_PERCENT_DUTY_CYCLE	PWM_RUN>>1		// set 50% duty cycle for PWM timer output

// Timer Rate Counts 
// -- SYSTEM CLOCK									// Rate = 8MHZ -- No Clock Div		: Period = 125ns
// -- PreScaled TIMER CLK (ie TICK TIMER clock)		// Rate = 125KHz ( /64 pre-scaler)	: Period = 8us (other opts:8x = 1us OR 256x = 32us OR 1024x = 128us)
#define TICK_TIMER_RATE_COUNT	52					// Time between Interrupts =	424us	-- NOTE USE 132 here for 20MHz crystal
#define BIT_RATE_COUNT			2					// Time = 848us -- corresponds to the time of 1/2 of a bit.
#define REPEAT_RATE_COUNT		256					// Time = 108ms (424us x 256 )
#define BLINK_RATE_COUNT		1187				// Time = 500ms (ie 1 sec on/off cycle)
#define WAKE_TIME_COUNT			2376				// Time = 20 Sec orig, now it is 1 second

/****************************************/
/* STRUCTURE/UNION/ENUM Definitions		*/
/* (with or without Typedefs)			*/
/****************************************/
typedef enum {bit_rate_timer, repeat_rate_timer, led_blink_timer, sleep_timer, timer_Number} timer_t;		// timers whose counts are tracked in timer_array[]
																											// must order timer id's in ascending order
/****************************/
/*   FUNCTION PROTOTYPES	*/
/****************************/
PUBLIC void timer_Init(void);
PUBLIC uint16_t timer_Get(timer_t);
PUBLIC void timer_Set(timer_t, uint16_t);

/********************************************************************/
/*   PUBLIC VARIABLES (Referencing Declarations) (extern)(g_prefix)	*/
/********************************************************************/

/************************************************/
/*	     Inline Function Definitions			*/
/************************************************/



#endif /* TIMER_H_ */