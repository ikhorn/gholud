#include "iic.h"
#include "delay.h"

//------------------------------------------------------------------------------
//порты
//------------------------------------------------------------------------------

#define SCL_PORT      						PORTB
#define SCL_DDR       						DDRB
#define SCL_PIN      						PINB
#define SCL_B    							6

#define SDA_PORT      						PORTB
#define SDA_DDR       						DDRB
#define SDA_PIN      						PINB
#define SDA_B    							7

//------------------------------------------------------------------------------
//задержки
//------------------------------------------------------------------------------

#if I2C_MSR_FRQ == ___I2C_100000

#define DELAY_LOW_LEV()						delay_us(5);						//задержка низкого уровня для 100 кГц (HI >= 4 мкс, LO >= 4,7 мкс)
#define DELAY_HIGH_LEV()					delay_us(5);						//задержка высокого уровня для 100 кГц (HI >= 4 мкс, LO >= 4,7 мкс)
#define DELAY_STOP_COND()					delay_us(5);						//задержка перед установлением стопового условия
#define DELAY_STR_COND()					delay_us(5);						//время выдержки стартового условия
#define DELAY_NEW_OPR()						delay_us(5);						//задержка перед новой операцией (bus must be free)
#define DELAY_SCL_TO_SDA()					delay_us(5);						//время выдержки установленого SCL перед уст. SDA (start set up time)
#define DELAY_PAST_STOP()	        	 	delay_us(10);						//задержка после стопа. Ее можно уменьшить. Выбиралась перед чтением.
#define DELAY_BEFOR_TRY()					delay_us(10);						//задержка перед очередной попыткой установления связи

#elif I2C_MSR_FRQ == ___I2C_400000

#define DELAY_LOW_LEV()						delay_us(2);//NOP6_();//NOP5_();	//задержка низкого уровня  min 1.2 мкс для 400 кГц
#define DELAY_HIGH_LEV()					delay_us(2);//NOP4_();//NOP4_();	//задержка высокого уровня min 0.6 мкс для 400 кГц
#define DELAY_STOP_COND()					delay_us(2);//NOP6_();//NOP6_();	//задержка перед установлением стопового условия min 1.2 мкс для 400 кГц
#define DELAY_STR_COND()					delay_us(2);//NOP4_();//NOP4_();	//время выдержки стартового условия (start hold_time) min 0.6 мкс для 400 кГц
#define DELAY_NEW_OPR()						delay_us(2);//NOP6_();//NOP6_();	//задержка перед новой операцией (bus must be free) min 1.2 мкс для 400 кГц
#define DELAY_SCL_TO_SDA()					delay_us(2);//NOP4_();//NOP4_();	//время выдержки установленого SCL перед уст. SDA (start set up time) min 0.6 мкс для 400 кГц
#define DELAY_PAST_STOP()	    	     	delay_us(10);						//задержка после стопа. Ее можно уменьшить. Выбиралась перед чтением.
#define DELAY_BEFOR_TRY()					delay_us(4);						//задержка перед очередной попыткой установления связи

#endif

#define DELAY_SLAVE_UNSLEEP()				delay_us(50);						//задержка на роздупление слейвов

//------------------------------------------------------------------------------
//всякие константы
//------------------------------------------------------------------------------

																				//дожидается освобождения линии для дальнейшей передачи
#define WAITE_RELEASE_SCL()														\
{																				\
	for (ubase_t k=0; k<25; k++)													\
	{																			\
		if (CHKB(SCL_PIN, SCL_B)) break;										\
	};																			\
}


#define WR_OPR								0									//операция записи
#define RD_OPR								1									//операция считывания

//------------------------------------------------------------------------------
//операции
//------------------------------------------------------------------------------

#define START_SCL()																\
{																				\
	CLRBIT(SCL_PORT, SCL_B);													\
	CLRBIT(SCL_DDR, SCL_B);														\
}


#define STOP_SCL()																\
{																				\
	CLRBIT(SCL_PORT, SCL_B);													\
	SETBIT(SCL_DDR, SCL_B);														\
}


#define START_SDA()																\
{																				\
	CLRBIT(SDA_PORT, SDA_B);													\
	CLRBIT(SDA_DDR, SDA_B);														\
}


#define STOP_SDA()																\
{																				\
	CLRBIT(SDA_PORT, SDA_B);													\
	SETBIT(SDA_DDR, SDA_B);														\
}

#define IS_SDA							CHKB(SDA_PIN, SDA_B)

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

void iic_Ini(void)
//---инициализация iic
{
	//iic_Ini_Ports();
	START_SDA();
	delay_us(1);
	START_SCL();
	/*for (BASEU_ i=0; i<9; i++)
	{
		STOP_SCL();
		DELAY_LOW_LEV();
		START_SCL();
		DELAY_LOW_LEV();
	}*/
	iic_Close();
	DELAY_SLAVE_UNSLEEP();														//задержка на роздупление слейвов (навсяк случай)
}


bool iic_Open(uint8_t dev, dir_t direct)
//---начинает связь
{
	DELAY_NEW_OPR();
	START_SCL();
	WAITE_RELEASE_SCL();
	DELAY_SCL_TO_SDA();
	STOP_SDA();
	DELAY_STR_COND();
	STOP_SCL();
	uint8_t slave = dev | direct;												//вставить в адрес прибора бит чтения или записи
	return (iic_Tx_Byte(slave));												//выбрать прибор
}


bool iic_Tx_Byte(uint8_t byte)
//---записывает байт
{
	for (ubase_t nbit=0; nbit<8; nbit++)
    {
		DELAY_LOW_LEV();														//задержка перед установлением SDA (для того чтобы успел подняться SCL после старта)
		if ((byte & 0x80))
		{
			START_SDA();														//настройка маски и вывод sda бита
		}
		else
		{
			STOP_SDA();
		}
		DELAY_LOW_LEV();
		START_SCL();
		WAITE_RELEASE_SCL();
		DELAY_HIGH_LEV();
		STOP_SCL();
		byte <<= 1;
    }
	DELAY_LOW_LEV();															//задержка перед установлением SDA (для того чтобы успел подняться SCL после старта)
    START_SDA();																//настроить sda бит для чтения
    DELAY_LOW_LEV();
	START_SCL();
	WAITE_RELEASE_SCL();														//дождаться освобождения SCL
    DELAY_HIGH_LEV();
    if (IS_SDA) return false;													//проверка АСК
	STOP_SCL();
    return true;
}


void iic_Rx_Byte(uint8_t *byte, ubase_t ACK)
//---считывает байт
{
	for (ubase_t nbit=0; nbit<8; nbit++)
    {
		*byte <<= 1;
		DELAY_LOW_LEV();
		START_SCL();
		WAITE_RELEASE_SCL();													//дождаться освобождения SCL
		DELAY_HIGH_LEV();
		if (IS_SDA) *byte |= 0x01;
		else *byte |= 0x00;
		STOP_SCL();
    }
	DELAY_LOW_LEV();															//дождатся опускания линии SCL
	if (ACK)
	{
		STOP_SDA();
	}
    DELAY_LOW_LEV();
	START_SCL();
	WAITE_RELEASE_SCL();
    DELAY_HIGH_LEV();
	STOP_SCL();
	DELAY_LOW_LEV();
	START_SDA();																//установить SDA для чтения
}


void iic_Close(void)
//---окончание передачи
{
	STOP_SDA();
    DELAY_LOW_LEV();
	START_SCL();
	DELAY_STOP_COND();
	START_SDA();
	DELAY_PAST_STOP();
}
