//-----------------------------------------------------------------
// Settings Object
// C++ Header - Settings.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"

//-----------------------------------------------------------------
// Settings Class
//-----------------------------------------------------------------
class Settings
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Settings();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Settings();

	//---------------------------
	// General Methods
	//---------------------------
	bool makeSettingFile();
	bool readFile();
	void saveSettings(bool, bool, bool, bool, bool,	bool, bool, QString, QString, QString, int, int, int, int);
	bool getAutoStart();
	bool getStartMinimized();
	bool getCloseToSystemtray();
	bool getAutoUpdate();
	bool getWidgetEnabled();
	bool getLog();
	bool getDegreeCelsius();
	bool settingsChanged();
	int getCheckTempEvery();
	int getLogTime();
	int getfontSize();
	int getHighTemperatureAbove();
	int getTime();
	int getXPositionScreen();
	int getYPositionScreen();
	int getWidthScreen();
	int getHeightScreen();
	QString getLanguage();
	QString getLettertype();
	void changeScreenGeometrySettings(int, int, int, int);
	void changeChanged(bool);

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void saveToFile();

	//--------------------------
	// Datamembers
	//--------------------------
	bool changed;
	bool AutoStart;
	bool StartMinimized;
	bool CloseToSystemtray;
	bool AutoUpdate;
	bool WidgetEnabled;

	bool Log;
	bool DegreeCelsius;

	int CheckTempEvery;
	int LogTime;
	int fontSize;
	int HighTemperatureAbove;
	int time;

	int XPositionScreen;
	int YPositionScreen;
	int WidthScreen;
	int HeightScreen;

	QString Language;
	QString Lettertype;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Settings(const Settings& t);
	Settings& operator=(const Settings& t);
};