#include "dsp.h"

#if (DSPC_INICATOR_MODEL == ___WH1602D)
#include "ind/dsp_WH1602D.h"

#elif(DSPC_INICATOR_MODEL == ___BC1602E)
#include "dsp_BC1602E.h"

#elif(DSPC_INICATOR_MODEL == ___BG12864F)
#include <ind/dsp_BG12864F.h>

#elif(DSPC_INICATOR_MODEL == ___RLD5621)
#include <ind/dsp_RLD5621.h>
#endif







































//void dsp_Print(ubase_t strnum, ubase_t strtype, const ubase_t __flash *cmd_str,...)
////---������� ����������� ������
//// ���� ������
////				%m -- ����� ������ flash
////				%s -- ����� ������ RAM
////				%c -- ����� �������
////				%f -- ����� ����� fl32_t
////				%u -- ����� ����� uint16_t
////				%# -- ����� �� ������(������ � ������)
////				%l, %L -- ����� ����� uint32_t
////				%x -- ����� hex ����� ���� int
////				%X -- ����� hex ����� ���� int32_t
//{
//	va_list lst;
//	va_start(lst, cmd_str);
//#if (DSP_HEX_USE == 1)
//	ubase_t base = 10;															//���� ����������
//#else
//	#define base 10
//#endif
//	ubase_t line[LCD_STR_LENTH];													//������ �������� ������
//	ubase_t x = 0;																//����� ������� �������� ������
//	uint32_t whole;																//����� ��������
//	uint32_t fract;																//������� ��������
//	ubase_t prec = 8;															//������ ����� �����
//#if DSP_FLOAT_USE == 1	
//	ubase_t fract_prec = 6;														//������ ������� �����
//#endif
//#if DSP_ONSTR_USE == 1
//	unsigned ubase_t *out_line;
//#endif
//	struct
//	{
//		ubase_t fract_fl32_t_is:1;												//���� �����������, ��� ���������� ��� fl32_t
//		ubase_t on_line:1;														//���� �����������, ��� ���� ����� �� ������
//		ubase_t prec_sel:1;														//���� �����������, ��� ������� �������� �����������
//		ubase_t is_int32_t:1;														//���� �����������, ��� ��������� ��� int32_t
//	} status;
//	status.fract_fl32_t_is = false;
//	status.on_line = false;
//	status.prec_sel = false;
//	status.is_int32_t = false;
//	
//	uint32_t td;																	//�������� �������� ��� ������ �����
//	uint32_t fract_td;															//��������� �������� ��� ������ ������� �����
//	ubase_t cmd_sym; 															//������ ��������� ������
//	
//	if (strtype == DSP_STR_ARROW_PUTTED) line[x++] = '>';						//���� �� �������� - ��������� �������
//	while(cmd_sym = *cmd_str++)
//	{
//		// ������ ���������
//		if (cmd_sym == '%')
//		{
//		#if (DSP_ACCURACY_USE == 1) && (DSP_FLOAT_USE == 1 || DSP_INT_USE == 1)
//			cmd_select:
//		#endif
//			switch(cmd_sym = *cmd_str++)
//			{
//			#if DSP_FLASH_USE == 1
//				case 'm':														//������������ ���� ������
//				{
//					unsigned ubase_t __flash *c = va_arg(lst, unsigned ubase_t __flash*);
//					while(*c) line[x++] = *c++;
//					continue;
//				}
//			#endif
//			#if DSP_RAM_STRING_USE == 1
//				case 's':														//������������ ram ������
//				{
//					unsigned ubase_t *c = va_arg(lst, unsigned ubase_t *);
//					while(*c) line[x++] = *c++;
//					continue;
//				}
//			#endif
//			#if DSP_SYMBOL_USE == 1
//				case 'c':														//������������ ������
//				{
//					line[x++] = va_arg(lst, int);
//					continue;
//				}
//			#endif
//		#if (DSP_INT_USE == 1) || (DSP_FLOAT_USE == 1 || DSP_HEX_USE == 1)
//			#if (DSP_HEX_USE == 1)
//				case 'X': base = 16;
//			#endif
//			#if DSP_INT_USE == 1
//				case 'l':
//				case 'L':
//				{
//					whole = va_arg(lst, uint32_t);
//					goto whole_put;
//				}
//			#endif
//			#if (DSP_HEX_USE == 1)
//				case 'x': base = 16;
//			#endif
//				case 'u':														//������������ uint16_t
//				{
//				#if DSP_INT_USE == 1
//					whole = va_arg(lst, uint16_t);
//					whole_put:													//����� ����� int32_t ��� int	
//					if(!whole) td = base;										//��������� ������ �����
//					else td = 1;
//					while(td <= whole) td *= base;
//				#endif
//					int_put:													//������� �����
//					for(;prec && (td/=base); prec--)
//					{
//						ubase_t sym = (whole/td)%base;
//						if(sym > 9) line[x++] = sym + '7';
//						else line[x++] = sym|'0';
//						
//					}
//				#if (DSP_HEX_USE == 1)
//					base = 10;
//				#endif
//					if (status.fract_fl32_t_is)
//					{
//						status.fract_fl32_t_is = false;
//						line[x++] = '.';										//������� �������
//						td = fract_td;
//						whole = fract;
//						goto int_put;
//					}
//					if(status.prec_sel)for(ubase_t i = 0 ; i < prec; i++) line[x++] = ' ';//�������� ��������� ������������ ' '
//					continue;
//				}
//		#endif
//			#if (DSP_ACCURACY_USE == 1) && (DSP_FLOAT_USE == 1)
//				case '.':														//��������� ��������� �������������� �����
//				{
//					cmd_sym = *cmd_str++;
//					fract_prec = cmd_sym&0x0F;									//�������� �������� ����� �������
//					goto cmd_select;
//				}
//			#endif
//			#if DSP_FLOAT_USE == 1
//				case 'f':														//������������ fl32_t
//				{
//					fl64_t flt = va_arg(lst, fl64_t);
//					if (!fract_prec) flt += 0.5;								//���� ������� ����� �� ������� - ��������� ����� �����
//					whole = (uint32_t)flt;										//�������� ����� �����
//					
//					if(!whole) td = 10;											//��������� ������ �����
//					else td = 1;
//					ubase_t whole_size = 0;
//					while(td <= whole)
//					{
//						td *= 10;
//						whole_size++;											//���������� ���������� �������� ����� �����
//					}
//					
//					if (prec <= whole_size)										//���. �������� �������
//					{
//						flt += 0.5;
//						whole = (uint32_t)flt;									//������������� ����� �����
//						goto int_put;
//					}
//					
//					status.fract_fl32_t_is = true;								//������������ ��� ������� �����
//					
//					ubase_t fract_rest = prec - whole_size;
//					if (fract_prec > fract_rest) fract_prec = fract_rest;		//��������������� ���. �������� ������� �����
//					flt -= whole;												//�������� ������� �����
//					
//					fract_td = 1;												//������� ������� ����� �����
//					for(ubase_t i = 0; i < fract_prec; i++)
//					{
//						flt *= 10.0;
//						fract_td *=10;
//					};
//					
//					flt += 0.5;													//��������� ����� �����
//					fract = (uint32_t)flt;
//					
//					if(fract >= fract_td)										//���� ���������� ������� � ���������� �� 1 ������
//					{
//						fract -= fract_td;
//						whole++;
//						if(td <= whole) td *= 10;
//					}
//					goto int_put;
//				}
//			#endif
//			#if DSP_ONSTR_USE == 1
//				case '#':														//��������������� ����� �� ������
//				{
//					status.on_line = true;										//���������� ����� �� ������
//					out_line = va_arg(lst, ubase_t *);
//					continue;
//				}
//			#endif
//				default:														//��������� ����� �������������� ��������
//				{
//				#if (DSP_ACCURACY_USE == 1) && (DSP_FLOAT_USE == 1 || DSP_INT_USE == 1)
//					if(cmd_sym >= '0' && cmd_sym <= '9')
//					{
//						prec = cmd_sym&0x0F;									//�������� ������ ����� �����
//						status.prec_sel = true;									//���������� ��� �������� ��������
//						goto cmd_select;
//					}
//				#endif
//					line[x] = 0;
//					va_end(lst);												//��������� ��������� ������ ����������
//					return;
//				}
//			}
//		}
//		else line[x++] = cmd_sym;												//��������� ������ �� ������ ����������
//	}
//	va_end(lst);																//��������� ��������� ������ ����������
//	line[x] = 0;
//	
//	/// ������������ � ������	
//	ubase_t add_sym;																//���������� ������������� ������
//	switch(strtype)
//	{
//		case DSP_STR_WOW_PUTTED: add_sym = '!';break;
//		case DSP_STR_STAR_PUTTED: add_sym = '*';break;
//        default: add_sym = ' '; break;
//	}
//
//	ubase_t size;																//���������� ������ ���������� ��������� ������� ������
//	if(strtype == DSP_STR_JUST_PUTTED) size = 0;
//	else
//	{
//		if(x>=DSP_VIR_STR_LENTH) x = DSP_VIR_STR_LENTH;
//		size = ((DSP_VIR_STR_LENTH - x) >> 1);
//	}
//	
//#if DSP_ONSTR_USE == 1
//	if(status.on_line)															///���� ����� �� ������
//	{
//		for(ubase_t i = 0, s = 0; i < LCD_VIR_STR_LENTH; i++)
//		{
//			if(i<size)
//			{
//				if(size == i+1) *out_line = ' ';
//				else *out_line = add_sym;
//			}
//			else if (s < x) *out_line = line[s++];
//			else
//			{
//				if(line_type == GUI_STR_JUST_PUTTED) break;
//				if(s==x)
//				{
//					*out_line = ' ';
//					s++;
//				}
//				else *out_line = add_sym;
//			}
//			out_line++;
//		}
//		*out_line=0;
//		return;
//	}
//#endif
//	
//	/// ���� ����� �� ���
//	ubase_t str_pos;																//���������� ��������� ��������� �������
//#if LCD_STR_NUM == 2
//    if(strnum == 1) str_pos = LCD_STR_START_1;
//    else str_pos = LCD_STR_START_2;
//#elif LCD_STR_NUM == 4
//    switch (strnum)
//	{
//		case 1: str_pos = LCD_STR_START_1; break;
//		case 2: str_pos = LCD_STR_START_2; break;
//		case 3: str_pos = LCD_STR_START_3; break;
//		case 4: str_pos = LCD_STR_START_4; break;
//	}
//#endif
//	
//	lcd_Set_SymAdr(str_pos);													//����� �� ��������� ������� � ������������� � ��������
//
//	for(ubase_t i = 0, s = 0; i < DSP_VIR_STR_LENTH; i++)						//������� ������
//	{
//		if(i < size)
//		{
//			if(size == i+1) lcd_Set_DueCurrentSym(' ');
//			else lcd_Set_DueCurrentSym(add_sym);
//		}
//		else if(s < x) lcd_Set_CurrentSym(line[s++]);
//		else
//		{
//			if(s == x)
//			{
//				lcd_Set_DueCurrentSym(' ');
//				s++;
//			}
//			else lcd_Set_DueCurrentSym(add_sym);
//		}
//	}
//}

