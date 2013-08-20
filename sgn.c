#include "sgn.h"
#include "dsp/dsp.h"
#include "zmr.h"
#include "res.h"
#include "light.h"
#include "delay.h"
#include "pdef.h"


//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

gsgn_t gSgn;
void sgn_Mel_2TickTone(uint16_t frq1, uint16_t frq2);								//---проигрывает 2 тона длиной в 1 сис такт каждая

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void sgn_Ini(void)
{
	gSgn.state = 0;
	gSgn.melody = SGN_MEL_BLINK_1;
	gSgn.counter = 0;
	gSgn.host_name = RES_IS_REST;
	gSgn.host_num = 0;
	STM_OFF(gSgn.timer);
	zmr_Off();
}

void sgn_Try_On(ubase_t host_name, ubase_t host_num, ubase_t melody)
//---попытаться занять ресурс
{
	if (gSgn.state)
	{
		if (SGN_MEL_MINUTE_MSG != gSgn.melody)
			return;
	}
	gSgn.melody = melody;
	gSgn.state = 1;
	gSgn.host_name = host_name;
	gSgn.host_num = host_num;
}

void sgn_Off(void)
//---освободить ресурс
{
	gSgn.state = 0;
	gSgn.counter = 0;
	STM_OFF(gSgn.timer);
	zmr_Off();
	if (gLight.state)
	{
		dsp_On_Light();
	}
	else
	{
		dsp_Off_Light();
	}
}


