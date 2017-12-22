/*
 * Switch_Scan.h
 * AeVee Labs
 *
 */ 


#ifndef Switch_Scan_H_
#define Switch_Scan_H_

/****************************/
/*         INCLUDES			*/
/****************************/

/****************************/
/*	      DEFINES			*/
/****************************/


/****************************************/
/* STRUCTURE/UNION/ENUM Definitions		*/
/* (with or without Typedefs)			*/
/****************************************/

/****************************/
/*   FUNCTION PROTOTYPES	*/
/****************************/
PUBLIC void switch_scan_Init(void);
PUBLIC void switch_scan_Control(void);

/********************************************************************/
/*   PUBLIC VARIABLES (Referencing Declarations) (extern)(g_prefix)	*/
/********************************************************************/
extern uint16_t g_IR_command_code;

/************************************************/
/*	     Inline Function Definitions			*/
/************************************************/



#endif /* Switch_Scan_H_ */