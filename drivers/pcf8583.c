#include "../iic.h"
#include "../crc.h"
#include "../delay.h"
#include "pcf8583.h"


//------------------------------------------------------------------------------
//адреса внутренних регистров
//------------------------------------------------------------------------------

#define CTRL_STATUS_RG_ADR					0x00								//адрес регистра управления/статуса часов
#define TIME_RG_ADR    						0x01								//начальный адрес регистра установки времени и даты
#define YE_DA_RG_ADR						0x05								//адрес регистра года и даты
#define AL_CTRL_RG_ADR    					0x08								//адрес регистра управления будильником
#define AL_TIME_RG_ADR						0x09								//начальный адрес регистра установки времени и даты будильника (без сотых долей секунд)
#define YEAR_NUM_CRT_RG_ADR					0x11								//адрес регистра в памяти часов, в котором хранится номер года
#define YEAR_CRT_RG_ADR						0x15								//адрес регистра в памяти часов, в котором хранится год от 0 - 99 (2000)

//------------------------------------------------------------------------------
//биты регистра статуса/управления
//------------------------------------------------------------------------------

#define TIMER_NIT_FL   						0									//флаг прерывания от переполнения таймера, устанавливаеться если будильник разрешен
#define AL_INT_FL							1									//флаг прерывания от совпадения значений часов или регистра таймера в будильнике
#define AL_EBL_B							2									//бит разрешения будильника (0 запрещен и SRAM нач. с адр. 08)
#define MASK_FL								3									//бит маски считывания дня месяца без года
#define FUN_MODE_B0							4									//--| биты выбора режима часов, 00 - 32 кГц; 01 - 50 Гц; 10 - собитие-клок режим
#define FUN_MODE_B1							5									//--| 11 - тестовый режим
#define HOLD_LAST_COUNT_B					6									//бит удержания в буфере для считывания текущего значения регистров
#define STOP_COUNT_B						7									//бит остановки часов

//------------------------------------------------------------------------------
//биты регистра управления будильником
//------------------------------------------------------------------------------

#define TIMER_FUN_B0						0									//--| биты функцион. таймера 000 - нет таймера; 001 - сотые части секунд
#define TIMER_FUN_B1						1									//  | 010 - секунды; 011 - минуты; 100 - часы; 101 - дни; 110 - не используется; 111 - тестовый режим
#define TIMER_FUN_B2						2									//--| в режиме от внешнего события  000 - нету; 001 - 1; 010 - 100; 011 - 10000; 100 - 1000000; остальное не разрешено
#define TIMER_INT_EBL_B						3									//бит прерывания от переполнения таймера, 0 - прерыванияе запрещено
#define CLK_AL_FUN_B0						4									//--| биты функцион. часов, 00 - нет события.; 01 - ежедневный буд.
#define CLK_AL_FUN_B1						5									//--| 10 - по дням недели; 11 - по дате
#define TIMER_AL_EBL_B						6									//бит разрешения прерывания от совпадения регистра таймера в будильнике, 0 - нету
#define AL_INT_EBL_FL						7									//бит прерывания от совпадания часов в будильнике, 0 - нету прерывания

//------------------------------------------------------------------------------
//всякие константы
//------------------------------------------------------------------------------

#define RTC_ADR								0xA0

#define YEAR_MASK							0xC0								//маска битов года в регистре года/даты
#define DATE_MASK							0x3F								//маска битов числа в месяце в регистре года/даты
#define MONTH_MASK							0x1F								//маска битов месяца в регистре неделя/месяц

#define YEAR_CRT_RG2_OFFSET					0x64								//смещение второго контрольного регситра коррекции года в мапяти RAM часов

//------------------------------------------------------------------------------
//прототипы функций
//------------------------------------------------------------------------------

bool pcf8583_Wr_Data(uint8_t adr, uint8_t* data, ubase_t dszie);					//---записывает во внутренюю RAM часов с проверкой
bool pcf8583_Tx_Data(uint8_t adr, uint8_t* data, ubase_t dsize);					//---пишет данные в RAM часов
bool pcf8583_Rx_Data(uint8_t adr, uint8_t* data, ubase_t dsize);					//---читает данные с RAM часов
bool pcf8583_Correct_RamYear(void);												//---корректирует в RAM год
//bool pcf8583_Put_Year_to_Date(DATE_ *date);										//---конвертирует год в код года для часов (добавляет в регистр числа)
bool pcf8583_Wr_Param(uint8_t adr, uint8_t* data, ubase_t dsize);				//---записывает регистр коррекции года (год или номер года)
bool pcf8583_Rd_Param(uint8_t adr, uint8_t* data, ubase_t dsize);				//---читает регистр коррекции года (год или номер года)
//bool pcf8583_Set_Alarming(uint8_t *time_arr);									//---настраивает будильник

