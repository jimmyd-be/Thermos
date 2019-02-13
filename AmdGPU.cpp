//-----------------------------------------------------------------
// AmdGPU Object
// C++ Source - AmdGPU.cpp - version v1.0 (2012-05-06)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "AmdGPU.h"

//-----------------------------------------------------------------
// AmdGPU Memory allocation function
//-----------------------------------------------------------------
void* __stdcall ADL_Main_Memory_Alloc( int iSize )
{
	void* lpBuffer = malloc ( iSize );
	return lpBuffer;
}

//-----------------------------------------------------------------
// AmdGPU methods
//-----------------------------------------------------------------
AmdGPU::AmdGPU(vector<Observer *> observers)
{
	this->observerList = observers;

	lpAdapterInfo = NULL;
	totalAdapters = 0;
	iAdapterIndex = 0;
	iNumberAdapters = 0;
	isAmdGPU =false;

	setSet(false);

	modelNames = 0;
	totalAdapters = 0;
	temperatures = 0;
	coreUse = 0;
	totalMemory = 0;
	maxGpufanSpeed = 0;
	minGpufanSpeed = 0;
	fanSpeed = 0;
	gpuClock = 0;
	memoryClock = 0;
	voltage = 0;
	memoryType = 0;

	this->loadLibrary();
}

AmdGPU::~AmdGPU(void)
{
	ADL_Main_Memory_Free ((void **) &lpAdapterInfo );
	ADL_Main_Control_Destroy ();

	FreeLibrary(hDLL);
	deleteData();
}

void AmdGPU::loadLibrary()
{
	//Load dll file
	hDLL = LoadLibrary(TEXT("atiadlxx.dll"));

	if (hDLL == NULL)
	{
		//if load is failed try using dll x64
		hDLL = LoadLibrary(TEXT("atiadlxy.dll"));
	}

	if (NULL != hDLL)
	{
		//get all the function pointers of the methods
		ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE) GetProcAddress(hDLL,"ADL_Main_Control_Create");
		ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY) GetProcAddress(hDLL,"ADL_Main_Control_Destroy");
		ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET) GetProcAddress(hDLL,"ADL_Adapter_NumberOfAdapters_Get");
		ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET) GetProcAddress(hDLL,"ADL_Adapter_AdapterInfo_Get");
		ADL_Overdrive5_Temperature_Get = (ADL_OVERDRIVE5_TEMPERATURE_GET) GetProcAddress(hDLL,"ADL_Overdrive5_Temperature_Get");
		ADL_Overdrive5_FanSpeedInfo_Get = (ADL_OVERDRIVE5_FANSPEEDINFO_GET) GetProcAddress(hDLL,"ADL_Overdrive5_FanSpeedInfo_Get");
		ADL_Adapter_MemoryInfo_Get = (ADL_ADAPTER_MEMORYINFO_GET) GetProcAddress(hDLL,"ADL_Adapter_MemoryInfo_Get");
		ADL_Adapter_Speed_Get = (ADL_ADAPTER_SPEED_GET) GetProcAddress(hDLL,"ADL_Adapter_Speed_Get");
		ADL_Overdrive5_FanSpeed_Get = (ADL_OVERDRIVE5_FANSPEED_GET) GetProcAddress(hDLL,"ADL_Overdrive5_FanSpeed_Get");
		ADL_Overdrive5_CurrentActivity_Get = (ADL_OVERDRIVE5_CURRENTACTIVITY_GET) GetProcAddress(hDLL,"ADL_Overdrive5_CurrentActivity_Get");
		ADL_Adapter_Active_Get = (ADL_ADAPTER_ACTIVE_GET) GetProcAddress(hDLL,"ADL_Adapter_Active_Get");

		if ( NULL != ADL_Main_Control_Create ||
			NULL != ADL_Main_Control_Destroy ||
			NULL != ADL_Adapter_NumberOfAdapters_Get ||
			NULL != ADL_Adapter_AdapterInfo_Get ||
			NULL != ADL_Overdrive5_Temperature_Get ||
			NULL != ADL_Overdrive5_FanSpeedInfo_Get||
			NULL != ADL_Adapter_MemoryInfo_Get ||
			NULL != ADL_Adapter_Speed_Get ||
			NULL != ADL_Overdrive5_FanSpeed_Get ||
			NULL != ADL_Overdrive5_CurrentActivity_Get ||
			NULL != ADL_Adapter_Active_Get)
		{
			//Get all adapters
			if ( ADL_OK == ADL_Main_Control_Create (ADL_Main_Memory_Alloc, 1) && ADL_OK == ADL_Adapter_NumberOfAdapters_Get ( &iNumberAdapters ) )
			{
				if ( 0 < iNumberAdapters )
				{
					lpAdapterInfo = static_cast<LPAdapterInfo>(malloc ( sizeof (AdapterInfo) * iNumberAdapters ));
					memset ( lpAdapterInfo,'\0', sizeof (AdapterInfo) * iNumberAdapters );

					ADL_Adapter_AdapterInfo_Get (lpAdapterInfo, sizeof (AdapterInfo) * iNumberAdapters);
					isAmdGPU = true;
				}
			}
		}
	}
}

