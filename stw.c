#include "stw.h"
#include "dsp.h"
#include "usr.h"
#include "fstw.h"
#include "bstw.h"


//------------------------------------------------------------------------------
//���������
//------------------------------------------------------------------------------

extern usr_wnd_t STW_RIGHTWINDOW;
extern usr_wnd_t STW_LEFTWINDOW;
extern const USRC_STR_ STWC_NOTE_TIMEFORWARD[];
extern const USRC_STR_ STWC_NOTE_TIMEBACK[];

//------------------------------------------------------------------------------
//����. �������
//------------------------------------------------------------------------------

void stw_Ini(void)
{
	fstw_Ini();
	bstw_Ini();
}

void stw(void)
{
	bstw();
}

//------------------------------------------------------------------------------
//USR!!!!
//------------------------------------------------------------------------------

void stw_I_Forward(void);
void stw_I_Back(void);
//������ ������������ �������
funcp_pr_t listStw[] =
{
 	stw_I_Forward,
 	stw_I_Back
};
usr_wnd_t stwW = {listStw, list_size(listStw)};


void stw_I_Forward(void)
{
	ubase_t strtype;
	if (USR_ITEM_ACTIVE)
	{
		strtype = DSP_STR_ARROW_PUTTED;
		if (usr_Key)
		{
			USR_REFRESH_DISP();
		}
		if (usr_Key == USR_ENTERKEY_1)
		{
			switch (gFstw.state)
			{
				case FSTW_IS_OFF:		fstw_Start(); break;
				case FSTW_IS_RUN:		fstw_Stop(); break;
				case FSTW_IS_STOP:		fstw_Continue(); break;
				default:				fstw_Off(); break;
			}
		}
		else if (usr_Key == USR_RETURNKEY) fstw_Off();
		else wrt_Scan_Key(&STW_LEFTWINDOW, &STW_RIGHTWINDOW);
		usr_Key = 0;
	}
	else strtype = DSP_STR_SPACE_PUTTED;
	
	time_t time;
	fstw_Rd_Time(&time);
	dsp_Print(usr_CurStrNum(), strtype, note_cmd_11, STWC_NOTE_TIMEFORWARD, &time);
}

void stw_I_Back(void)
{
	ubase_t strtype;
	if (USR_ITEM_ACTIVE)
	{
		if (USR_IS_INPUT) usr_Disp_Input_Time();
		else strtype = DSP_STR_ARROW_PUTTED;
		
		if (gBstw.state == BSTW_IS_OFF && (USR_IS_INPUT || usr_Key == USR_ENTERKEY_1))
		{
			time_t time;
			if (usr_Input_Time(&time))
			{
				bstw_Start(&time);
			}
			return;
		}
		else
		{
			if (usr_Key)
			{
				USR_REFRESH_DISP();
			}
			if (usr_Key == USR_ENTERKEY_1)
			{
				switch (gBstw.state)
				{
					case BSTW_IS_RUN:		bstw_Stop(); break;
					case BSTW_IS_STOP:		bstw_Continue(); break;
					default:				bstw_Off(); break;
				}
			}
			else if (usr_Key == USR_RETURNKEY) bstw_Off();
			else wrt_Scan_Key(&STW_LEFTWINDOW, &STW_RIGHTWINDOW);
			usr_Key = 0;
		}
	}
	else strtype = DSP_STR_SPACE_PUTTED;
	time_t time;
	bstw_Rd_Time(&time);
	dsp_Print(usr_CurStrNum(), strtype, note_cmd_11, STWC_NOTE_TIMEBACK, &time);
}


