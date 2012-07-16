/* Функции отображения модуля usr
 * обновлен.....................................................................09.03.2011 13:05:11
*/

#include "dsp.h"
#include "usrc.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

ubase_t gUsr_waite_dotnum = 0;
void usr_Disp_Input_DT(uint8_t sym, ubase_t symnum);								//---отображает введенное время/дату

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//отображение ввода
//------------------------------------------------------------------------------

#if USRC_USE_TEXT_EDITOR == 1 && USRC_TEXT_BUF_TYPE == ___USRC_LARGE_RAM
void usr_Disp_Input(ubase_t symnum)
//---выводит строку ввода
{
	dsp_Clr_Str(gUsr.sp->act_strnum);											//очистить строку ввода
	
	uint8_t *str;
	ubase_t thd;
	if (gUsr.inp_pos < DSP_VIR_STR_LENTH)
	{
		str = gUsr.inp; thd = gUsr.inp_pos;
	}
	else
	{
		str = &gUsr.inp[gUsr.inp_pos-DSP_VIR_STR_LENTH+1];
		thd = DSP_VIR_STR_LENTH-1;
	}
	
	ubase_t i;
	for (i=0; i<thd; i++)
	{
		dsp_Set_CurrentSym(*str++);
	}
#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
	dsp_Set_CursorPos(gUsr.sp->act_strnum, i);	
	if (gUsr.inp_pos >= symnum || STM_CHECK(gUsr.t_sym_input))					//если максимальный символ выключаем блинк
	{
		dsp_Off_Cursor();
	}
	else
	{
		dsp_Set_CursorBlink();
	}
#elif USRC_CURSOR == ___USRC_SYM_CURSOR
	if (gUsr.inp_pos >= symnum || STM_CHECK(gUsr.t_sym_input)) return;			//если максимальный символ выключаем блинк
	#if USRC_DISP_REFRESH_FRQ == ___USRC_ONE_SYSTICK
	if (STM_IS_2TICK) dsp_Set_CurrentSym(USRC_CURSOR_SYM);
	#elif USRC_DISP_REFRESH_FRQ == ___USRC_TWO_SYSTICK
	if (STM_IS_4TICK) dsp_Set_CurrentSym(USRC_CURSOR_SYM);
	#endif
#endif
}

#else
void usr_Disp_Input(ubase_t symnum)
//---выводит строку ввода
{
	dsp_Clr_Str(gUsr.sp->act_strnum);											//очистить строку ввода
	
	uint8_t* str = gUsr.inp;
	
#if USRC_USE_TEXT_EDITOR == 0 || USRC_TEXT_BUF_TYPE == ___USRC_SMALL_RAM
	for (ubase_t i=0; i < gUsr.inp_pos; i++)
#elif USRC_USE_TEXT_EDITOR == 1
	for (ubase_t i=0; (i < gUsr.inp_pos) && (i < DSP_VIR_STR_LENTH-1); i++)
#endif
	{
		dsp_Set_CurrentSym(*str++);
	}
	
#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
	dsp_Set_CursorPos(gUsr.sp->act_strnum, gUsr.inp_pos);
	
	#if USRC_USE_TEXT_EDITOR == 1
		if (gUsr.inp_pos >= symnum || STM_CHECK(gUsr.t_sym_input))				//если максимальный символ выключаем блинк
	#elif USRC_USE_TEXT_EDITOR == 0
		if (gUsr.inp_pos >= symnum)												//если максимальный символ выключаем блинк
	#endif
		{
			dsp_Off_Cursor();
		}
		else
		{
			dsp_Set_CursorBlink();
		}
#elif USRC_CURSOR == ___USRC_SYM_CURSOR
	#if USRC_USE_TEXT_EDITOR == 1
		if (gUsr.inp_pos >= symnum || STM_CHECK(gUsr.t_sym_input)) return;		//если максимальный символ выключаем блинк
	#elif USRC_USE_TEXT_EDITOR == 0
		if (gUsr.inp_pos >= symnum) return;										//если максимальный символ выключаем блинк
	#endif
	#if USRC_DISP_REFRESH_FRQ == ___USRC_ONE_SYSTICK
		if (STM_IS_2TICK) dsp_Set_CurrentSym(USRC_CURSOR_SYM);
	#elif USRC_DISP_REFRESH_FRQ == ___USRC_TWO_SYSTICK
		if (STM_IS_4TICK) dsp_Set_CurrentSym(USRC_CURSOR_SYM);
	#endif
#endif
}
#endif


void usr_Disp_Input_Time(void)
//---отображает введенное время
{
	usr_Disp_Input_DT(DSPC_TIME_SEPARATOR, 6);
}


void usr_Disp_Input_Date(void)
//---отображает введенное дату
{
#if USRC_USE_FULL_DATE == 1
	usr_Disp_Input_DT(DSPC_DATE_SEPARATOR, 8);
#elif USRC_USE_FULL_DATE == 0
	usr_Disp_Input_DT(DSPC_DATE_SEPARATOR, 6);
#endif
}


