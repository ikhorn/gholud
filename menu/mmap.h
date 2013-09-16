/* Карта меню прибора Жолудь
 * обновлен.....................................................................06.01.2011 12:44:16
*/

//------------------------------------------------------------------------------
//							ВХОД В МЕНЮ
//------------------------------------------------------------------------------
void menu_I_Enter_Title(void);
void menu_I_Enter_Enter(void);
//массив обработчиков пунктов
funcp_pr_t listEnter[] =
{
	menu_I_Enter_Title,
	menu_I_Enter_Enter,
};
usr_wnd_t menW_Enter = {listEnter, list_size(listEnter)};
//номера пунктов
#define ITEM_Enter_Enter					1
//входные меню пунктов
#define ENTERMENU_Enter_Enter				menW_Menu
#define ENTERITEM_Enter_Enter				ITEM_Menu_Alms
//------------------------------------------------------------------------------
//							ПРОВЕРКА ПАРОЛЯ
//------------------------------------------------------------------------------
void menu_I_ChkPsw_Title(void);
void menu_I_ChkPsw_Enter(void);
//массив обработчиков пунктов
funcp_pr_t listChkPsw[] =
{
	menu_I_ChkPsw_Title,
	menu_I_ChkPsw_Enter,
};
usr_wnd_t menW_ChkPsw = {listChkPsw, list_size(listChkPsw)};
//номера пунктов
#define ITEM_ChkPsw_Enter					1
//входные меню пунктов
#define ENTERMENU_ChkPsw_Enter				menW_Menu
#define ENTERITEM_ChkPsw_Enter				ITEM_Menu_Alms
//------------------------------------------------------------------------------
//						ВВОД НОМЕРА НАПОМИНАЛКИ
//------------------------------------------------------------------------------
void menu_I_AskNum_Title(void);
void menu_I_AskNum_Num(void);
//массив обработчиков пунктов
funcp_pr_t listAskNum[] =
{
	menu_I_AskNum_Title,
	menu_I_AskNum_Num,
};
usr_wnd_t menW_AskNum = {listAskNum, list_size(listAskNum)};
//номера пунктов
#define ITEM_AskNum_Num							1
//входные меню пунктов
#define ENTERMENU_AskNum_Num					menW_Rmds
#define ENTERITEM_AskNum_Num					ITEM_Rmds_Rmd
//------------------------------------------------------------------------------
//				Menu
//------------------------------------------------------------------------------

void menu_I_Menu_Title(void);
void menu_I_Menu_Alms(void);
void menu_I_Menu_Rmds(void);
void menu_I_Menu_Tmrs(void);
void menu_I_Menu_TempSens(void);
void menu_I_Menu_Trgs(void);
void menu_I_Menu_Progs(void);
void menu_I_Menu_General(void);

//array of item's handlers
funcp_pr_t listMenu[] =
{
	menu_I_Menu_Title,
	menu_I_Menu_Alms,
	menu_I_Menu_Rmds,
	menu_I_Menu_Tmrs,
	menu_I_Menu_TempSens,
	menu_I_Menu_Trgs,
	menu_I_Menu_Progs,
	menu_I_Menu_General
};
usr_wnd_t menW_Menu = {listMenu, list_size(listMenu)};

//item's numbers
#define ITEM_Menu_Alms					1
#define ITEM_Menu_Rmds					2
#define ITEM_Menu_Tmrs					3
#define ITEM_Menu_TempSens				4
#define ITEM_Menu_Trgs					5
#define ITEM_Menu_Progs					6
#define ITEM_Menu_General				7

//input menus of items
#define ENTERMENU_Menu_TimeDate				menW_TimeDate
#define ENTERITEM_Menu_TimeDate				ITEM_TimeDate_Time

#define ENTERMENU_Menu_Alms				menW_Alms
#define ENTERITEM_Menu_Alms				ITEM_Alms_Alm

#define ENTERMENU_Menu_Rmds				menW_Rmds
#define ENTERITEM_Menu_Rmds				ITEM_Rmds_Rmd

#define ENTERMENU_Menu_Tmrs				menW_Tmrs
#define ENTERITEM_Menu_Tmrs				ITEM_Tmrs_Tmr

#define ENTERMENU_Menu_TempSens				menW_TempSens
#define ENTERITEM_Menu_TempSens				ITEM_TempSens_Select

