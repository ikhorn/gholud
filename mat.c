#include "mat.h"


//------------------------------------------------------------------------------
// внешние функции
//------------------------------------------------------------------------------


int32_t mat_Round_FL64(fl64_t num)
//---округляет число типа float до целого числа
{
	int32_t i_num = (int32_t)num;
	num -= i_num;
	if (num >= 0.5) i_num++;
	else if (num <= -0.5) --i_num;
	return i_num;
}


fl32_t mat_Mod_FL32(fl32_t num)
//---выдает модуль с float числа num
{
	if (num < 0) num *= -1;
	return num;
}


uint16_t mat_Mod_INT16S(int16_t num)
//---возвращает модуль с числа INT16S
{
	if (num < 0) num *= -1;
	return num;
}

uint32_t mat_Mod_INT32S(int32_t num)
//---возвращает модуль с числа INT32S
{
	if (num < 0) num *= -1;
	return num;
}


uint32_t mat_Average(uint32_t* data, uint8_t dsize)
//---возвращает усредненное значение, желательно, чтобы при dsize = 256, data =< 0xffffff
{
	uint32_t sum=0;
	for (uint8_t i=0; i<dsize; i++)
	{
		sum += *data++;
	}
	return (sum/dsize);
}