//bool pcf8583_Chk_Param(uint8_t adr, uint8_t* data, ubase_t dnum);

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

bool pcf8583_Ini(void)
//---инициализация
{
	iic_Ini();
	uint8_t ctrl_status_rg = 0;													//запустить часы
	if (pcf8583_Wr_Data(CTRL_STATUS_RG_ADR, &ctrl_status_rg, 1)) return pcf8583_LINK;
	return pcf8583_Correct_RamYear();											//произвести коррекцию года
}


bool pcf8583_Wr_Time(time_t* time)
//---устанавливает время
{
	uint8_t ctrl_status_rg = B(STOP_COUNT_B);									//остановить часы
	if (pcf8583_Wr_Data(CTRL_STATUS_RG_ADR, &ctrl_status_rg, 1)) return pcf8583_LINK;
	
	bcd_t arr[4];
	arr[0] = 0;																	//записать время
	arr[1] = time->sec;
	arr[2] = time->min;
	arr[3] = time->hour;
	if (pcf8583_Wr_Data(TIME_RG_ADR, arr, 4)) return pcf8583_LINK;	
	ctrl_status_rg = 0;															//запустить часы
	if (pcf8583_Wr_Data(CTRL_STATUS_RG_ADR, &ctrl_status_rg, 1)) return pcf8583_LINK;
	return pcf8583_TRUE;
}	


bool pcf8583_Rd_Time(time_t* time)
//---читает время
{
	uint8_t ctrl_status_rg;
	if (pcf8583_Rx_Data(CTRL_STATUS_RG_ADR, &ctrl_status_rg, 1))				//проверить или часы включены
		return pcf8583_LINK;
	if (CHKB(ctrl_status_rg, STOP_COUNT_B))
	{
		ctrl_status_rg = 0;
		if (pcf8583_Wr_Data(CTRL_STATUS_RG_ADR, &ctrl_status_rg, 1)) return pcf8583_LINK;
	}
	
	bcd_t arr[4];
	if (pcf8583_Rx_Data(TIME_RG_ADR, arr, 4)) return pcf8583_LINK;				//считать время
	time->sec = arr[1];
	time->min = arr[2];
	time->hour = arr[3];
	return pcf8583_TRUE;
}


bool pcf8583_Wr_Date(date_t* date)
//---устанавливает дату
{
	uint8_t ctrl_status_rg = B(STOP_COUNT_B);									//остановить часы
	if (pcf8583_Wr_Data(CTRL_STATUS_RG_ADR, &ctrl_status_rg, 1)) return pcf8583_LINK;	

	day_t day;
	day = tdf_Date_to_Day(date);
	day_t pcf8583_day = DAY_NO;
	for (ubase_t i=0; i<8; i++)
	{
		if (CHKB(day, 0))
		{
			pcf8583_day = i;
			break;
		}
		day >>= 1;
	}

	uint8_t arr[2];
	uint8_t num_year;
    bcd_Bcd_to_Num(date->year, &num_year);
    num_year = tdf_NumYear(num_year);
    
	arr[0] = (date->date | (num_year << 6));									//записать дату
	arr[1] = (date->month | (pcf8583_day << 5));
	
	if (pcf8583_Wr_Data(YE_DA_RG_ADR, arr, 2)) return pcf8583_LINK;
	
	ctrl_status_rg = 0;															//запустить часы
	if (pcf8583_Wr_Data(CTRL_STATUS_RG_ADR, &ctrl_status_rg, 1)) return pcf8583_LINK;
	if (pcf8583_Wr_Param(YEAR_NUM_CRT_RG_ADR, &num_year, 1)) return pcf8583_LINK;//обновить регистры коррекции года в RAM часов
		
	arr[0] = date->year;
	arr[1] = date->ctry;
	if (pcf8583_Wr_Param(YEAR_CRT_RG_ADR, arr, 2)) return pcf8583_LINK;
	return pcf8583_TRUE;
}


