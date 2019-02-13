//-----------------------------------------------------------------
// Update Object
// C++ Header - Update.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"

//-----------------------------------------------------------------
// Update Class
//-----------------------------------------------------------------
class Update
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Update(void);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Update(void);
	void removePreviousVersion(QString);
	
	//---------------------------
	// General Methods
	//---------------------------
	QString* checkUpdate();
	QString getUrl();

private:
	//---------------------------
	// Private Methods
	//---------------------------
	QFile * downloadUpdate(QString);
	QString url;

	//--------------------------
	// Datamembers
	//--------------------------
		
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Update(const Update& t);
	Update& operator=(const Update& t);
};