#define ENTERMENU_Menu_Trgs				menW_Trgs
#define ENTERITEM_Menu_Trgs				ITEM_Trgs_Trg

#define ENTERMENU_Menu_Progs				menW_Progs
#define ENTERITEM_Menu_Progs				ITEM_Progs_Clc

#define ENTERMENU_Menu_General				menW_General
#define ENTERITEM_Menu_General				ITEM_General_TimeDate

//------------------------------------------------------------------------------
//						БУДИЛЬНИКИ
//------------------------------------------------------------------------------
void menu_I_Alms_Title(void);
void menu_I_Alms_Alm(void);
//массив обработчиков пунктов
funcp_pr_t listAlms[] =
{
	menu_I_Alms_Title,
	menu_I_Alms_Alm
};
usr_wnd_t menW_Alms = {listAlms, list_size(listAlms)};
//номера пунктов
#define ITEM_Alms_Alm							1
//входные меню пунктов
#define ENTERMENU_Alms_Alm						menW_Alm
#define ENTERITEM_Alms_Alm						ITEM_Alm_OnOff

//------------------------------------------------------------------------------
//						БУДИЛЬНИК
//------------------------------------------------------------------------------
void menu_I_Alm_Title(void);
void menu_I_Alm_OnOff(void);
void menu_I_Alm_TimeOn(void);
void menu_I_Alm_Melody(void);
void menu_I_Alm_TimeDur(void);
void menu_I_Alm_TimeRep(void);
void menu_I_Alm_Repnum(void);
void menu_I_Alm_Days(void);
void menu_I_Alm_Message(void);
void menu_I_Alm_OneTime(void);
//массив обработчиков пунктов
funcp_pr_t listAlm[] =
{
	menu_I_Alm_Title,
	menu_I_Alm_OnOff,
	menu_I_Alm_TimeOn,
	menu_I_Alm_Melody,
	menu_I_Alm_TimeDur,
	menu_I_Alm_TimeRep,
	menu_I_Alm_Repnum,
	menu_I_Alm_Days,
	menu_I_Alm_Message,
	menu_I_Alm_OneTime
};
usr_wnd_t menW_Alm = {listAlm, list_size(listAlm)};
//номера пунктов
#define ITEM_Alm_OnOff							1
#define ITEM_Alm_TimeOn							2
#define ITEM_Alm_Melody							3
#define ITEM_Alm_TimeDur						4
#define ITEM_Alm_TimeRep						5
#define ITEM_Alm_Repnum							6
#define ITEM_Alm_Days							7
#define ITEM_Alm_Message						8
#define ITEM_Alm_OneTime						9

//входные меню пунктов
#define ENTERMENU_Alm_OnOff						menW_----
#define ENTERITEM_Alm_OnOff						ITEM_----_---

#define ENTERMENU_Alm_TimeOn					menW_----
#define ENTERITEM_Alm_TimeOn					ITEM_----_---

#define ENTERMENU_Alm_Melody					menW_Melody
#define ENTERITEM_Alm_Melody					ITEM_Melody_Set

#define ENTERMENU_Alm_TimeDur					menW_----
#define ENTERITEM_Alm_TimeDur					ITEM_----_---

#define ENTERMENU_Alm_TimeRep					menW_----
#define ENTERITEM_Alm_TimeRep					ITEM_----_---

#define ENTERMENU_Alm_Repnum					menW_----
#define ENTERITEM_Alm_Repnum					ITEM_----_---

#define ENTERMENU_Alm_Days						menW_Days
#define ENTERITEM_Alm_Days						ITEM_Days_AllDays

#define ENTERMENU_Alm_Message					menW_---
#define ENTERITEM_Alm_Message					ITEM_---_---

#define ENTERMENU_Alm_OneTime					menW_---
#define ENTERITEM_Alm_OneTime					ITEM_---_---
//------------------------------------------------------------------------------
//						ДНИ НЕДЕЛИ
//------------------------------------------------------------------------------
void menu_I_Days_Title(void);
void menu_I_Days_AllDays(void);
void menu_I_Days_Monday(void);
void menu_I_Days_Tuesday(void);
void menu_I_Days_Wednesday(void);
void menu_I_Days_Thursday(void);
void menu_I_Days_Friday(void);
void menu_I_Days_Saturday(void);
void menu_I_Days_Sunday(void);

