//-----------------------------------------------------------------
// AmdGPU Object
// C++ Header - AmdGPU.h - version v1.0 (2012-05-06)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include <tchar.h>
#include "include\adl_sdk.h"
#include <iostream>
#include <stdio.h>
#include "HardwareComponent.h"
#include "Observer.h"
using namespace std;

//-----------------------------------------------------------------
// AmdGPU Class
//-----------------------------------------------------------------
class AmdGPU: public HardwareComponent
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	AmdGPU(vector<Observer *>);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~AmdGPU(void);

	//---------------------------
	// General Methods
	//---------------------------
	bool getGrapichCardIsAmd();
	void run();
	void pushDataToWindow();
	void setObserverList(vector <Observer *>);

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void loadLibrary();
	void setGpudata();
	void deleteData();
	void __stdcall ADL_Main_Memory_Free(void** );

	//--------------------------
	// Datamembers
	//--------------------------
	// Definitions of the used function pointers.
	typedef int ( *ADL_MAIN_CONTROL_CREATE )(ADL_MAIN_MALLOC_CALLBACK, int );
	typedef int ( *ADL_MAIN_CONTROL_DESTROY )();
	typedef int ( *ADL_ADAPTER_NUMBEROFADAPTERS_GET ) ( int* );
	typedef int ( *ADL_ADAPTER_ADAPTERINFO_GET ) ( LPAdapterInfo, int );
	typedef int ( *ADL_DISPLAY_COLORCAPS_GET ) ( int, int, int *, int * );
	typedef int ( *ADL_DISPLAY_COLOR_GET ) ( int, int, int, int *, int *, int *, int *, int * );
	typedef int ( *ADL_DISPLAY_COLOR_SET ) ( int, int, int, int );
	typedef int ( *ADL_DISPLAY_DISPLAYINFO_GET ) ( int, int *, ADLDisplayInfo **, int );
	typedef int (*ADL_OVERDRIVE5_TEMPERATURE_GET)(int, int, ADLTemperature *);
	typedef int (*ADL_OVERDRIVE5_FANSPEEDINFO_GET)(int, int, ADLFanSpeedInfo * );
	typedef int (*ADL_ADAPTER_MEMORYINFO_GET) (int, ADLMemoryInfo *);
	typedef int (*ADL_ADAPTER_SPEED_GET)(int, int*, int *);
	typedef int (*ADL_OVERDRIVE5_FANSPEED_GET) (int, int, ADLFanSpeedValue *);
	typedef int (*ADL_OVERDRIVE5_CURRENTACTIVITY_GET)(int, 	ADLPMActivity * );
	typedef int (*ADL_ADAPTER_ACTIVE_GET)(int, 	int * );

	ADL_MAIN_CONTROL_CREATE          ADL_Main_Control_Create;
	ADL_MAIN_CONTROL_DESTROY         ADL_Main_Control_Destroy;
	ADL_ADAPTER_NUMBEROFADAPTERS_GET ADL_Adapter_NumberOfAdapters_Get;
	ADL_DISPLAY_DISPLAYINFO_GET      ADL_Display_DisplayInfo_Get;
	ADL_OVERDRIVE5_TEMPERATURE_GET ADL_Overdrive5_Temperature_Get;
	ADL_OVERDRIVE5_FANSPEEDINFO_GET ADL_Overdrive5_FanSpeedInfo_Get;
	ADL_ADAPTER_MEMORYINFO_GET ADL_Adapter_MemoryInfo_Get;
	ADL_ADAPTER_SPEED_GET ADL_Adapter_Speed_Get;
	ADL_OVERDRIVE5_FANSPEED_GET ADL_Overdrive5_FanSpeed_Get;
	ADL_OVERDRIVE5_CURRENTACTIVITY_GET ADL_Overdrive5_CurrentActivity_Get;
	ADL_ADAPTER_ADAPTERINFO_GET			ADL_Adapter_AdapterInfo_Get;
	ADL_ADAPTER_ACTIVE_GET ADL_Adapter_Active_Get;

	LPAdapterInfo     lpAdapterInfo;

	int  iNumberAdapters;
	int  iAdapterIndex;

	HINSTANCE hDLL;
	vector<Observer *> observerList;
	string * modelNames;
	int totalAdapters;
	int * temperatures;
	int *coreUse;
	int *totalMemory;
	int* maxGpufanSpeed;
	int* minGpufanSpeed;
	int * fanSpeed;
	int * gpuClock;
	int * memoryClock;
	float *voltage;
	string* memoryType;
	bool isAmdGPU;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	AmdGPU(const AmdGPU& t);
	AmdGPU& operator=(const AmdGPU& t);
};
