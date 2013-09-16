#include "../dsp/dsp.h"
#include "usr.h"
#include "../stm.h"
#include "../prm.h"


//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

no_init_ gusr_t gUsr;
extern ubase_t gKey;
extern usr_wnd_t wrtW_Ini;

void usr_Open_Window(void);														//---открывает текущее окно
void usr_ScrollUp_Window(void);													//---прокручивает окно вверх
void usr_ScrollDown_Window(void);												//---прокручивает окно вниз

#include "usr_menu.h"
#include "usr_msg.h"
#include "usr_prm.h"
#include "usr_wrt.h"
#include "usr_disp.h"
#include "usr_input.h"

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

func_t *usr_Rd_Item(const usr_wnd_t* wnd, int item_num)
{
	usr_list_t list;
	prm_Rd_Data(&list, sizeof(usr_list_t), &(wnd->item0));
	funcp_t item;
	prm_Rd_Data(&item, sizeof(funcp_t), &(list[item_num]));
	return item;
}


ubase_t usr_Rd_WndSize(const usr_wnd_t* wnd)
{
	ubase_t wnd_size;
	prm_Rd_Data(&wnd_size, sizeof(ubase_t), &(wnd->size0));
	return wnd_size;
}


void usr_Ini(void)
//---инициализация модуля пользовательского интерфейса
{
	for (ubase_t s=0; s < USRC_STRNUM; s++)										//проинициализировать текущие обработчики строк
	{
		ubase_t wnd_size;
		wnd_size = usr_Rd_WndSize(&wrtW_Ini);
		if (s < wnd_size)
			//gUsr.str[s] = wrtW_Ini.item[s];
			gUsr.str[s] = usr_Rd_Item(&wrtW_Ini, s);
		else
			gUsr.str[s] = menu_I_StrByDefault;
	}
	
	gUsr.exe_handler = usr_Execute_Default;
	gUsr.sp->act_strnum = 0;/////////////////////////////////////////////////////////////
	
	gUsr.sp = gUsr.rstack;	
	gUsr.sp->window = &wrtW_Ini;
	gUsr.sp->pnum = 0;/////////////////////////////////////////////////////////////
	gUsr.sp->wtype = USR_WND_WRT;

	gUsr.refresh = true;
	gUsr.write = false;
	gUsr.execute = false;
	
	gUsr.msg_ebl = true;
#if USRC_USE_ASK_MSG == 1
	gUsr.ask_msg = false;
	gUsr.ans_msg = false;
#endif
	
	STM_OFF(gUsr.t_msg);
	STM_OFF(gUsr.t_sym_input);
	
	gUsr.layout = 'A'-1;
}


void usr(void)
//---обновляет содержимое экрана
{
	if (gUsr.execute) gUsr.exe_handler();										//обработать дополнительный процесс
	if (!(gKey || gUsr.refresh)) return;
	usr_Key = gKey; gKey = 0;
	gUsr.refresh = false;
	
#if USRC_USE_ASK_MSG == 1
	
	if (gUsr.ans_msg)															//если сообщение пропустили - стереть его
	{
		gUsr.ans_msg = false;
		gUsr.ask_msg = false;
	}
	
#endif
	
	stp_Handler();																//обработчик статус панели
//	
//	gUsr.strnum = 0;															//обработать заглавную строку
//	if (USR_WND_WRT == gUsr.sp->wtype) gUsr.strtype = USRC_WRT_TYPE_TITLE_STR;
//	else if (USR_WND_MENU == gUsr.sp->wtype) gUsr.strtype = USRC_MENU_TYPE_TITLE_STR;
//	else gUsr.strtype = STM_IS_2TICK ? DSP_STR_WOW_PUTTED:DSP_STR_SPACE_PUTTED;
//	gUsr.str[0]();
//	
//	for (gUsr.strnum=1; gUsr.strnum< USRC_STRNUM; gUsr.strnum++)				//обработать остальные строки
//	{
//		if (gUsr.strnum == gUsr.sp->act_strnum) gUsr.strtype = USRC_TYPE_ACTITEM_STR;
//		else gUsr.strtype = USRC_TYPE_ITEM_STR;
//		gUsr.str[gUsr.strnum]();
//	}
//	
	gUsr.strnum = 0;															//обработать заглавную строку
	if (gUsr.sp->wtype < USR_WND_MENU) gUsr.strtype = USRC_TYPE_ITEM_STR;
	else if (USR_WND_MENU == gUsr.sp->wtype) gUsr.strtype = USRC_MENU_TYPE_TITLE_STR;
#if USRC_DISP_REFRESH_FRQ == ___USRC_ONE_SYSTICK
	else gUsr.strtype = STM_IS_2TICK ? USRC_MSG_TYPE_TITLE_STR:USRC_TYPE_ITEM_STR;
#elif USRC_DISP_REFRESH_FRQ == ___USRC_TWO_SYSTICK
	else gUsr.strtype = STM_IS_4TICK ? USRC_MSG_TYPE_TITLE_STR:USRC_TYPE_ITEM_STR;
#endif 
	gUsr.str[0]();
	
	for (gUsr.strnum=1; gUsr.strnum< USRC_STRNUM; gUsr.strnum++)				//обработать остальные строки
	{
		if (gUsr.strnum == gUsr.sp->act_strnum && USR_WND_MENU == gUsr.sp->wtype)
		{
			gUsr.strtype = USRC_TYPE_ACTITEM_STR;
		}
		else gUsr.strtype = USRC_TYPE_ITEM_STR;
		gUsr.str[gUsr.strnum]();
	}
}


