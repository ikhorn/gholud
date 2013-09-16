/* Подмодуль wrt модуля usr
 * обновлен.....................................................................24.12.2010 15:39:14
*/

#include "usr.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

extern gusr_t gUsr;
extern usr_wnd_t menW_Enter;

#if USRC_USE_WRT_SUCH_HOTKEY_1													//обработчики горячих клавиш рабочего стола
extern void wrt_Handle_HotKey_1(void);
#endif

#if USRC_USE_WRT_SUCH_HOTKEY_2
extern void wrt_Handle_HotKey_2(void);
#endif

#if USRC_USE_WRT_SUCH_HOTKEY_3
extern void wrt_Handle_HotKey_3(void);
#endif

#if USRC_USE_WRT_SUCH_HOTKEY_4
extern void wrt_Handle_HotKey_4(void);
#endif

#if USRC_USE_WRT_SUCH_HOTKEY_5
extern void wrt_Handle_HotKey_5(void);
#endif

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


void wrt_Scan_Key(const usr_wnd_t* left_window, const usr_wnd_t* right_window)
//---навигация по рабочему столу
{
	if (!usr_Key) return;
	switch (usr_Key)
	{
		case USRC_ENTERKEY_1:
#if USRC_USE_ENTERKEY_2 == 1
		case USRC_ENTERKEY_2:
#endif
		case USRC_TOGGLEMENU_KEY:
		{
			gUsr.sp->pnum = 1;
			gUsr.sp++;															//закинуть в стек новое окно
			gUsr.sp->window = &menW_Enter;
			gUsr.sp->pnum = 1;
			gUsr.sp->act_strnum = 1;
			gUsr.sp->wtype = USR_WND_MENU;
			usr_Open_Window();
			break;
		}
		case USRC_LEFTKEY:
		{
			usr_Replace(left_window, 0, USR_WND_WRT);
			gUsr.sp->act_strnum = 0;
			break;
		}
		case USRC_RIGHTKEY:
		{
			usr_Replace(right_window, 0, USR_WND_WRT);
			gUsr.sp->act_strnum = 0;
			break;
		}
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
	#if USRC_USE_WRT_SUCH_HOTKEY_1												//горячие клавиши рабочего стола
		case USRC_USE_WRT_SUCH_HOTKEY_1:
		{
			wrt_Handle_HotKey_1();
			break;
		}
	#endif
	#if USRC_USE_WRT_SUCH_HOTKEY_2
		case USRC_USE_WRT_SUCH_HOTKEY_2:
		{
			wrt_Handle_HotKey_2();
			break;
		}
	#endif
	#if USRC_USE_WRT_SUCH_HOTKEY_3
		case USRC_USE_WRT_SUCH_HOTKEY_3:
		{
			wrt_Handle_HotKey_3();
			break;
		}
	#endif
	#if USRC_USE_WRT_SUCH_HOTKEY_4
		case USRC_USE_WRT_SUCH_HOTKEY_4:
		{
			wrt_Handle_HotKey_4();
			break;
		}
	#endif
	#if USRC_USE_WRT_SUCH_HOTKEY_5
		case USRC_USE_WRT_SUCH_HOTKEY_5:
		{
			wrt_Handle_HotKey_5();
			break;
		}
	#endif
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