bool pcf8583_Rd_Date(date_t* date, day_t* day)
//---читает дату
{
	uint8_t arr[2];
	if (pcf8583_Rx_Data(YE_DA_RG_ADR, arr, 2)) return pcf8583_LINK;				//считать дату
	date->date = arr[0] & DATE_MASK;											//очистить год
	date->month = arr[1] & MONTH_MASK;											//очистить дни недели
	uint8_t d;
	d = arr[1] >> 5;
	*day = DAY_NO;
	SETB(*day, d);
	
	ubase_t err;
	if ((err = pcf8583_Correct_RamYear())) return err;							//сделать коррекцию года
	if ((err = pcf8583_Rd_Param(YEAR_CRT_RG_ADR, arr, 2))) return err;			//прочитать год
	date->year = arr[0];
	date->ctry = arr[1];
	return pcf8583_TRUE;
}


bool pcf8583_Wr_Alarm(time_t* time)
//---настраивает будильник, совпадания по времени каждый день
{
	uint8_t al_ctrl_rg = B(CLK_AL_FUN_B0);										//запретить генерацию прерывания
	if (pcf8583_Wr_Data(AL_CTRL_RG_ADR, &al_ctrl_rg, 1)) return pcf8583_LINK;
	
	bcd_t time_arr[4];
	time_arr[0] = 0;
	time_arr[1] = time->sec;
	time_arr[2] = time->min;
	time_arr[3] = time->hour;
	if (pcf8583_Wr_Data(AL_TIME_RG_ADR, time_arr, 4)) return pcf8583_LINK;
	
	al_ctrl_rg = B(AL_INT_EBL_FL)|B(CLK_AL_FUN_B0);								//разрешить генерацию прерывания
	if (pcf8583_Wr_Data(AL_CTRL_RG_ADR, &al_ctrl_rg, 1)) return pcf8583_LINK;

	uint8_t ctrl_status_rg = B(AL_EBL_B);										//обнулить флаги прерывания
	if (pcf8583_Wr_Data(CTRL_STATUS_RG_ADR, &ctrl_status_rg, 1)) return pcf8583_LINK;
	return pcf8583_TRUE;
}


bool pcf8583_Rd_Alarm(time_t *time)
//---считывает время будильника
{
	bcd_t arr[4];
	if (pcf8583_Rx_Data(AL_TIME_RG_ADR, arr, 4)) return pcf8583_LINK;
	time->sec = arr[0];
	time->min = arr[1];
	time->hour = arr[2];
	return pcf8583_TRUE;
}


bool pcf8583_Off_Alarm(void)
//---запрещает будильник
{
	uint8_t al_ctrl_rg = B(CLK_AL_FUN_B0);
	if (pcf8583_Wr_Data(AL_CTRL_RG_ADR, &al_ctrl_rg, 1)) return pcf8583_LINK;
	return pcf8583_TRUE;
}

//------------------------------------------------------------------------------
//внутренние функции
//------------------------------------------------------------------------------
//
//bool pcf8583_Wr_Param(uint8_t adr, uint8_t* data, ubase_t dnum)
////---пишет RAM с запасом
//{
//	uint8_t wr_arr[3];
//	for (ubase_t i=0; i<dnum; i++) wr_arr[i] = data[i];
//	wr_arr[dnum] = crc_Count_Crc8(data, dnum);
//	if (pcf8583_Wr_Data(adr, wr_arr, dnum+1)) return pcf8583_LINK;				//записать регистр и CRC регистра в RAM
//	if (pcf8583_Wr_Data(adr+YEAR_CRT_RG2_OFFSET, wr_arr, dnum+1)) return pcf8583_LINK;//записать контрольный регистр его CRC в RAM
//	return pcf8583_TRUE;
//}
//
//bool pcf8583_Rd_Param(uint8_t adr, uint8_t* data, ubase_t dnum)
////---читает регистр RAM с учетом запаса
//{
//	uint8_t arr[3];
//	if (pcf8583_Rx_Data(adr, arr, dnum+1)) return pcf8583_LINK;
//	if (arr[dnum] == crc_Count_Crc8(arr, dnum))
//	{
//		for (ubase_t i=0; i<dnum; i++) data[i] = arr[i];
//		return pcf8583_TRUE;
//	}
//		
//	if (pcf8583_Rx_Data((adr+YEAR_CRT_RG2_OFFSET), arr, dnum+1)) return pcf8583_LINK;
//	if (arr[dnum] != crc_Count_Crc8(arr, dnum)) return pcf8583_LOST;
//	
//	for (ubase_t i=0; i<dnum; i++) data[i] = arr[i];
//	if (pcf8583_Wr_Data(adr, arr, dnum+1)) return pcf8583_LINK;
//	return pcf8583_TRUE;
//}

