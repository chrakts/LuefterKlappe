/*
 * Globals.cpp
 *
 * Created: 19.03.2017 09:24:37
 *  Author: Christof
 */
#define EXTERNALS_H_

#include "LuefterKlappe.h"

#define KK "KK"

const char *Node = NODE_STRING;
char IDNumber[12] EEMEM = "1784324-01";
char SerialNumber[12] EEMEM = "1958632254";
char IndexNumber[2] EEMEM = "A";

const char *luefterStatusStrings[9]={"zu","offen-L0","offen-L1","offen-L2","Auto","geschlossen","geoeffnet","schliesst","oeffnet"};

uint16_t actReportBetweenBlocks  = REPORT_BETWEEN_BLOCKS;
uint16_t actReportBetweenSensors = REPORT_BETWEEN_SENSORS;
uint16_t actWaitAfterLastSensor  = WAIT_AFTER_LAST_SENSOR;

volatile char strStatusNachtabsenkung[5];
volatile bool statusNachtabsenkung=false;

volatile double  fTemperatur=-999,fHumidity=-999,fDewPoint=-999,fAbsHumitdity=-999;
volatile double  fF1Swell=25,fF1Hysterese=1,fF2Swell=35,fF2Hysterese=2;

volatile uint8_t u8FanSetStatus=FAN_STATUS_AUTO,u8FanActualStatus=FAN_STATUS_2,u8FanActualStatusOld=FAN_STATUS_UNVALID;

volatile uint8_t u8FlapSetStatus=FLAP_STATUS_CLOSED;
volatile uint8_t u8FlapSetStatusOld=FLAP_STATUS_UNVALID;
volatile uint8_t u8FlapActualStatus=FLAP_STATUS_CLOSED;
volatile uint8_t u8FlapActualStatusOld=FLAP_STATUS_UNVALID;
volatile uint8_t u8FlapMomentaryStatus=FLAP_STATUS_IS_CLOSED;
volatile uint8_t u8FlapMomentaryOld=FLAP_STATUS_UNVALID;

volatile double  foldF1Swell,foldF1Hysterese,foldF2Swell,foldF2Hysterese;


float  EEMEM ee_fF1Swell=75,ee_fF1Hysterese=5,ee_fF2Swell=85,ee_fF2Hysterese=5;
uint8_t EEMEM ee_u8FlapSetStatus=FLAP_STATUS_AUTO;


volatile uint8_t statusReport = TEMPREPORT;
volatile bool    sendStatusReport = false;

int errno;      // Globale Fehlerbehandlung

char SecurityLevel = 0;

uint8_t actNumberSensors = 0;

volatile bool nextSendReady=false;

Communication cnet(0,Node,5,true);
ComReceiver cnetRec(&cnet,Node,cnetCommands,NUM_COMMANDS,information,NUM_INFORMATION,NULL,NULL);