/////////////------------------------------------------------------------

//////////////////////

//	ubase_t strsize = buf - line - 1;
//	buf = line;
//	
//	ubase_t fullsize;															//���������� ������ ���������� ��������� ������� ������
//	if(strtype == GUI_STR_JUST_PUTTED) fullsize = 0;
//	else
//	{
//		if(strsize >= LCD_VIR_STR_LENTH) strsize = LCD_VIR_STR_LENTH;
//		fullsize = ((LCD_VIR_STR_LENTH - strsize) >> 1);
//	}
//	
//#if DSP_ONSTR_USE == 1
//	if(status.on_line)															//���� ����� �� ������
//	{
//		for(ubase_t i = 0, s = 0; i < LCD_VIR_STR_LENTH; i++)
//		{
//			if(i<fullsize)
//			{
//				if(fullsize == i+1) *out_line = ' ';
//				else *out_line = add_sym;
//			}
//			else if (s < strsize) *out_line = line[s++];
//			else
//			{
//				if(line_type == GUI_STR_JUST_PUTTED) break;
//				if(s==strsize)
//				{
//					*out_line = ' ';
//					s++;
//				}
//				else *out_line = add_sym;
//			}
//			out_line++;
//		}
//		*out_line=0;
//		return;
//	}
//#endif
//	
//	/// ���� ����� �� ���
//	ubase_t str_pos;																//���������� ��������� ��������� �������
//#if LCD_STR_NUM == 2
//    if(strnum == 1) str_pos = LCD_STR_START_1;
//    else str_pos = LCD_STR_START_2;
//#elif LCD_STR_NUM == 4
//    switch (strnum)
//	{
//		case 1: str_pos = LCD_STR_START_1; break;
//		case 2: str_pos = LCD_STR_START_2; break;
//		case 3: str_pos = LCD_STR_START_3; break;
//		case 4: str_pos = LCD_STR_START_4; break;
//	}
//#endif
//	
//	lcd_SymAdr_Set(str_pos);													//����� �� ��������� ������� � ������������� � ��������
//
//	for(ubase_t i = 0, s = 0; i < LCD_VIR_STR_LENTH; i++)						//������� ������
//	{
//		if(i < fullsize)
//		{
//			if(fullsize == i+1) lcd_DueCurrentSym_Put(' ');
//			else lcd_DueCurrentSym_Put(add_sym);
//		}
//		else if(s < strsize) 
//		{
//			lcd_CurrentSym_Put(*buf++);
//			s++;
//		}
//		else
//		{
//			if(s == strsize)
//			{
//				lcd_DueCurrentSym_Put(' ');
//				s++;
//			}
//			else lcd_DueCurrentSym_Put(add_sym);
//		}
//	}
//}
//
//
//
////=== ������ �������������� ������ (� ������ ��� �������� 19)
//
//void lcd_Line_Unwrap(ubase_t line_num, const ubase_t *line)
//{
//	ubase_t str_pos;
//	switch (line_num)															// ���������� �������� �� ������
//	{
//		case 1: str_pos = LCD_LINE_START_1; break;
//		case 2: str_pos = LCD_LINE_START_2; break;
//		case 3: str_pos = LCD_LINE_START_3; break;
//		case 4: str_pos = LCD_LINE_START_4; break;
//	}
//	ubase_t max_left_lcd_pos = str_pos + 9;										// ������������ ������� �� ��� ������ �����
//	ubase_t str_left_lcd_pos = max_left_lcd_pos;									// ��������� ������� �� ��� ������ �����
//	ubase_t left_lcd_pos;														// ������� �� ��� ������ �����
//	ubase_t right_lcd_pos;														// ������� �� ��� ������� �����
//	ubase_t str_right_line_pos = 19;												// ��������� ������� ������� ������ ������� �����
//	ubase_t right_line_pos;														// ������� ������� ������ ������� �����
//	ubase_t left_line_pos;														// ������� ������� ������ ������ �����
//	lcd_Sym_on_Pos_Put(max_left_lcd_pos, line[9]);								// ��������� ������� ������
//	while(str_left_lcd_pos != str_pos)
//	{
//		delay_ms(8);															// �������� ����� ���������������
//		LCD_LIGHT_SET;
//		delay_ms(8);
//		LCD_LIGHT_USET;
//		delay_ms(8);
//		LCD_LIGHT_SET;
//		str_left_lcd_pos--;
//		left_lcd_pos = str_left_lcd_pos; 										// ������� �� ���
//		right_lcd_pos = max_left_lcd_pos + 1; 									//(����������)
//		
//		// ������� �� ������
//		left_line_pos = 0; //(����������)
//		str_right_line_pos--;
//		right_line_pos = str_right_line_pos;
//		while (left_lcd_pos < max_left_lcd_pos)
//		{
//			lcd_Sym_on_Pos_Put(left_lcd_pos, line[left_line_pos]);
//			left_line_pos++;
//			left_lcd_pos++;
//			lcd_Sym_on_Pos_Put(right_lcd_pos, line[right_line_pos]);
//			right_lcd_pos++;
//			right_line_pos++;
//		}
//		delay_ms(8);															// �������� ����� ���������������
//		LCD_LIGHT_USET;
//		delay_ms(8);
//		LCD_LIGHT_SET;
//		delay_ms(8);
//		LCD_LIGHT_USET;
//	}
//}
//
//
//void gui_Print_Put_Whole(uint32_t* whole, uint32_t* td, ubase_t base, ubase_t prec, ubase_t** buf);
//void gui_Print_Put_Float(fl64_t* flt, ubase_t prec, ubase_t fract_prec, ubase_t** buf);
//
//
//void gui_Print_Put_Whole(uint32_t* whole, uint32_t* td, ubase_t base, ubase_t prec, ubase_t** buf)
//{
//	for(;prec && (*td/=base); prec--)
//	{
//		ubase_t sym = ((*whole)/(*td))%base;
//		if(sym > 9) *(*buf)++ = sym + '7';
//		else *(*buf)++ = sym|'0';
//	}
//}
//
//void gui_Print_Put_Integer(uint32_t* whole, ubase_t base, ubase_t prec, ubase_t** buf)
//{
//	uint32_t td;
//	td = (!(*whole)) ? base : 1;												//��������� ������ �����
//	while(td <= *whole) td *= base;
//	gui_Print_Put_Whole(whole, &td, base, prec, buf);							//������� �����
//#if (DSP_HEX_USE == 1)
//	base = 10;
//#endif
//	for(ubase_t i = 0 ; i < prec; i++) *(*buf)++ = ' ';							//�������� ��������� ������������ ' '
//}
//
//
//void gui_Print_Put_Float(fl64_t* flt, ubase_t prec, ubase_t fract_prec, ubase_t** buf)
//{
//	if (!fract_prec) (*flt) += 0.5;												//���� ������� ����� �� ������� - ��������� ����� �����
//	uint32_t whole = (uint32_t)flt;												//�������� ����� �����
//	
//	
//	uint32_t td;
//	td = (!whole) ? 10 : 1;														//��������� ������ �����
//	ubase_t whole_size = 0;
//	while(td <= whole)
//	{
//		td *= 10;
//		whole_size++;															//���������� ���������� �������� ����� �����
//	}
//	
//	
//	if (prec <= whole_size)														//���. �������� �������
//	{
//		(*flt) += 0.5;
//		whole = (uint32_t)(*flt);												//������������� ����� �����
//		gui_Print_Put_Whole(&whole, &td, 10, prec, buf);
//		return;
//	}
//	
//	ubase_t fract_rest = prec - whole_size;
//	if(fract_prec > fract_rest) fract_prec = fract_rest;						//��������������� ���. �������� ������� �����
//	
//	(*flt) -= whole;															//�������� ������� �����
//	
//	uint32_t fract_td = 1;														//������� ������� ����� �����
//	for(ubase_t i = 0; i < fract_prec; i++)
//	{
//		(*flt) *= 10.0;
//		fract_td *=10;
//	};
//	
//	(*flt) += 0.5;																//��������� ����� �����
//	uint32_t fract = (uint32_t)flt;
//	
//	if(fract >= fract_td)														//���� ���������� ������� � ���������� �� 1 ������
//	{
//		fract -= fract_td;
//		whole++;
//		if(td <= whole) td *= 10;
//	}
//	
//	gui_Print_Put_Whole(&whole, &td, 10, prec, buf);							//������� ����� �����
//	*(*buf)++ = '.';															//������� �������
//	gui_Print_Put_Whole(&fract, &fract_td, 10, prec, buf);						//������� ������� �����
//}
//
//void gui_Print(ubase_t strnum, ubase_t strtype, const ubase_t __flash *cmd_str,...)
////---������� ����������� ������
//// ���� ������
////				%m -- ����� ������ flash
////				%s -- ����� ������ RAM
////				%c -- ����� �������
////				%f -- ����� ����� fl32_t
////				%u -- ����� ����� uint16_t
////				%# -- ����� �� ������(������ � ������)
////				%l, %L -- ����� ����� uint32_t
////				%x -- ����� hex ����� ���� int
////				%X -- ����� hex ����� ���� int32_t
//
//{
//	va_list lst;
//	va_start(lst, cmd_str);
//#if (DSP_HEX_USE == 1)
//	ubase_t base = 10;															//���� ����������
//#else
//	#define base 10
//#endif
//	ubase_t line[LCD_STR_LENTH];													//������ �������� ������
//	ubase_t* buf = line;
//	ubase_t prec = 8;															//������ ����� �����
//#if DSP_FLOAT_USE == 1	
//	ubase_t fract_prec = 6;														//������ ������� �����
//#endif
//#if DSP_ONSTR_USE == 1
//	unsigned ubase_t *out_line;
//#endif
//#if (DSP_ONSTR_USE == 1)
//	ubase_t on_line = false;
//#endif
//
//	ubase_t cmd_sym; 															//������ ��������� ������
//	if (strtype == GUI_STR_ARROW_PUTTED) *buf++ = '>';							//���� �� �������� - ��������� �������
//	while(cmd_sym = *cmd_str++)
//	{
//		// ������ ���������
//		if (cmd_sym != '%')
//		{
//			*buf++ = cmd_sym;													//��������� ������ �� ������ ����������
//			continue;
//		}
//		#if (DSP_ACCURACY_USE == 1) && (DSP_FLOAT_USE == 1 || DSP_INT_USE == 1)
//	cmd_select:
//		#endif
//		switch(cmd_sym = *cmd_str++)
//		{
//		#if DSP_FLASH_USE == 1
//			case 'm':															//������������ ���� ������
//			{
//				unsigned ubase_t __flash *c = va_arg(lst, unsigned ubase_t __flash*);
//				while(*c) *buf++ = *c++;
//				continue;
//			}
//		#endif
//		#if DSP_RAM_STRING_USE == 1
//			case 's':															//������������ ram ������
//			{
//				unsigned ubase_t *c = va_arg(lst, unsigned ubase_t *);
//				while(*c) *buf++ = *c++;
//				continue;
//			}
//		#endif
//		#if DSP_SYMBOL_USE == 1
//			case 'c':															//������������ ������
//			{
//				*buf++ = va_arg(lst, int);
//				continue;
//			}
//		#endif
//		#if (DSP_HEX_USE == 1)
//			case 'X': base = 16;
//		#endif
//		#if (DSP_INT_USE == 1) || (DSP_HEX_USE == 1)
//			case 'l':
//			case 'L':
//			{
//				uint32_t whole = va_arg(lst, uint32_t);
//				gui_Print_Put_Integer(&whole, base, prec, &buf);
//				continue;
//			}
//		#endif
//		#if (DSP_HEX_USE == 1)
//			case 'x': base = 16;
//		#endif
//		#if (DSP_INT_USE == 1) || (DSP_HEX_USE == 1)
//			case 'u':															//������������ uint16_t
//			{
//			
//				uint32_t whole = 0;
//				whole = va_arg(lst, uint16_t);
//				gui_Print_Put_Integer(&whole, base, prec, &buf);
//				continue;
//			}
//		#endif
//		#if (DSP_ACCURACY_USE == 1) && (DSP_FLOAT_USE == 1)
//			case '.':															//��������� ��������� �������������� �����
//			{
//				cmd_sym = *cmd_str++;
//				fract_prec = cmd_sym&0x0F;										//�������� �������� ����� �������
//				goto cmd_select;
//			}
//		#endif
//		#if DSP_FLOAT_USE == 1
//			case 'f':															//������������ fl32_t
//			{
//				fl64_t flt = va_arg(lst, fl64_t);
//				gui_Print_Put_Float(&flt, prec, fract_prec, &buf);
//				for(ubase_t i = 0; i < prec; i++) *buf++ = ' ';					//�������� ��������� ������������ ' '
//				continue;
//			}
//		#endif
//		#if DSP_ONSTR_USE == 1
//			case '#':															//��������������� ����� �� ������
//			{
//				on_line = true;													//���������� ����� �� ������
//				out_line = va_arg(lst, unsigned ubase_t *);
//				continue;
//			}
//		#endif
//			default:															//��������� ����� �������������� ��������
//			{
//			#if (DSP_ACCURACY_USE == 1) && (DSP_FLOAT_USE == 1 || DSP_INT_USE == 1)
//				if(cmd_sym >= '0' && cmd_sym <= '9')
//				{
//					prec = cmd_sym&0x0F;										//�������� ������ ����� �����
//					goto cmd_select;
//				}
//			#endif
//				*buf++ = 0;
//				va_end(lst);													//��������� ��������� ������ ����������
//				return;
//			}
//		}
//	}
//	va_end(lst);																//��������� ��������� ������ ����������
//	*buf++ = 0;
//	
//	/// ������������ � ������	
//	ubase_t add_sym;																//���������� ������������� ������
//	switch(strtype)
//	{
//		case GUI_STR_WOW_PUTTED: add_sym = '!';break;
//		case GUI_STR_STAR_PUTTED: add_sym = '*';break;
//        default: add_sym = ' '; break;
//	}
//
//	ubase_t strsize = buf - line - 1;
//	buf = line;
//	
//	ubase_t fullsize;															//���������� ������ ���������� ��������� ������� ������
//	if(strtype == GUI_STR_JUST_PUTTED) fullsize = 0;
//	else
//	{
//		if(strsize >= LCD_VIR_STR_LENTH) strsize = LCD_VIR_STR_LENTH;
//		fullsize = ((LCD_VIR_STR_LENTH - strsize) >> 1);
//	}
//	
//#if DSP_ONSTR_USE == 1
//	if(status.on_line)															///���� ����� �� ������
//	{
//		for(ubase_t i = 0, s = 0; i < LCD_VIR_STR_LENTH; i++)
//		{
//			if(i<fullsize)
//			{
//				if(fullsize == i+1) *out_line = ' ';
//				else *out_line = add_sym;
//			}
//			else if (s < strsize) *out_line = line[s++];
//			else
//			{
//				if(line_type == GUI_STR_JUST_PUTTED) break;
//				if(s==strsize)
//				{
//					*out_line = ' ';
//					s++;
//				}
//				else *out_line = add_sym;
//			}
//			out_line++;
//		}
//		*out_line=0;
//		return;
//	}
//#endif
//	
//	/// ���� ����� �� ���
//	ubase_t str_pos;																//���������� ��������� ��������� �������
//#if LCD_STR_NUM == 2
//    if(strnum == 1) str_pos = LCD_STR_START_1;
//    else str_pos = LCD_STR_START_2;
//#elif LCD_STR_NUM == 4
//    switch (strnum)
//	{
//		case 1: str_pos = LCD_STR_START_1; break;
//		case 2: str_pos = LCD_STR_START_2; break;
//		case 3: str_pos = LCD_STR_START_3; break;
//		case 4: str_pos = LCD_STR_START_4; break;
//	}
//#endif
//	
//	lcd_SymAdr_Set(str_pos);													//����� �� ��������� ������� � ������������� � ��������
//
//	for(ubase_t i = 0, s = 0; i < LCD_VIR_STR_LENTH; i++)						//������� ������
//	{
//		if(i < fullsize)
//		{
//			if(fullsize == i+1) lcd_DueCurrentSym_Put(' ');
//			else lcd_DueCurrentSym_Put(add_sym);
//		}
//		else if(s < strsize) 
//		{
//			lcd_CurrentSym_Put(*buf++);
//			s++;
//		}
//		else
//		{
//			if(s == strsize)
//			{
//				lcd_DueCurrentSym_Put(' ');
//				s++;
//			}
//			else lcd_DueCurrentSym_Put(add_sym);
//		}
//	}
//}
