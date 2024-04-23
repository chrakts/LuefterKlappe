#include "Klappe.h"

enum{FLAP_IN_BETWEEN, FLAP_OPENED, FLAP_CLOSED, FLAP_CLOSING, FLAP_OPENING, FLAP_STOPPED};

void initKlappe()
{
  HBRIDGE_MODE1_SET;
  HBRIDGE_MODE2_CLR;
  HBRIDGE_CLOCKWISE;
  HBRIDGE_WAKEUP;
  HBRIDGE_SLEEP;
  PORTA_INTCTRL = PORT_INT0LVL0_bm; // Low-Level interrupt 0 for PORTA
	PORTA_INT0MASK = KLAPPE_ES_OPEN_PIN | KLAPPE_ES_CLOSE_PIN;
	PORTA_PIN7CTRL = PORT_ISC_FALLING_gc | PORT_OPC_PULLUP_gc | PORT_SRLEN_bm;
	PORTA_PIN1CTRL = PORT_ISC_FALLING_gc | PORT_OPC_PULLUP_gc | PORT_SRLEN_bm;
}

void openKlappe()
{
  if(u8FlapMomentaryStatus != FLAP_STATUS_OPENED)
  {
    u8FlapMomentaryStatus = FLAP_STATUS_OPENING;
    HBRIDGE_CLOCKWISE;
    HBRIDGE_WAKEUP;
    HBRIDGE_START;
  }
}

void closeKlappe()
{
  if(u8FlapMomentaryStatus != FLAP_STATUS_CLOSED)
  {
    u8FlapMomentaryStatus = FLAP_STATUS_CLOSING;
    HBRIDGE_CCLOCKWISE;
    HBRIDGE_WAKEUP;
    HBRIDGE_START;
  }
}

void stopKlappe()
{
  HBRIDGE_SLEEP;
}

uint8_t getStatusKlappe()
{
  if(KLAPPE_ES_OPEN_IS_CLEAR)
  {

    LEDBLAU_OFF;
    return(FLAP_OPENED);
  }
  else
  {
    if(KLAPPE_ES_CLOSE_IS_CLEAR)
    {
      LEDBLAU_OFF;
      return(FLAP_CLOSED);
    }
    else
    {
      return(FLAP_IN_BETWEEN);
    }
  }
}

SIGNAL(PORTA_INT0_vect)
{
  LEDBLAU_ON;
  if( KLAPPE_ES_OPEN_IS_CLEAR ) //(u8FlapMomentaryStatus==FLAP_STATUS_OPENING) &&
  {
    u8FlapMomentaryStatus = FLAP_STATUS_IS_OPENED;
    HBRIDGE_STOP;
    HBRIDGE_SLEEP;
    LEDROT_ON;
  }
  if( KLAPPE_ES_CLOSE_IS_CLEAR ) //(u8FlapMomentaryStatus==FLAP_STATUS_CLOSING) &&
  {
    u8FlapMomentaryStatus = FLAP_STATUS_IS_CLOSED;
    HBRIDGE_STOP;
    HBRIDGE_SLEEP;
    LEDROT_OFF;
  }

}

