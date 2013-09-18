#include "tmp_logger.h"
#include "wbuf.h"

static void tmplog_roll_day(struct tmp_logger *logger);
static void tmplog_roll_week(struct tmp_logger *logger);
static void tmplog_roll_month(struct tmp_logger *logger);
static void tmplog_trigger(struct tmp_logger *logger);

/*
 * tmplog_Init - initialize given logger. All data used by logger doesn't
 * protected by any lock. So it can be used only state machine OS. Be careful,
 * in case with multitasking it should be reworked.
 *
 * date - pointer to date struct, that is updated in some other place
 * time - pointer to time struct, that is updated in some other place
 * value - pointer to value to log, that is updated in some other place
 */
void tmplog_init(struct tmp_logger *logger, time_t *time, date_t *date, int16_t* value)
{
	logger->start_date = date->date;
	logger->start_month = date->month;
	logger->time = time;
	logger->date = date;
	logger->value = value;

	wbuf_init(logger->last_days);
	wbuf_init(logger->last_weeks);
	wbuf_init(logger->last_monthes);

	tmplog_roll_day(logger);
	tmplog_roll_week(logger);
	tmplog_roll_month(logger);

	for (int i=0; i<TMPLOG_DAY_COUNT; i++) {
		logger->last_days.array[i].date = 0;
	}

	for (int i=0; i<TMPLOG_WEEK_COUNT; i++) {
		logger->last_weeks.array[i].max.date = 0;
	}

	for (int i=0; i<TMPLOG_MONTH_COUNT; i++) {
		logger->last_monthes.array[i].max.date = 0;
	}
}

/*
 * carry out the logging
 */
void tmplog_log(struct tmp_logger *logger)
{
	struct tmp_day *day;
	struct tmp_week *week;
	struct tmp_month *month;
	wbuf_getlast(logger->last_days, day);
	wbuf_getlast(logger->last_weeks, week);
	wbuf_getlast(logger->last_monthes, month);

	tmplog_trigger(logger);

	if (*logger->value > day->max.value) {
		day->max.hour = logger->time->hour;
		day->max.min = logger->time->min;
		day->max.value = *logger->value;

		if (*logger->value > week->max.value) {
			week->max.date = logger->date->date;
			week->max.month = logger->date->month;
			week->max.hour = logger->time->hour;
			week->max.min = logger->time->min;
			week->max.value = *logger->value;

			if (*logger->value > month->max.value) {
				month->max.date = logger->date->date;
				month->max.hour = logger->time->hour;
				month->max.min = logger->time->min;
				month->max.value = *logger->value;
			}
		}
	}

	if (*logger->value > day->min.value) {
		day->min.hour = logger->time->hour;
		day->min.min = logger->time->min;
		day->min.value = *logger->value;

		if (*logger->value > week->min.value) {
			week->min.date = logger->date->date;
			week->min.month = logger->date->month;
			week->min.hour = logger->time->hour;
			week->min.min = logger->time->min;
			week->min.value = *logger->value;

			if (*logger->value > month->max.value) {
				month->min.date = logger->date->date;
				month->min.hour = logger->time->hour;
				month->min.min = logger->time->min;
				month->min.value = *logger->value;
			}
		}
	}
}

/*
 * tmplog_roll_day - add new day information by currnet
 */
static void tmplog_roll_day(struct tmp_logger *logger)
{
	struct tmp_day *new_day;
	wbuf_getnext(logger->last_days, new_day);

	new_day->date = logger->date->date;
	new_day->month = logger->date->month;

	new_day->max.hour = logger->time->hour;
	new_day->max.min = logger->time->min;
	new_day->max.value = *logger->value;

	new_day->min.hour = logger->time->hour;
	new_day->min.min = logger->time->min;
	new_day->min.value = *logger->value;
}

/*
 * tmplog_roll_week - add new week information by currnet
 */
static void tmplog_roll_week(struct tmp_logger *logger)
{
	struct tmp_week *new_week;
	wbuf_getnext(logger->last_weeks, new_week);

	new_week->max.date = logger->date->date;
	new_week->max.month = logger->date->month;
	new_week->max.hour = logger->time->hour;
	new_week->max.min = logger->time->min;
	new_week->max.value = *logger->value;

	new_week->min.date = logger->date->date;
	new_week->min.month = logger->date->month;
	new_week->min.hour = logger->time->hour;
	new_week->min.min = logger->time->min;
	new_week->min.value = *logger->value;
}

/*
 * tmplog_roll_month - add new month information by currnet
 */
static void tmplog_roll_month(struct tmp_logger *logger)
{
	struct tmp_month *new_month;
	wbuf_getnext(logger->last_monthes, new_month);

	new_month->month = logger->date->month;

	new_month->max.date = logger->date->date;
	new_month->max.hour = logger->time->hour;
	new_month->max.min = logger->time->min;
	new_month->max.value = *logger->value;

	new_month->min.date = logger->date->date;
	new_month->min.hour = logger->time->hour;
	new_month->min.min = logger->time->min;
	new_month->min.value = *logger->value;
}
/*
 * tmplog_trigger - trigger logger history. If day is changed then save it in
 * last_days stack, for weeks and month does the same.
 */
static void tmplog_trigger(struct tmp_logger *logger)
{
	struct tmp_day *day;
	struct tmp_month *month;
	wbuf_getlast(logger->last_days, day);

	/* day has been triggered */
	if (logger->date->date != day->date) {
		tmplog_roll_day(logger);

		/* week has been triggered */
		if (tdf_Date_to_Day(logger->date) == DAY_MON)
			tmplog_roll_week(logger);

		/* month has been triggered */
		wbuf_getlast(logger->last_monthes, month);
		if (logger->date->month != month->month)
			tmplog_roll_month(logger);
	}
}
