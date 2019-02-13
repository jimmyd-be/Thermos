//-----------------------------------------------------------------
// CPU Object
// C++ Source - CPU.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "CPU.h"

//-----------------------------------------------------------------
// CPU methods
//-----------------------------------------------------------------
CPU::CPU(vector<Observer *> observers)
{
	observerList = observers;
	totalCores = 0;
	logicalCores = 0;
	hres = NULL;
	pLoc  = 0;
	pSvc  = 0;
	pEnumerator = 0;
	pclsObj = 0;
	ULONG uReturn = 0;

	totalCores = 0;
	logicalCores = 0;
	cpuSpeed = 0;
	cpuLoad = 0;
	cpuName = "";
	setSet(false);
}

CPU::~CPU(void)
{
	//Release all data
	if(pclsObj != 0)
	{
		pclsObj->Release();
	}

	if(pSvc != 0)
	{
		pSvc->Release();
	}

	if(pLoc != 0)
	{
		pLoc->Release();
	}

	if(pEnumerator != 0)
	{
		pEnumerator->Release();
	}
	VariantClear(&vtProp);
	CoUninitialize();
}

void CPU::loadWMIData()
{
	//initialize WMi
	hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 

	if (!FAILED(hres))
	{
		//Create a instance for a object associated to CLSID
		hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &pLoc);

		if (FAILED(hres))
		{
			CoUninitialize();
		}
		else
		{
			//Connect to local WMI server
			hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);

			if (FAILED(hres))
			{
				if(pSvc != 0)
				{
					pSvc->Release();
					pSvc = 0;
				}

				if(pLoc != 0)
				{
					pLoc->Release();     
					pLoc = 0;
				}

				CoUninitialize();
			}

			else
			{
				//Set autentication information
				hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

				//Query data
				hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_Processor"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL,	&pEnumerator);

				if (FAILED(hres))
				{
					if(pSvc != 0)
					{
						pSvc->Release();
						pSvc = 0;
					}

					if(pLoc != 0)
					{
						pLoc->Release();     
						pLoc = 0;
					}

					CoUninitialize();
				}
			}
		}
	}
}


void CPU::setData()
{
	if (!FAILED(hres))
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if(0 != uReturn)
		{
			//get all CPU data
			hres = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
			totalCores = vtProp.intVal;

			hres = pclsObj->Get(L"NumberOfLogicalProcessors", 0, &vtProp, 0, 0);
			logicalCores = vtProp.intVal;

			hres = pclsObj->Get(L"LoadPercentage", 0, &vtProp, 0, 0);
			cpuLoad = vtProp.intVal;

			VariantClear(&vtProp);
			
		}
	}
}


void CPU::setCPUSpeed() {
	LARGE_INTEGER ulFreq, ulTicks, ulValue, ulStartCounter, ulEAX_EDX;

	// Get high-resolution counter frequency
	if (QueryPerformanceFrequency(&ulFreq)) {
		// Query current value:
		QueryPerformanceCounter(&ulTicks);
		// Calculate end value (one second interval)
		ulValue.QuadPart = ulTicks.QuadPart + ulFreq.QuadPart;
		// Read CPU time-stamp counter:
		__asm RDTSC
		// Save CPU time-stamp counter in ulEAX_EDX
		__asm mov ulEAX_EDX.LowPart, EAX
		__asm mov ulEAX_EDX.HighPart, EDX
		// Save starting counter value
		ulStartCounter.QuadPart = ulEAX_EDX.QuadPart;
		//While loop that simulate 1 second (measured with the high-resolution counter)
		do {
			QueryPerformanceCounter(&ulTicks);
		} while (ulTicks.QuadPart <= ulValue.QuadPart);
		//Read CPU time-stamp counter
		__asm RDTSC
		//save CPU time-stamp counter in ulEAX_EDX
		__asm mov ulEAX_EDX.LowPart, EAX
		__asm mov ulEAX_EDX.HighPart, EDX
		//Calculate number of cycles done in 1 second
		cpuSpeed = ((ulEAX_EDX.QuadPart - ulStartCounter.QuadPart) / 1000000);
	}
}

