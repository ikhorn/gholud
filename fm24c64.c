#include "iic.h"
#include "fm24c64.h"

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

void fm24c64_Ini(void)
//---инициализация микросхемы памяти памяти
{
	iic_Ini();
}


bool fm24c64_Open(uint8_t dev_adr, uint16_t adr, dir_t dir)
//---открывает память
{
	if (!iic_Open(dev_adr, DIR_WR))
	{
		return false;
	}
	if (!iic_Tx_Byte(adr>>8)) return false;
	if (!iic_Tx_Byte(adr)) return false;
	if (dir == DIR_RD)
	{
		return iic_Open(dev_adr, DIR_RD);
	}
	return true;
}


bool fm24c64_Tx_Byte(uint8_t byte)
//---передает байт
{
	return iic_Tx_Byte(byte);
}


void fm24c64_Rx_Byte(uint8_t* byte)
//---принимает байт
{
	iic_Rx_Byte(byte, true);
}


void fm24c64_Close(dir_t dir)
//---обрывает связь с памятю
{
	if (dir == DIR_RD)
	{
		uint8_t idle;
		iic_Rx_Byte(&idle, false);
		iic_Close();
	}
	else
	{
		iic_Close();
	}
}