bool pcf8583_Wr_1Param(uint8_t adr, uint8_t* data, ubase_t dnum)					//запись одного параметра
//---пишет RAM с запасом
{
	uint8_t wr_arr[3];
	for (ubase_t i=0; i<dnum; i++) wr_arr[i] = data[i];
	wr_arr[dnum] = crc_Count_Crc8(data, dnum);
	if (pcf8583_Wr_Data(adr, wr_arr, dnum+1)) return pcf8583_LINK;				//записать регистр и CRC регистра в RAM
	return pcf8583_TRUE;
}

bool pcf8583_Rd_1Param(uint8_t adr, uint8_t* data, ubase_t dnum)					//прочитать один параметр
//---читает регистр RAM с учетом запаса
{
	uint8_t arr[3];
	if (pcf8583_Rx_Data(adr, arr, dnum+1)) return pcf8583_LINK;
	if (arr[dnum] != crc_Count_Crc8(arr, dnum)) return pcf8583_LOST;
	for (ubase_t i=0; i<dnum; i++) data[i] = arr[i];
	return pcf8583_TRUE;
}

bool pcf8583_Rd_Param(uint8_t adr, uint8_t* data, ubase_t dnum)					//прочитать два параметра
{
	if (!pcf8583_Rd_1Param(adr, data, dnum))
	{
		if (!pcf8583_Rd_1Param((adr+YEAR_CRT_RG2_OFFSET), data, dnum)) return pcf8583_TRUE;
		if (pcf8583_Wr_1Param((adr+YEAR_CRT_RG2_OFFSET), data, dnum)) return pcf8583_LINK;
		return pcf8583_TRUE;
	}
	if (pcf8583_Rd_1Param((adr+YEAR_CRT_RG2_OFFSET), data, dnum)) return pcf8583_LOST;
	if (pcf8583_Wr_1Param(adr, data, dnum)) return pcf8583_LINK;
	return pcf8583_TRUE;
}

bool pcf8583_Wr_Param(uint8_t adr, uint8_t* data, ubase_t dnum)					//запись двух параметров
//---пишет RAM с запасом
{
	
	if (pcf8583_Wr_1Param(adr, data, dnum)) return pcf8583_LINK;				//записать регистр и CRC регистра в RAM
	if (pcf8583_Wr_Data(adr+YEAR_CRT_RG2_OFFSET, data, dnum)) return pcf8583_LINK;//записать контрольный регистр его CRC в RAM
	return pcf8583_TRUE;
}
///////////////////////////////////////////////////////////////////////////////
bool pcf8583_Correct_RamYear(void)
//---корректирует в RAM год
{
	ubase_t err;
	uint8_t year_date_rg;														//прочитать год
	if (pcf8583_Rx_Data(YE_DA_RG_ADR, &year_date_rg, 1)) return pcf8583_LINK;
	
	uint8_t year_ram_rg;														//прочитать регистр номера года с памяти
	if ((err = pcf8583_Rd_Param(YEAR_NUM_CRT_RG_ADR, &year_ram_rg, 1)))
		return err;
	
    year_date_rg &= YEAR_MASK;													//убрать с регистра года дату
	year_date_rg >>= 6;															//сдвинуть, чтобы сравнить и возможно потом сохранить
	
	if (year_date_rg == year_ram_rg) return pcf8583_TRUE;						//год не изменился
	
	uint8_t offset;																//вычислить смещение года
	if (year_date_rg > year_ram_rg) offset = year_date_rg - year_ram_rg;
	else offset = 4 - year_ram_rg + year_date_rg;

	bcd_t arr[2];																//прочитать бывший год с памяти
	if ((err = pcf8583_Rd_Param(YEAR_CRT_RG_ADR, arr, 2)))
		return err;
	uint8_t last_year;
	bcd_Bcd_to_Num(arr[0], &last_year);
		
	uint8_t sum = last_year + offset;											//добавить смещение
	if (sum > 99)
	{
		sum -= 100;
		bcd_Plus_Bcd(arr[1], 0x01, &arr[1]);
	}
	bcd_Num_to_Bcd(sum, &arr[0]);
	
	if (pcf8583_Wr_Param(YEAR_CRT_RG_ADR, arr, 2)) return pcf8583_LINK;			//обновить регистр года в памяти
	if (pcf8583_Wr_Param(YEAR_NUM_CRT_RG_ADR, &year_date_rg, 1)) return pcf8583_LINK;//обновить регистр номера года в памяти
	return pcf8583_TRUE;
}


