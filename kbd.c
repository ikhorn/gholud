#include "cint.h"
#include "tc0.h"
#include "delay.h"

#define ROW_NUM			3

/*
 * ports
 */
#define ROW_PORT		PORTD
#define ROW_DDR			DDRD
#define ROW_PIN			PIND
#define ROW1_B			0
#define ROW2_B			1
#define ROW3_B			2

#define COL_PORT		PORTD
#define COL_DDR			DDRD
#define COL_PIN			PIND
#define COL1_B			3
#define COL2_B			4
#define COL3_B			5
#define COL4_B			6
#define COL5_B			7

/*
 * interrupt port
 */
#define INT_PORT		PORTE
#define INT_DDR			DDRE
#define INT_PIN			PINE
#define INT_B			6

/*
 * key's map
 */

/* row 1 */
#define ROW1_1			'c'
#define ROW1_2			'7'
#define ROW1_3			'8'
#define ROW1_4			'9'
#define ROW1_5			'='
/* row 2 */
#define ROW2_1			'b'
#define ROW2_2			'4'
#define ROW2_3			'5'
#define ROW2_4			'6'
#define ROW2_5			'.'
/* row 3 */
#define ROW3_1			'a'
#define ROW3_2			'1'
#define ROW3_3			'2'
#define ROW3_4			'3'
#define ROW3_5			'0'

/* others macroses */

#define ROW_MASK		(B(ROW1_B)|B(ROW2_B)|B(ROW3_B))

#define COL_MASK		(B(COL1_B)|B(COL2_B)|B(COL3_B)|B(COL4_B)|B(COL5_B))

#define ROWS			((~ROW_PIN) & ROW_MASK)
#define COLS			(((~COL_PIN) & COL_MASK) >> ROW_NUM)

#define PULLUP_ROWS()			\
{					\
	SETMSK(ROW_PORT, ROW_MASK);	\
	CLRMSK(ROW_DDR, ROW_MASK);	\
}

#define DOWN_ROWS()			\
{					\
	CLRMSK(ROW_PORT, ROW_MASK);	\
	SETMSK(ROW_DDR, ROW_MASK);	\
}

#define DOWN_COLS()			\
{					\
	CLRMSK(COL_PORT, COL_MASK);	\
	SETMSK(COL_DDR, COL_MASK);	\
}

#define PULLUP_COLS()			\
{					\
	CLRMSK(COL_DDR, COL_MASK);	\
	SETMSK(COL_PORT, COL_MASK);	\
}

#define INT_ROW1_CLEAR_FL()			INT_CLR_INT0_FL()
#define INT_ROW1_SET_TYPE()			INT_SET_INT0_FALLTYPE()
#define INT_ROW1_DBL()				INT_DBL_INT0()
#define INT_ROW1_WAS                            INT_WAS_INT0

#define INT_ROW2_CLEAR_FL()			INT_CLR_INT1_FL()
#define INT_ROW2_SET_TYPE()			INT_SET_INT1_FALLTYPE()
#define INT_ROW2_DBL()				INT_DBL_INT1()
#define INT_ROW2_WAS                            INT_WAS_INT1

#define INT_ROW3_CLEAR_FL()			INT_CLR_INT2_FL()
#define INT_ROW3_SET_TYPE()			INT_SET_INT2_FALLTYPE()
#define INT_ROW3_DBL()				INT_DBL_INT2()
#define INT_ROW3_WAS                            INT_WAS_INT2

/* set interrupt's types */
#define INT_ROW_SET_TYPES()		\
{					\
	INT_ROW1_SET_TYPE();		\
	INT_ROW2_SET_TYPE();		\
	INT_ROW3_SET_TYPE();		\
}

/* clear flags from all interrupts */
#define INT_ROW_CLEAR_FLS()		\
{					\
	INT_ROW1_CLEAR_FL();		\
	INT_ROW2_CLEAR_FL();		\
	INT_ROW3_CLEAR_FL();		\
}

/* disable all interrupts */
#define INT_ROW_DBL()			\
{					\
	INT_ROW1_DBL();			\
	INT_ROW2_DBL();			\
	INT_ROW3_DBL();			\
}

#define ROW1_INTERRUPT_VECT			INT_INT0_VECT
#define ROW2_INTERRUPT_VECT			INT_INT1_VECT
#define ROW3_INTERRUPT_VECT			INT_INT2_VECT

/*
 * ekey (additional key for some auxiliary function)
 */

