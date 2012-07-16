/*	Модуль работы с системным таймером
 *	обновлен....................................................................23.12.2010 18:00:05
 */

#include "stmm.h"

//------------------------------------------------------------------------------
//описание
//------------------------------------------------------------------------------
//Наводится таймер функциями stm_Set_Tick, stm_Set, ими же и отключаеться.
//Обработчик таймера должен быть помещен в подпрограмму после происхождения
//системного тика, тоесть в програмный таймер (ptimer). Процедуру обработки
//события таймера нужно заключить между обрамляющими макросами STM_BEGIN_ACT()
// и STM_END_ACT.
//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------

#define STM_ONECLOCK							STMC_ONECLOCK					//время одного системного клока

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void stm_Ini(void);																//---инициализация системного таймера
bool stm_Is_Tick(void);															//---проверяет системный тик

//------------------------------------------------------------------------------
//макро функции
//------------------------------------------------------------------------------
																				
#define STM_SET_TICK(TIMER, TICK)				STM_SET_TICK_(TIMER, TICK)		//---настраивает таймер по тикам
#define STM_SET(TIMER, SEC)						STM_SET_(TIMER, SEC)			//---настараивает таймер по секундам
#define STM_OFF(TIMER)							STM_OFF_(TIMER)					//---выключает таймер
#define STM_CHECK(TIMER)						STM_CHECK_(TIMER)				//---проверяет состояние таймера
#define STM_BEGIN_ACT(TIMER)					STM_BEGIN_ACT_(TIMER)			//---шапка проверки таймера (внутри помещать код)
#define STM_END_ACT								STM_END_ACT_					//---конец проверки таймера
#define STM_IS_2TICK							STM_IS_2TICK_					//---проверяет 2-й тик
#define STM_IS_4TICK							STM_IS_4TICK_					//---проверяет 4-й тик
#define STM_IS_8TICK							STM_IS_8TICK_					//---проверяет 8-й тик
//#define STM_IS_TICK(NTICK)						STM_IS_TICK_(NTICK)				//---проверяет N-й тик

