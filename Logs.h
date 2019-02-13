//-----------------------------------------------------------------
// Logs Object
// C++ Header - Logs.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Observer.h"
#include "Language.h"
#include "Settings.h"
#include "Hardware.h"
#include <QTime>
 #include <QDate>

//-----------------------------------------------------------------
// Logs Class
//-----------------------------------------------------------------
class Logs: public QThread, public Observer
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Logs(Language *, Settings *, Hardware *);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Logs();

	//---------------------------
	// General Methods
	//---------------------------
	void setMemory(int, int, int);
	void setGPU(string *, int *, int *, int *, int*, int);
	void setGPU(int, int *, int *, int *, int*, int *, int *, int *, int*, float *, string *, string *);
	void setCPU(int, int, int, int, int, string);
	void setHardDisk(vector<string>, vector<string>, vector<unsigned __int64>, vector<unsigned __int64>, vector<unsigned __int64>);

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void run();
	void createLogFile();
	void readFile();
	void writeFile();
	int exec();
	QString getCurrentTime();
	
	//--------------------------
	// Datamembers
	//--------------------------
	Language * language;
	Hardware * hardware;
	Settings * settings;
	QFile * logFile;

	int MemoryTotal;
	int MemoryFree;
	int MemoryUsed;
	int CPUSpeed;
	int CPUTotalCores;
	int CPULogicalCores;
	int CPULoad;
	int CPUTemperature;
	int GPUTotalAdapters;

	string CPUName;
	QString fileText;

	vector<int> GPUTotalCores;
	vector<int> GPUTotalMemory;
	vector<int> GPUTemperatures;
	vector<int> GPUCoreUse;
	vector<int> GPUMaxFanSpeed;
	vector<int> GPUMinFanSpeed;
	vector<int>  GPUFanSpeed;
	vector<int>  GPUClockSpeed;
	vector<int>  GPUMemoryClock;
	vector<float>  GPUVoltage;

	vector<string> GPUModelNames;
	vector<string> GPUMemoryType;
	vector<string> HardDiskDriveLetter;
	vector<string> HardDiskLabel;

	vector<unsigned __int64> HardDiskCapacity;
	vector<unsigned __int64> HardDiskFreeSpace;
	vector<unsigned __int64> HardDiskUsedSpace;

	bool firstWrite;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Logs(const Logs& t);
	Logs& operator=(const Logs& t);

	private slots:
		void writedata();
};

