/*
* LuefterKlappe.cpp
*
* Created: 16.03.2017 13:03:01
* Author : a16007
*/

#include "LuefterKlappe.h"
#include "CommandFunctions.h"

void setup()
{
  init_clock(SYSCLK,PLL,true,CLOCK_CALIBRATION);
  PORTA_DIRCLR = 0xff;
  PORTA_DIRSET = 0b00100001;
  PORTA_OUTSET = 0b11011110;

  PORTB_DIRSET = 0xff;  // nicht benutzt

  PORTC_DIRSET = HBRIDGE_MODE1_PIN | HBRIDGE_MODE2_PIN | HBRIDGE_PHASE_PIN | HBRIDGE_NSLEEP_PIN | HBRIDGE_ENABLE_PIN;
  PORTC_DIRCLR = PIN0_bm | PIN1_bm | HBRIDGE_NFAULT_PIN;
  PORTC_OUTCLR = HBRIDGE_MODE1_PIN | HBRIDGE_MODE2_PIN | HBRIDGE_PHASE_PIN | HBRIDGE_NSLEEP_PIN | HBRIDGE_ENABLE_PIN;

  PORTD_DIRSET = LEDROT_PIN | LEDGRUEN_PIN | LEDBLAU_PIN | LUEFTER_ON_PIN | LUEFTER_PWM_PIN;
  PORTD_DIRCLR = LUEFTER_TASTE_PIN | KLAPPE_TASTE_PIN;
  PORTD_OUTSET = LEDROT_PIN;
  PORTD_OUTCLR = LEDGRUEN_PIN | LEDBLAU_PIN | LUEFTER_ON_PIN | LUEFTER_PWM_PIN;

  PORTE_DIRSET = PIN0_bm | PIN1_bm | PIN3_bm;
  PORTE_DIRCLR = PIN2_bm;

  LEDGRUEN_OFF;
  LEDROT_OFF;
  LEDBLAU_OFF;

	for(uint8_t i=0;i<20;i++)
	{
		LEDGRUEN_TOGGLE;
		_delay_ms(50);
	}

  LEDGRUEN_OFF;
  LEDROT_OFF;
  LEDBLAU_OFF;

  PORTD_INTCTRL = PORT_INT0LVL0_bm | PORT_INT1LVL0_bm;
	PORTD_INT0MASK = KLAPPE_TASTE_PIN;
	PORTD_INT1MASK = LUEFTER_TASTE_PIN;
	PORTD_PIN6CTRL = PORT_ISC_FALLING_gc | PORT_OPC_PULLUP_gc | PORT_SRLEN_bm;
	PORTD_PIN7CTRL = PORT_ISC_FALLING_gc | PORT_OPC_PULLUP_gc | PORT_SRLEN_bm;

	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();

	cnet.open(Serial::BAUD_57600,F_CPU);
}

