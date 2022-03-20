/*
 * CommandFunctions.h
 *
 * Created: 26.04.2017 14:55:18
 *  Author: a16007
 */


#ifndef COMMANDFUNCTIONS_H_
#define COMMANDFUNCTIONS_H_

#include "LuefterKlappe.h"
extern COMMAND cnetCommands[];
extern INFORMATION information[];

#define NUM_COMMANDS 12+CMULTI_STANDARD_NUM
#define NUM_INFORMATION 1


void gotStatusNachtabsenkung();

void jobSetLuefter1OnValue(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetLuefter2OnValue(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetLuefter1HystValue(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetLuefter2HystValue(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetLuefterSetStatus(ComReceiver *comRec, char function,char address,char job, void * pMem);

void jobGetLuefter1OnValue(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetLuefter2OnValue(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetLuefter1HystValue(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetLuefter2HystValue(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetLuefterSetStatus(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetLuefterActualStatus(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetLuefterSetStatus(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetLuefterActualStatus(ComReceiver *comRec, char function,char address,char job, void * pMem);

void jobSetTimeBetweenBlocks(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetTimeBetweenSensors(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobWaitAfterLastSensor(ComReceiver *comRec, char function,char address,char job, void * pMem);

void reportFlapSetStatus(Communication *com);
void reportFlapActualStatus(Communication *com);
#endif /* COMMANDFUNCTIONS_H_ */
