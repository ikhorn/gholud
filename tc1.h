/* Модуль 16-битного тамера
 * процессор....................................................................atmega128
 * номер таймера................................................................1
 * обновлен.....................................................................22.11.2010 9:37:54
*/

//------------------------------------------------------------------------------
//варианты инициализации таймера
//------------------------------------------------------------------------------
/*
																				//инициализация таймера, срабатывание по сравнению, выходы отключены, прерывания запрещены
#define TC1_INI(SOURCE, TOP)													\
{																				\
	TC1_SET_SOURCE(TC1_NO_CLOCK);												\
	TC1_MODE_CTC_OC();															\
																				\
	TC1_OFF_OUT_OCA();															\
	TC1_OFF_OUT_OCB();															\
	TC1_OFF_OUT_OCC();															\
																				\
	TC1_SET_COUNTER(0);															\
																				\
	TC1_CLR_ACOMP_INTFL();														\
	TC1_CLR_BCOMP_INTFL();														\
	TC1_CLR_CCOMP_INTFL();														\
	TC1_CLR_OVER_INTFL();														\
																				\
	TC1_DBL_ACOMP_INT();														\
	TC1_DBL_BCOMP_INT();														\
	TC1_DBL_CCOMP_INT();														\
	TC1_DBL_OVER_INT();															\
	TC1_DBL_INPCOMP_INT();														\
																				\
	TC1_SET_TOPA(TOP);															\
	TC1_SET_SOURCE(SOURCE);														\
}
*/

//------------------------------------------------------------------------------
//порты
//------------------------------------------------------------------------------

#define TC1_OCA_PORT						PORTB
#define TC1_OCA_PIN							PINB
#define TC1_OCA_DDR							DDRB
#define TC1_OCA_B							5

#define TC1_OCB_PORT						PORTB
#define TC1_OCB_PIN							PINB
#define TC1_OCB_DDR							DDRB
#define TC1_OCB_B							6

#define TC1_OCC_PORT						PORTB
#define TC1_OCC_PIN							PINB
#define TC1_OCC_DDR							DDRB
#define TC1_OCC_B							7

#define TC1_IC_PORT							PORTD
#define TC1_IC_DDR							DDRD
#define TC1_IC_PIN							PIND
#define TC1_IC_B							4

//------------------------------------------------------------------------------
//рабочие регистры
//------------------------------------------------------------------------------

#define TC1_TCNT_RG							TCNT1								//счетчик таймера
#define TC1_TCNT_RGL						TCNT1L
#define TC1_TCNT_RGH						TCNT1H

#define TC1_IC_RG							ICR1								//регистр захвата
#define TC1_IC_RGL							ICR1L
#define TC1_IC_RGH							ICR1H

#define TC1_OCA_RG							OCR1A								//регистр сравнения А
#define TC1_OCA_RGL							OCR1AL
#define TC1_OCA_RGH							OCR1AH

#define TC1_OCB_RG							OCR1B								//регистр сравнения B
#define TC1_OCB_RGL							OCR1BL
#define TC1_OCB_RGH							OCR1BH

#define TC1_OCC_RG							OCR1C								//регистр сравнения C
#define TC1_OCC_RGL							OCR1CL
#define TC1_OCC_RGH							OCR1CH

#define TC1_TIMSK_RG						TIMSK								//регистр 1 маски прерываний всех таймеров
#define TC1_OCIE2_B							7
#define TC1_TOIE2_B							6
#define TC1_TICIE1_B						5
#define TC1_OCIE1A_B						4
#define TC1_OCIE1B_B						3
#define TC1_TOIE1_B							2
#define TC1_OCIE0_B							1
#define TC1_TOIE0_B							0

#define TC1_ETIMSK_RG						ETIMSK								//регистр 2 маски прерываний всех таймеров
#define TC1_TICIE3_B						5
#define TC1_OCIE3A_B						4
#define TC1_OCIE3B_B						3
#define TC1_TOIE3_B							2
#define TC1_OCIE3C_B						1
#define TC1_OCIE1C_B						0

