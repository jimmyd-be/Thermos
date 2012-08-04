//-----------------------------------------------------------------
// Language Object
// C++ Header - Language.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"

//-----------------------------------------------------------------
// Language Class
//-----------------------------------------------------------------
class Language
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Language(QString);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Language();

	//---------------------------
	// General Methods
	//---------------------------
	QString gettranslation(QString);
	vector<QString> getLanguageNames();
	void changeLanguage(QString);

private:
	//---------------------------
	// Private Methods
	//---------------------------
	vector<QString> getLanguageFiles();
	bool checkLanguageFile(QString);
	bool readTranslationFile(QString);
	void resize();
	QString findLanguadeFile(QString);
	
	//--------------------------
	// Datamembers
	//--------------------------
	QString** translations;
	int sizeList;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Language(const Language& t);
	Language& operator=(const Language& t);
};

