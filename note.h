/* Модуль строк
 * проект.......................................................................ЖОЛУДЬ
 * обновлен.....................................................................06.01.2011 12:44:01
*/

#include "menucore/usr.h"
#include "pdef.h"

#ifndef NOTE_H_
#define NOTE_H_

//------------------------------------------------------------------------------
//строки
//------------------------------------------------------------------------------
																				//для usr
extern const USRC_STR_ note_wrong[];			//неправ.
extern const USRC_STR_ note_input[];	//ввод
extern const USRC_STR_ note_value[];	//значение
extern const USRC_STR_ note_less_eq[];	//<="
extern const USRC_STR_ note_more_eq[];	//>="

extern const USRC_STR_ note_aclocks[];	//будильники"
extern const USRC_STR_ note_you_exactly[];	//вы точно"
extern const USRC_STR_ note_in_own_mind[];	//в своем уме?
extern const USRC_STR_ note_ascinput[];	//введите"
extern const USRC_STR_ note_up_1_hour[];	//на час вперед"
extern const USRC_STR_ note_down_1_hour[];	//на час назад"
extern const USRC_STR_ note_aclock[];	//Б"
extern const USRC_STR_ note_Bd[];	//Бд"
extern const USRC_STR_ note_on[];	//вкл."
extern const USRC_STR_ note_will_on[];	//включите"
extern const USRC_STR_ note_ring[];	//звон:"
extern const USRC_STR_ note_timeon[];	//T.в"
extern const USRC_STR_ note_timeoff[];	//T.о"
extern const USRC_STR_ note_timedur[];	//T.р"
extern const USRC_STR_ note_timerep[];	//T.п"
extern const USRC_STR_ note_all[];	//все"
extern const USRC_STR_ note_monday[];	//пн"
extern const USRC_STR_ note_tuesday[];	//вт"
extern const USRC_STR_ note_wednesday[];	//ср"
extern const USRC_STR_ note_thursday[];	//чт"
extern const USRC_STR_ note_friday[];	//пт"
extern const USRC_STR_ note_saturday[];	//сб"
extern const USRC_STR_ note_sunday[];	//вс"
extern const USRC_STR_ note_use_password[];	//исп.пароль"
extern const USRC_STR_ note_weekdays[];	//дни недели"
extern const USRC_STR_ note_wdays[];	//дни нед."
extern const USRC_STR_ note_days[];	//дни:"
extern const USRC_STR_ note_dateon[];	//д.в"
extern const USRC_STR_ note_temp_sens[];	//дат.темп."
extern const USRC_STR_ note_inv[];	//инвер."
extern const USRC_STR_ note_use_sens[];	//исп.дат."
extern const USRC_STR_ note_use_statistic[];	//исп.статист."
extern const USRC_STR_ note_day_inf[];	//инф. о дне"
extern const USRC_STR_ note_calc[];	//кальк."
extern const USRC_STR_ note_conv_temp[];	//конв. темп."
extern const USRC_STR_ note_menu[];	//меню"
extern const USRC_STR_ note_melody[];	//мелодия"
extern const USRC_STR_ note_mel[];	//мел:"
extern const USRC_STR_ note_month[];	//месяц"
extern const USRC_STR_ note_reminders[];	//напоминалки"
extern const USRC_STR_ note_reminder[];	//напом."
extern const USRC_STR_ note_new_password[];	//нов.пар."
extern const USRC_STR_ note_not_can[];	//не можно"
extern const USRC_STR_ note_off[];	//отк"
extern const USRC_STR_ note_general[];	//общие"
extern const USRC_STR_ note_ask_exchange_at[];	//обмен на"
extern const USRC_STR_ note_password[];	//пароль"
extern const USRC_STR_ note_lightning[];	//подсветка"
extern const USRC_STR_ note_bydef[];	//поумолчанию"
extern const USRC_STR_ note_waite[];	//подождите..."
//extern const USRC_STR_ note_converted[];	//переведено"
extern const USRC_STR_ note_progs[];	//проги"
extern const USRC_STR_ note_shift_time[];	//автопер.вр."
extern const USRC_STR_ note_device_started[];	//прибор запущен"
extern const USRC_STR_ note_firstly[];	//впервые?"
extern const USRC_STR_ note_stopwatch[];	//секундомер"
extern const USRC_STR_ note_state[];	//статус"
extern const USRC_STR_ note_str[];	//строка"
extern const USRC_STR_ note_message[];	//сообщ."
extern const USRC_STR_ note_one_time[];	//один раз"
extern const USRC_STR_ note_timers[];	//таймера"
extern const USRC_STR_ note_timer[];	//T"
extern const USRC_STR_ note_trg[];	//термостат"
extern const USRC_STR_ note_outtype[];	//тип вых."
extern const USRC_STR_ note_lost[];	//утер."
extern const USRC_STR_ note_bla_bla[];	//---"
extern const USRC_STR_ note_timedate[];	//время/дата"
extern const USRC_STR_ note_filtr[];	//фильтр:"
extern const USRC_STR_ note_year[];	//год"
extern const USRC_STR_ note_hour_sgn[];	//час сигн."
extern const USRC_STR_ note_timeforward[];	//T.пр"
extern const USRC_STR_ note_timeback[];	//T.об"
extern const USRC_STR_ note__error[];	//#error"
extern const USRC_STR_ note_not_respond[];	//не отв."
extern const USRC_STR_ note_rtc[];	//RTC"
extern const USRC_STR_ note_mem[];	//MEM"
extern const USRC_STR_ note_iic[];	//IIC"
extern const USRC_STR_ note_tempsens1[];	//DS1631"
extern const USRC_STR_ note_tempsens2[];	//DS18B20"
extern const USRC_STR_ note_Tb_more_Tt[];	//tн < tв"
	
