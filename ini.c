#include "alm.h"
#include "zmr.h"
#include "dsp/dsp.h"
#include "usr.h"
#include "rmd.h"
#include "tmp.h"
#include "tmr.h"
#include "trg.h"
#include "ini.h"
#include "kbd.h"
#include "mem.h"
#include "rtc.h"
#include "sgn.h"
#include "def.h"
#include "usr.h"
#include "msg.h"
#include "stw.h"
#include "wdt.h"
#include "note.h"
#include "mctr.h"
#include "cint.h"
#include "dmem.h"
#include "light.h"
#include "delay.h"
#include "ptimer.h"


//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

void ini_Boot_Signal(void);														//---сигнал загрузки
bool ini_Prepare_FirstStart(void);												//---подготовить, если первый старт

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

/*
 * Initialization of peripheries
 */
void ini(void)
{
	delay_ms(5);
	dsp_Ini();
	dsp_Print(0, DSP_STR_SPACE_PUTTED, note_cmd_1, note_devname);
	dsp_Print(1, DSP_STR_SPACE_PUTTED, note_cmd_1, note_version);
	delay_ms(500);
	sta_Ini();
	sgn_Ini();
	usr_Ini();
	msg_Ini();
	menu_Ini();
	mem_Ini();
	kbd_Ini();
	zmr_Ini();

	/*
	 * If started at first time
	 */
	if (ini_Prepare_FirstStart()) {
		gAlm.waite_vector = 0;
		INT_EBL_INT();
		for (;;)
		{
			usr();
			USR_REFRESH_DISP();
			kbd();
		}
	}

	rtc_Ini();
	tmp_Ini();
	alm_Ini();
	tmr_Ini();
	rmd_Ini();
	stw_Ini();
	trg_Ini();
	light_Ini();
	ini_Boot_Signal();
	ptimer_Ini();
	INT_EBL_INT();
	wdt_On();
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

void ini_Boot_Signal(void)
{
	for (ubase_t i=0; i<4; i++)
	{
		delay_ms(100);
		dsp_Toggle_Light();
	}
	zmr_On(ZMR_01KHZ);
	delay_ms(100);
	zmr_Off();
}


bool ini_Prepare_FirstStart(void)
//---подготовить, если первый старт
{
	if (!dmem_Is_First_Start()) return 0;
	usr_temp_t var;
	msg_Post(&msgW_Firstly, MSG_BLINK_TYPE, &var);
	return 1;
}




