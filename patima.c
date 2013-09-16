#include "pdef.h"
#include "wdt.h"
#include "rtc.h"
#include "ini.h"
#include "alm.h"
#include "tmr.h"
#include "tmp.h"
#include "rmd.h"
#include "trg.h"
#include "menucore/usr.h"
#include "sgn.h"
#include "menucore/prog/stw.h"
#include "ptimer.h"
#include "kbd.h"


int main(void)
{
	ini();
	for (;;)
	{
		rtc();	//время
		tmp();	//температуры
		stw();
		usr();
		alm();	//будильники
		tmr();	//таймера
		rmd();	//напоминалки
		trg();	//терморегуляторы
		sgn();
		kbd();
		ptimer();
		WDT_RESET();
	}
	return 0;
}




/*
void lcd_Line_Unwrap(uint8_t line_num, const uint8_t *line)
{
	uint8_t str_pos;
	switch (line_num)															// определить смещение от строки
	{
		case 1: str_pos = LCD_LINE_START_1; break;
		case 2: str_pos = LCD_LINE_START_2; break;
		case 3: str_pos = LCD_LINE_START_3; break;
		case 4: str_pos = LCD_LINE_START_4; break;
	}
	uint8_t max_left_lcd_pos = str_pos + 9;										// максимальное позиция на ЖКИ левого крыла
	uint8_t str_left_lcd_pos = max_left_lcd_pos;									// стартовая позиция на ЖКИ левого крыла
	uint8_t left_lcd_pos;															// позиция на ЖКИ левого крыла
	uint8_t right_lcd_pos;															// позиция на ЖКИ правого крыла
	uint8_t str_right_line_pos = 19;												// стартовая позиция символа строки правого крыла
	uint8_t right_line_pos;														// позиция символа строки правого крыла
	uint8_t left_line_pos;															// позиция символа строки левого крыла
	lcd_Sym_on_Pos_Put(max_left_lcd_pos, line[9]);								// выставить средний символ
	while(str_left_lcd_pos != str_pos)
	{
		delay_ms(8);															// задержка между разворачиванием
		LCD_LIGHT_SET;
		delay_ms(8);
		LCD_LIGHT_USET;
		delay_ms(8);
		LCD_LIGHT_SET;
		str_left_lcd_pos--;
		left_lcd_pos = str_left_lcd_pos; 										// позиция на ЖКИ
		right_lcd_pos = max_left_lcd_pos + 1; 									//(постоянная)

		// позиция на строке
		left_line_pos = 0; //(постоянная)
		str_right_line_pos--;
		right_line_pos = str_right_line_pos;
		while (left_lcd_pos < max_left_lcd_pos)
		{
			lcd_Sym_on_Pos_Put(left_lcd_pos, line[left_line_pos]);
			left_line_pos++;
			left_lcd_pos++;
			lcd_Sym_on_Pos_Put(right_lcd_pos, line[right_line_pos]);
			right_lcd_pos++;
			right_line_pos++;
		}
		delay_ms(8);															// задержка между разворачиванием
		LCD_LIGHT_USET;
		delay_ms(8);
		LCD_LIGHT_SET;
		delay_ms(8);
		LCD_LIGHT_USET;
	}
}
 *
*/

// Marta Ivanovna
