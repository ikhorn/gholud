#include "tmp_logger.h"

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

	for (int i=0; i<TMPLOG_DAY_COUNT; i++) {
		logger->last_days[i].date = 0;
	}

	for (int i=0; i<TMPLOG_WEEK_COUNT; i++) {
		logger->last_weeks[i].max.date = 0;
	}

	for (int i=0; i<TMPLOG_MONTH_COUNT; i++) {
		logger->last_month[i].max.date = 0;
	}
}

/*
 * carry out the logging
 */
void tmplog_log(struct tmp_logger *logger)
{
	tmplog_trigger(logger);

	if (*logger->value > logger->last_days[0].max.value) {
		logger->last_days[0].max.hour = logger->time->hour;
		logger->last_days[0].max.min = logger->time->min;
		logger->last_days[0].max.value = *logger->value;

		if (*logger->value > logger->last_weeks[0].max.value) {
			logger->last_weeks[0].max.date = logger->date->date;
			logger->last_weeks[0].max.month = logger->date->month;
			logger->last_weeks[0].max.hour = logger->time->hour;
			logger->last_weeks[0].max.min = logger->time->min;
			logger->last_weeks[0].max.value = *logger->value;

			if (*logger->value > logger->last_month[0].max.value) {
				logger->last_month[0].max.date = logger->date->date;
				logger->last_month[0].max.month = logger->date->month;
				logger->last_month[0].max.hour = logger->time->hour;
				logger->last_month[0].max.min = logger->time->min;
				logger->last_month[0].max.value = *logger->value;
			}
		}
	}

	if (*logger->value < logger->last_days[0].min.value) {
		logger->last_days[0].min.hour = logger->time->hour;
		logger->last_days[0].min.min = logger->time->min;
		logger->last_days[0].min.value = *logger->value;

		if (*logger->value > logger->last_weeks[0].min.value) {
			logger->last_weeks[0].min.date = logger->date->date;
			logger->last_weeks[0].min.month = logger->date->month;
			logger->last_weeks[0].min.hour = logger->time->hour;
			logger->last_weeks[0].min.min = logger->time->min;
			logger->last_weeks[0].min.value = *logger->value;

			if (*logger->value > logger->last_month[0].min.value) {
				logger->last_month[0].min.date = logger->date->date;
				logger->last_month[0].min.month = logger->date->month;
				logger->last_month[0].min.hour = logger->time->hour;
				logger->last_month[0].min.min = logger->time->min;
				logger->last_month[0].max.value = *logger->value;
			}
		}
	}
}

/*
 * tmplog_init_day_info - init new day information by current value
 */
static void tmplog_init_day_info(struct tmp_logger *logger)
{
	logger->last_days[0].date = logger->date->date;
	logger->last_days[0].month = logger->date->month;
	logger->last_days[0].max.hour = logger->time->hour;
	logger->last_days[0].max.min = logger->time->min;
	logger->last_days[0].max.value = *logger->value;
	logger->last_days[0].min.hour = logger->time->hour;
	logger->last_days[0].min.hour = logger->time->min;
	logger->last_days[0].min.value = *logger->value;
}

/*
 * tmplog_trigger - trigger logger history. If day is changed then save it in
 * last_days stack, for weeks and month does the same.
 */
static void tmplog_trigger(struct tmp_logger *logger)
{
	if (logger->date->date != logger->last_days[0].date) {
		/* resort day stack */
		for (int i=TMPLOG_DAY_COUNT-1; i; i--) {
			logger->last_days[i] = logger->last_days[i-1];
		}

		tmplog_init_day_info(logger);

		/* resort week stack */
	}
}
