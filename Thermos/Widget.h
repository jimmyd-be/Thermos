//-----------------------------------------------------------------
// Widget Object
// C++ Header - Widget.h - version v1.0 (2012-05-04)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Observer.h"
#include "Hardware.h"
#include "Settings.h"


//-----------------------------------------------------------------
// Widget Class
//-----------------------------------------------------------------
class Widget: public QThread,  public Observer
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Widget(Hardware * , Settings * );

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Widget(void);

	//---------------------------
	// General Methods
	//---------------------------
	void setMemory(int, int, int);
	void setGPU(string *, int *, int *, int *, int*, int);
	void setGPU(int, int *, int *, int *, int*, int *, int *, int *, int*, float *, string *, string *);
	void setCPU(int, int, int, int, string);
	void setHardDisk(vector<string>, vector<string>, vector<unsigned __int64>, vector<unsigned __int64>, vector<unsigned __int64>);

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void run();
	int exec();
	void writeToFile();

	//--------------------------
	// Datamembers
	//--------------------------
	Hardware * hardware;
	Settings * settings;
	QString widgetFile;

	QString cpuName;
	QString cpuLoad;
	QString cpuSpeed;
	QString memoryText;

	QString gpuName;
	int gpuTemp;
	QString gpuLoad;
	QString Hdtext;
	int totalHardDisks;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Widget(const Widget& t);
	Widget& operator=(const Widget& t);
};