#define TC1_TIFR_RG							TIFR								//регистр 1 флагов прерываний всех таймеров
#define TC1_OCF2_B							7
#define TC1_TOV2_B							6
#define TC1_ICF1_B							5
#define TC1_OCF1A_B							4
#define TC1_OCF1B_B							3
#define TC1_TOV1_B							2
#define TC1_OCF0_B							1
#define TC1_TOV0_B							0

#define TC1_ETIFR_RG						ETIFR								//регистр 2 флагов прерываний всех таймеров
#define TC1_ICF3_B							5
#define TC1_OCF3A_B							4
#define TC1_OCF3B_B							3
#define TC1_TOV3_B							2
#define TC1_OCF3C_B							1
#define TC1_OCF1C_B							0

#define TC1_CTRL_A_RG						TCCR1A
#define TC1_COMA1_B							7									//бит выбора действия на выходе пина A
#define TC1_COMA0_B							6									//бит выбора действия на выходе пина A
#define TC1_COMB1_B							5									//бит выбора действия на выходе пина B
#define TC1_COMB0_B							4									//бит выбора действия на выходе пина B
#define TC1_COMC1_B							3									//бит выбора действия на выходе пина C
#define TC1_COMC0_B							2									//бит выбора действия на выходе пина C
#define TC1_WGM1_B							1									//бит установления режима
#define TC1_WGM0_B							0									//бит установления режима

#define TC1_CTRL_B_RG						TCCR1B
#define TC1_ICNC_B							7									//бит включения шумо-подавителя на входе
#define TC1_ICES_B							6									//бит выбора события по входу (спадающий или возрастающий фронт)
#define TC1_WGM3_B							4									//бит выбора режима работы таймра/счетчика
#define TC1_WGM2_B							3									//бит выбора режима работы таймра/счетчика
#define TC1_CS2_B							2									//бит выбора предделителя
#define TC1_CS1_B							1									//бит выбора предделителя
#define TC1_CS0_B							0									//бит выбора предделителя

#define TC1_CTRL_C_RG						TCCR1C
#define TC1_FOCA_B							7									//бит принудительного установления пина OCOA в соот. состояние
#define TC1_FOCB_B							6									//бит принудительного установления пина OCOB в соот. состояние
#define TC1_FOCC_B							5									//бит принудительного установления пина OCOС в соот. состояние

//------------------------------------------------------------------------------
//варианты источника сигнала
//------------------------------------------------------------------------------

#define TC1_NO_CLOCK						0x00								//выключить вообще источник (первая обязательная операция)
#define TC1_DIV1							0x01								//настроить предделитель на /1
#define TC1_DIV8							0x02								//настроить предделитель на /8
#define TC1_DIV64							0x03								//настроить предделитель на /64
#define TC1_DIV256							0x04								//настроить предделитель на /256
#define TC1_DIV1024							0x05								//настроить предделитель на /1024
#define TC1_T0FALL							0x06								//настроить счетчик на внешний T0 по спаду
#define TC1_T0RISE							0x07								//настроить счетчик на внешний T0 по возрастанию

//------------------------------------------------------------------------------
//вектора прерываний
//------------------------------------------------------------------------------

#define TC1_COMPA_INTVECT					TIMER1_COMPA_vect
#define TC1_COMPB_INTVECT					TIMER1_COMPB_vect
#define TC1_COMPC_INTVECT					TIMER1_COMPC_vect
#define TC1_INPCAPT_INTVECT					TIMER1_CAPT_vect

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//функции для работы с флагами прерываний
//------------------------------------------------------------------------------

#define TC1_CLR_ACOMP_INTFL()				SETB(TC1_TIFR_RG, TC1_OCF1A_B)		//---очищает флаг прерывания от сравнения с рег. А
#define TC1_CLR_BCOMP_INTFL()				SETB(TC1_TIFR_RG, TC1_OCF1B_B)		//---очищает флаг прерывания от сравнения с рег. B
#define TC1_CLR_CCOMP_INTFL()				SETB(TC1_ETIFR_RG, TC1_OCF1C_B)		//---очищает флаг прерывания от сравнения с рег. C
#define TC1_CLR_OVER_INTFL()				SETB(TC1_TIFR_RG, TC1_TOV1_B)		//---очищает флаг от переполнения
#define TC1_CLR_INPCAPT_INTFL()				SETB(TC1_TIFR_RG, TC1_ICF1_B)		//---очищает флаг от захвата

