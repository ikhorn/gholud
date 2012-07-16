/* Модуль 8-битного тамера
 * процессор....................................................................atmega128
 * номер таймера................................................................0
 * обновлен.....................................................................08.12.2010 18:06:00
*/

//------------------------------------------------------------------------------
//варианты инициализации таймера
//------------------------------------------------------------------------------
/*
																				//инициализация таймера, срабатывание по сравнению, выходи отключены, прерывания запрещены
#define TC0_INI(SOURCE, TOP)													\
{																				\
	TC0_SET_SOURCE(TC0_NO_CLOCK);												\
	TC0_MODE_CTC_OC();															\
																				\
	TC0_OFF_OUT_OC();															\
																				\
	TC0_SET_COUNTER(0);															\
																				\
	TC0_CLR_COMP_INTFL();														\
	TC0_CLR_OVER_INTFL();														\
																				\
	TC0_DBL_COMP_INT();															\
	TC0_DBL_OVER_INT();															\
																				\
	TC0_SET_TOP(TOP);															\
	TC0_SET_SOURCE(SOURCE);														\
}
*/

//------------------------------------------------------------------------------
//порты
//------------------------------------------------------------------------------

#define TC0_OC_PORT							PORTB
#define TC0_OC_PIN							PINB
#define TC0_OC_DDR							DDRB
#define TC0_OC_B							4

//------------------------------------------------------------------------------
//рабочие регистры
//------------------------------------------------------------------------------

#define TC0_CTRL_RG							TCCR0								//регистр контроля
#define TC0_FOC_B							7									//бит принудительного установления пина OCO в соот. состояние
#define TC0_WGM0_B							6									//бит установления режима
#define TC0_COM1_B							5									//бит выбора действия на выходе пина
#define TC0_COM0_B							4									//бит выбора действия на выходе пина
#define TC0_WGM1_B							3									//бит установления режима
#define TC0_CS2_B							2									//бит выбора источника (предделителя)
#define TC0_CS1_B							1									//бит выбора источника (предделителя)
#define TC0_CS0_B							0									//бит выбора источника (предделителя)

#define TC0_TIFR_RG							TIFR								//регистр флагов прерываний всех таймеров
#define TC0_OCF2_B							7
#define TC0_TOV2_B							6
#define TC0_ICF1_B							5
#define TC0_OCF1A_B							4
#define TC0_OCF1B_B							3
#define TC0_TOV1_B							2
#define TC0_OCF0_B							1
#define TC0_TOV0_B							0

#define TC0_TIMSK_RG						TIMSK								//регистр маски прерываний всех таймеров
#define TC0_OCIE2_B							7
#define TC0_TOIE2_B							6
#define TC0_TICIE1_B						5
#define TC0_OCIE1A_B						4
#define TC0_OCIE1B_B						3
#define TC0_TOIE1_B							2
#define TC0_OCIE0_B							1
#define TC0_TOIE0_B							0

#define TC0_ASSR_RG							ASSR								//асинхронный статус регистр
#define TC0_AS_B							3									//бит включения асинхронного клока
#define TC0_TCNUB_B							2									//флаг обновленности ЕСТ регистра в асин. режиме
#define TC0_OCRUB_B							1									//флаг обновленности OCR регистра в асин. режиме
#define TC0_TCRUB_B							0									//флаг обновленности регистра контроля в асин. режиме

#define TC0_OCR								OCR0								//регистр сравнения
#define TC0_TCNT_RG							TCNT0								//регистр таймера/счетчика

//------------------------------------------------------------------------------
//варианты источника сигнала
//------------------------------------------------------------------------------

#define TC0_NO_CLOCK						0x00								//выключить вообще источник
#define TC0_DIV1							0x01								//настроить предделитель на /1
#define TC0_DIV8							0x02								//настроить предделитель на /8
#define TC0_DIV32							0x03								//настроить предделитель на /32
#define TC0_DIV64							0x04								//настроить предделитель на /64
#define TC0_DIV128							0x05								//настроить предделитель на /128
#define TC0_DIV256							0x06								//настроить предделитель на /256
#define TC0_DIV1024							0x07								//настроить предделитель на /1024

//------------------------------------------------------------------------------
//вектора прерываний
//------------------------------------------------------------------------------

#define TC0_COMP_INTVECT					TIMER0_COMP_vect

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//функции настройки значений
//------------------------------------------------------------------------------

