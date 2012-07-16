#include "arr.h"


bool arr_Asci_to_Date(uint8_t* asci, date_t* date, ubase_t type)
//---translate ASCI line to date, type - full or not full year
{
	date->date = (*asci++ << 4);
	date->date |= (*asci++ & 0x0F);
	date->month = (*asci++ << 4);
	date->month |= (*asci++ & 0x0F);
	if (type == TDF_FULL_YEAR)
	{
		date->ctry = (*asci++ << 4);
		date->ctry |= (*asci++ & 0x0F);
		date->year = (*asci++ << 4);
		date->year |= (*asci & 0x0F);
	}
	else
	{
		date->year = (*asci++ << 4);
		date->year |= (*asci & 0x0F);
	}
	return tdf_Is_GoodDate(date);
}


bool arr_Asci_to_Fl64(uint8_t* asci, fl64_t* num)
//---translate ASCI line to number of fl64_t type
{
	ubase_t minus_flag = 0;
	if (*asci == '-')
	{
		asci++;
		minus_flag = 1;
	}
	if (!(*asci))
	{
		*num = 0.0;
		return 0;
	}

	*num = 0;
	ubase_t div_N = 0;
																				//translating to number
	for (ubase_t n = 0; *asci; n++)
	{
		if (*asci == '.')														//if comma
		{
			if (div_N) return 0;
			div_N++;
		}
		else if ((0x0F & *asci) <= 9)											//if number
		{
			*num *= 10.0;
			*num +=  0x0F & *asci;
        	if (div_N) div_N++;
		}
        else return 0;
		asci++;
	}
																				//setting the comma
    if (!div_N)
    {
    	if (minus_flag) *num = -*num;
    	return 1;
    }
	div_N--;
	for (ubase_t n = 0; n < div_N; n++) *num /= 10.0;
	if (minus_flag) *num = -*num;
	return 1;
}


bool arr_Asci_to_Int32(uint8_t* asci, int32_t* num)
//---translate ASCI line to number of int32_t type
{
	uint32_t unum;
	if (!arr_Asci_to_Uint32(((*asci == '-')?(asci+1):asci), &unum)) return 0;
	if (*asci == '-' || !unum) *num = -unum;
	else *num = unum;
	return 1;
}


bool arr_Asci_to_Time(uint8_t *asci, time_t *time)
//---translate ASCI line to time
{
	time->hour = (*asci++ << 4);
	time->hour |= (*asci++ & 0x0F);
	time->min = (*asci++ << 4);
	time->min |= (*asci++ & 0x0F);
	time->sec = (*asci++ << 4);
	time->sec |= (*asci & 0x0F);
	return tdf_Is_GoodTime(time);
}


bool arr_Asci_to_Uint32(uint8_t* asci, uint32_t* num)
//---translate ASCI line to number of uint32_t type
{
	*num = 0;
	for (;*asci;)
	{
		if ((0x0F & *asci) > 9) return 0;
		*num *= 10;
		*num += 0x0F & *asci++;
	}
	return 1;
}


ubase_t arr_Clear_Str(uint8_t* str, uint8_t* syms)
//---clear line "str" from symbols
{
    uint16_t i;
    uint16_t j;
	for (j=0, i=0; j < 65534;)
	{
		if (arr_Is_Sym(syms, str[j])) j++;
		else if (str[j]) str[i++] = str[j++];
		else
		{
			str[i] = 0;
			return 1;
		}
	}
	return 0;
}


void arr_Date_to_Asci(date_t* date, uint8_t* asci, ubase_t type)
//---add a date (bcd) to an ASCI line on a given position, type - full or not full year
{
	*asci++ = (date->date>>4)|0x30;
	*asci++ = (date->date&0x0F)|0x30;
	*asci++ = '.';
	*asci++ = (date->month>>4)|0x30;
	*asci++ = (date->month&0x0F)|0x30;
	*asci++ = '.';
	if (type == TDF_FULL_YEAR)
	{
		*asci++ = (date->ctry>>4)|0x30;
		*asci++ = (date->ctry&0x0F)|0x30;
		*asci++ = (date->year>>4)|0x30;
		*asci++ = (date->year&0x0F)|0x30;
	}
	else
	{
		*asci++ = (date->year>>4)|0x30;
		*asci++ = (date->year&0x0F)|0x30;
	}
}


ubase_t arr_Is_Sym(uint8_t* str, uint8_t sym)
//---find a symbol in a line
{
	uint16_t i;
	for (i=0; *str && i<65534; i++)
	{
		if (*str++ == sym) return 1;
	}
	return 0;
}


void arr_Roll_Str(uint8_t* str, ubase_t* pos, ubase_t out_size, uint8_t* out)
//---roll an array in a given line
{
	ubase_t size = arr_Str_Size(str);

	if (*pos > size+out_size-2) *pos = 0;

	if (size <= out_size)														//the size of the line less then screen
	{
		for (ubase_t i=0; i<out_size; i++)
		{
			out[i] = str[i];
			if (!str[i]) return;
		}
	}
	else if (*pos < size)														//the line partly full by spaces at the end
	{
		ubase_t end = size - *pos;
		for (ubase_t i=0; i<out_size; i++)
		{
			if (i < end) out[i] = str[*pos+i];
			else out[i] = ' ';
		}
		(*pos)++;
	}
	else
	{
		ubase_t end = (out_size - 1) - (*pos - size);
		for (ubase_t i=0, j=0; i<out_size; i++)									//the line partly full by spaces at the top
		{
			if (i < end) out[i] = ' ';
			else out[i] = str[j++];
		}


		if (*pos > size+out_size-2) *pos = 1;
		else (*pos)++;
	}
	out[out_size] = 0;
}


ubase_t arr_Str_Size(uint8_t* str)
//---defines a size of a string by the NUL symbol at the end
{
	ubase_t i=0;
	for (; *str && i<255; i++, str++);
	return i;
}


void arr_Time_to_Asci(time_t *time, uint8_t *asci)
//---add a time (bcd) to an ASCI line on a given position
{
	*asci++ = (time->hour>>4)|0x30;
	*asci++ = (time->hour&0x0F)|0x30;
	*asci++ = ':';
	*asci++ = (time->min>>4)|0x30;
	*asci++ = (time->min&0x0F)|0x30;
	*asci++ = ':';
	*asci++ = (time->sec>>4)|0x30;
	*asci++ = (time->sec&0x0F)|0x30;
}

