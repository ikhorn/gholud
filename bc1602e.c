#include "bc1602e.h"
#include "delay.h"


//------------------------------------------------------------------------------
//работа с портами
//------------------------------------------------------------------------------
																				//RS
#define ON_RS()																	\
{																				\
	SETB(BC1602E_RS_DDR, BC1602E_RS_B);											\
	SETB(BC1602E_RS_PORT, BC1602E_RS_B);										\
}

#define OFF_RS()																\
{																				\
	SETB(BC1602E_RS_DDR, BC1602E_RS_B);											\
	CLRB(BC1602E_RS_PORT, BC1602E_RS_B);										\
}

																				//RW
#define ON_RW()																	\
{																				\
	SETB(BC1602E_RW_DDR, BC1602E_RW_B);											\
	SETB(BC1602E_RW_PORT, BC1602E_RW_B);										\
}

#define OFF_RW()																\
{																				\
	SETB(BC1602E_RW_DDR, BC1602E_RW_B);											\
	CLRB(BC1602E_RW_PORT, BC1602E_RW_B);										\
}

																				//EBL
#define ON_EBL()																\
{																				\
	SETB(BC1602E_EBL_DDR, BC1602E_EBL_B);										\
	SETB(BC1602E_EBL_PORT, BC1602E_EBL_B);										\
}

#define OFF_EBL()																\
{																				\
	SETB(BC1602E_EBL_DDR, BC1602E_EBL_B);										\
	CLRB(BC1602E_EBL_PORT, BC1602E_EBL_B);										\
}

																				//DATA
#define RX_DATA(DATA)															\
{																				\
	BC1602E_DATA_DDR = 0x00;													\
	DATA = BC1602E_DATA_PIN;													\
}

#define TX_DATA(DATA)															\
{																				\
	BC1602E_DATA_DDR = 0xFF;													\
	BC1602E_DATA_PORT = DATA;													\
}

#define DATA_TO_WR()															\
{																				\
	BC1602E_DATA_PORT = 0x00;													\
	BC1602E_DATA_DDR = 0xFF;													\
}

#define DATA_TO_RD()															\
{																				\
	BC1602E_DATA_PORT = 0x00;													\
	BC1602E_DATA_DDR = 0x00;													\
}

#define IS_RDY					(!CHKB(BC1602E_DATA_PIN, BC1602E_DATA_7B))		//проверяет готовность

//------------------------------------------------------------------------------
//остальные константы
//------------------------------------------------------------------------------

#define AC_MASK								0x7F								//маска для чтения счетчика адреса

//------------------------------------------------------------------------------
//макро функции
//------------------------------------------------------------------------------
																				//---стробирует
#define STROBE()																\
{																				\
	ON_EBL();																	\
	DELAY_STROBE();																\
	OFF_EBL();																	\
}

#define DELAY_STROBE()						delay_us(2);						//---стробовая задержка

static void bc1602e_Poll_Ready(void);

void bc1602e_Ini(void)
{
	bc1602e_Off_Light();
	DATA_TO_WR();
	OFF_RS();
	OFF_RW();
	delay_ms(40);
	TX_DATA(BC1602E_INIT_BYTE);
	STROBE();
	delay_us(40);
	TX_DATA(BC1602E_DISPON);
	STROBE();
	delay_us(40);
	TX_DATA(BC1602E_CLEAR_DISP);
	STROBE();
	delay_ms(2);
	TX_DATA(BC1602E_INCREMENTAC_SHIFTOFF);
	STROBE();
}

void bc1602e_Cmd(uint8_t cmd)
//---посылает команду, для CLEAR_DISP нужна зад. 2мс
{
	bc1602e_Poll_Ready();
	OFF_RS();
	OFF_RW();
	TX_DATA(cmd);
	STROBE();
}


void bc1602e_Set_SpecSymPos(uint8_t adr)
//---перейти на адрес специального символа
{
	bc1602e_Poll_Ready();
	OFF_RS();
	OFF_RW();
	CLRB(adr, BC1602E_DATA_7B);
	SETB(adr, BC1602E_DATA_6B);
	TX_DATA(adr);
	STROBE();
}


void bc1602e_Set_SymPos(uint8_t adr)
//---устанавливает курсор
{
	bc1602e_Poll_Ready();
	OFF_RS();
	OFF_RW();
	SETB(adr, BC1602E_DATA_7B);
	TX_DATA(adr);
	STROBE();
}

void bc1602e_Set_CurrentByte(uint8_t sym)
//---выводит должный символ
{
	bc1602e_Poll_Ready();
	OFF_RW();
	ON_RS();
	TX_DATA(sym);
	STROBE();
}


void bc1602e_Set_Byte(uint8_t pos, uint8_t sym)
//---выводит должный символ на указанную позицию
{
	bc1602e_Set_SymPos(pos);
	bc1602e_Set_CurrentByte(sym);
}


void bc1602e_Get_Sym(uint8_t *sym)
//---прочетать текущий символ
{
	bc1602e_Poll_Ready();
	DATA_TO_RD();
	ON_RW();
	ON_RS();
//	__no_operation();
	ON_EBL();
	DELAY_STROBE();
	RX_DATA(*sym);
	OFF_EBL();
	OFF_RW();
	DATA_TO_WR();
}


void bc1602e_Get_Pos(uint8_t *AC)
//---читает счетчик адреса
{
	bc1602e_Poll_Ready();
	DATA_TO_RD();
	OFF_RS();
	ON_RW();
	NOP_();
	ON_EBL();
	DELAY_STROBE();
	*AC = BC1602E_DATA_PIN & (AC_MASK);
	OFF_EBL();
	OFF_RW();
	DATA_TO_WR();
}

/*
bool bc1602e_Check(void)
//---проверка
{
	enum{CHECK_POS = 79, CHECK_SYMBOL = ' '};
	bc1602e_Ini_Ports();
	uint8_t sym_pos;
	bc1602e_Get_Pos(&sym_pos);													//прочитать текущую позицию курсора
    bc1602e_Set_SymPos(CHECK_POS);												//перейти на проверочную позицию
	uint8_t sym;
	bc1602e_Get_Sym(&sym);														//скопировать символ
	bc1602e_Cmd(BC1602E_MOVE_CURSOR_LEFT);
	bc1602e_Set_CurrentByte(CHECK_SYMBOL);										//вставить проверочный символ
	bc1602e_Cmd(BC1602E_MOVE_CURSOR_LEFT);
	uint8_t check_sym = 0;
	bc1602e_Get_Sym(&check_sym);												//прочитать проверочный символ
	if (check_sym != CHECK_SYMBOL) return false;								//проверить соответствие проверочного символа
	bc1602e_Cmd(BC1602E_MOVE_CURSOR_LEFT);
    bc1602e_Set_CurrentByte(sym);												//вставить символ, тот что был
	bc1602e_Set_SymPos(sym_pos);												//вернуться на прежнюю позицию
	return true;
}*/


static void bc1602e_Poll_Ready(void)
//---дожидается готовности ЖКИ
{
	enum {WAITE=60000};
	DATA_TO_RD();
	OFF_RS();
	ON_RW();
	ON_EBL();
	for (uint16_t i=0; i<WAITE; i++)
	{
		if (IS_RDY) break;
	}
	OFF_EBL();
	OFF_RW();
	DATA_TO_WR();
}

