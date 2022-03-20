#include "Klappe.h"

void initKlappe()
{
  HBRIDGE_MODE1_SET;
  HBRIDGE_MODE2_CLR;
  HBRIDGE_CLOCKWISE;

  HBRIDGE_WAKEUP;

}

void openKlappe()
{

}

void closeKlappe()
{

}