#define TC0_SET_COUNTER(X)					TC0_TCNT_RG = (X)					//---настраивает счетчик таймера
#define TC0_SET_TOP(X)						TC0_OCR = (X)						//---настраивает вершину таймера

//------------------------------------------------------------------------------
//функции для работы с флагами прерываний
//------------------------------------------------------------------------------

#define TC0_CLR_COMP_INTFL()				SETB(TC0_TIFR_RG, TC0_OCF0_B)		//---очищает флаг прерывания от сравнения с рег. А
#define TC0_CLR_OVER_INTFL()				SETB(TC0_TIFR_RG, TC0_TOV0_B)		//---очищает флаг от переполнения
#define TC0_WAS_COMP_INT					CHKB(TC0_TIFR_RG, TC0_OCF0_B)

//------------------------------------------------------------------------------
//функции настройки прерываний
//------------------------------------------------------------------------------

#define TC0_EBL_COMP_INT()					SETB(TC0_TIMSK_RG, TC0_OCIE0_B)		//---разрешает прерывания от сравнения с рег. A
#define TC0_DBL_COMP_INT()					CLRB(TC0_TIMSK_RG, TC0_OCIE0_B)		//---запрещает прерывания от сравнения с рег. A
#define TC0_EBL_OVER_INT()					SETB(TC0_TIMSK_RG, TC0_TOIE0_B)		//---разрешает прерывания по переполнению счетчика
#define TC0_DBL_OVER_INT()					CLRB(TC0_TIMSK_RG, TC0_TOIE0_B)		//---запрещает прерывания по переполнению счетчика

//------------------------------------------------------------------------------
//функции настройки источника сигнала для таймера
//------------------------------------------------------------------------------
																				//выставляет делитель тактового сигнала
#define TC0_SET_SOURCE(DIV)														\
{																				\
	TC0_CTRL_RG &= ~(B(TC0_CS0_B)|B(TC0_CS1_B)|B(TC0_CS2_B));					\
	TC0_CTRL_RG |= (DIV);														\
}

#define TC0_OFF()							TC0_SET_SOURCE(TC0_NO_CLOCK);		//выключает таймер

//------------------------------------------------------------------------------
//функции настройки действий на выходе
//------------------------------------------------------------------------------

																				//ВЫХОД OC
																				//отключение выхода OC
#define TC0_OFF_OUT_OC()														\
{																				\
	CLRB(TC0_CTRL_RG, TC0_COM0_B);												\
	CLRB(TC0_CTRL_RG, TC0_COM1_B);												\
}

																				//настроить на переключение выход OC
#define TC0_SET_OUT_TOGGLE_OC()													\
{																				\
	CLRB(TC0_OC_PORT, TC0_OC_B);												\
	SETB(TC0_OC_DDR, TC0_OC_B);													\
	SETB(TC0_CTRL_RG, TC0_COM0_B);												\
	CLRB(TC0_CTRL_RG, TC0_COM1_B);												\
}

																				//настроить на очищение выход OC
#define TC0_SET_OUT_CLR_OC()													\
{																				\
	CLRB(TC0_OC_PORT, TC0_OC_B);												\
	SETB(TC0_OC_DDR, TC0_OC_B);													\
	CLRB(TC0_CTRL_RG, TC0_COM0_B);												\
	SETB(TC0_CTRL_RG, TC0_COM1_B);												\
}

																				//настроить на установку выход OC
#define TC0_SET_OUT_SET_OC()													\
{																				\
	CLRB(TC0_OC_PORT, TC0_OC_B);												\
	SETB(TC0_OC_DDR, TC0_OC_B);													\
	SETB(TC0_CTRL_RG, TC0_COM0_B);												\
	SETB(TC0_CTRL_RG, TC0_COM1_B);												\
}

																				//форсировать установку на выходе OC в соот. уровень
#define TC0_FORCE_OUT_OC()														\
{																				\
	SETB(TC0_CTR_RG, TC0_FOC_B);												\
}

//------------------------------------------------------------------------------
//функции настройки режима работы
//------------------------------------------------------------------------------
																				//нормальный режим работы
#define TC0_MODE_NORMAL()														\
{																				\
	CLRB(TC0_CTRL_RG, TC0_WGM0_B);												\
	CLRB(TC0_CTRL_RG, TC0_WGM1_B);												\
}

																				//режим сравнения с выходным регистром OCR
#define TC0_MODE_CTC_OC()														\
{																				\
	CLRB(TC0_CTRL_RG, TC0_WGM0_B);												\
	SETB(TC0_CTRL_RG, TC0_WGM1_B);												\
}
