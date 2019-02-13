//-----------------------------------------------------------------
// HardDisk Object
// C++ Header - HardDisk.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Observer.h"
#include "HardwareComponent.h"

#include <stdlib.h>
#include <wchar.h>
using namespace std;

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define _WIN32_DCOM
#pragma comment(lib, "wbemuuid.lib")

//-----------------------------------------------------------------
// HardDisk Class
//-----------------------------------------------------------------
class HardDisk:  public HardwareComponent
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	HardDisk(vector<Observer *> );

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~HardDisk(void);

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
	void loadLibrary();
	void setData();

	vector<unsigned __int64> getFreeSpace();
	vector<unsigned __int64> getUsedSpace();
	vector<unsigned __int64> getCapacity();
	vector<string> getDriveLetter();
	vector<string> getLabel();

	//--------------------------
	// Datamembers
	//--------------------------
	HRESULT hres;
	IWbemLocator *pLoc;
	IWbemServices *WbemServicesPointer;
	IEnumWbemClassObject* pEnumerator;
	IWbemClassObject *pclsObj;
    ULONG uReturn;
	VARIANT vtProp;
	
	vector<unsigned __int64> freeSpace;
	vector<unsigned __int64> usedSpace;
	vector<unsigned __int64> capacity;
	vector<string> driveLetter;
	vector<string> label;

	vector<Observer *> observerList;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	HardDisk(const HardDisk& t);
	HardDisk& operator=(const HardDisk& t);
};

