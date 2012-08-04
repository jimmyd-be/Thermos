//-----------------------------------------------------------------
// Memeory Object
// C++ Header - Memory.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Observer.h"
#include "HardwareComponent.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define _WIN32_DCOM
#pragma comment(lib, "wbemuuid.lib")

//-----------------------------------------------------------------
// Memory Class
//-----------------------------------------------------------------
class Memory:  public HardwareComponent
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Memory(vector<Observer *> );

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Memory(void);

	//---------------------------
	// General Methods
	//---------------------------
	void setObserverList(vector <Observer *>);
	void pushDataToWindow();

	unsigned int getTotalMemory();
	unsigned int getFreeMemory();
	unsigned int getUsedMemory();
	
private:
	//---------------------------
	// Private Methods
	//---------------------------
	void loadLibrary();
	void setTotalMemory();
	void setFreeMemory();
	void setUsedMemory();
	void run();

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

	unsigned int totalMemory;
	unsigned int freeMemory;
	unsigned int usedMemory;
	vector<Observer *> observerList;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Memory(const Memory& t);
	Memory& operator=(const Memory& t);
};