void usr_Disp_Input_Psw(ubase_t symnum)
//---отображает ввод пароля
{
	dsp_Clr_Str(gUsr.sp->act_strnum);											//очистить строку ввода
	for (ubase_t i=0; i < gUsr.inp_pos; i++)
	{
		dsp_Set_CurrentSym(USRC_PSW_SYM);
	}
	
#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
	dsp_Set_CursorPos(gUsr.sp->act_strnum, gUsr.inp_pos);
	if (gUsr.inp_pos >= symnum)													//если максимальный символ выключаем блинк
	{
		dsp_Off_Cursor();
	}
	else
	{
		dsp_Set_CursorBlink();
	}
#elif USRC_CURSOR == ___USRC_SYM_CURSOR
	if (gUsr.inp_pos >= symnum) return;
	#if USRC_DISP_REFRESH_FRQ == ___USRC_ONE_SYSTICK
	if (STM_IS_2TICK) dsp_Set_CurrentSym(USRC_CURSOR_SYM);
	#elif USRC_DISP_REFRESH_FRQ == ___USRC_TWO_SYSTICK
	if (STM_IS_4TICK) dsp_Set_CurrentSym(USRC_CURSOR_SYM);
	#endif
#endif
}

//------------------------------------------------------------------------------
//отображение параметров
//------------------------------------------------------------------------------


void usr_Disp_Str(const uint8_pr_t *str)
//---выводит строку
{
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_1, str);
}


void usr_Disp_Str_INT16U(const uint8_pr_t *str, uint16_t val)
//---выводит строку с номером в конце
{
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_2, str, val);
}


void usr_Disp_2Str(const uint8_pr_t *str1, const uint8_pr_t *str2)
//---выводит две строки
{
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_3, str1, str2);
}


void usr_Disp_2Str_INT16U(const uint8_pr_t *str1, const uint8_pr_t *str2, uint16_t val)
//---выводит две строки + uint16_t
{
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_4, str1, str2, val);
}

void usr_Disp_Str_INT32U(const uint8_pr_t *str, uint32_t* val)
//---выводит строку + uint32_t
{
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_5, str, *val);
}

void usr_Disp_Str_INT32S(const uint8_pr_t *str, int32_t* val)
//---выводит строку + int32_t
{
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_9, str, *val);
}

void usr_Disp_Str_FL32(const uint8_pr_t *str, fl32_t* val)
//---выводит строку и дополнительно число float
{
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_6, str, *val);
}


void usr_Disp_StrNum_Str(const uint8_pr_t *str1, uint16_t val, const uint8_pr_t *str2)
//---выводит троку с номером + строка
{	
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_10, str1, val, str2);
}

void usr_Disp_WaiteStr(void)
//---отображает строку ожидания
{
	uint8_t dots[5];
	for (ubase_t i=0; i<STRSIZE(dots)-1; i++)
	{
		if (i <= gUsr_waite_dotnum) dots[i] = '.';
		else dots[i] = DSP_SYM_SPACE;
	}
	dots[4] = 0;
	dsp_Print(gUsr.strnum, gUsr.strtype, note_cmd_8, dots);
	gUsr_waite_dotnum++;
	if (gUsr_waite_dotnum > 3) gUsr_waite_dotnum = 0;
}


void usr_Disp_PorgressStr(uint16_t maxv, uint16_t curv)
//---отображает строку прогресса
{
	uint32_t temp = 0;
	temp = curv*((uint32_t)DSP_VIR_STR_LENTH);
	ubase_t progress;
	progress = temp/maxv;
	uint8_t string[DSP_VIR_STR_LENTH+1];
	for (ubase_t i=0; i<DSP_VIR_STR_LENTH; i++)
	{
		if (i <= progress) string[i] = USRC_PROGRESS_FULL_SYM;
		else string[i] = USRC_PROGRESS_EMPTY_SYM;
	}
	string[DSP_VIR_STR_LENTH] = 0;
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_8, string);
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

void usr_Disp_Input_DT(uint8_t sym, ubase_t symnum)
//---отображает введенное время/дату
{
	uint8_t emptystr[] = {' ', ' ', sym, ' ', ' ', sym, 0};
	dsp_Print(gUsr.sp->act_strnum, DSP_STR_LEFT_PUTTED, note_cmd_8, emptystr);
	dsp_Set_CursorPos(gUsr.sp->act_strnum, 0);
	
	uint8_t* str = gUsr.inp;
#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
	ubase_t endpos = gUsr.inp_pos;
#endif
	for (ubase_t i=0; i < gUsr.inp_pos; i++)
	{
		dsp_Set_CurrentSym(*str++);
		if (i == 1 || i == 3)													//перепрыгиваем символ
		{
			dsp_Set_CurrentSym(sym);
#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
			endpos++;
#endif
		}
	}
		
#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
	dsp_Set_CursorPos(gUsr.sp->act_strnum, endpos);
	if (gUsr.inp_pos >= symnum)													//если максимальный символ выключаем блинк
	{
		dsp_Off_Cursor();
	}
	else
	{
		dsp_Set_CursorBlink();
	}
#elif USRC_CURSOR == ___USRC_SYM_CURSOR
	if (gUsr.inp_pos >= symnum) return;
	#if USRC_DISP_REFRESH_FRQ == ___USRC_ONE_SYSTICK
	if (STM_IS_2TICK) dsp_Set_CurrentSym(USRC_CURSOR_SYM);
	#elif USRC_DISP_REFRESH_FRQ == ___USRC_TWO_SYSTICK
	if (STM_IS_4TICK) dsp_Set_CurrentSym(USRC_CURSOR_SYM);
	#endif
#endif
}