//массив обработчиков пунктов
funcp_pr_t listDays[] =
{
	menu_I_Days_Title,
	menu_I_Days_AllDays,
	menu_I_Days_Monday,
	menu_I_Days_Tuesday,
	menu_I_Days_Wednesday,
	menu_I_Days_Thursday,
	menu_I_Days_Friday,
	menu_I_Days_Saturday,
	menu_I_Days_Sunday
};
usr_wnd_t menW_Days = {listDays, list_size(listDays)};
//номера пунктов
#define ITEM_Days_AllDays					1
#define ITEM_Days_Monday					2
#define ITEM_Days_Tuesday					3
#define ITEM_Days_Wednesday					4
#define ITEM_Days_Thursday					5
#define ITEM_Days_Friday					6
#define ITEM_Days_Saturday					7
#define ITEM_Days_Sunday					8
//входные меню пунктов
#define ENTERMENU_Days_AllDays				menW_----
#define ENTERITEM_Days_AllDays				ITEM_----_---

#define ENTERMENU_Days_Monday				menW_----
#define ENTERITEM_Days_Monday				ITEM_----_---

#define ENTERMENU_Days_Tuesday				menW_----
#define ENTERITEM_Days_Tuesday				ITEM_----_---

#define ENTERMENU_Days_Wednesday			menW_----
#define ENTERITEM_Days_Wednesday			ITEM_----_---

#define ENTERMENU_Days_Thursday				menW_----
#define ENTERITEM_Days_Thursday				ITEM_----_---

#define ENTERMENU_Days_Friday				menW_----
#define ENTERITEM_Days_Friday				ITEM_----_---

#define ENTERMENU_Days_Saturday				menW_----
#define ENTERITEM_Days_Saturday				ITEM_----_---

#define ENTERMENU_Days_Sunday				menW_----
#define ENTERITEM_Days_Sunday				ITEM_----_---

//------------------------------------------------------------------------------
//							МЕЛОДИЯ
//------------------------------------------------------------------------------

void menu_I_Melody_Title(void);
void menu_I_Melody_Set(void);
//массив обработчиков пунктов
funcp_pr_t listMelody[] =
{
	menu_I_Melody_Title,
	menu_I_Melody_Set,
};
usr_wnd_t menW_Melody = {listMelody, list_size(listMelody)};
//номера пунктов
#define ITEM_Melody_Set						1

//------------------------------------------------------------------------------
//							ТАЙМЕРА
//------------------------------------------------------------------------------

void menu_I_Tmrs_Title(void);
void menu_I_Tmrs_Tmr(void);
//массив обработчиков пунктов
funcp_pr_t listTmrs[] =
{
	menu_I_Tmrs_Title,
	menu_I_Tmrs_Tmr
};
usr_wnd_t menW_Tmrs = {listTmrs, list_size(listTmrs)};
//номера пунктов
#define ITEM_Tmrs_Tmr							1
//входные меню пунктов
#define ENTERMENU_Tmrs_Tmr						menW_Tmr
#define ENTERITEM_Tmrs_Tmr						ITEM_Tmr_OnOff

//------------------------------------------------------------------------------
//							ТАЙМЕР
//------------------------------------------------------------------------------

void menu_I_Tmr_Title(void);
void menu_I_Tmr_OnOff(void);
void menu_I_Tmr_TimeOn(void);
void menu_I_Tmr_TimeOff(void);
void menu_I_Tmr_Days(void);
void menu_I_Tmr_Type(void);
//массив обработчиков пунктов
funcp_pr_t listTmr[] =
{
	menu_I_Tmr_Title,
	menu_I_Tmr_OnOff,
	menu_I_Tmr_TimeOn,
	menu_I_Tmr_TimeOff,
	menu_I_Tmr_Days,
	menu_I_Tmr_Type
};
usr_wnd_t menW_Tmr = {listTmr, list_size(listTmr)};
//номера пунктов
#define ITEM_Tmr_OnOff							1
#define ITEM_Tmr_Time							2
#define ITEM_Tmr_Days							3
#define ITEM_Tmr_Type							4
//входные меню пунктов
#define ENTERMENU_Tmr_OnOff						menW_----
#define ENTERITEM_Tmr_OnOff						ITEM_----_---

