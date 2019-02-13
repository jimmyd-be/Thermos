//-----------------------------------------------------------------
// HardDisk Object
// C++ Source - HardDisk.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "HardDisk.h"

//-----------------------------------------------------------------
// HardDisk methods
//-----------------------------------------------------------------
HardDisk::HardDisk(vector<Observer *> observers):	hres(0), pLoc(0), WbemServicesPointer(0), pEnumerator(0), pclsObj(0), uReturn(0)
{
	this->observerList = observers;
	setSet(false);
}

HardDisk::~HardDisk(void)
{
	//Delete all variables if they exist
	
	CoUninitialize();
	hres = 0;
	if(pLoc != 0)
	{
		pLoc->Release();
		pLoc = 0;
	}

	if(WbemServicesPointer != 0)
	{
		WbemServicesPointer->Release();
		WbemServicesPointer = 0;
	}

	if(pEnumerator != 0)
	{
		pEnumerator->Release();
			pEnumerator= 0;
	}

	if(pclsObj != 0)
	{
		pclsObj->Release();
		pclsObj = 0;
	}

	uReturn = 0;



}

void HardDisk::setObserverList(vector <Observer *> list)
{
	this->observerList = list;
}

void HardDisk::loadLibrary()
{
	//Initialize COM library
	hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 

	//If initialize is not failed
	if (!FAILED(hres))
	{
		hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &pLoc);

		//If failed uninitialize COM library
		if (FAILED(hres))
		{
			CoUninitialize();
		}
		else
		{
			//Connect to local computer WMI server
			hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &WbemServicesPointer);
		}


		if (FAILED(hres))
		{
			if(WbemServicesPointer != 0)
			{
				WbemServicesPointer->Release();
				WbemServicesPointer = 0;
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
			//Query WMI
			hres = CoSetProxyBlanket(WbemServicesPointer, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

			hres = WbemServicesPointer->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_Volume"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
		}

		if (FAILED(hres))
		{
			if(WbemServicesPointer != 0)
			{
				WbemServicesPointer->Release();
				WbemServicesPointer = 0;
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

void HardDisk::setData()
{
	this->loadLibrary();

	if (!FAILED(hres))
	{
		//Check every data in the enum
		while(pEnumerator)
		{
			hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			//If something failes break the while loop
			if(0 == uReturn)
			{
				break;
			}

			if(0 != uReturn)
			{
				hres = pclsObj->Get(L"DriveType", 0, &vtProp, 0, 0);

				//If data is hardDisk get all data of the hard disk
				if(vtProp.intVal == 3)
				{
					unsigned __int64 capacity = 0;
					unsigned __int64 freeSpace = 0;

					hres = pclsObj->Get(L"Capacity", 0, &vtProp, 0, 0);

					this->capacity.push_back(( _wtoi64(vtProp.bstrVal)/1024)/1024);
					capacity = _wtoi64(vtProp.bstrVal)/1024/1024;

					hres = pclsObj->Get(L"FreeSpace", 0, &vtProp, 0, 0);

					this->freeSpace.push_back(( _wtoi64(vtProp.bstrVal)/1024)/1024);
					freeSpace = _wtoi64(vtProp.bstrVal)/1024/1024;

					hres = pclsObj->Get(L"DriveLetter", 0, &vtProp, 0, 0);
					this->driveLetter.push_back(vtProp.pcVal);

					hres = pclsObj->Get(L"Label", 0, &vtProp, 0, 0);
					std::wstring label(vtProp.bstrVal);
					string labelString(label.begin(), label.end()); 

					this->label.push_back(labelString);

					this->usedSpace.push_back(capacity - freeSpace);

					VariantClear(&vtProp);
					
				}
			}
			
		}
		
		CoUninitialize();
	}
}

vector<unsigned __int64> HardDisk::getFreeSpace()
{
	return this->freeSpace;
}

vector<unsigned __int64> HardDisk::getCapacity()
{
	return this->capacity;
}

vector<string> HardDisk::getDriveLetter()
{
	return this->driveLetter;
}

vector<unsigned __int64> HardDisk::getUsedSpace()
{
	return this->usedSpace;
}

vector<string> HardDisk::getLabel()
{
	return this->label;
}

void HardDisk::run()
{
	this->setData();

	//Release all Wmi connections
	hres = 0;
	if(pLoc != 0)
	{
		pLoc->Release();
		pLoc = 0;
	}

	if(WbemServicesPointer != 0)
	{
		WbemServicesPointer->Release();
		WbemServicesPointer = 0;
	}

	if(pEnumerator != 0)
	{
		pEnumerator->Release();
			pEnumerator= 0;
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

void HardDisk::pushDataToWindow()
{
	vector<Observer *>::iterator iterator;

	//Push data to all windows
	for(iterator = observerList.begin(); iterator != observerList.end(); ++iterator)
	{
		(*iterator)->setHardDisk(this->getDriveLetter(), this->getLabel(), this->getCapacity(), this->getFreeSpace(), this->getUsedSpace());
	}


	usedSpace.clear();
	freeSpace.clear();
	capacity.clear();
	driveLetter.clear();
	label.clear();
}