#define EKEY_PORT				PORTE
#define EKEY_DDR				DDRE
#define EKEY_B  				4

#define EKEY_DBL_INT()				INT_DBL_INT4();
#define EKEY_CLEAR_FL()                     	INT_CLR_INT4_FL();
#define EKEY_SET_INT_TYPE()			INT_SET_INT4_FALLTYPE();
#define EKEY_WAS_INT				INT_WAS_INT4

#define EKEY_PULLUP()			\
{					\
	SETB(EKEY_PORT, EKEY_B);	\
	CLRB(EKEY_DDR, EKEY_B);		\
}

/*
 * ktmr (press key timer for delay versus denouncing)
 */
#define KTMR_TOP				255
#define KTMR_SOURCE				TC0_DIV1024
#define KTMR_WAS_INT                    	TC0_WAS_COMP_INT

#define KTMR_SET()                     \
{                                      \
	TC0_SET_COUNTER(0);            \
	TC0_SET_TOP(KTMR_TOP);         \
	TC0_SET_SOURCE(KTMR_SOURCE);   \
	TC0_CLR_COMP_INTFL();          \
}

#define KTMR_OFF()				TC0_SET_SOURCE(TC0_NO_CLOCK);

/*
 * prototypes
 */

ubase_t gKey;
ubase_t gKey_waite_fl;
static ubase_t kbd_Rx_Key(void);

void kbd_Ini(void)
{
	PULLUP_ROWS();
	DOWN_COLS();
	INT_ROW_SET_TYPES();
	INT_ROW_CLEAR_FLS();
	INT_ROW_DBL();

	/* ekey */
	EKEY_PULLUP();
	EKEY_SET_INT_TYPE();
	EKEY_DBL_INT();
	EKEY_CLEAR_FL();

	/* ktmr */
	TC0_MODE_CTC_OC();
	TC0_OFF_OUT_OC();
	TC0_DBL_COMP_INT();
	KTMR_SET();
	gKey_waite_fl = 0;
}

/*
 * Function to cycle
 */
void kbd(void)
{
	if (KTMR_WAS_INT) {
		if (gKey_waite_fl) {
			gKey_waite_fl = 0;
			INT_ROW_CLEAR_FLS();
			EKEY_CLEAR_FL();
			/* KTMR_OFF(); */
			return;
		}
		if (INT_ROW1_WAS || INT_ROW2_WAS || INT_ROW3_WAS) {
			ubase_t k1, k2, k3;
			k1 = kbd_Rx_Key();
			k2 = kbd_Rx_Key();
			k3 = kbd_Rx_Key();
			if (k1 != 0) {
				if (k2 == k1 && k2 == k3)
					gKey = k1;
			}
			INT_ROW_CLEAR_FLS();
			KTMR_SET();
			gKey_waite_fl = 1;
		} else if (EKEY_WAS_INT) {
			gKey = 'd';
			EKEY_CLEAR_FL();
			KTMR_SET();
			gKey_waite_fl = 1;
		}
	}
}

/*
 * Read the key
 */
static ubase_t kbd_Rx_Key(void)
{
	uint8_t rows;	/* row map */
	uint8_t cols;	/* column map */
	rows = ROWS;
	DOWN_ROWS();
	PULLUP_COLS();
	delay_us(4);
	cols = COLS;
	DOWN_COLS();
	PULLUP_ROWS();

	if (rows == 0x00)
		return 0;
	/* row 1 */
	else if (rows == 0x01) {
		if (cols == 0x01)
			return ROW1_1;
		else if (cols == 0x02)
			return ROW1_2;
		else if (cols == 0x04)
			return ROW1_3;
		else if (cols == 0x08)
			return ROW1_4;
		else if (cols == 0x10)
			return ROW1_5;
		else
			return 0;
	/* row 2 */
	} else if (rows == 0x02) {
		if (cols == 0x01)
			return ROW2_1;
		else if (cols == 0x02)
			return ROW2_2;
		else if (cols == 0x04)
			return ROW2_3;
		else if (cols == 0x08)
			return ROW2_4;
		else if (cols == 0x10)
			return ROW2_5;
		else
			return 0;
	/* row 3 */
	} else if (rows == 0x04) {
		if (cols == 0x01)
			return ROW3_1;
		else if (cols == 0x02)
			return ROW3_2;
		else if (cols == 0x04)
			return ROW3_3;
		else if (cols == 0x08)
			return ROW3_4;
		else if (cols == 0x10)
			return ROW3_5;
		else
			return 0;
	}
	return 0;
}