bool AmdGPU::getGrapichCardIsAmd()
{
	return isAmdGPU;
}

void AmdGPU::deleteData()
{
	if(modelNames != 0)
	{
		delete[] modelNames;
		modelNames = 0;
	}

	if(temperatures != 0)
	{
		delete[] temperatures;
		temperatures = 0;
	}

	if(coreUse != 0)
	{
		delete[] coreUse;
		coreUse = 0;
	}

	if(totalMemory != 0)
	{
		delete[] totalMemory;
		totalMemory = 0;
	}

	if(maxGpufanSpeed != 0)
	{
		delete[] maxGpufanSpeed;
		maxGpufanSpeed = 0;
	}

	if(minGpufanSpeed != 0)
	{
		delete[] minGpufanSpeed;
		minGpufanSpeed = 0;
	}

	if(memoryType != 0)
	{
		delete[] memoryType;
		memoryType = 0;
	}

	if(fanSpeed != 0)
	{
		delete[] fanSpeed;
		fanSpeed = 0;
	}

	if(voltage != 0)
	{
		delete[] voltage;
		voltage = 0;
	}

	if(gpuClock != 0)
	{
		delete[] gpuClock;
		gpuClock = 0;
	}

	if(memoryClock != 0)
	{
		delete[] memoryClock;
		memoryClock = 0;
	}
}

