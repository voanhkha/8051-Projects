/*******************************************************************
  *	@file 		main.h
  * @author  	MinhHaGroup
  * @version 	V1.0.0
  * @date   
  * @brief  	File  cau hinh su dung cho AT89S52
********************************************************************/
#ifndef __MAIN_
#define __MAIN_

#include <regx52.h>
#include <DELAY\delay.h>
#include <GLCD\glcd.h>
#include "i2c/i2c.h"
#include "24cxx/24cxx.h"

/* =================CONFIGURATION======================= */
// --------------------------------------------------------
// PORT0 --------------------------------------------------
sbit LCD_RS=P1^2;
sbit LCD_RW=P1^3;
sbit LCD_E=P1^4;
sbit LCD_PSB =P1^5;		
sbit LCD_RST =P0^7;
#define SCL P3_4
#define SDA P3_5
#define OUT P1

// PORT1 --------------------------------------------------
// PORT2 --------------------------------------------------
#define LCD_Data P2
#define Busy    0x80 

// PORT3 --------------------------------------------------

#endif
/* -----------------END OF FILE--------------------------- */