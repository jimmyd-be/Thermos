//-----------------------------------------------------------------
// GPUNvidia Object
// C++ Header - GPUNvidia.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "nvapi.h"
#include "NvApiDriverSettings.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "HardwareComponent.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define NVAPI_MAX_USAGES_PER_GPU 34

//-----------------------------------------------------------------
// GPUNvidia Class
//-----------------------------------------------------------------
class GPUNvidia: public HardwareComponent
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	GPUNvidia(vector<Observer *>);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~GPUNvidia(void);

	//---------------------------
	// General Methods
	//---------------------------
	void getAdapters();
	void getModels();
	void getTemperature();
	void getTotalCores();
	void getTotalMemory();
	void getCoreUse();
	bool getGraficCardIsNvidia();
	void pushDataToWindow();

	void run();

	// function pointer types
	typedef int *(*NvAPI_QueryInterface_t)(unsigned int offset);
	typedef int (*NvAPI_EnumPhysicalGPU_t)(int **handles, int *count);
	typedef int (*NvAPI_GPU_GetUsages_t)(int *handle, unsigned int *usages);

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void loadLibrary();

	//--------------------------
	// Datamembers
	//--------------------------
	vector<Observer *> observerList;
	int * temperature;
	int * totalCores;
	int * totalMemory;
	string * modelName;
	int coreUse;
	int totalGpu;

	NvAPI_QueryInterface_t NvAPI_QueryInterface;
	NvAPI_EnumPhysicalGPU_t NvAPI_EnumPhysicalGPU;
	NvAPI_GPU_GetUsages_t NvAPI_GPU_GetUsages;

	NvAPI_Status status;
	NvDRSSessionHandle hSession;

	NvPhysicalGpuHandle * physicalGPUs;
	NvU32 * physicalGPUCount;
	NV_GPU_THERMAL_SETTINGS *  pThermalSettings;
	HMODULE lib;

	bool GraficCardIsNvidia;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	GPUNvidia(const GPUNvidia& t);
	GPUNvidia& operator=(const GPUNvidia& t);
};

