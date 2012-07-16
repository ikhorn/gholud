/*	таймер/счетчик 1(16 бит) для ATMEGA 128
 *	обновлен....................................................................29.11.2010 14:40:53
 */



#include "tc1.h"
#include "stm.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

no_init_ gtimer_t gTimer;

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

void stm_Ini(void)
//---инициализация системного таймера
{
	//настройка тикового таймера
	TC1_SET_SOURCE(TC1_NO_CLOCK);
	TC1_MODE_CTC_OC();

	TC1_OFF_OUT_OCA();
	TC1_OFF_OUT_OCB();
	TC1_OFF_OUT_OCC();

	TC1_SET_COUNTER(0);

	TC1_CLR_ACOMP_INTFL();
	TC1_CLR_BCOMP_INTFL();
	TC1_CLR_CCOMP_INTFL();
	TC1_CLR_OVER_INTFL();

	TC1_DBL_ACOMP_INT();
	TC1_DBL_BCOMP_INT();
	TC1_DBL_CCOMP_INT();
	TC1_DBL_OVER_INT();
	TC1_DBL_INPCAPT_INT();

	TC1_SET_TOPA(STMC_TOP);
	TC1_SET_SOURCE(STMC_SOURCE);
	
	//настройка переменных
	gTimer.clock = 0;
}


bool stm_Is_Tick(void)
//---проверяет системный тик
{
	if (!TC1_WAS_ACOMP_INT) return 0;
	TC1_CLR_ACOMP_INTFL();
	gTimer.clock++;
	return 1;
}

