#include "myTimers.h"
#include "ledHardware.h"
#include "CommandFunctions.h"

// 1:  9.9  ms
// 2:  19.8 ms
// 5:  49.4 ms
//10:  99.0 ms
//101: 1000 ms

volatile TIMER MyTimers[MYTIMER_NUM]= { {TM_START,RESTART_YES,actReportBetweenSensors,0,nextReportStatus},
                                        {TM_STOP,RESTART_NO,5000,0,nowSaveEEProm},
                                        {TM_STOP,RESTART_NO,50,0,tasteKlappeGedrueckt},
                                        {TM_STOP,RESTART_NO,50,0,tasteLuefterGedrueckt}
};



void led1Blinken(uint8_t test)
{
	LEDROT_TOGGLE;
}

void led2Blinken(uint8_t test)
{
//	LED_KLINGEL_TOGGLE;
}

void tasteKlappeGedrueckt(uint8_t test)
{
    LEDROT_OFF;
    u8FlapSetStatus += 1;
    if(u8FlapSetStatus>FLAP_STATUS_AUTO)
      u8FlapSetStatus = FLAP_STATUS_CLOSED;
}

void tasteLuefterGedrueckt(uint8_t test)
{
    LEDGRUEN_OFF;
}

void nowSaveEEProm(uint8_t test)
{
  eeprom_update_float(&ee_fF1Swell,fF1Swell);
  eeprom_update_float(&ee_fF1Hysterese,fF1Hysterese);
  eeprom_update_float(&ee_fF2Swell,fF2Swell);
  eeprom_update_float(&ee_fF2Hysterese,fF2Hysterese);
  eeprom_update_byte(&ee_u8FlapSetStatus,u8FlapSetStatus);
  LEDGRUEN_OFF;
}


void nextReportStatus(uint8_t test)
{
	sendStatusReport = true;
	statusReport+=1;
	if( statusReport > LASTREPORT )
        statusReport = TEMPREPORT;
}