#define TC1_WAS_ACOMP_INT					CHKB(TC1_TIFR_RG, TC1_OCF1A_B)
#define TC1_WAS_BCOMP_INT					CHKB(TC1_TIFR_RG, TC1_OCF1B_B)
#define TC1_WAS_OVER_INT					CHKB(TC1_TIFR_RG, TC1_TOV1_B)
#define TC1_WAS_INPCAPT_INT					CHKB(TC1_TIFR_RG, TC1_ICF1_B)

//------------------------------------------------------------------------------
//функции настройки прерываний
//------------------------------------------------------------------------------

#define TC1_EBL_ACOMP_INT()					SETB(TC1_TIMSK_RG, TC1_OCIE1A_B)	//---разрешает прерывания от сравнения с рег. A
#define TC1_DBL_ACOMP_INT()					CLRB(TC1_TIMSK_RG, TC1_OCIE1A_B)	//---запрещает прерывания от сравнения с рег. A
#define TC1_EBL_BCOMP_INT()					SETB(TC1_TIMSK_RG, TC1_OCIE1B_B)	//---разрешает прерывания от сравнения с рег. В
#define TC1_DBL_BCOMP_INT()					CLRB(TC1_TIMSK_RG, TC1_OCIE1B_B)	//---запрещает прерывания от сравнения с рег. B
#define TC1_EBL_CCOMP_INT()					SETB(TC1_ETIMSK_RG, TC1_OCIE1C_B)	//---разрешает прерывания от сравнения с рег. C
#define TC1_DBL_CCOMP_INT()					CLRB(TC1_ETIMSK_RG, TC1_OCIE1C_B)	//---запрещает прерывания от сравнения с рег. C
#define TC1_EBL_OVER_INT()					SETB(TC1_TIMSK_RG, TC1_TOIE1_B)		//---разрешает прерывания по переполнению счетчика
#define TC1_DBL_OVER_INT()					CLRB(TC1_TIMSK_RG, TC1_TOIE1_B)		//---запрещает прерывания по переполнению счетчика
#define TC1_EBL_INPCAPT_INT()				SETB(TC1_TIMSK_RG, TC1_TICIE1_B)	//---разрешает прерывания по входу
#define TC1_DBL_INPCAPT_INT()				CLRB(TC1_TIMSK_RG, TC1_TICIE1_B)	//---запрещает прерывания по входу

//------------------------------------------------------------------------------
//функции настройки и считки значений
//------------------------------------------------------------------------------

																				//---настраивает счетчик
#define TC1_SET_COUNTER(X)														\
{																				\
	TC1_TCNT_RGH = (uint8_t)((X)>>8);											\
	TC1_TCNT_RGL = (uint8_t)(X);												\
}
																				//---читает счетчик
#define TC1_READ_COUNTER(X)														\
{																				\
	(X) = TC1_TCNT_RGL;															\
	(X) |= ((uint16_t)TC1_TCNT_RGH<<8);											\
}
																				//---читает регистр захвата
#define TC1_READ_ICR(X)															\
{																				\
	(X) = TC1_IC_RGL;															\
	(X) |= ((uint16_t)TC1_IC_RGH<<8);											\
}
																				//---настраивает A вершину
#define TC1_SET_TOPA(X)															\
{																				\
	TC1_OCA_RGH = (uint8_t)((X)>>8);											\
	TC1_OCA_RGL = (uint8_t)(X);													\
}
																				//---настраивает B вершину
#define TC1_SET_TOPB(X)															\
{																				\
	TC1_OCB_RGH = (uint8_t)((X)>>8);											\
	TC1_OCB_RGL = (uint8_t)(X);													\
}
																				//---настраивает C вершину
#define TC1_SET_TOPC(X)															\
{																				\
	TC1_OCC_RGH = (uint8_t)((X)>>8);											\
	TC1_OCC_RGL = (uint8_t)(X);													\
}

