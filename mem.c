#include "crc.h"
#include "mem.h"


//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

extern void mem_Ini(void);
extern bool mem_Open(dir_t direct);
extern bool mem_Tx_Byte(uint8_t byte);
extern bool mem_Rx_Byte(uint8_t* byte);
extern bool mem_Tx_Data(uint8_t* data, ubase_t dsize);
extern bool mem_Rx_Data(uint8_t* data, ubase_t dsize);
extern void mem_Close(dir_t direct);

no_init_ gMEM_ gMem;

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

//--#include "err.h"//////////////////////////////////////////////////////////////////
bool mem_Write_Param(uint32_t adr, const void* data, ubase_t dsize)
//---пишет с проверкой + crc и с попытками в случае ошибки
{
	uint8_t crc;
	crc = crc_Count_Crc8(data, dsize);
	for (ubase_t i=0; i<PTRY; i++)
	{
		mem_Close(DIR_RD);
		uint8_t par[MEM_MAXPARSIZE+1];											//скопировать данные в буфер передачи
		for (ubase_t i=0; i<dsize; i++) par[i] = ((uint8_t*)data)[i];
		par[dsize] = crc;

		gMem.adr = adr;															//записать параметр
		if (!mem_Open(DIR_WR)) continue;
		if (!mem_Tx_Data(par, dsize+1)) continue;
		mem_Close(DIR_WR);
		
		gMem.adr = adr;															//считать и проверить
		if (!mem_Open(DIR_RD)) continue;
		if (!mem_Rx_Data(par, dsize+1)) continue;
		ubase_t j=0;
		for (; j<dsize; j++)
		{
			if (par[j] != ((uint8_t*)data)[j]) break;
		}
		
		if (j != dsize) continue;
		if (par[j] != crc) continue;
		mem_Close(DIR_RD);
		return true;
	}
	mem_Close(DIR_RD);
	//--err_FM24C64();///////////////////////////////////////////////////////////////////
	return false;
}


bool mem_Read_Param(uint32_t adr, void* data, ubase_t dsize)
//---считывает с попытками + проверка crc
{
	for (ubase_t i=0; i<PTRY; i++)
	{
		uint8_t par[MEM_MAXPARSIZE+1];
		gMem.adr = adr;															//считать параметр
		if (!mem_Open(DIR_RD)) continue;
		if (!mem_Rx_Data(par, dsize+1)) continue;
		mem_Close(DIR_RD);
		uint8_t crc = crc_Count_Crc8(par, dsize);
		if (crc != par[dsize]) continue;
		for (ubase_t i=0; i<dsize; i++) ((uint8_t*)data)[i] = par[i];
		return true;
	}
	return false;
}


bool mem_Write_ResParam(uint32_t adr, uint32_t res_adr, const void* data, ubase_t dsize)
//---записывает с проверкой и попытками в случае ошибки + crc и с учетом резервного места
{
	if (!mem_Write_Param(adr, data, dsize)) return false;
	if (!mem_Write_Param(res_adr, data, dsize)) return false;
	return true;
}


bool mem_Read_ResParam(uint32_t adr, uint32_t res_adr, void *data, ubase_t dsize)
//---читает данные попытками + crc с учетом резервного места
{
	if (mem_Read_Param(adr, data, dsize))
	{
		if (mem_Read_Param(res_adr, data, dsize)) return true;
		if (!mem_Write_Param(res_adr, data, dsize)) return false;
		return true;
	}
	if (mem_Read_Param(res_adr, data, dsize))									//восстановить с резервного места
	{
		if (!mem_Write_Param(adr, data, dsize)) return false;
		return true;
	}
	return false;
}