bool pcf8583_Wr_Data(uint8_t adr, uint8_t* data, ubase_t dsize)
//---записывает во внутренюю RAM часов с проверкой
{
	for (ubase_t j=0; j<PTRY; j++)
	{
		if (!iic_Open(RTC_ADR, DIR_WR)) continue;								//записать
		if (!iic_Tx_Byte(adr)) continue;
		uint8_t* dp;
		dp = data;
		for (ubase_t i=0; i<dsize; i++)
		{
			if (!iic_Tx_Byte(*dp))
			{
				iic_Close();
				return pcf8583_LINK;
			}
			dp++;
		}
		iic_Close();
		//delay_ms(3);
		if (!iic_Open(RTC_ADR, DIR_WR)) break;									//проверить запись
		if (!iic_Tx_Byte(adr)) break;
		if (!iic_Open(RTC_ADR, DIR_RD)) break;
		dp = data;
		ubase_t i=dsize;
		for (; i;)
		{
			--i;
			uint8_t byte;
			iic_Rx_Byte(&byte, i);
			if (byte != *dp) break;
			dp++;
		}
	
		iic_Close();
		if (i) continue;
		return pcf8583_TRUE;
	}
	iic_Close();
	return pcf8583_LINK;
}



bool pcf8583_Tx_Data(uint8_t adr, uint8_t* data, ubase_t dsize)
//---записывает данные в RAM часов
{
	for (ubase_t j=0; j<PTRY; j++)
	{
		if (!iic_Open(RTC_ADR, DIR_WR)) continue;
		if (!iic_Tx_Byte(adr)) continue;
		for (ubase_t i=0; i<dsize; i++)
		{
			if (!iic_Tx_Byte(*data++))
			{
				iic_Close();
				return pcf8583_LINK;
			}
		}
		iic_Close();
		return pcf8583_TRUE;	
	}
	iic_Close();
	return pcf8583_LINK;
}


bool pcf8583_Rx_Data(uint8_t adr, uint8_t* data, ubase_t dsize)
//---считывает данные с RAM часов
{
	for (ubase_t j=0; j<PTRY; j++)
	{
		if (!iic_Open(RTC_ADR, DIR_WR)) continue;								//проверить запись
		if (!iic_Tx_Byte(adr)) continue;
		if (!iic_Open(RTC_ADR, DIR_RD)) continue;
		
		for (ubase_t i=dsize; i;)
		{
			iic_Rx_Byte(data++, --i);
		}
		iic_Close();
		return pcf8583_TRUE;
	}
	iic_Close();
	return pcf8583_LINK;
}




//bool pcf8583_Set_Alarming(uint8_t *time_arr)
////---настраивает будильник
//{
////	gI2c.adr_size = 1;
////	gI2c.slave = RTC_ADR;
//	uint8_t al_ctrl_rg = BIT(CLK_AL_FUN_BIT0);									//запретить генерацию прерывания
//	uint8_t adr = AL_CTRL_RG_ADR;
//	if(!i2c_Write_With_Check(&adr, &al_ctrl_rg, 1)) return false;
//
//	adr = AL_TIME_RG_ADR;
//	if(!i2c_Write_With_Check(&adr, time_arr, 4)) return false;
//	
//	al_ctrl_rg = BIT(AL_INT_EBL_FL)|BIT(CLK_AL_FUN_BIT0);						//разрешить генерацию прерывания
//	adr = AL_CTRL_RG_ADR;
//	if(!i2c_Write_With_Check(&adr, &al_ctrl_rg, 1)) return false;
//
//    CLRBIT(INT_DDR, INT_BIT);													//настроить порт генерации прерывания
//	CLRBIT(INT_PORT, INT_BIT);
//	
//	CLRBIT(INT_CTRL_RG, INT_CTRL_RG_INT4_0BIT);									//настроить прерывание по спаду
//	SETBIT(INT_CTRL_RG, INT_CTRL_RG_INT4_1BIT);
//	
//	SETBIT(INT_MASK_RG, INT_MASK_INT4_BIT);										//разрешить прерывания от слейва
//	SETBIT(INT_FL_RG, INT_FL_RG_INT4_FL);										//очистить флаг прерывания
//	
//	adr = CTRL_STATUS_RG_ADR;													//обнулить флаги прерывания
//	uint8_t ctrl_status_rg = BIT(AL_EBL_BIT);
//	if(!i2c_Write_With_Check(&adr, &ctrl_status_rg, 1)) return false;
//	return true;
//}
//