int main(void)
{
  WDT_Disable();
  setup();
	cnet.broadcastUInt8((uint8_t) RST.STATUS,'S','0','R');
  readEEData();
	init_mytimer();
	WDT_EnableAndSetTimeout(WDT_PER_8KCLK_gc);
	WDT_Reset();
  initKlappe();
	WDT_Reset();
	WDT_EnableAndSetTimeout(WDT_PER_1KCLK_gc);
	while (1)
	{
    WDT_Reset();
		cnetRec.comStateMachine();
		cnetRec.doJob();
    // Falls sich der Klappenstatus geändert, wird dieser gesendet
    if( u8FlapSetStatusOld != u8FlapSetStatus)
    {
      reportFlapSetStatus(&cnet);
      u8FlapSetStatusOld=u8FlapSetStatus;
      switch(u8FlapSetStatus)
      {
        case FLAP_STATUS_OPENED:
          openKlappe();
          LUEFTER_OFF;
        break;
        case FLAP_STATUS_CLOSED:
          LUEFTER_OFF;
          closeKlappe();
        break;
        case FLAP_STATUS_OPENL1:
          LUEFTER_ON;
          openKlappe();
        break;
        case FLAP_STATUS_OPENL2:
          openKlappe();
          LUEFTER_ON;
        break;
        default:
          ;
      }
    }
    if( u8FlapActualStatusOld != u8FlapActualStatus)
    {
      reportFlapActualStatus(&cnet);
      u8FlapActualStatusOld = u8FlapActualStatus;
    }
    if( u8FlapMomentaryOld != u8FlapMomentaryStatus)
    {
      reportFlapMomentaryStatus(&cnet);
      u8FlapMomentaryOld = u8FlapMomentaryStatus;
    }
		// Ermittlung des neuen Lüfterstatus
		if(u8FlapSetStatus!=FLAP_STATUS_AUTO)
      u8FlapActualStatus = u8FlapSetStatus;
    else // FAN = Auto
    {
      /*
      double fHumi = fHumidity;
      switch(u8FanActualStatus)
      {
        case FAN_STATUS_OFF:
          if( fHumi > fF1Swell)
            u8FanActualStatus = FAN_STATUS_1;
        break;
        case FAN_STATUS_1:
          if( fHumi < fF1Swell-fF1Hysterese)
            u8FanActualStatus = FAN_STATUS_OFF;
          else if( fHumi > fF2Swell )
            u8FanActualStatus = FAN_STATUS_2;
        break;
        case FAN_STATUS_2:
          if( fHumi < fF2Swell-fF2Hysterese)
            u8FanActualStatus = FAN_STATUS_1;
        break;
      }*/
    }
    // Falls sich der Lüfterstatus geändert, wird dieser gesendet
		if( sendStatusReport )
    {
        //char buffer[16];
        sendStatusReport = false;
        MyTimers[TIMER_REPORT].value = actReportBetweenSensors;
        MyTimers[TIMER_REPORT].state = TM_START;
        switch(statusReport)
        {
            /*
            case TEMPREPORT:
                LEDGRUEN_ON;
                sprintf(buffer,"%.1f",(double)fTemperatur);
                cnet.sendStandard(buffer,BROADCAST,'C','1','t','F');
            break;
            case HUMIREPORT:
                sprintf(buffer,"%.1f",(double)fHumidity);
                cnet.sendStandard(buffer,BROADCAST,'C','1','h','F');
            break;
            case ABSHUMIREPORT:
                sprintf(buffer,"%.1f",(double)fAbsHumitdity);
                cnet.sendStandard(buffer,BROADCAST,'C','1','a','F');
            break;
            case DEWPOINTREPORT:
                sprintf(buffer,"%.1f",(double)fDewPoint);
                cnet.sendStandard(buffer,BROADCAST,'C','1','d','F');
            break;
            case L1SWELLREPORT:
                cnet.broadcastDouble(fF1Swell,'L','1','L');
                //sprintf(buffer,"%u",(uint8_t)u8F1Swell);
                //cnet.sendStandard(buffer,BROADCAST,'L','1','L','F');
            break;
            case L2SWELLREPORT:
                cnet.broadcastDouble(fF2Swell,'L','1','G');
                //sprintf(buffer,"%u",(uint8_t)u8F2Swell);
                //cnet.sendStandard(buffer,BROADCAST,'L','1','G','F');
            break;
            case L1HYSTREPORT:
                cnet.broadcastDouble(fF1Hysterese,'L','1','H');
                //sprintf(buffer,"%u",(uint8_t)u8F1Hysterese);
                //cnet.sendStandard(buffer,BROADCAST,'L','1','H','F');
            break;
            case L2HYSTREPORT:
                cnet.broadcastDouble(fF2Hysterese,'L','1','I');
                //sprintf(buffer,"%u",(uint8_t)u8F2Hysterese);
                //cnet.sendStandard(buffer,BROADCAST,'L','1','I','F');
            break;
            */
            case FANACTUALSTATUSREPORT:
                reportFlapActualStatus(&cnet);
            break;
            case FANSETSTATUSREPORT:
                reportFlapSetStatus(&cnet);
            break;
            case FANMOMENTARYSTATUSREPORT:
                reportFlapMomentaryStatus(&cnet);
            break;
            case LASTREPORT:
                MyTimers[TIMER_REPORT].value = actReportBetweenBlocks;
                MyTimers[TIMER_REPORT].state = TM_START;
            break;
        }
    }

    if( (foldF1Swell      != fF1Swell     ) |
        (foldF1Hysterese  != fF1Hysterese ) |
        (foldF2Swell      != fF2Swell     ) |
        (foldF2Hysterese  != fF2Hysterese )
      )
    {
      writeEEData();
      foldF1Swell      = fF1Swell     ;
      foldF1Hysterese  = fF1Hysterese ;
      foldF2Swell      = fF2Swell     ;
      foldF2Hysterese  = fF2Hysterese ;
    }
	}
}

void readEEData()
{
  fF1Swell      = eeprom_read_float(&ee_fF1Swell);
  fF1Hysterese  = eeprom_read_float(&ee_fF1Hysterese);
  fF2Swell      = eeprom_read_float(&ee_fF2Swell);
  fF2Hysterese  = eeprom_read_float(&ee_fF2Hysterese);
  u8FlapSetStatus = eeprom_read_byte(&ee_u8FlapSetStatus);
  if (u8FlapSetStatus==FLAP_STATUS_UNVALID)
    u8FlapSetStatus = FLAP_STATUS_AUTO;
  foldF1Swell      = fF1Swell     ;
  foldF1Hysterese  = fF1Hysterese ;
  foldF2Swell      = fF2Swell     ;
  foldF2Hysterese  = fF2Hysterese ;
  u8FlapSetStatusOld = u8FlapSetStatus;
}

void writeEEData()
{
  //LEDGRUEN_ON;
	MyTimers[TIMER_SAVE_DELAY].state = TM_START; // Speicherverzögerung läuft los
}

SIGNAL(PORTD_INT0_vect)
{
  LEDROT_ON;
	MyTimers[TIMER_START_KLAPPETASTE].state = TM_START;
}

SIGNAL(PORTD_INT1_vect)
{
  LEDGRUEN_ON;
	MyTimers[TIMER_START_LUEFTERTASTE].state = TM_START;
}

