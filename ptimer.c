#include "usr.h"
#include "msg.h"
#include "stm.h"
#include "rtc.h"
#include "sgn.h"
#include "tmp.h"
#include "err.h"
#include "dsp.h"
#include "light.h"
#include "mopr.h"
#include "ptimer.h"

//------------------------------------------------------------------------------
//типы
//------------------------------------------------------------------------------

extern gtimer_t gTimer;
ubase_t dsp_check_timer;

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

void ptimer_Ini(void)
//---инициализирует програмные таймера
{
	stm_Ini();
    STM_SET(dsp_check_timer, 5);
	//...
}


void ptimer(void)
//---проверка програмных таймеров
{
	if (!stm_Is_Tick()) return;

	USR_CHECK_TIMERS();															//проверить таймера usr модуля
	SGN_CHECK_TIMERS();															//проверить таймера сигнала
	RTC_CHECK_TIMERS();															//проверить таймера обновления часов
	MSG_CHECK_TIMERS();															//проверить таймера сообщения
	TMP_CHECK_TIMERS();															//проверить или датчики работают циклично
    STM_BEGIN_ACT(dsp_check_timer)
        if (!dsp_Check_Disp())
        {
            dsp_Ini();
    		if (gLight.state)
    		{
    			dsp_On_Light();
    		}
        }
        STM_SET(dsp_check_timer, 5);
    STM_END_ACT

	//...
	//...
	//...
	//...
}
