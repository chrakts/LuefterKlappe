#ifndef KLAPPE_H_INCLUDED
#define KLAPPE_H_INCLUDED

#include "LuefterKlappe.h"

void initKlappe();
void openKlappe();
void closeKlappe();
void stopKlappe();
bool isKlappeOpened();
bool isKlappeClosed();
uint8_t getStatusKlappe();

#endif // KLAPPE_H_INCLUDED