#define ENTERMENU_Tmr_TimeOn					menW_----
#define ENTERITEM_Tmr_TimeOn					ITEM_----_---

#define ENTERMENU_Tmr_TimeOff					menW_----
#define ENTERITEM_Tmr_TimeOff					ITEM_----_---

#define ENTERMENU_Tmr_Days						menW_Days
#define ENTERITEM_Tmr_Days						ITEM_Days_AllDays

#define ENTERMENU_Tmr_Type						menW_----
#define ENTERITEM_Tmr_Type						ITEM_----_---

//------------------------------------------------------------------------------
//						ТЕРМОРЕГУЛЯТОРЫ
//------------------------------------------------------------------------------
void menu_I_Trgs_Title(void);
void menu_I_Trgs_Trg(void);
//массив обработчиков пунктов
funcp_pr_t listTrgs[] =
{
	menu_I_Trgs_Title,
	menu_I_Trgs_Trg
};
usr_wnd_t menW_Trgs = {listTrgs, list_size(listTrgs)};
//номера пунктов
#define ITEM_Trgs_Trg							1
//входные меню пунктов
#define ENTERMENU_Trgs_Trg						menW_Trg
#define ENTERITEM_Trgs_Trg						ITEM_Trg_OnOff

//------------------------------------------------------------------------------
//						ТЕРМОРЕГУЛЯТОР
//------------------------------------------------------------------------------
void menu_I_Trg_Title(void);
void menu_I_Trg_OnOff(void);
void menu_I_Trg_Tbottom(void);
void menu_I_Trg_Ttop(void);
void menu_I_Trg_Type(void);
//массив обработчиков пунктов
funcp_pr_t listTrg[] =
{
	menu_I_Trg_Title,
	menu_I_Trg_OnOff,
	menu_I_Trg_Tbottom,
	menu_I_Trg_Ttop,
	menu_I_Trg_Type
};
usr_wnd_t menW_Trg = {listTrg, list_size(listTrg)};
//номера пунктов
#define ITEM_Trg_OnOff						1
#define ITEM_Trg_Tbottom					2
#define ITEM_Trg_Ttop						3
#define ITEM_Trg_Type						4
//входные меню пунктов
#define ENTERMENU_Trg_OnOff					menW_----
#define ENTERITEM_Trg_OnOff					ITEM_----_---

#define ENTERMENU_Trg_Tbottom				menW_----
#define ENTERITEM_Trg_Tbottom				ITEM_----_---

#define ENTERMENU_Trg_Ttop					menW_----
#define ENTERITEM_Trg_Ttop					ITEM_----_---

#define ENTERMENU_Trg_Type					menW_----
#define ENTERITEM_Trg_Type					ITEM_----_---

//------------------------------------------------------------------------------
//						НАПОМИНАЛКИ
//------------------------------------------------------------------------------
void menu_I_Rmds_Title(void);
void menu_I_Rmds_Rmd(void);
//массив обработчиков пунктов
funcp_pr_t listRmds[] =
{
	menu_I_Rmds_Title,
	menu_I_Rmds_Rmd,
};
usr_wnd_t menW_Rmds = {listRmds, list_size(listRmds)};
//номера пунктов
#define ITEM_Rmds_Rmd							1
//входные меню пунктов
#define ENTERMENU_Rmds_Rmd						menW_Rmd
#define ENTERITEM_Rmds_Rmd						ITEM_Rmd_OnOff

//------------------------------------------------------------------------------
//						СМЕНА НОМЕРА НАПОМИНАЛКИ
//------------------------------------------------------------------------------
void menu_I_RmdExchange_Title(void);
void menu_I_RmdExchange_NewNumber(void);
//массив обработчиков пунктов
funcp_pr_t listRmdExchange[] =
{
	menu_I_RmdExchange_Title,
	menu_I_RmdExchange_NewNumber,
};
usr_wnd_t menW_RmdExchange = {listRmdExchange, list_size(listRmdExchange)};
//номера пунктов
#define ITEM_RmdExchange_NewNumber				1
//входные меню пунктов
#define ENTERMENU_RmdExchange_NewNumber		    menW_Rmds	
#define ENTERITEM_RmdExchange_NewNumber			ITEM_Rmds_Rmd