void CPU::setCPUName()
{
	string BrandString;
	char Parts[48];
	long unsigned CheckBrandString;
	char Answer;

	_asm
	{
		mov EAX, 0x80000000
			CPUID
			mov CheckBrandString, EAX
	}

	//Brandstring is available
	if(CheckBrandString > 0x80000000)
	{
		_asm
		{
			//Get First Part of the Brand String
			mov EAX, 0x80000002
				CPUID

				mov Parts[0], al
				mov Parts[1], ah
				shr EAX, 16
				mov Parts[2], al
				mov Parts[3], ah
				mov Parts[4], bl
				mov Parts[5], bh
				shr EBX, 16
				mov Parts[6], bl
				mov Parts[7], bh
				mov Parts[8], cl
				mov Parts[9], ch
				shr ECX, 16
				mov Parts[10], cl
				mov Parts[11], ch
				mov Parts[12], dl
				mov Parts[13], dh
				shr EDX, 16
				mov Parts[14], dl
				mov Parts[15], dh

				//Get Second Part of the Brand String
				mov EAX, 0x80000003
				CPUID
				mov Parts[16], al
				mov Parts[17], ah
				shr EAX, 16
				mov Parts[18], al
				mov Parts[19], ah
				mov Parts[20], bl
				mov Parts[21], bh
				shr EBX, 16
				mov Parts[22], bl
				mov Parts[23], bh
				mov Parts[24], cl
				mov Parts[25], ch
				shr ECX, 16
				mov Parts[26], cl
				mov Parts[27], ch
				mov Parts[28], dl
				mov Parts[29], dh
				shr EDX, 16
				mov Parts[30], dl
				mov Parts[31], dh

				//Get Third Part of the Brand String
				mov EAX, 0x80000004
				CPUID
				mov Parts[32], al
				mov Parts[33], ah
				shr EAX, 16
				mov Parts[34], al
				mov Parts[35], ah
				mov Parts[36], bl
				mov Parts[37], bh
				shr EBX, 16
				mov Parts[38], bl
				mov Parts[39], bh
				mov Parts[40], cl
				mov Parts[41], ch
				shr ECX, 16
				mov Parts[42], cl
				mov Parts[43], ch
				mov Parts[44], dl
				mov Parts[45], dh
				shr EDX, 16
				mov Parts[46], dl
				mov Parts[47], dh
		}
	}
	cpuName = BrandString.append(Parts, 48);
}

int CPU::getCPUSpeed()
{
	return cpuSpeed;
}

string CPU::getCPUName()
{
	return cpuName;
}

int CPU::getTotalCores()
{
	return totalCores;
}

int CPU::getLogicalCores()
{
	return logicalCores;
}

int CPU::getLoad()
{
	return cpuLoad;
}

void CPU::run()
{
	loadWMIData();
	setCPUSpeed();
	setCPUName();

	setData();

	hres = 0;
	if(pLoc != 0)
	{
		pLoc->Release();
		pLoc = 0;
	}

	if(pSvc != 0)
	{
		pSvc->Release();
		pSvc = 0;
	}

	if(pEnumerator)
	{
		pEnumerator->Release();
		pEnumerator = 0;
	}

	if(pclsObj != 0)
	{
		pclsObj->Release();
		pclsObj = 0;
	}

	uReturn = 0;
	VariantClear(&vtProp);
	CoUninitialize();

	this->setSet(true);
	exit();
}

void CPU::setObserverList(vector <Observer *> observers)
{
	this->observerList = observers;
}

void CPU::pushDataToWindow()
{
	vector<Observer *>::iterator iterator;

	//Push data to observers
	for(iterator = observerList.begin(); iterator != observerList.end(); ++iterator)
	{

		(*iterator)->setCPU(getCPUSpeed(), getTotalCores(), getLogicalCores(), getLoad(), getCPUName());
	}

	totalCores = 0;
	logicalCores = 0;
	hres = NULL;
	pLoc  = 0;
	pSvc  = 0;
	pEnumerator = 0;
	pclsObj = 0;
	uReturn = 0;

	totalCores = 0;
	logicalCores = 0;
	cpuSpeed = 0;
	cpuLoad = 0;
	cpuName = "";
}