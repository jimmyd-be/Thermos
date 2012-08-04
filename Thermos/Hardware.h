//-----------------------------------------------------------------
// Hardware Object
// C++ Header - Hardware.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Observer.h"
#include "Memory.h"
#include "Settings.h"
#include "GPUNvidia.h"
#include "CPU.h"
#include "HardDisk.h"
#include "AmdGPU.h"
using namespace std;

//-----------------------------------------------------------------
// Hardware Class
//-----------------------------------------------------------------
class Hardware: public QObject
{
	Q_OBJECT

public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Hardware(Settings *);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Hardware(void);

	//---------------------------
	// General Methods
	//---------------------------
	void addObserver(Observer *);
	void deleteObserver(Observer *);

private slots:
	//---------------------------
	// Private Slots Methods
	//---------------------------
	void getData();

private:
	//---------------------------
	// Private Methods
	//---------------------------

	//--------------------------
	// Datamembers
	//--------------------------
	vector<Observer *> observerList;
	vector<HardwareComponent *>hardwareList;
	Settings * settings;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Hardware(const Hardware& t);
	Hardware& operator=(const Hardware& t);
};