//------------------------------------------------------------------------------
//						НАПОМИНАЛКА
//------------------------------------------------------------------------------
void menu_I_Rmd_Title(void);
void menu_I_Rmd_OnOff(void);
void menu_I_Rmd_TimeOn(void);
void menu_I_Rmd_DateOn(void);
void menu_I_Rmd_Filtr(void);
void menu_I_Rmd_String(void);
void menu_I_Rmd_Melody(void);
void menu_I_Rmd_TimeDur(void);
void menu_I_Rmd_TimeRep(void);
void menu_I_Rmd_Repnum(void);

//массив обработчиков пунктов
funcp_pr_t listRmd[] =
{
	menu_I_Rmd_Title,
	menu_I_Rmd_OnOff,
	menu_I_Rmd_TimeOn,
	menu_I_Rmd_DateOn,
	menu_I_Rmd_Filtr,
	menu_I_Rmd_String,
	menu_I_Rmd_Melody,
	menu_I_Rmd_TimeDur,
	menu_I_Rmd_TimeRep,
	menu_I_Rmd_Repnum
};
usr_wnd_t menW_Rmd = {listRmd, list_size(listRmd)};
//номера пунктов
#define ITEM_Rmd_OnOff							1
#define ITEM_Rmd_TimeOn							2
#define ITEM_Rmd_DateOn							3
#define ITEM_Rmd_Filtr							4
#define ITEM_Rmd_String							5
#define ITEM_Rmd_Melody							6
#define ITEM_Rmd_TimeDur						7
#define ITEM_Rmd_TimeRep						8
#define ITEM_Rmd_Repnum							9
//входные меню пунктов
#define ENTERMENU_Rmd_OnOff						menW_----
#define ENTERITEM_Rmd_OnOff						ITEM_----_---

#define ENTERMENU_Rmd_TimeOn					menW_----
#define ENTERITEM_Rmd_TimeOn					ITEM_----_---

#define ENTERMENU_Rmd_DateOn					menW_----
#define ENTERITEM_Rmd_DateOn					ITEM_----_---

#define ENTERMENU_Rmd_Filtr						menW_Filtr
#define ENTERITEM_Rmd_Filtr						ITEM_Filtr_Month

#define ENTERMENU_Rmd_String					menW_----
#define ENTERITEM_Rmd_String					ITEM_----_---

#define ENTERMENU_Rmd_Melody					menW_Melody
#define ENTERITEM_Rmd_Melody					ITEM_Melody_Set

//------------------------------------------------------------------------------
//						ПОВТОР
//------------------------------------------------------------------------------
void menu_I_Filtr_Title(void);
void menu_I_Filtr_Month(void);
void menu_I_Filtr_Year(void);
void menu_I_Filtr_Off(void);

//массив обработчиков пунктов
funcp_pr_t listFiltr[] =
{
	menu_I_Filtr_Title,
	menu_I_Filtr_Month,
	menu_I_Filtr_Year,
	menu_I_Filtr_Off
};
usr_wnd_t menW_Filtr = {listFiltr, list_size(listFiltr)};
//номера пунктов
#define ITEM_Filtr_Title						1
#define ITEM_Filtr_Month						2
#define ITEM_Filtr_Year							3
#define ITEM_Filtr_Off							4
//входные меню пунктов

//------------------------------------------------------------------------------
//				General settings
//------------------------------------------------------------------------------

void menu_I_General_Title(void);
void menu_I_General_TimeDate(void);
void menu_I_General_Lightning(void);
void menu_I_General_HourSgn(void);
void menu_I_General_ByDefault(void);
void menu_I_General_AvtoTimeShift(void);
void menu_I_General_UsePassword(void);
void menu_I_General_NewPassword(void);

//array of item's handlers
funcp_pr_t listGeneral[] =
{
	menu_I_General_Title,
	menu_I_General_TimeDate,
	menu_I_General_Lightning,
	menu_I_General_HourSgn,
	menu_I_General_ByDefault,
	menu_I_General_AvtoTimeShift,
	menu_I_General_UsePassword,
	menu_I_General_NewPassword
};
usr_wnd_t menW_General = {listGeneral, list_size(listGeneral)};