void sgn(void)
{
	if (!gSgn.state) return;
	switch (gSgn.melody)
	{
		case SGN_MEL_MINUTE_MSG:
		{
			if (STM_IS_2TICK)
			{
				if (!CHKB(gSgn.counter, 7))
				{
					SETB(gSgn.counter, 7);
					gSgn.counter++;
				}
				dsp_On_Light();
				zmr_On(ZMR_02KHZ);
			}
			else
			{
				if (gSgn.counter > 0x81)										//switch off itself
				{
					sgn_Off();
					break;
				}
				CLRB(gSgn.counter, 7);
				dsp_Off_Light();
				zmr_Off();
			}
			break;
		}
		case SGN_MEL_BLINK_1:
		{
			if (STM_IS_4TICK)
			{
				dsp_On_Light();
				zmr_On(ZMR_02KHZ);
			}
			else
			{
				dsp_Off_Light();
				zmr_Off();
			}
			break;
		}
		case SGN_MEL_BLINK_2:
		{
			if (STM_IS_8TICK)
			{
				dsp_On_Light();
				zmr_On(ZMR_01KHZ);
			}
			else
			{
				dsp_Off_Light();
				zmr_Off();
			}
			break;
		}
		case SGN_MEL_BOMB:
		{
			if (STM_CHECK(gSgn.timer)) break;
			uint16_t frq;
			switch (gSgn.counter)
			{
				case 1: frq = ZMR_125HZ; break;
				case 2: frq = ZMR_250HZ; break;
				case 3: frq = ZMR_500HZ; break;
				case 4: frq = ZMR_01KHZ; break;
				case 5: frq = ZMR_02KHZ; break;
				case 6: frq = ZMR_04KHZ; break;
				case 7: frq = ZMR_08KHZ; break;
				case 8: frq = ZMR_16KHZ; break;
				default:
				{
					gSgn.counter = 0;
					frq = ZMR_062HZ;
					break;
				}
			}
			gSgn.counter++;

			STM_SET(gSgn.timer, 0.25);
			dsp_On_Light();
			zmr_On(frq);
			break;
		}
		case SGN_MEL_CHAOS:
		{
			if (STM_IS_4TICK)
			{
				uint16_t frq;
				switch (gSgn.counter)
				{
					case 1: frq = ZMR_125HZ; break;
					case 2: frq = ZMR_250HZ; break;
					case 3: frq = ZMR_500HZ; break;
					case 4: frq = ZMR_01KHZ; break;
					case 5: frq = ZMR_02KHZ; break;
					case 6: frq = ZMR_04KHZ; break;
					case 7: frq = ZMR_08KHZ; break;
					case 8: frq = ZMR_16KHZ; break;
					default:
					{
						gSgn.counter = 0;
						frq = ZMR_062HZ;
						break;
					}
				}
				gSgn.counter++;

				dsp_On_Light();
				zmr_On(frq);
			}
			else
			{
				dsp_Off_Light();
				zmr_Off();
			}
			break;
		}
		case SGN_MEL_MARCH:
		{
			if (STM_CHECK(gSgn.timer)) break;
			uint16_t frq;
			uint8_t ticknum;
			switch (gSgn.counter)
			{
				case 1: frq = ZMR_125HZ; ticknum = 2; break;
				case 2: frq = ZMR_250HZ; ticknum = 3; break;
				case 3: frq = ZMR_500HZ; ticknum = 4; break;
				case 4: frq = ZMR_01KHZ; ticknum = 5; break;
				case 5: frq = ZMR_02KHZ; ticknum = 4; break;
				case 6: frq = ZMR_04KHZ; ticknum = 3; break;
				case 7: frq = ZMR_08KHZ; ticknum = 2; break;
				case 8: frq = ZMR_16KHZ; ticknum = 1; break;
				default:
				{
					gSgn.counter = 0;
					frq = ZMR_062HZ; ticknum = 1;
					break;
				}
			}
			gSgn.counter++;

			STM_SET_TICK(gSgn.timer, ticknum);
			dsp_On_Light();
			zmr_On(frq);
			break;
		}
		case SGN_MEL_SILENCE:
		{
			sgn_Mel_2TickTone(ZMR_031HZ, ZMR_062HZ);
			break;
		}
		case SGN_MEL_EMERGE:
		{
			sgn_Mel_2TickTone(ZMR_500HZ, ZMR_02KHZ);
			break;
		}
		case SGN_MEL_PUM:
		{
			if (STM_CHECK(gSgn.timer)) break;
			switch (gSgn.counter)
			{
				case 1: zmr_Off(); break;
				case 2: zmr_On(ZMR_02KHZ); break;
				case 3: zmr_Off(); break;
				case 4: zmr_On(ZMR_250HZ); break;
				default:
				{
					gSgn.counter = 0;
					zmr_On(ZMR_02KHZ);
					break;
				}
			}
			gSgn.counter++;

			STM_SET(gSgn.timer, 0.25);
			dsp_On_Light();
			break;
		}
		case SGN_MEL_PIM:
		{
			if (STM_CHECK(gSgn.timer)) break;
			switch (gSgn.counter)
			{
				case 1: zmr_Off(); break;
				case 2: zmr_On(ZMR_125HZ); break;
				case 3: zmr_Off(); break;
				case 4: zmr_On(ZMR_250HZ); break;
				case 5: zmr_Off(); break;
				case 6: zmr_On(ZMR_500HZ); break;
				case 7: zmr_Off(); break;
				case 8: zmr_On(ZMR_01KHZ); break;
				case 9: zmr_Off(); break;
				case 10: zmr_On(ZMR_02KHZ); break;
				case 11: zmr_Off(); break;
				case 12: zmr_On(ZMR_04KHZ); break;
				case 13: zmr_Off(); break;
				default:
				{
					gSgn.counter = 0;
					zmr_On(ZMR_062HZ);
					break;
				}
			}
			gSgn.counter++;

			STM_SET(gSgn.timer, 0.25);
			dsp_On_Light();
			break;
		}
		case SGN_MEL_CLAVA:
		{
			if (STM_CHECK(gSgn.timer)) break;
			switch (gSgn.counter)
			{
				case 1: zmr_Off(); break;
				case 2: zmr_On(ZMR_03KHZ); break;
				case 3: zmr_Off(); break;
				case 4: zmr_On(ZMR_500HZ); break;
				case 5: zmr_On(ZMR_125HZ); break;
				case 6: zmr_On(ZMR_04KHZ); break;
				case 7: zmr_Off(); break;
				default:
				{
					gSgn.counter = 0;
					zmr_On(ZMR_03KHZ);
					break;
				}
			}
			gSgn.counter++;

			STM_SET(gSgn.timer, 0.25);
			dsp_On_Light();
			break;
		}
		case SGN_MEL_CONVER:
		{
			if (STM_CHECK(gSgn.timer)) break;
			switch (gSgn.counter)
			{
				case 1: zmr_On(ZMR_16KHZ); break;
				case 2: zmr_On(ZMR_062HZ); break;
				case 3: zmr_On(ZMR_08KHZ); break;
				case 4: zmr_On(ZMR_125HZ); break;
				case 5: zmr_On(ZMR_04KHZ); break;
				case 6: zmr_On(ZMR_250HZ); break;
				case 7: zmr_On(ZMR_03KHZ); break;
				case 8: zmr_On(ZMR_500HZ); break;
				case 9: zmr_On(ZMR_02KHZ); break;
				case 10: zmr_On(ZMR_01KHZ); break;
				case 11: zmr_Off(); break;
				default:
				{
					gSgn.counter = 0;
					zmr_On(ZMR_031HZ);
					break;
				}
			}
			gSgn.counter++;

			STM_SET(gSgn.timer, 0.25);
			dsp_On_Light();
			break;
		}
		default:
		{
			gSgn.melody = SGN_MEL_BLINK_1;
			break;
		}
	}
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

void sgn_Mel_2TickTone(uint16_t frq1, uint16_t frq2)
//---проигрывает 2 тона длиной в 1 сис такт каждая
{
	//if (STM_CHECK(gSgn.timer)) return;

	if (gSgn.counter && STM_IS_4TICK)
	{
		gSgn.counter = 0;
		zmr_On(frq1);
		dsp_On_Light();
	}
	else if (!gSgn.counter && !STM_IS_4TICK)
	{
		gSgn.counter = 1;
		zmr_On(frq2);
		dsp_Off_Light();
	}

	//STM_SET(gSgn.timer, 0.5);
	//
}
