/*	Макро переопределения системного таймера
 *	обновлен....................................................................29.11.2010 15:53:02
 */

#include "stmc.h"

//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------

#define STM_ONOFF_THD							64								//порог включения таймера
#define STM_BOTTOM_THD							128								//порог срабатывания таймера

//------------------------------------------------------------------------------
//структура системного таймера
//------------------------------------------------------------------------------

#ifndef STM
#define STM
typedef
struct
{
	uint16_t clock;																//счетчик системного таймера
} gtimer_t;
#endif
extern gtimer_t gTimer;

//------------------------------------------------------------------------------
//тела макро функций
//------------------------------------------------------------------------------
																				//---настраивает таймер по тикам
#define STM_SET_TICK_(TIMER, TICK)												\
	(TIMER) = STM_BOTTOM_THD + (TICK)

																				//---настраивает таймер по секундам
#define STM_SET_(TIMER, SEC)													\
	STM_SET_TICK((TIMER), (uint32_t)((fl64_t)(SEC)/STM_ONECLOCK))
		
																				//---выключает таймер
#define STM_OFF_(TIMER)															\
	(TIMER) = STM_ONOFF_THD/2
		
																				//---проверяет состояние таймера
#define STM_CHECK_(TIMER)														\
	((TIMER) > STM_ONOFF_THD)		
		
		
																				//---шапка проверки таймера
#define STM_BEGIN_ACT_(TIMER)													\
if ((TIMER) > STM_ONOFF_THD)													\
{																				\
	if (--(TIMER) < STM_BOTTOM_THD)												\
	{																			\
		STM_OFF(TIMER);		

																				//---конец проверки таймера
#define STM_END_ACT_															\
	}																			\
}


#define STM_IS_2TICK_							(gTimer.clock&0x01)				//---проверяет 2-й тик
#define STM_IS_4TICK_							(gTimer.clock&0x02)				//---проверяет 4-й тик
#define STM_IS_8TICK_							(gTimer.clock&0x04)				//---проверяет 8-й тик
//#define STM_IS_TICK_(NTICK)						(gTimer.clock&NTICK == NTICK)	//---проверяет N-й тик