//item's numbers
#define ITEM_General_TimeDate					1
#define ITEM_General_Lightning					2
#define ITEM_General_HourSgn					3
#define ITEM_General_ByDefault					4
#define ITEM_General_AvtoTimeShift				5
#define ITEM_General_UsePassword				6
#define ITEM_General_NewPassword				7

//input menus of items
#define ENTERMENU_General_TimeDate				menW_TimeDate
#define ENTERITEM_General_TimeDate				ITEM_TimeDate_Time

#define ENTERMENU_General_ByDefault				menW_Confidence
#define ENTERITEM_General_ByDefault				ITEM_Confidence_OK

#define ENTERMENU_General_NewPassword				menW_ChkPsw
#define ENTERITEM_General_NewPassword				ITEM_ChkPsw_Enter

//------------------------------------------------------------------------------
//						НАСТРОЙКА ВРЕМЕНИ/ДАТЫ
//------------------------------------------------------------------------------
void menu_I_TimeDate_Title(void);
void menu_I_TimeDate_Time(void);
void menu_I_TimeDate_Date(void);
//массив обработчиков пунктов
funcp_pr_t listTimeDate[] =
{
	menu_I_TimeDate_Title,
	menu_I_TimeDate_Time,
	menu_I_TimeDate_Date
};
usr_wnd_t menW_TimeDate = {listTimeDate, list_size(listTimeDate)};
//номера пунктов
#define ITEM_TimeDate_Time						1
#define ITEM_TimeDate_Date						2
//входные меню пунктов
#define ENTERMENU_TimeDate_Time					menW_----
#define ENTERITEM_TimeDate_Time					ITEM_----_---

#define ENTERMENU_TimeDate_Date					menW_----
#define ENTERITEM_TimeDate_Date					ITEM_----_----

//------------------------------------------------------------------------------
//						ПРОВЕРКА УВЕРЕННОСТИ
//------------------------------------------------------------------------------
void menu_I_Confidence_Title(void);
void menu_I_Confidence_OK(void);
//массив обработчиков пунктов
funcp_pr_t listConfidence[] =
{
	menu_I_Confidence_Title,
	menu_I_Confidence_OK
};
usr_wnd_t menW_Confidence = {listConfidence, list_size(listConfidence)};
//номера пунктов
#define ITEM_Confidence_OK						1
//входные меню пунктов
#define ENTERMENU_Confidence_OK					menW_----
#define ENTERITEM_Confidence_OK					ITEM_----_---

//------------------------------------------------------------------------------
//							НОВЫЙ ПАРОЛЬ
//------------------------------------------------------------------------------
void menu_I_Password_Title(void);
void menu_I_Password_InpPassword(void);
//массив обработчиков пунктов
funcp_pr_t listPassword[] =
{
	menu_I_Password_Title,
	menu_I_Password_InpPassword
};
usr_wnd_t menW_Password = {listPassword, list_size(listPassword)};
//номера пунктов
#define ITEM_Password_InpPassword				1
//входные меню пунктов
#define ENTERMENU_Password_InpPassword			menW_---
#define ENTERITEM_Password_InpPassword			ITEM_---_----

//------------------------------------------------------------------------------
//				Temperature sensor select
//------------------------------------------------------------------------------
void menu_I_TempSens_Title(void);
void menu_I_TempSens_Select(void);

//array of item's handlers
funcp_pr_t listTempSens[] =
{
	menu_I_TempSens_Title,
	menu_I_TempSens_Select,
};
usr_wnd_t menW_TempSens = {listTempSens, list_size(listTempSens)};

//item's numbers
#define ITEM_TempSens_Select				1

//input menus of items
#define ENTERMENU_TempSens_Select			menW_TempSensSettings
#define ENTERITEM_TempSens_Select			ITEM_TempSensSettings_On

//------------------------------------------------------------------------------
//				Temperature sensor settings
//------------------------------------------------------------------------------
void menu_I_TempSensSettings_Title(void);
void menu_I_TempSensSettings_On(void);
void menu_I_TempSensSettings_StatisticOn(void);
void menu_I_TempSensSettings_Statistic(void);

//array of item's handlers
funcp_pr_t listTempSensSettings[] =
{
	menu_I_TempSensSettings_Title,
	menu_I_TempSensSettings_On,
	menu_I_TempSensSettings_StatisticOn,
	menu_I_TempSensSettings_Statistic,
};
usr_wnd_t menW_TempSensSettings = {listTempSensSettings, list_size(listTempSensSettings)};

