#include "bcd.h"


//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

bool bcd_Bcd_to_Num(bcd_t bcd_num, uint8_t *num)
//---переводит число в bcd в обычное число <= 99
{
	uint8_t hi_bcd;
	uint8_t lo_bcd;
	lo_bcd = bcd_num & 0x0F;
	hi_bcd = bcd_num >> 4;
	if ((lo_bcd > 9) || (hi_bcd > 9)) return false;
	hi_bcd *= 10;
	*num = hi_bcd + lo_bcd;
	return true;
}


bool bcd_Num_to_Bcd(uint8_t num, bcd_t *bcd_num)
//---переобразовует число в bcd формат, для чисел меньше 99
{
	if (num < 10)
	{
		*bcd_num = num;
		return true;
	}
	
	if (num > 99) return false;
	
	fl32_t float_num = num;
	uint16_t int_num;
	int_num = (uint16_t)float_num/10;
	*bcd_num = int_num;
	*bcd_num <<= 4;
	int_num *= 10;
	num -= int_num;
    *bcd_num += num;
	return true;
}


bool bcd_Plus_Bcd(bcd_t x1, bcd_t x2, bcd_t *y)
//---суммирует два числа в формате bcd, результат bcd
{
	uint8_t bcd_lo_x1;
	uint8_t bcd_lo_x2;
	uint8_t bcd_hi_x1;
	uint8_t bcd_hi_x2;
	bcd_lo_x1 = x1 & 0x0F;
	bcd_hi_x1 = x1 >> 4;
	bcd_lo_x2 = x2 & 0x0F;
	bcd_hi_x2 = x2 >> 4;
	
	if ((bcd_lo_x1 > 9) || (bcd_lo_x2 > 9) || (bcd_hi_x1 > 9) || (bcd_hi_x2 > 9))//проверить или формат bcd коректный
	{
		return false;
	}
	
	uint8_t bcd_lo_y = (bcd_lo_x1 + bcd_lo_x2);
	uint8_t bcd_hi_y = 0;
	if (bcd_lo_y > 9)
	{
		bcd_hi_y = 1;
		bcd_lo_y -= 10;
	}
	bcd_hi_y += bcd_hi_x1 + bcd_hi_x2;
	
	if (bcd_hi_y > 9) return false; 											//проверить или сумарное число коректно
	bcd_hi_y <<= 4;
	*y = bcd_hi_y | bcd_lo_y;
	return true;
}

bool bcd_Minus_Bcd(bcd_t x1, bcd_t x2, bcd_t *y)
//---тнимает два bcd числа, результат bcd
{
	if (x2 > x1) return 0;
	uint8_t bcd_lo_x1;
	uint8_t bcd_lo_x2;
	uint8_t bcd_hi_x1;
	uint8_t bcd_hi_x2;
	bcd_lo_x1 = x1 & 0x0F;
	bcd_hi_x1 = x1 >> 4;
	bcd_lo_x2 = x2 & 0x0F;
	bcd_hi_x2 = x2 >> 4;
	
	if ((bcd_lo_x1 > 9) || (bcd_lo_x2 > 9) ||
		(bcd_hi_x1 > 9) || (bcd_hi_x2 > 9)) return 0;							//проверить или формат bcd коректный
	
	if (bcd_lo_x1 < bcd_lo_x2)
	{
		bcd_lo_x1 += 10;
		--bcd_hi_x1;
	}
	
	*y = ((bcd_hi_x1 - bcd_hi_x2) << 4) | (bcd_lo_x1 - bcd_lo_x2);
	return 1;
}