extern const USRC_STR_ note_blink_1[];	//blink 1"
extern const USRC_STR_ note_blink_2[];	//blink 2"
extern const USRC_STR_ note_bomb[];	//bomb"
extern const USRC_STR_ note_chaos[];	//chaos"
extern const USRC_STR_ note_march[];	//march"
extern const USRC_STR_ note_silence[];	//silence"
extern const USRC_STR_ note_emerge[];	//emerge"
extern const USRC_STR_ note_pum[];	//pum"
extern const USRC_STR_ note_pim[];	//pim"
extern const USRC_STR_ note_clava[];	//clava"
extern const USRC_STR_ note_conver[];	//conver"
	
extern const USRC_STR_ note_new_luna[];	//новая луна"
extern const USRC_STR_ note_waxing_crescent[];	//н. полумесяц"
extern const USRC_STR_ note_first_quarter[];	//пер. четверть"
extern const USRC_STR_ note_waxing_gibbous[];	//нар. полная"
extern const USRC_STR_ note_full_luna[];	//полная луна"
extern const USRC_STR_ note_waning_gibbous[];	//убыв. полная"
extern const USRC_STR_ note_last_quarter[];	//пос. четверть"
extern const USRC_STR_ note_waning_crescent[];	//серп луны"
extern const USRC_STR_ note_LD[];	//ЛД:"
extern const USRC_STR_ note_Ph[];	//Ф:"
extern const USRC_STR_ note_L[];	//L:"
	
extern const USRC_STR_ note_12345678[];	//12345678"
extern const USRC_STR_ note_0123456789[];	//0123456789"
extern const USRC_STR_ note_12[];	//12"
extern const USRC_STR_ note_Tm[];	//Тм"
extern const USRC_STR_ note_Np[];	//Нп:"
extern const USRC_STR_ note_Tr[];	//Тр"
extern const USRC_STR_ note_devname[];	//ПАТИМА"
extern const USRC_STR_ note_version[];	//v.0.2.9"

extern const USRC_STR_ note_repnum[];	//N пов"
extern const USRC_STR_ note_tbottom[];	//Tн"
extern const USRC_STR_ note_ttop[];	//Tв"
extern const USRC_STR_ note_gC[];	//C"
extern const USRC_STR_ note_defualt_reminder_string[];


extern const USRC_STR_ note_cmd_23[];	//%m %3.1f %m"
extern const USRC_STR_ note_cmd_24[];	//%c %m"
extern const USRC_STR_ note_cmd_27[];	//%m%u %T %c"
extern const USRC_STR_ note_cmd_28[];	//%D %m"
extern const USRC_STR_ note_cmd_29[];	//T%u %3.1f-%3.1f %c"
extern const USRC_STR_ note_cmd_30[];	//%m %s"
extern const USRC_STR_ note_cmd_31[];	//%#%T"
extern const USRC_STR_ note_cmd_32[];	//%#t1:%3.1f t2:%3.1f"
extern const USRC_STR_ note_cmd_33[];	//%c%2u%s"
extern const USRC_STR_ note_cmd_34[];	//%2u+ %s"
extern const USRC_STR_ note_cmd_35[];	//%#%D %m"
extern const USRC_STR_ note_cmd_36[];	//%c %m %u"
extern const USRC_STR_ note_cmd_37[];	//%m%3.2f %m%f"
extern const USRC_STR_ note_cmd_38[];	//%m%3.2f %m%u %m"
extern const USRC_STR_ note_cmd_39[];	//%u:%m"

#define NOTE_SYM_BELL							DSP_SPEC_STR + 4			//символ звоночка
#define NOTE_SYM_RADIOON						DSP_SPEC_STR + 5			//символ включенного переключателя
#define NOTE_SYM_RADIOOFF						DSP_SPEC_STR + 6			//символ выключенного переключателя

#endif

