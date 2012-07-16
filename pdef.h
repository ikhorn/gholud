/* Модуль преобявлений
 * обновлен.....................................................................08.11.2010 10:53:37
*/


#ifndef PDEF_H_
#define PDEF_H_

#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <stdio.h>

//------------------------------------------------------------------------------
//спецификаторы
//------------------------------------------------------------------------------

#define eeprom_									//__eeprom
#define flash_									__attribute__((progmem))
#define no_init_								//__no_init
#define interrupt_								__interrupt
#define enable_interrupts_						sei()
#define disable_interrupts_						cli()

#define NOP_()									asm("NOP");
#define NOP2_()									NOP_();NOP_();
#define NOP3_()									NOP2_();NOP_();
#define NOP4_()									NOP2_();NOP2_();
#define NOP5_()									NOP4_();NOP_();
#define NOP6_()									NOP5_();NOP_();
#define NOP7_()									NOP6_();NOP_();

//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------

#define B(X)									(1<<(X))
#define CLRB(RG, X)								(RG) &= ~B(X)
#define SETB(RG, X)								(RG) |= B(X)
#define INVB(RG, X)								(RG) ^= B(X)
#define CHKB(RG, X)								((RG) & B(X))
#define WTHB(RG, X)								((RG) | B(X))

#define BIT(X)									(1<<(X))
#define CLRBIT(RG, X)							(RG) &= ~BIT(X)
#define SETBIT(RG, X)							(RG) |= BIT(X)
#define INVBIT(RG, X)							(RG) ^= BIT(X)
#define CHKBIT(RG, X)							((RG) & BIT(X))
#define WTHBIT(RG, X)							((RG) | BIT(X))
#define SETMSK(RG, MASK)						(RG) |= (MASK)
#define CLRMSK(RG, MASK)						(RG) &= ~(MASK)
#define CHKMSK(RG, MASK)						((RG) & (MASK))
#define STRSIZE(STR)							(sizeof(STR)/sizeof(STR[0]))
/*
#define ___CPU_3686400			3686400
#define ___CPU_5529600			5529600
#define ___CPU_7372800			7372800
#define ___CPU_14745600			14745600
#define CPU_FRQ									___CPU_3686400					//тактовая частота*/
/*
#define ___CPU_ATMEGA128		1
#define ___CPU_ATTINY2313		2
#define CPU__									___CPU_ATMEGA128				//процессор*/

#define PTRY									2								//количество попыток


#include "types.h"
#include "sta.h"
#include "stm.h"

#endif


