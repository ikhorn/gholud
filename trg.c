#include "trg.h"
#include "tmp.h"
#include "usr.h"
#include "msg.h"
#include "def.h"
#include "dmem.h"

//------------------------------------------------------------------------------
//работа с портами
//------------------------------------------------------------------------------

#define TRG_1_ON_WARM()															\
{																				\
	SETB(TRM_1_DDR, TRM_1_B);													\
	SETB(TRM_1_PORT, TRM_1_B);													\
}

#define TRG_1_OFF_WARM()														\
{																				\
	SETB(TRM_1_DDR, TRM_1_B);													\
	CLRB(TRM_1_PORT, TRM_1_B);													\
}

#define TRG_1_TO_Z()															\
{																				\
	CLRB(TRM_1_DDR, TRM_1_B);													\
	CLRB(TRM_1_PORT, TRM_1_B);													\
}

#define TRG_2_ON_WARM()															\
{																				\
	SETB(TRM_2_DDR, TRM_2_B);													\
	SETB(TRM_2_PORT, TRM_2_B);													\
}

#define TRG_2_OFF_WARM()														\
{																				\
	SETB(TRM_2_DDR, TRM_2_B);													\
	CLRB(TRM_2_PORT, TRM_2_B);													\
}

#define TRG_2_TO_Z()															\
{																				\
	CLRB(TRM_2_DDR, TRM_2_B);													\
	CLRB(TRM_2_PORT, TRM_2_B);													\
}

#define TRG_CHECK_TRG1_OUT()					CHKB(TRM_1_PORT, TRM_1_B)
#define TRG_CHECK_TRG2_OUT()					CHKB(TRM_2_PORT, TRM_2_B)

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

gTRG_ gTrg;
void trg_Handle(ubase_t tnum);													//---обработка терморегулятора
void trg_On_Warm(ubase_t tnum);
void trg_Off_Warm(ubase_t tnum);
void trg_Out_to_Z(ubase_t tnum);													//---переводит выход в "Z" состояние

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void trg_Ini(void)
{
	gTrg.state_vector = 0;														//обновить статусы
	gTrg.type_vector = 0;
	for (ubase_t tnum=0; tnum<TRG_NUM; tnum++)
	{
		uint8_t state = DEF_TRG_STATE;
		uint8_t type = DEF_TRG_TYPE;
		dmem_Rd_Trg_Tbottom(tnum, &gTrg.tbottom[tnum]);
		dmem_Rd_Trg_Ttop(tnum, &gTrg.ttop[tnum]);
		dmem_Rd_Trg_State(tnum, &state);
		dmem_Rd_Trg_Type(tnum, &type);
		if (state) SETB(gTrg.state_vector, tnum);
		if (type) SETB(gTrg.type_vector, tnum);
		//trg_Off_Warm(tnum);
		trg_Out_to_Z(tnum);
	}
}


void trg(void)
{
	for (ubase_t tnum=0; tnum<TRG_NUM; tnum++)
	{
		trg_Handle(tnum);
	}
}


bool trg_On(ubase_t tnum)
//---включает терморегулятор
{
	uint8_t state;
	uint8_t type;
	if (!dmem_Rd_Trg_Tbottom(tnum, &gTrg.tbottom[tnum])) return 0;
	if (!dmem_Rd_Trg_Ttop(tnum, &gTrg.ttop[tnum])) return 0;
	if (!dmem_Rd_Trg_State(tnum, &state)) return 0;
	if (!dmem_Rd_Trg_Type(tnum, &type)) return 0;
	if (type) SETB(gTrg.type_vector, tnum);
	trg_Off_Warm(tnum);

	if (!dmem_Wr_Trg_State(tnum, 1)) return 0;
	SETB(gTrg.state_vector, tnum);
	return 1;
}


bool trg_Off(ubase_t tnum)
//---выключает терморегулятор
{
	//trg_Off_Warm(tnum);
	trg_Out_to_Z(tnum);
	CLRB(gTrg.state_vector, tnum);
	if (!dmem_Wr_Trg_State(tnum, 0)) return 0;
	return 1;
}


