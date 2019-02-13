//-----------------------------------------------------------------
// GPUNvidia Object
// C++ Source - GPUNvidia.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "StdAfx.h"
#include "GPUNvidia.h"

//-----------------------------------------------------------------
// Update methods
//-----------------------------------------------------------------
GPUNvidia::GPUNvidia(vector<Observer *> observers)
{
	GraficCardIsNvidia = true;
	observerList = observers;
	hSession = 0;
	temperature = 0;
	totalCores = 0;
	totalMemory = 0;
	modelName = 0;
	physicalGPUs = new NvPhysicalGpuHandle[NVAPI_MAX_PHYSICAL_GPUS];
	physicalGPUCount = new NvU32();
	loadLibrary();

	if(lib != NULL)
	{
		// load nvapi_QueryInterface
		NvAPI_QueryInterface = (NvAPI_QueryInterface_t) GetProcAddress(lib, "nvapi_QueryInterface");

		NvAPI_EnumPhysicalGPU = (NvAPI_EnumPhysicalGPU_t) (*NvAPI_QueryInterface)(0xE5AC921F);
		NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t) (*NvAPI_QueryInterface)(0x189A1FDF);
	}

	else
	{
		GraficCardIsNvidia = false;
	}
}

GPUNvidia::~GPUNvidia(void)
{
	if(temperature != 0)
	{
		delete [] temperature;
	}
	if(totalCores != 0)
	{
		delete [] totalCores;
	}
	if(totalMemory !=0)
	{
		delete [] totalMemory;
	}

	if(modelName !=0)
	{
		delete [] modelName;
	}

	NvAPI_DRS_DestroySession(hSession);
	hSession = 0;
	NvAPI_Unload();
}

bool GPUNvidia::getGraficCardIsNvidia()
{
	return GraficCardIsNvidia;
}

void GPUNvidia::loadLibrary()
{
	//Initialize nvAPI
	status = NvAPI_Initialize();

	if (status == NVAPI_OK)
	{
		//Create session for access driver settings
		status = NvAPI_DRS_CreateSession(&hSession);
	}

	if (status == NVAPI_OK)
	{
		// Save driver settngs in the session
		status = NvAPI_DRS_LoadSettings(hSession);
	}

	if (status == NVAPI_OK)
	{
		//Load dll library
		lib = LoadLibraryA("nvapi.dll");
	}
}

void GPUNvidia::getAdapters()
{
	//Get all adapters in the system
	status = NvAPI_EnumPhysicalGPUs(physicalGPUs, physicalGPUCount);

	if (status == NVAPI_OK)
	{
		//Create arrays
		totalGpu = *physicalGPUCount;
		totalCores = new int[*physicalGPUCount];
		totalMemory = new int[*physicalGPUCount];
		modelName = new string[*physicalGPUCount];
		temperature = new int[*physicalGPUCount];
	}
}

void GPUNvidia::getTotalCores()
{
	NvU32 *  countCore = new NvU32();

	//get for every adapter the total cores
	for(unsigned long count = 0; count < *physicalGPUCount; count++)
	{
		status = NvAPI_GPU_GetGpuCoreCount(physicalGPUs[count], countCore);
		totalCores[count] = (int)countCore;
	}

	delete countCore;
	countCore =0;
}

void GPUNvidia::getTotalMemory()
{
	NvU32 *  countMemory = new NvU32();

	//get for all adapters the total memeory
	for(unsigned long count = 0; count < *physicalGPUCount; count++)
	{
		status = NvAPI_GPU_GetPhysicalFrameBufferSize(physicalGPUs[count], countMemory);
		totalMemory[count] = (int)countMemory;
	}

	delete countMemory;
	countMemory = 0;
}

void GPUNvidia::getModels()
{
	NvAPI_ShortString modelName;

	//Get all modelnames
	for(unsigned long count = 0; count < *physicalGPUCount; count++)
	{
		NvAPI_GPU_GetFullName(physicalGPUs[count], modelName);
		modelName[count] = *modelName;
	}
}

void GPUNvidia::getTemperature()
{
	NV_GPU_THERMAL_SETTINGS * pThermalSettings = new NV_GPU_THERMAL_SETTINGS();

	//get all temperature of all gpu adapters
	for(unsigned long count = 0; count < *physicalGPUCount; count++)
	{
		pThermalSettings->count = NVAPI_MAX_THERMAL_SENSORS_PER_GPU;
		pThermalSettings->sensor[NVAPI_MAX_THERMAL_SENSORS_PER_GPU].controller = NVAPI_THERMAL_CONTROLLER_UNKNOWN;
		pThermalSettings->sensor[NVAPI_MAX_THERMAL_SENSORS_PER_GPU].currentTemp = 0;
		pThermalSettings->sensor[NVAPI_MAX_THERMAL_SENSORS_PER_GPU].target = NVAPI_THERMAL_TARGET_GPU;
		status = NvAPI_GPU_GetThermalSettings(physicalGPUs[count], NVAPI_THERMAL_TARGET_GPU, pThermalSettings);

		if (status == NVAPI_OK)
		{
			temperature[count] = pThermalSettings->sensor[0].currentTemp;
		}
	}

	delete pThermalSettings;
	pThermalSettings = 0;
}

void GPUNvidia::getCoreUse()
{
	if (NvAPI_Initialize != NULL || NvAPI_EnumPhysicalGPU != NULL || NvAPI_GPU_GetUsages != NULL)
	{
		//get core use of the root adapter
		int gpuCount = 0;
		int *gpuHandles[NVAPI_MAX_PHYSICAL_GPUS] = { NULL };
		unsigned int gpuUsages[NVAPI_MAX_USAGES_PER_GPU] = { 0 };

		gpuUsages[0] = (NVAPI_MAX_USAGES_PER_GPU * 4) | 0x10000;

		(*NvAPI_EnumPhysicalGPU)(gpuHandles, &gpuCount);

		(*NvAPI_GPU_GetUsages)(gpuHandles[0], gpuUsages);
		this->coreUse = gpuUsages[3];
	}
}

void GPUNvidia::run()
{
	getAdapters();
	getModels();
	getTemperature();
	getTotalCores();
	getTotalMemory();
	getCoreUse();

	this->setSet(true);
	exit();
}


void GPUNvidia::pushDataToWindow()
{
	vector<Observer *>::iterator iterator;

	//Push all data to de observers
	for(iterator = observerList.begin(); iterator != observerList.end(); iterator++)
	{
		(*iterator)->setGPU(modelName, totalCores, totalMemory, temperature, &coreUse, totalGpu);
	}

	delete modelName;
	delete totalCores;
	delete totalMemory;
	delete temperature;
}