//------------------------------------------------------------------------------
//функции настройки источника сигнала для таймера
//------------------------------------------------------------------------------
																				//выставляет делитель тактового сигнала
#define TC1_SET_SOURCE(DIV)														\
{																				\
	TC1_CTRL_B_RG &= ~(B(TC1_CS0_B)|B(TC1_CS1_B)|B(TC1_CS2_B));					\
	TC1_CTRL_B_RG |= (DIV);														\
}

#define TC1_OFF()							TC1_SET_SOURCE(TC1_NO_CLOCK);		//выключает таймер

//------------------------------------------------------------------------------
//функции настройки действий на выходе
//------------------------------------------------------------------------------
																				//ВХОД ICA
																				//настроить захват по спадающему фронту
#define TC1_SET_INCAPT_FALLEDGE()												\
{																				\
	CLRB(TC1_IC_DDR, TC1_IC_B);													\
	CLRB(TC1_IC_PORT, TC1_IC_B);												\
	CLRB(TC1_CTRL_B_RG, TC1_ICES_B);											\
}

																				//настроить захват по наростающему фронту
#define TC1_SET_INCAPT_RISEEDGE()												\
{																				\
	CLRB(TC1_IC_DDR, TC1_IC_B);													\
	CLRB(TC1_IC_PORT, TC1_IC_B);												\
	SETB(TC1_CTRL_B_RG, TC1_ICES_B);											\
}
																				//включает шумовой фильтр по входу
#define TC1_ON_NOISECANCELER()													\
{																				\
	SETB(TC1_CTRL_B_RG, TC1_ICNC_B);											\
}

																				//отключает шумовой фильтр по входу
#define TC1_OFF_NOISECANCELER()													\
{																				\
	CLRB(TC1_CTRL_B_RG, TC1_ICNC_B);											\
}
																				//ВЫХОД OCA
																				//отключение выхода OCA
#define TC1_OFF_OUT_OCA()														\
{																				\
	CLRB(TC1_CTRL_A_RG, TC1_COMA0_B);											\
	CLRB(TC1_CTRL_A_RG, TC1_COMA1_B);											\
}

																				//настроить на переключение выход OCA
#define TC1_SET_OUT_TOGGLE_OCA()												\
{																				\
	CLRB(TC1_OCA_PORT, TC1_OCA_B);												\
	SETB(TC1_OCA_DDR, TC1_OCA_B);												\
	SETB(TC1_CTRL_A_RG, TC1_COMA0_B);											\
	CLRB(TC1_CTRL_A_RG, TC1_COMA1_B);											\
}

																				//настроить на очищение выход OCA
#define TC1_SET_OUT_CLR_OCA()													\
{																				\
	CLRB(TC1_OCA_PORT, TC1_OCA_B);												\
	SETB(TC1_OCA_DDR, TC1_OCA_B);												\
	CLRB(TC1_CTRL_A_RG, TC1_COMA0_B);											\
	SETB(TC1_CTRL_A_RG, TC1_COMA1_B);											\
}

																				//настроить на установку выход OCA
#define TC1_SET_OUT_SET_OCA()													\
{																				\
	CLRB(TC1_OCA_PORT, TC1_OCA_B);												\
	SETB(TC1_OCA_DDR, TC1_OCA_B);												\
	SETB(TC1_CTRL_A_RG, TC1_COMA0_B);											\
	SETB(TC1_CTRL_A_RG, TC1_COMA1_B);											\
}

																				//форсировать установку на выходе OCA в соот. уровень
#define TC1_FORCE_OUT_OCA()														\
{																				\
	SETB(TC1_CTRL_C_RG, TC1_FOCA_B);											\
}

																				//ВЫХОД OCB
																				//отключение выхода OCB
#define TC1_OFF_OUT_OCB()														\
{																				\
	CLRB(TC1_CTRL_A_RG, TC1_COMB0_B);											\
	CLRB(TC1_CTRL_A_RG, TC1_COMB1_B);											\
}

																				//настроить на переключение выход OCB
