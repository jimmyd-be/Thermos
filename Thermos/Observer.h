//-----------------------------------------------------------------
// Observer Object
// C++ Header - Observer.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"

//-----------------------------------------------------------------
// Observer Class
//-----------------------------------------------------------------
class Observer
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Observer(void);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Observer(void);

	//---------------------------
	// General Methods
	//---------------------------
	virtual void setMemory(int, int, int);
	virtual void setGPU(string *, int *, int *, int *, int*, int);
	virtual void setGPU(int, int *, int *, int *, int*, int *, int *, int *, int*, float *, string *, string *);
	virtual void setCPU(int, int, int, int, string);
	virtual void setHardDisk(vector<string>, vector<string>, vector<unsigned __int64>, vector<unsigned __int64>, vector<unsigned __int64>);

private:
	//--------------------------
	// Datamembers
	//--------------------------
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Observer(const Observer& t);
	Observer& operator=(const Observer& t);
};

