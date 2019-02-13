//-----------------------------------------------------------------
// HardwareComponent Object
// C++ Header - HardwareComponent.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Observer.h"

//-----------------------------------------------------------------
// HardwareComponent Class
//-----------------------------------------------------------------
class HardwareComponent: public QThread
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	HardwareComponent(void);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~HardwareComponent(void);

	//---------------------------
	// General Methods
	//---------------------------
	virtual void setObserverList(vector <Observer *>);
	virtual void pushDataToWindow();
	bool getSet();
	void setSet(bool);

private:
	//---------------------------
	// Private Methods
	//---------------------------

	//--------------------------
	// Datamembers
	//--------------------------
	bool set;
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	HardwareComponent(const HardwareComponent& t);
	HardwareComponent& operator=(const HardwareComponent& t);
};

