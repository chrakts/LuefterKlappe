/*
 * CommandFunctions.cpp
 *
 * Created: 26.04.2017 14:54:45
 *  Author: a16007
 */

#include "CommandFunctions.h"
#include "External.h"
#include "../Secrets/secrets.h"

INFORMATION information[NUM_INFORMATION]=
{
  {"DT",'t','1','N',STRING,3,(void*)strStatusNachtabsenkung,gotStatusNachtabsenkung},
};

COMMAND cnetCommands[NUM_COMMANDS] =
	{
    cmultiStandardCommands,
		{'L','L',CUSTOMER,FLOAT,1,jobSetLuefter1OnValue},
		{'L','G',CUSTOMER,FLOAT,1,jobSetLuefter2OnValue},
		{'L','H',CUSTOMER,FLOAT,1,jobSetLuefter1HystValue},
		{'L','I',CUSTOMER,FLOAT,1,jobSetLuefter2HystValue},
		{'L','S',CUSTOMER,STRING,8,jobSetLuefterSetStatus},
		{'L','l',CUSTOMER,NOPARAMETER,0,jobGetLuefter1OnValue},
		{'L','g',CUSTOMER,NOPARAMETER,0,jobGetLuefter2OnValue},
		{'L','h',CUSTOMER,NOPARAMETER,0,jobGetLuefter1HystValue},
		{'L','i',CUSTOMER,NOPARAMETER,0,jobGetLuefter2HystValue},
		{'T','B',CUSTOMER,UINT_16,1,jobSetTimeBetweenBlocks},
		{'T','S',CUSTOMER,UINT_16,1,jobSetTimeBetweenSensors},
		{'T','W',CUSTOMER,UINT_16,1,jobWaitAfterLastSensor}
	};

//		{'L','a',CUSTOMER,NOPARAMETER,0,jobGetLuefterActualStatus},
//		{'L','s',CUSTOMER,NOPARAMETER,0,jobGetLuefterSetStatus},


void gotStatusNachtabsenkung()
{
	if(strStatusNachtabsenkung[1]=='n')
	  statusNachtabsenkung = true;
  else
	  statusNachtabsenkung = false;
}


void jobGetLuefter1OnValue(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerDouble(function,address,job,fF1Swell,true);
}

void jobGetLuefter2OnValue(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerDouble(function,address,job,fF2Swell,true);
}

void jobGetLuefter1HystValue(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerDouble(function,address,job,fF1Hysterese,true);
}

void jobGetLuefter2HystValue(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerDouble(function,address,job,fF2Hysterese,true);
}

void jobGetLuefterSetStatus(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  cnet.sendStandard(luefterStatusStrings[u8FlapSetStatus],BROADCAST,'L','1','s',true);
}

void jobGetLuefterActualStatus(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  comRec->sendAnswer(luefterStatusStrings[u8FlapActualStatus],'L','1','a',true);
}

void jobSetLuefter1OnValue(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	fF1Swell = ( (double*) pMem )[0];
	MyTimers[TIMER_SAVE_DELAY].state = TM_START;
	comRec->Getoutput()->broadcastDouble(fF1Swell,function,address,job);
}

void jobSetLuefter2OnValue(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	fF2Swell = ( (double*) pMem )[0];
	MyTimers[TIMER_SAVE_DELAY].state = TM_START;
	comRec->Getoutput()->broadcastDouble(fF2Swell,function,address,job);
}

void jobSetLuefter1HystValue(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	fF1Hysterese = ( (double*) pMem )[0];
	MyTimers[TIMER_SAVE_DELAY].state = TM_START;
	comRec->Getoutput()->broadcastDouble(fF1Hysterese,function,address,job);
}

void jobSetLuefter2HystValue(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	fF2Hysterese = ( (double*) pMem )[0];
	MyTimers[TIMER_SAVE_DELAY].state = TM_START;
	comRec->Getoutput()->broadcastDouble(fF2Hysterese,function,address,job);
}

void jobSetLuefterSetStatus(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
char *temp;
	temp = (char*) pMem;
	switch(temp[0])
	{
    case 'z':
      u8FlapSetStatus = FLAP_STATUS_CLOSED;
    break;
    case 'o':
      switch( temp[7] )
      {
        case '0':
          u8FlapSetStatus = FLAP_STATUS_OPENED;
        break;
        case '1':
          u8FlapSetStatus = FLAP_STATUS_OPENL1;
        break;
        case '2':
          u8FlapSetStatus = FLAP_STATUS_OPENL2;
        break;
      }
    break;
    case 'A':
      u8FlapSetStatus = FLAP_STATUS_AUTO;
    break;
	}
	//reportFlapSetStatus(comRec->Getoutput());
	//u8FlapSetStatusOld = u8FlapSetStatus;
}

void jobSetTimeBetweenBlocks(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	actReportBetweenBlocks = ( (uint16_t*) pMem )[0];
	comRec->sendAnswerInt(function,address,job,actReportBetweenBlocks,true);
}

void jobSetTimeBetweenSensors(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	actReportBetweenSensors = ( (uint16_t*) pMem )[0];
	comRec->sendAnswerInt(function,address,job,actReportBetweenSensors,true);
}

void jobWaitAfterLastSensor(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	actWaitAfterLastSensor = ( (uint16_t*) pMem )[0];
	comRec->sendAnswerInt(function,address,job,actWaitAfterLastSensor,true);
}

void reportFlapSetStatus(Communication *com)
{
  com->sendStandard(luefterStatusStrings[u8FlapSetStatus],BROADCAST,'L','1','s','T');
}

void reportFlapActualStatus(Communication *com)
{
  com->sendStandard(luefterStatusStrings[u8FlapActualStatus],BROADCAST,'L','1','a','T');
}
void reportFlapMomentaryStatus(Communication *com)
{
  com->sendStandard(luefterStatusStrings[u8FlapMomentaryStatus],BROADCAST,'L','1','m','T');
}
