//-----------------------------------------------------------------
// CPU Object
// C++ Header - CPU.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <cstring>
#include "HardwareComponent.h"
#include "Observer.h"
using namespace std;

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define _WIN32_DCOM
# pragma comment(lib, "wbemuuid.lib")

//-----------------------------------------------------------------
// CPU Class
//-----------------------------------------------------------------
class CPU: public HardwareComponent
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	CPU(vector<Observer *>);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~CPU(void);

	//---------------------------
	// General Methods
	//---------------------------
	void setObserverList(vector <Observer *>);
	void pushDataToWindow();
	void run();

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void setCPUSpeed();
	void setCPUName();
	void loadWMIData();
	void setData();

	int getCPUSpeed();
	string getCPUName();
	int getTotalCores();
	int getLogicalCores();
	int getLoad();

	//--------------------------
	// Datamembers
	//--------------------------
	HRESULT hres;
	IWbemLocator *pLoc;
	IWbemServices *pSvc;
	IEnumWbemClassObject* pEnumerator;
	IWbemClassObject *pclsObj;
	ULONG uReturn;
	VARIANT vtProp;
	int totalCores;
	int logicalCores;
	int cpuSpeed;
	int cpuLoad;
	string cpuName;
	vector<Observer *> observerList;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	CPU(const CPU& t);
	CPU& operator=(const CPU& t);
};