void usr_Execute_Default(void)
//---выполнение процесса по умолчанию
{
	gUsr.execute = false;
	return;
}


void usr_Off_Key(uint8_t key)
//---отключает навигационную клавишу
{
	if (USR_IS_KEY(key)) usr_Key = 0;
}

void usr_Goto(const usr_wnd_t* enter_window, ubase_t enter_pnum, ubase_t wnd_type)
//---переходит на окно
{
	gUsr.sp++;																	//выделить новое окно в стеке
	gUsr.sp->window = enter_window;
	gUsr.sp->pnum = enter_pnum;
	if (wnd_type < USR_WND_MENU) gUsr.sp->act_strnum = 0;
	else gUsr.sp->act_strnum = 1;
	gUsr.sp->wtype = wnd_type;
	usr_Open_Window();
	usr_Key = 0;
}


void usr_Replace(const usr_wnd_t* enter_window, ubase_t enter_pnum, ubase_t wnd_type)
//---заменяет текущее окно
{
	gUsr.sp->window = enter_window;
	gUsr.sp->pnum = enter_pnum;
	if (wnd_type < USR_WND_MENU) gUsr.sp->act_strnum = 0;
	else gUsr.sp->act_strnum = 1;
	gUsr.sp->wtype = wnd_type;
	usr_Open_Window();
	usr_Key = 0;
}


void usr_Exit(void)
//---возвращает на предидущее окно
{
	if (gUsr.sp == gUsr.rstack) return;
	--gUsr.sp;
	usr_Open_Window();
	usr_Key = 0;
}


void usr_Exit_to_Wrt(void)
//---возвращает на рабочий стол
{
	gUsr.sp = gUsr.rstack;
	usr_Open_Window();
	usr_Key = 0;
}


//------------------------------------------------------------------------------
//внутренние функции
//------------------------------------------------------------------------------

void usr_Open_Window(void)
//---открывает текущее окно
{
	const usr_wnd_t* open_window = gUsr.sp->window;
	ubase_t pnum = gUsr.sp->pnum;
	ubase_t window_size_ = usr_Rd_WndSize(open_window)-1;
	if (window_size_ < USRC_STRNUM)												//строк меньше чем на мониторе
	{
		gUsr.sp->act_strnum = pnum;
		for (ubase_t s=0; s<USRC_STRNUM; s++)
		{
			//if (s <= window_size_) gUsr.str[s] = open_window->item[s];
			if (s <= window_size_) gUsr.str[s] = usr_Rd_Item(open_window, s);
			else gUsr.str[s] = menu_I_StrByDefault;
		}
	}
	else																		//строк больше чем на мониторе
	{
		gUsr.str[0] = usr_Rd_Item(open_window, 0);
		for (ubase_t a=gUsr.sp->act_strnum, i=pnum; a<USRC_STRNUM; a++, i++)
		{
			if (i > window_size_) i=1;
			gUsr.str[a] = usr_Rd_Item(open_window, i);
		}
		for (ubase_t a=gUsr.sp->act_strnum-1, i=pnum-1; a; --a, --i)
		{
			if (i < 1) i = window_size_;
			gUsr.str[a] = usr_Rd_Item(open_window, i);
		}
	}
	gUsr.refresh = true;	
}


