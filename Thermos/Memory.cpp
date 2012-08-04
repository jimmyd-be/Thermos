//-----------------------------------------------------------------
// Memory Object
// C++ Source - Memory.cpp - version v1.0(2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Memory.h"

//-----------------------------------------------------------------
// Update methods
//-----------------------------------------------------------------

Memory::Memory(vector<Observer *> observers)
{
	this->observerList = observers;

	this->totalMemory = 0;
	this->freeMemory = 0;
	this->usedMemory = 0;

	hres = 0;
	pLoc= 0;
	pSvc = 0;
	pEnumerator = 0;
	pclsObj = 0;
	uReturn = 0;
}

//Remove variables and close connection
Memory::~Memory(void)
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

	if(pEnumerator != 0)
	{
		pEnumerator->Release();
	}
}

void Memory::setObserverList(vector <Observer *> list)
{
	this->observerList = list;
}

void Memory::loadLibrary()
{
	hres =  CoInitializeEx(0, COINIT_APARTMENTTHREADED); 

	if (!FAILED(hres))
	{
		hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &pLoc);

		if (FAILED(hres))
		{
			CoUninitialize();
		}
		else
		{
			hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
		}

		//If wmi connection failed delete all variabels
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
			//Push query to WMI
			hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

			hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM CIM_OperatingSystem"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL,	&pEnumerator);
		}

		//If wmi query failed delete all variabels
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

void Memory::setTotalMemory()
{
	this->loadLibrary();

	if (!FAILED(hres))
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if(0 != uReturn)
		{
			//Check TotalVisibleMemorySize in WMI
			hr = pclsObj->Get(L"TotalVisibleMemorySize", 0, &vtProp, 0, 0);
			this->totalMemory = (unsigned int) _wtoi(vtProp.bstrVal);

			//Delete all properly
			VariantClear(&vtProp);
			CoUninitialize();
		}
	}
}

void Memory::setFreeMemory()
{
	if (!FAILED(hres))
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		//Check FreePhysicalMemory in WMI
		hr = pclsObj->Get(L"FreePhysicalMemory", 0, &vtProp, 0, 0);
		this->freeMemory= (unsigned int) _wtoi(vtProp.bstrVal);

		//Delete all properly
		VariantClear(&vtProp);

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

		if(pEnumerator != 0)
		{
			pEnumerator->Release();
		}
	}
}

void Memory::setUsedMemory()
{
	this->usedMemory = this->totalMemory - this->freeMemory;
}

unsigned int Memory::getTotalMemory()
{
	return totalMemory;
}

unsigned int Memory::getFreeMemory()
{
	return freeMemory;
}

unsigned int Memory::getUsedMemory()
{
	return usedMemory;
}

void Memory::run()
{
	this->freeMemory = 0;
	this->totalMemory = 0;
	this->usedMemory = 0;

	setTotalMemory();
	setFreeMemory();
	setUsedMemory();

	this->freeMemory /= 1024;

	this->totalMemory /= 1024;
	this->usedMemory /= 1024;

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
		pEnumerator = 0;
	}

	if(pclsObj != 0)
	{
		pclsObj->Release();
		pclsObj = 0;
	}

	uReturn = 0;

	CoUninitialize();

	this->setSet(true);
	exit();
}

void Memory::pushDataToWindow()
{
	vector<Observer *>::iterator iterator;

	//Push all data to observers
	for(iterator = observerList.begin(); iterator != observerList.end(); ++iterator)
	{
		(*iterator)->setMemory(getTotalMemory(), getFreeMemory(), getUsedMemory());
	}
}