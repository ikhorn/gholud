#include "bc1602e.h"
#include "../delay.h"


/*
 * work with ports
 */
/* RS */
#define ON_RS()\
{\
	SETB(BC1602E_RS_DDR, BC1602E_RS_B);\
	SETB(BC1602E_RS_PORT, BC1602E_RS_B);\
}

#define OFF_RS()\
{\
	SETB(BC1602E_RS_DDR, BC1602E_RS_B);\
	CLRB(BC1602E_RS_PORT, BC1602E_RS_B);\
}

/* RW */
#define ON_RW()\
{\
	SETB(BC1602E_RW_DDR, BC1602E_RW_B);\
	SETB(BC1602E_RW_PORT, BC1602E_RW_B);\
}

#define OFF_RW()\
{\
	SETB(BC1602E_RW_DDR, BC1602E_RW_B);\
	CLRB(BC1602E_RW_PORT, BC1602E_RW_B);\
}

/* EBL */
#define ON_EBL()\
{\
	SETB(BC1602E_EBL_DDR, BC1602E_EBL_B);\
	SETB(BC1602E_EBL_PORT, BC1602E_EBL_B);\
}

#define OFF_EBL()\
{\
	SETB(BC1602E_EBL_DDR, BC1602E_EBL_B);\
	CLRB(BC1602E_EBL_PORT, BC1602E_EBL_B);\
}

/* DATA */
#define RX_DATA(DATA)\
{\
	BC1602E_DATA_DDR = 0x00;\
	DATA = BC1602E_DATA_PIN;\
}

#define TX_DATA(DATA)\
{\
	BC1602E_DATA_DDR = 0xFF;\
	BC1602E_DATA_PORT = DATA;\
}

#define DATA_TO_WR()\
{\
	BC1602E_DATA_PORT = 0x00;\
	BC1602E_DATA_DDR = 0xFF;\
}

#define DATA_TO_RD()\
{\
	BC1602E_DATA_PORT = 0x00;\
	BC1602E_DATA_DDR = 0x00;\
}

/* check readiness */
#define IS_RDY		(!CHKB(BC1602E_DATA_PIN, BC1602E_DATA_7B))

/* them mask to read adress counter */
#define AC_MASK		0x7F

/* strobing */
#define STROBE()\
{\
	ON_EBL();\
	DELAY_STROBE();\
	OFF_EBL();\
}

/* strobe delay */
#define DELAY_STROBE()	delay_us(2);

static void bc1602e_poll_ready(void);

void bc1602e_ini(void)
{
	bc1602e_off_light();
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

/* send command, (take in mind than for CLEAR_DISP delay in 2ms is needed) */
void bc1602e_cmd(uint8_t cmd)
{
	bc1602e_poll_ready();
	OFF_RS();
	OFF_RW();
	TX_DATA(cmd);
	STROBE();
}

/* go to an adress of special symbol */
void bc1602e_set_spec_sym_pos(uint8_t adr)
{
	bc1602e_poll_ready();
	OFF_RS();
	OFF_RW();
	CLRB(adr, BC1602E_DATA_7B);
	SETB(adr, BC1602E_DATA_6B);
	TX_DATA(adr);
	STROBE();
}

/* set cursor */
void bc1602e_set_sym_pos(uint8_t adr)
{
	bc1602e_poll_ready();
	OFF_RS();
	OFF_RW();
	SETB(adr, BC1602E_DATA_7B);
	TX_DATA(adr);
	STROBE();
}

/* output given symbol */
void bc1602e_set_current_byte(uint8_t sym)
{
	bc1602e_poll_ready();
	OFF_RW();
	ON_RS();
	TX_DATA(sym);
	STROBE();
}


/* output given symbol to the given position */
void bc1602e_set_byte(uint8_t pos, uint8_t sym)
{
	bc1602e_set_sym_pos(pos);
	bc1602e_set_current_byte(sym);
}


/* read current symbol */
void bc1602e_get_sym(uint8_t *sym)
{
	bc1602e_poll_ready();
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


/* read adress counter */
void bc1602e_get_pos(uint8_t *AC)
{
	bc1602e_poll_ready();
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
//---checking
bool bc1602e_check(void)
{
	enum{CHECK_POS = 79, CHECK_SYMBOL = ' '};
	bc1602e_ini_ports();
	uint8_t sym_pos;
	bc1602e_get_pos(&sym_pos);													//прочитать текущую позицию курсора
    bc1602e_Set_sym_pos(CHECK_POS);												//перейти на проверочную позицию
	uint8_t sym;
	bc1602e_get_sym(&sym);														//скопировать символ
	bc1602e_cmd(BC1602E_MOVE_CURSOR_LEFT);
	bc1602e_set_current_byte(CHECK_SYMBOL);										//вставить проверочный символ
	bc1602e_cmd(BC1602E_MOVE_CURSOR_LEFT);
	uint8_t check_sym = 0;
	bc1602e_get_sym(&check_sym);												//прочитать проверочный символ
	if (check_sym != CHECK_SYMBOL) return false;								//проверить соответствие проверочного символа
	bc1602e_cmd(BC1602E_MOVE_CURSOR_LEFT);
    bc1602e_set_current_byte(sym);												//вставить символ, тот что был
	bc1602e_set_sym_pos(sym_pos);												//вернуться на прежнюю позицию
	return true;
}*/


/* waite display readiness */
static void bc1602e_poll_ready(void)
{
	enum {WAITE=60000};
	DATA_TO_RD();
	OFF_RS();
	ON_RW();
	ON_EBL();

	for (uint16_t i = 0; i < WAITE; i++) {
		if (IS_RDY)
			break;
	}

	OFF_EBL();
	OFF_RW();
	DATA_TO_WR();
}

