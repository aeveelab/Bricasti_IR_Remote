/*
 * General.h
 * AeVee Labs
 *
 */ 


#ifndef General_H_
#define General_H_

/****************************/
/*         INCLUDES			*/
/****************************/

/****************************/
/*	      DEFINES			*/
/****************************/
#if 0
	/* INTERRUPTS: save, disable, ... restore */ 
	uint8_t cSREG;
	cSREG = SREG;	// store SREG value
	_CLI();
	...
	SREG = cSREG	// Restore SREG value (I-bit)
#endif

#define ENABLE_INTERRUPTS()		sei()
#define DISABLE_INTERRUPTS()	cli()
#define PUBLIC
#define PRIVATE	static
#define HIGH				1
#define LOW					0
#define TRUE				1
#define FALSE				0

#define bit_0				0
#define bit_1				1
#define bit_2				2
#define bit_3				3
#define bit_4				4
#define bit_5				5
#define bit_6				6
#define bit_7				7

/****************************************/
/* STRUCTURE/UNION/ENUM Definitions		*/
/* (with or without Typedefs)			*/
/****************************************/

/****************************/
/*   FUNCTION PROTOTYPES	*/
/****************************/

/********************************************************************/
/*   PUBLIC VARIABLES (Referencing Declarations) (extern)(g_prefix)	*/
/********************************************************************/

/************************************************/
/*	     Inline Function Definitions			*/
/************************************************/



#endif /* General_H_ */