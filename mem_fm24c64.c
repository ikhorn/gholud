#include "mem_fm24c64.h"
#include "mem.h"


//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

bool mem_Fm24c64_Open(uint8_t chip, dir_t dir)
//---открывает доступ к памяти
{
	if (gMem.chip_adr > FM24C64_SIZE) return false;
	if (fm24c64_Open(chip, (uint16_t)gMem.chip_adr, dir)) return true;
	return false;
}


ubase_t mem_Fm24c64_Tx_Data(uint8_t* data, ubase_t dsize)
//---пишет в память, возвращает зписанное количество
{
	for (ubase_t i=0; i<dsize; i++)
	{
		if (gMem.chip_adr > FM24C64_SIZE) return i;
		if (!fm24c64_Tx_Byte(*data++)) return i;
		gMem.chip_adr++;
	}
	return dsize;
}


ubase_t mem_Fm24c64_Rx_Data(uint8_t* data, ubase_t dsize)
//---читает из памяти, возвращает считанное кол. байт
{
	for (ubase_t i=0; i<dsize; i++)
	{
		if (gMem.chip_adr > FM24C64_SIZE) return i;
		fm24c64_Rx_Byte(data++);
		gMem.chip_adr++;
	}
	return dsize;
}


void mem_Fm24c64_Close(dir_t dir)
//---закрывает доступ к памяти
{
	fm24c64_Close(dir);
}
