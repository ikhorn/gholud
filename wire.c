#include "wire.h"
#include "delay.h"

//------------------------------------------------------------------------------
//работа с портами
//------------------------------------------------------------------------------

#define DQ_DOWN()\
{\
	CLRB(WIRE_DQ_PORT, WIRE_DQ_B);\
	SETB(WIRE_DQ_DDR, WIRE_DQ_B);\
}

#define DQ_VOID()\
{\
	CLRB(WIRE_DQ_DDR, WIRE_DQ_B);\
	CLRB(WIRE_DQ_PORT, WIRE_DQ_B);\
}

#define DQ_IS_HI		CHKB(WIRE_DQ_PIN, WIRE_DQ_B)

//------------------------------------------------------------------------------
//задержки
//------------------------------------------------------------------------------

#define DELAY_RESET_PULSE()	delay_us(481)	/* задржка reset */
#define DELAY_PRESENCE_CHK()	delay_us(270)	/* задржка для проверки соеденения */
#define DELAY_PAST_RESET_RDY()	delay_us(211)	/* задержка после перезагрузки */
#define DELAY_START()		delay_us(1)	/* задержка на старте тайм слота */

#define DELAY_SLOTE()		delay_us(40)	/* задержка на тайм слот */
#define DELAY_RECOV()		delay_us(2)	/* задержка на восстановление, после тайм слота */


void wire_Ini(void)
{
	DQ_VOID();
}

/*
 * wire_Start - открывает сеанс связи
 */
bool wire_Start(void)
{
	DQ_DOWN();
	DELAY_RESET_PULSE();
	DQ_VOID();
	DELAY_PRESENCE_CHK();
	if (!DQ_IS_HI)
		return 0;
	DELAY_PAST_RESET_RDY();
	return 1;
}

/*
 * wire_Tx_Byte - отправляет байт
 */
void wire_Tx_Byte(uint8_t byte)
{
	for (ubase_t nb=0; nb<8; nb++) {
		DQ_DOWN();
		DELAY_START();

		if (byte & 0x01) {
			DQ_VOID();
		} else {
			DQ_DOWN();
		}

		DELAY_SLOTE();
		DQ_VOID();
		DELAY_RECOV();
		byte >>= 1;
	}
}

/*
 * wire_Rx_Byte - принимает байт
 */
void wire_Rx_Byte(uint8_t* byte)
{
	for (ubase_t nb=0; nb<8; nb++) {
		*byte >>= 1;
		DQ_DOWN();
		DELAY_START();
		DQ_VOID();
		DELAY_START();

		if (DQ_IS_HI)
			*byte |= 0x80;
		else
			*byte |= 0x00;

		DELAY_SLOTE();
		DELAY_RECOV();
	}
}

/*
 * wire_Rx_Bit - принимает бит инфы
 */
bool wire_Rx_Bit(void)
{
	ubase_t b;
	DQ_DOWN();
	DELAY_START();
	DQ_VOID();
	DELAY_START();

	if (DQ_IS_HI)
		b = 1;
	else
		b = 0;

	DELAY_SLOTE();
	DELAY_RECOV();
	return b;
}
