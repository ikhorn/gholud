/* Файл с сигналами для различных таймеров, будильников, напоминалок и т.д
 * обновлен.....................................................................06.01.2011 12:40:57
*/

#include "pdef.h"

//------------------------------------------------------------------------------
//мелодии
//------------------------------------------------------------------------------

#define SGN_MEL_BLINK_1							0
#define SGN_MEL_BLINK_2							1
#define SGN_MEL_BOMB							2
#define SGN_MEL_CHAOS							3
#define SGN_MEL_MARCH							4
#define SGN_MEL_SILENCE							5
#define SGN_MEL_EMERGE							6
#define SGN_MEL_PUM								7
#define SGN_MEL_PIM								8
#define SGN_MEL_CLAVA							9
#define SGN_MEL_CONVER							10
#define SGN_MEL_MINUTE_MSG						11

#define SGN_MEL_MAX								10								//Max number to use by external user

//------------------------------------------------------------------------------
//типы
//------------------------------------------------------------------------------

#ifndef SGN
#define SGN
typedef struct
{
	ubase_t state;
	ubase_t melody;
	ubase_t counter;
	ubase_t timer;

	ubase_t host_name;
	ubase_t host_num;
} gsgn_t;
#endif

extern gsgn_t gSgn;

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void sgn_Ini(void);
void sgn_Try_On(ubase_t host_name, ubase_t host_num, ubase_t melody);			//---попытаться занять ресурс
void sgn_Off(void);																//---освободить ресурс
void sgn(void);


#define SGN_CHECK_TIMERS()														\
{																				\
	STM_BEGIN_ACT(gSgn.timer)													\
	STM_END_ACT																	\
}


