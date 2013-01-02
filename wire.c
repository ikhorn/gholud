#include "wire.h"
#include "delay.h"

//------------------------------------------------------------------------------
//������ � �������
//------------------------------------------------------------------------------

#define DQ_DOWN()																\
{																				\
	CLRB(WIRE_DQ_PORT, WIRE_DQ_B);												\
	SETB(WIRE_DQ_DDR, WIRE_DQ_B);												\
}

#define DQ_VOID()																\
{																				\
	CLRB(WIRE_DQ_DDR, WIRE_DQ_B);												\
	CLRB(WIRE_DQ_PORT, WIRE_DQ_B);												\
}

#define DQ_IS_HI								CHKB(WIRE_DQ_PIN, WIRE_DQ_B)

//------------------------------------------------------------------------------
//��������
//------------------------------------------------------------------------------

#define DELAY_RESET_PULSE()						delay_us(481)					//������� reset
#define DELAY_PRESENCE_CHK()					delay_us(270)					//������� ��� �������� ����������
#define DELAY_PAST_RESET_RDY()					delay_us(211)					//�������� ����� ������������

#define DELAY_START()							delay_us(1)						//�������� �� ������ ���� �����
#define DELAY_SLOTE()							delay_us(40)					//�������� �� ���� ����
#define DELAY_RECOV()							delay_us(2)						//�������� �� ��������������, ����� ���� �����

//------------------------------------------------------------------------------
//����. �������
//------------------------------------------------------------------------------


void wire_Ini(void)
{
	DQ_VOID();
}

bool wire_Start(void)
//---��������� ����� �����
{
	DQ_DOWN();
	DELAY_RESET_PULSE();
	DQ_VOID();
	DELAY_PRESENCE_CHK();
	if (!DQ_IS_HI) return 0;
	DELAY_PAST_RESET_RDY();
	return 1;
}

void wire_Tx_Byte(uint8_t byte)
//---���������� ����
{
	for (ubase_t nb=0; nb<8; nb++)
	{
		DQ_DOWN();
		DELAY_START();
		if (byte & 0x01)
		{
			DQ_VOID();
		}
		else
		{
			DQ_DOWN();
		}
		DELAY_SLOTE();
		DQ_VOID();
		DELAY_RECOV();
		byte >>= 1;
	}
}

void wire_Rx_Byte(uint8_t* byte)
//---��������� ����
{
	for (ubase_t nb=0; nb<8; nb++)
	{
		*byte >>= 1;
		DQ_DOWN();
		DELAY_START();
		DQ_VOID();
		DELAY_START();
		if (DQ_IS_HI) *byte |= 0x80;
		else *byte |= 0x00;
		DELAY_SLOTE();
		DELAY_RECOV();
	}
}

bool wire_Rx_Bit(void)
//---��������� ��� ����
{
	ubase_t b;
	DQ_DOWN();
	DELAY_START();
	DQ_VOID();
	DELAY_START();
	if (DQ_IS_HI) b = 1;
	else b = 0;
	DELAY_SLOTE();
	DELAY_RECOV();
	return b;
}



