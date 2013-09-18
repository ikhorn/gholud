/*
 * temperature sensor module
*/

#include "tmp.h"
#include "trg.h"
#include "err.h"
#include "def.h"
#include "dmem.h"
#include "delay.h"

#define TMP_SENS_USAGE		0x01
#define TMP_SENS_STAT_USAGE	0x02


gtmp_t gTmp;

static void tmp_Rd_Ds1631_TmpCode(void);
static void tmp_Rd_Ds18B20_TmpCode(void);

void tmp_Ini(void)
{
	for (ubase_t i=0; i<TMP_NUM; i++) {
		uint8_t settings;
		if (!dmem_Rd_Tmp_Settings(i ,&settings))
			return;

		if (settings & TMP_SENS_USAGE)
			SETB(gTmp.state_vector, i);
		else
			CLRB(gTmp.state_vector, i);

		if (settings & TMP_SENS_STAT_USAGE)
			SETB(gTmp.state_stat_vector, i);
		else
			CLRB(gTmp.state_stat_vector, i);
	}

	if (CHKB(gTmp.state_vector, 0)) {
		STA_N(STA_TMP_1);
		if (!ds1631_Ini())
			err_Ds1631();
	}

	if (CHKB(gTmp.state_vector, 1)) {
		STA_N(STA_TMP_2);
		TMP_2_SET_TMR();
		if (!ds18b20_Ini())
			err_Ds18b20();
	}
}

void tmp(void)
{
	if (CHKB(gTmp.state_vector, 0))
		tmp_Rd_Ds1631_TmpCode();

	if (CHKB(gTmp.state_vector, 1))
		tmp_Rd_Ds18B20_TmpCode();
}

/*
 * tmp_Set_Usage - set temperature sensor usage
 */
bool tmp_Set_Usage(ubase_t sens, uint8_t use_state)
{
	if (use_state) {
		if (!CHKB(gTmp.state_vector, sens)) {
			if (sens) {
				STA_N(STA_TMP_1);
				TMP_2_SET_TMR();
				if (!ds18b20_Ini()) {
					err_Ds18b20();
					return 0;
				}
			} else {
				STA_N(STA_TMP_2);
				if (!ds1631_Ini()) {
					err_Ds1631();
					return 0;
				}
			}
		}
		SETB(gTmp.state_vector, sens);
	} else {
		trg_Off(sens);
		CLRB(gTmp.state_vector, sens);
	}

	if (!dmem_Update_Tmp_Settings(sens, TMP_SENS_USAGE,
				      use_state ? TMP_SENS_USAGE : 0))
		return 0;

	return 1;
}

/*
 * tmp_Set_Statistic_Usage - set temperature sensor statistic usage
 */
bool tmp_Set_Statistic_Usage(ubase_t sens, uint8_t use_state)
{
	if (use_state)
		SETB(gTmp.state_stat_vector, sens);
	else
		CLRB(gTmp.state_stat_vector, sens);

	if (!dmem_Update_Tmp_Settings(sens, TMP_SENS_STAT_USAGE,
				      use_state ? TMP_SENS_STAT_USAGE: 0))
		return 0;

	return 1;
}

/*
 * tmp_Default - set default settings
 */
void tmp_Default(ubase_t snum)
{
	dmem_Wr_Tmp_Settings(snum, DEF_TMP_SETTINGS);
}


/*
 * tmp_Rd_Ds1631_TmpCode - читает температуру с датчика DS1631
 */
static void tmp_Rd_Ds1631_TmpCode(void)
{
	for (ubase_t i=0; i<PTRY; i++) {
		if (!ds1631_Rx_TempCode(&gTmp.code[0])) {
			delay_ms(10);
			continue;
		}
		STA_N(STA_TMP_1);
		STA_N(STA_IIC);
		return;
	}
	err_Ds1631();
}

/*
 * tmp_Rd_Ds18B20_TmpCode - читает температуру с датчика DS18B20
 */
static void tmp_Rd_Ds18B20_TmpCode(void)
{
	if (!ds18b20_Convert_Is_Done())
		return;

	TMP_2_SET_TMR();
	for (ubase_t i=0; i<PTRY; i++) {
		if (!ds18b20_Rd_Temp(&gTmp.code[1])) {
			delay_ms(10);
			continue;
		}

		if (!ds18b20_Start_Convert())
			break;

		STA_N(STA_TMP_2);
		return;
	}

	err_Ds18b20();
}