//item's numbers
#define ITEM_TempSensSettings_On			1
#define ITEM_TempSensSettings_StatisticOn		2
#define ITEM_TempSensSettings_Statistic			3

//input menus of items
#define ENTERMENU_TempSensSettings_Statistic		menW_TempSensStatistic
#define ENTERITEM_TempSensSettings_Statistic		ITEM_TempSensStatistic_DayFrom

//------------------------------------------------------------------------------
//				Temperature sensor statistic
//------------------------------------------------------------------------------
void menu_I_TempSensSettings_Title(void);
void menu_I_TempSensStatistic_DayFrom(void);
void menu_I_TempSensStatistic_Day(void);
void menu_I_TempSensStatistic_Week(void);
void menu_I_TempSensStatistic_Month(void);

//array of item's handlers
funcp_pr_t listTempSensStatistic[] =
{
	menu_I_TempSensSettings_Title,
	menu_I_TempSensStatistic_DayFrom,
	menu_I_TempSensStatistic_Day,
	menu_I_TempSensStatistic_Week,
	menu_I_TempSensStatistic_Month,
};
usr_wnd_t menW_TempSensStatistic = {listTempSensStatistic, list_size(listTempSensStatistic)};

//item's numbers
#define ITEM_TempSensStatistic_DayFrom			1

//input menus of items
#define ENTERMENU_TempSensStatistic_On			menW_---

//------------------------------------------------------------------------------
//						ПРОГРАММЫ
//------------------------------------------------------------------------------

void menu_I_Progs_Title(void);
void menu_I_Progs_Clc(void);
void menu_I_Progs_Ctmp(void);
void menu_I_Progs_DayInf(void);
//массив обработчиков пунктов
funcp_pr_t listProgs[] =
{
	menu_I_Progs_Title,
	menu_I_Progs_Clc,
	menu_I_Progs_Ctmp,
	menu_I_Progs_DayInf
};
usr_wnd_t menW_Progs = {listProgs, list_size(listProgs)};
//номера пунктов
#define ITEM_Progs_Clc							1
#define ITEM_Progs_Ctmp							2
#define ITEM_Progs_DayInf						3
//входные меню пунктов
#define ENTERMENU_Progs_Clc						clcW
#define ENTERITEM_Progs_Clc						CLC_ITEMNUM

#define ENTERMENU_Progs_Ctmp					ctmpW
#define ENTERITEM_Progs_Ctmp					CTMP_ITEMNUM

#define ENTERMENU_Progs_DayInf					menW_DayInf
#define ENTERITEM_Progs_DayInf					1

//------------------------------------------------------------------------------
//						ИНФОРМАЦИЯ О ДНЕ
//------------------------------------------------------------------------------

void menu_I_DayInf_Inf(void);
void menu_I_DayInf_Date(void);
//массив обработчиков пунктов
funcp_pr_t listDayInf[] =
{
	menu_I_DayInf_Inf,
	menu_I_DayInf_Date
};
usr_wnd_t menW_DayInf = {listDayInf, list_size(listDayInf)};
//номера пунктов
#define ITEM_DayInf_Inf							1
#define ITEM_DayInf_Date						2
//входные меню пунктов
#define ENTERMENU_DayInf_Inf					---
#define ENTERITEM_DayInf_Inf					---

#define ENTERMENU_DayInf_Date					---
#define ENTERITEM_DayInf_Date					---

//------------------------------------------------------------------------------
//						ВЫБОР БУДИЛЬНИКА ДЛЯ ОТКЛЮЧЕНИЯ
//------------------------------------------------------------------------------

void menu_I_ActRing_Title(void);
void menu_I_ActRing_Enter(void);
//массив обработчиков пунктов
funcp_pr_t listActRing[] =
{
	menu_I_ActRing_Title,
	menu_I_ActRing_Enter,
};
usr_wnd_t menW_ActRing = {listActRing, list_size(listActRing)};
//номера пунктов
#define ITEM_ActRing_Enter					1
//входные меню пунктов
#define ENTERMENU_ActRing_Enter				---
#define ENTERITEM_ActRing_Enter				---
