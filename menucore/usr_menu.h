/* Подмодуль menu модуля usr
 * обновлен.....................................................................24.12.2010 15:38:48
*/

#include "usr.h"

//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------

#define menu_Filtr_Proc()					if (gUsr.sp->act_strnum != gUsr.strnum || !usr_Key) return;

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

extern gusr_t gUsr;

flash_ const usr_rstack_t* const gUsrRstackEnd = &gUsr.rstack[USRC_RETURN_STACK_SIZE-1];

#if USRC_USE_SUCH_GHOTKEY_1														//обработчики глобальных горячих клавиш
extern void usr_Handle_HotKey_1(void);
#endif

#if USRC_USE_SUCH_GHOTKEY_2
extern void usr_Handle_HotKey_2(void);
#endif

#if USRC_USE_SUCH_GHOTKEY_3
extern void usr_Handle_HotKey_3(void);
#endif

#if USRC_USE_SUCH_GHOTKEY_4
extern void usr_Handle_HotKey_4(void);
#endif

#if USRC_USE_SUCH_GHOTKEY_5
extern void usr_Handle_HotKey_5(void);
#endif

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

void menu_Navigate(void)
//---навигация оконечного меню
{
	menu_Filtr_Proc();
	switch (usr_Key)
	{
		case USRC_UPKEY:
		{
			usr_ScrollUp_Window();
			break;
		}
		case USRC_DOWNKEY: 
		{
			usr_ScrollDown_Window();
			break;
		}
		case USRC_RETURNKEY:
		{
			--gUsr.sp;
			usr_Open_Window();
			break;
		}
		case USRC_TOGGLEMENU_KEY:
		{
			gUsr.sp = gUsr.rstack;
			gUsr.sp->wtype = USR_WND_WRT;
			gUsr.sp->pnum = 0;				//////////////////
			gUsr.sp->act_strnum = 0;		/////////////////
			usr_Open_Window();
			break;
		}
	#if USRC_USE_SUCH_GHOTKEY_1													//глобальные горячие клавиши
		case USRC_USE_SUCH_GHOTKEY_1:
		{
			usr_Handle_HotKey_1();
			break;
		}
	#endif
	#if USRC_USE_SUCH_GHOTKEY_2
		case USRC_USE_SUCH_GHOTKEY_2:
		{
			usr_Handle_HotKey_2();
			break;
		}
	#endif
	#if USRC_USE_SUCH_GHOTKEY_3
		case USRC_USE_SUCH_GHOTKEY_3:
		{
			usr_Handle_HotKey_3();
			break;
		}
	#endif
	#if USRC_USE_SUCH_GHOTKEY_4
		case USRC_USE_SUCH_GHOTKEY_4:
		{
			usr_Handle_HotKey_4();
			break;
		}
	#endif
	#if USRC_USE_SUCH_GHOTKEY_5
		case USRC_USE_SUCH_GHOTKEY_5:
		{
			usr_Handle_HotKey_5();
			break;
		}
	#endif
		default: break;
	}
	usr_Key = 0;
}


void menu_NavigateEnterMenu(const usr_wnd_t* enter_window, ubase_t enter_pnum)
//---навигация по меню
{
	menu_Filtr_Proc();															//фильтры обработки
#if USRC_USE_ENTERKEY_2 == 1
	if (usr_Key == USRC_ENTERKEY_1 || usr_Key == USRC_ENTERKEY_2)
#elif USRC_USE_ENTERKEY_2 == 0
	if (usr_Key == USRC_ENTERKEY_1)
#endif
	{
		usr_Goto(enter_window, enter_pnum, USR_WND_MENU);
	}
	else menu_Navigate();
}