bool trg_Set_Tbottom(ubase_t tnum, int16_t tbottom)
//---устанавливает tbottom
{
	if (gTrg.ttop[tnum]-1 <= tbottom)
	{
		msg_Post(&msgW_TrgTbmoreTt, MSG_TIME_TYPE, 0);
		return 0;
	}

	if (!dmem_Wr_Trg_Tbottom(tnum, tbottom)) return 0;
	gTrg.tbottom[tnum] = tbottom;
	return 1;
}


bool trg_Set_Ttop(ubase_t tnum, int16_t ttop)
//---устанавливает ttop
{
	if (ttop <= (gTrg.tbottom[tnum]+1))
	{
		msg_Post(&msgW_TrgTbmoreTt, MSG_TIME_TYPE, 0);
		return 0;
	}

	if (!dmem_Wr_Trg_Ttop(tnum, ttop)) return 0;
	gTrg.ttop[tnum] = ttop;
	return 1;
}


bool trg_Set_Type(ubase_t tnum, uint8_t type)
//---устанавливает type
{
	if (!dmem_Wr_Trg_Type(tnum, type)) return 0;
	if (type) SETB(gTrg.type_vector, tnum);
	else CLRB(gTrg.type_vector, tnum);
	return 1;
}

void trg_Default(ubase_t tnum)
//---по умолчанию значения терморегулятора
{
	dmem_Wr_Trg_Tbottom(tnum, DEF_TRG_TBOTTOM);
	dmem_Wr_Trg_Ttop(tnum, DEF_TRG_TTOP);
	dmem_Wr_Trg_State(tnum, DEF_TRG_STATE);
	dmem_Wr_Trg_Type(tnum, DEF_TRG_TYPE);
}

bool trg_OutState(ubase_t tnum)
//---узнает включен ли выход у данного термостата
{
	if (tnum == 0)
	{
		if (TRG_CHECK_TRG1_OUT() && CHKB(gTrg.type_vector, 0)) return 1;
		else if (!TRG_CHECK_TRG1_OUT() && !CHKB(gTrg.type_vector, 0)) return 1;
		else return 0;
	}
	else
	{
		if (TRG_CHECK_TRG2_OUT() && CHKB(gTrg.type_vector, 1)) return 1;
		else if (!TRG_CHECK_TRG2_OUT() && !CHKB(gTrg.type_vector, 1)) return 1;
		else return 0;
	}
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

void trg_Handle(ubase_t tnum)
//---обработка терморегулятора
{
	if (!CHKB(gTrg.state_vector, tnum)) return;
	ubase_t sta;
	if (tnum)
		sta = STA_TMP_2;
	else
		sta = STA_TMP_1;
	if (STA_IS_E(sta))
	{
		trg_Off_Warm(tnum);
		return;
	}
	if (!CHKB(gTmp.state_vector, tnum)) return;

	if (gTmp.code[tnum] > gTrg.ttop[tnum])
	{
		trg_Off_Warm(tnum);
	}
	else if (gTmp.code[tnum] < gTrg.tbottom[tnum])
	{
		trg_On_Warm(tnum);
	}
}

void trg_On_Warm(ubase_t tnum)
{
	switch (tnum)
	{
		case 0:
		{
			if (CHKB(gTrg.type_vector, 0))
			{
				TRG_1_ON_WARM();
			}
			else
			{
				TRG_1_OFF_WARM();
			}
			break;
		}
		case 1:
		{
			if (CHKB(gTrg.type_vector, 1))
			{
				TRG_2_ON_WARM();
			}
			else
			{
				TRG_2_OFF_WARM();
			}
			break;
		}
		default: break;
	}
}


void trg_Off_Warm(ubase_t tnum)
{
	switch (tnum)
	{
		case 0:
		{
			if (CHKB(gTrg.type_vector, 0))
			{
				TRG_1_OFF_WARM();
			}
			else
			{
				TRG_1_ON_WARM();
			}
			break;
		}
		case 1:
		{
			if (CHKB(gTrg.type_vector, 1))
			{
				TRG_2_OFF_WARM();
			}
			else
			{
				TRG_2_ON_WARM();
			}
			break;
		}
		default: break;
	}
}


void trg_Out_to_Z(ubase_t tnum)
//---переводит выход в "Z" состояние
{
	if (tnum)
	{
		TRG_2_TO_Z();
	}
	else
	{
		TRG_1_TO_Z();
	}
}
