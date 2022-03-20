/*
 * MyTimer.h
 *
 * Created: 11.02.2016 20:20:03
 *  Author: Christof
 */


#ifndef MYTIMERS_H_
#define MYTIMERS_H_

#include "timer.h"
#include "External.h"
#include "myconstants.h"

#define MYTIMER_NUM	4


enum{TIMER_REPORT, TIMER_SAVE_DELAY, TIMER_START_KLAPPETASTE, TIMER_START_LUEFTERTASTE};


void nextTemperatureStatus(uint8_t test);
void nextReportStatus(uint8_t test);
void nowSaveEEProm(uint8_t test);
void tasteKlappeGedrueckt(uint8_t test);
void tasteLuefterGedrueckt(uint8_t test);


extern volatile TIMER MyTimers[MYTIMER_NUM];

#endif /* MYTIMERS_H_ */