void usr_ScrollUp_Window(void)
//---прокручивает окно вверх
{
	ubase_t window_size_ = usr_Rd_WndSize(gUsr.sp->window)-1;
	ubase_t typesize;
	if (gUsr.sp->wtype >= USR_WND_MENU) typesize = 1;							//если тип окна - меню или сообщение
	else typesize = 0;															//если рабочий стол или пространство
	
	if (window_size_ < USRC_STRNUM)												//строк < чем на мониторе
	{
		if (gUsr.sp->act_strnum > typesize)
		{
			--gUsr.sp->pnum;													//изменить указатель и все!
			--gUsr.sp->act_strnum;
		}
		else
		{
			gUsr.sp->pnum = window_size_;
			gUsr.sp->act_strnum = window_size_;
		}
	}
	else																		//строк > чем на мониторе
	{
		if (gUsr.sp->pnum > typesize) --gUsr.sp->pnum;
		else gUsr.sp->pnum = window_size_;
	
		if (gUsr.sp->act_strnum > typesize) --gUsr.sp->act_strnum;
		else																	//листануть
		{
			for (ubase_t s=(USRC_STRNUM-1); s > typesize; --s) gUsr.str[s] = gUsr.str[s-1];
			gUsr.str[typesize] = usr_Rd_Item(gUsr.sp->window, gUsr.sp->pnum);
		}
	}
	gUsr.refresh = true;
}


void usr_ScrollDown_Window(void)
//---прокручивает окно вниз
{
	ubase_t window_size_ = usr_Rd_WndSize(gUsr.sp->window)-1;
	ubase_t typesize;
	if (gUsr.sp->wtype >= USR_WND_MENU) typesize = 1;							//если тип окна - меню или сообщение
	else typesize = 0;															//если рабочий стол или пространство
	
	if (window_size_ < USRC_STRNUM)												//строк < чем на мониторе
	{
		if (gUsr.sp->act_strnum < window_size_)
		{
			gUsr.sp->act_strnum++;
			gUsr.sp->pnum++;
		}
		else
		{
			gUsr.sp->pnum = typesize;
			gUsr.sp->act_strnum = typesize;
		}
	}
	else																		//строк > чем на мониторе
	{
		if (gUsr.sp->pnum < window_size_) gUsr.sp->pnum++;						//назначить новый пункт
		else gUsr.sp->pnum = typesize;
							
		enum {MAX_ACT_STRNUM = USRC_STRNUM-1};
		if (gUsr.sp->act_strnum < MAX_ACT_STRNUM) gUsr.sp->act_strnum++;
		else
		{
			for (ubase_t s=typesize; s < MAX_ACT_STRNUM; s++) gUsr.str[s] = gUsr.str[s+1];
			gUsr.str[MAX_ACT_STRNUM] = usr_Rd_Item(gUsr.sp->window, gUsr.sp->pnum);
		}
	}
	gUsr.refresh = true;
}

//void usr_ScrollUp_Window(void)
////---прокручивает окно вверх
//{
//	ubase_t window_size_ = gUsr.sp->window->size-1;
//	if (window_size_ < USRC_STRNUM)												//строк < чем на мониторе
//	{
//		if (gUsr.sp->act_strnum > 1)
//		{
//			--gUsr.sp->pnum;													//изменить указатель и все!
//			--gUsr.sp->act_strnum;
//		}
//		else
//		{
//			gUsr.sp->pnum = window_size_;
//			gUsr.sp->act_strnum = window_size_;
//		}
//	}
//	else																		//строк > чем на мониторе
//	{
//		if (gUsr.sp->pnum > 1) --gUsr.sp->pnum;
//		else gUsr.sp->pnum = window_size_;
//	
//		if (gUsr.sp->act_strnum > 1) --gUsr.sp->act_strnum;
//		else																	//листануть
//		{
//			for (ubase_t s=USRC_STRNUM-1; s > 1; --s) gUsr.str[s] = gUsr.str[s-1];
//			gUsr.str[1] = gUsr.sp->window->item[gUsr.sp->pnum];
//		}
//	}
//	gUsr.refresh = true;
//}
//
//void usr_ScrollDown_Window(void)
////---прокручивает окно вниз
//{
//	ubase_t window_size_ = gUsr.sp->window->size-1;
//	if (window_size_ < USRC_STRNUM)												//строк < чем на мониторе
//	{
//		if (gUsr.sp->act_strnum < window_size_)
//		{
//			gUsr.sp->act_strnum++;
//			gUsr.sp->pnum++;
//		}
//		else
//		{
//			gUsr.sp->pnum = 1;
//			gUsr.sp->act_strnum = 1;
//		}
//	}
//	else																		//строк > чем на мониторе
//	{
//		if (gUsr.sp->pnum < window_size_) gUsr.sp->pnum++;						//назначить новый пункт
//		else gUsr.sp->pnum = 1;
//		
//		enum {MAX_ACT_STRNUM = USRC_STRNUM-1};
//		if (gUsr.sp->act_strnum < MAX_ACT_STRNUM) gUsr.sp->act_strnum++;
//		else
//		{
//			for (ubase_t s=1; s < MAX_ACT_STRNUM; s++) gUsr.str[s] = gUsr.str[s+1];
//			gUsr.str[MAX_ACT_STRNUM] = gUsr.sp->window->item[gUsr.sp->pnum];
//		}
//	}
//	gUsr.refresh = true;
//}