void AmdGPU::setGpudata()
{
	deleteData();
	totalAdapters = 0;
	modelNames = new string[iNumberAdapters];
	temperatures = new int [iNumberAdapters];
	coreUse = new int [iNumberAdapters];
	totalMemory = new int [iNumberAdapters];
	maxGpufanSpeed = new int [iNumberAdapters];
	minGpufanSpeed = new int [iNumberAdapters];
	memoryType = new string[iNumberAdapters];
	fanSpeed = new int[iNumberAdapters];
	voltage = new float[iNumberAdapters];
	gpuClock = new int[iNumberAdapters];
	memoryClock = new int[iNumberAdapters];

	//For every founded adapter
	for (int i = 0; i < iNumberAdapters; i++)
	{
		int * active = new int();
		iAdapterIndex = lpAdapterInfo[i].iAdapterIndex;
		bool found = false;

		//if the card is active
		for(int j = 0; j < i; j++)
		{
			if(lpAdapterInfo[i].iBusNumber == lpAdapterInfo[j].iBusNumber)
			{
				found = true;
			}
		}

		if(ADL_OK == ADL_Adapter_Active_Get(iAdapterIndex, active) && !found)
		{
			ADLTemperature *  	lpTemperature = new ADLTemperature();
			ADLFanSpeedInfo *  	lpFanSpeedInfo = new ADLFanSpeedInfo();
			ADLMemoryInfo *  	lpMemoryInfo = new ADLMemoryInfo();
			ADLFanSpeedValue *  lpFanSpeedValue = new ADLFanSpeedValue();
			ADLAdapterODClockInfo *  	lpOdClockInfo = new ADLAdapterODClockInfo();
			ADLPMActivity * lpActivity = new ADLPMActivity();
			int * lpCurrent = new int();
			int * lpDefault = new int();
			int *  	lpStatus = new int();

			totalAdapters += 1;
			modelNames[i] = lpAdapterInfo->strAdapterName;

			//get temperature of the adapter
			if( ADL_OK == ADL_Overdrive5_Temperature_Get(iAdapterIndex, 0, lpTemperature))
			{
				temperatures[i] = lpTemperature->iTemperature/1000;

			}

			else
			{
				temperatures[i] = 0;
			}

			//get Fan speed data from the adapter
			if(ADL_OK == ADL_Overdrive5_FanSpeedInfo_Get(iAdapterIndex, 0, lpFanSpeedInfo))
			{
				maxGpufanSpeed[i] = lpFanSpeedInfo->iMaxRPM;
				minGpufanSpeed[i] = lpFanSpeedInfo->iMinRPM;
			}

			else
			{
				maxGpufanSpeed[i] = 0;
				minGpufanSpeed[i] = 0;
			}

			//Get memory info from the adapter
			if(ADL_OK == ADL_Adapter_MemoryInfo_Get(iAdapterIndex, lpMemoryInfo) )
			{
				totalMemory[i] = lpMemoryInfo->iMemorySize/1024/1024;
				memoryType[i] = lpMemoryInfo->strMemoryType;
			}
			else
			{
				totalMemory[i] = 0;
				memoryType[i]= "";
			}

			//Get current fan speed from the adapter
			if(ADL_OK == ADL_Overdrive5_FanSpeed_Get(iAdapterIndex, 0, lpFanSpeedValue))
			{
				fanSpeed[i] = lpFanSpeedValue->iFanSpeed;

			}
			else
			{
				fanSpeed[i] = 0;
			}

			//Get usage, votage, clocks from the adapter
			if(ADL_OK == ADL_Overdrive5_CurrentActivity_Get(iAdapterIndex, lpActivity))
			{

				voltage[i] = (float)lpActivity->iVddc/1000;
				coreUse[i] =  lpActivity->iActivityPercent;
				gpuClock[i] = lpActivity->iEngineClock/100;
				memoryClock[i] = lpActivity->iMemoryClock/100;

			}
			else
			{
				coreUse[i] = 0;
				voltage[i] = 0;
				gpuClock[i] = 0;
				memoryClock[i] = 0;

			}

			delete lpFanSpeedInfo;
			delete lpMemoryInfo;
			delete lpFanSpeedValue;
			delete lpOdClockInfo;
			delete lpActivity;
			delete lpStatus;
			delete lpCurrent;
			delete lpDefault;
			delete lpTemperature;
		}
		delete active;
	}

}

void AmdGPU::run()
{
	this->setGpudata();
	this->setSet(true);
	exit();
}

void AmdGPU::pushDataToWindow()
{
	vector<Observer *>::iterator iterator;

	//Push data to windows
	for(iterator = observerList.begin(); iterator != observerList.end(); iterator++)
	{
		(*iterator)->setGPU(totalAdapters, temperatures, coreUse, totalMemory, maxGpufanSpeed, minGpufanSpeed, fanSpeed, gpuClock, memoryClock, voltage, modelNames, memoryType);
	}
}

void AmdGPU::setObserverList(vector <Observer *> observers)
{
	this->observerList = observers;
}


void __stdcall AmdGPU::ADL_Main_Memory_Free(void** lpBuffer)
{
	if ( NULL != *lpBuffer )
	{
		free ( *lpBuffer );
		*lpBuffer = NULL;
	}
}