#define TC1_SET_OUT_TOGGLE_OCB()												\
{																				\
	CLRB(TC1_OCB_PORT, TC1_OCB_B);												\
	SETB(TC1_OCB_DDR, TC1_OCB_B);												\
	SETB(TC1_CTRL_A_RG, TC1_COMB0_B);											\
	CLRB(TC1_CTRL_A_RG, TC1_COMB1_B);											\
}

																				//настроить на очищение выход OCB
#define TC1_SET_OUT_CLR_OCB()													\
{																				\
	CLRB(TC1_OCB_PORT, TC1_OCB_B);												\
	SETB(TC1_OCB_DDR, TC1_OCB_B);												\
	CLRB(TC1_CTRL_A_RG, TC1_COMB0_B);											\
	SETB(TC1_CTRL_A_RG, TC1_COMB1_B);											\
}

																				//настроить на установку выход OCB
#define TC1_SET_OUT_SET_OCB()													\
{																				\
	CLRB(TC1_OCB_PORT, TC1_OCB_B);												\
	SETB(TC1_OCB_DDR, TC1_OCB_B);												\
	SETB(TC1_CTRL_A_RG, TC1_COMB0_B);											\
	SETB(TC1_CTRL_A_RG, TC1_COMB1_B);											\
}

																				//форсировать установку на выходе OCB в соот. уровень
#define TC1_FORCE_OUT_OCB()														\
{																				\
	SETB(TC1_CTRL_C_RG, TC1_FOCB_B);											\
}

																				//ВЫХОД OCC
																				//отключение выхода OCC
#define TC1_OFF_OUT_OCC()														\
{																				\
	CLRB(TC1_CTRL_A_RG, TC1_COMC0_B);											\
	CLRB(TC1_CTRL_A_RG, TC1_COMC1_B);											\
}

																				//настроить на переключение выход OCC
#define TC1_SET_OUT_TOGGLE_OCC()												\
{																				\
	CLRB(TC1_OCC_PORT, TC1_OCC_B);												\
	SETB(TC1_OCC_DDR, TC1_OCC_B);												\
	SETB(TC1_CTRL_A_RG, TC1_COMC0_B);											\
	CLRB(TC1_CTRL_A_RG, TC1_COMC1_B);											\
}

																				//настроить на очищение выход OCC
#define TC1_SET_OUT_CLR_OCC()													\
{																				\
	CLRB(TC1_OCC_PORT, TC1_OCC_B);												\
	SETB(TC1_OCC_DDR, TC1_OCC_B);												\
	CLRB(TC1_CTRL_A_RG, TC1_COMC0_B);											\
	SETB(TC1_CTRL_A_RG, TC1_COMC1_B);											\
}

																				//настроить на установку выход OCC
#define TC1_SET_OUT_SET_OCC()													\
{																				\
	CLRB(TC1_OCC_PORT, TC1_OCC_B);												\
	SETB(TC1_OCC_DDR, TC1_OCC_B);												\
	SETB(TC1_CTRL_A_RG, TC1_COMC0_B);											\
	SETB(TC1_CTRL_A_RG, TC1_COMC1_B);											\
}

																				//форсировать установку на выходе OCC в соот. уровень
#define TC1_FORCE_OUT_OCC()														\
{																				\
	SETB(TC1_CTRL_C_RG, TC1_FOCC_B);											\
}

//------------------------------------------------------------------------------
//функции настройки режима работы
//------------------------------------------------------------------------------
																				//нормальный режим работы
#define TC1_MODE_NORMAL()														\
{																				\
	CLRB(TC1_CTRL_A_RG, TC1_WGM0_B);											\
	CLRB(TC1_CTRL_A_RG, TC1_WGM1_B);											\
	CLRB(TC1_CTRL_B_RG, TC1_WGM2_B);											\
	CLRB(TC1_CTRL_B_RG, TC1_WGM3_B);											\
}

																				//режим сравнения с выходным регистром OCR
#define TC1_MODE_CTC_OC()														\
{																				\
	CLRB(TC1_CTRL_A_RG, TC1_WGM0_B);											\
	CLRB(TC1_CTRL_A_RG, TC1_WGM1_B);											\
	CLRB(TC1_CTRL_B_RG, TC1_WGM3_B);											\
	SETB(TC1_CTRL_B_RG, TC1_WGM2_B);											\
}




