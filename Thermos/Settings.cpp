//-----------------------------------------------------------------
// Settings Object
// C++ Source - Settings.cpp - version v1.0(2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Settings.h"

//-----------------------------------------------------------------
// Update methods
//-----------------------------------------------------------------
Settings::Settings()
{
	AutoStart = false;
	StartMinimized = false;
	CloseToSystemtray = false;
	AutoUpdate = true;
	WidgetEnabled = false;
	Log = true;
	DegreeCelsius = true;
	changed = false;

	CheckTempEvery = 1000;
	LogTime = 5000;
	fontSize = 35;
	HighTemperatureAbove = 70;
	time = 24;
	XPositionScreen = 100;
	YPositionScreen = 100;
	WidthScreen =100;
	HeightScreen =100;

	Language ="";
	Lettertype = "";

	readFile();
}

Settings::~Settings()
{
	//nothing to destroy
}

//make new settings files
bool Settings::makeSettingFile()
{
	QFile file("Thermos.ini");

	//if file not exist and create en open file
	if (!file.exists() && file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		//Write all settings in new settingsfile
		QTextStream writer(&file);
		writer << "[Thermos settings]" << "\n";
		writer << "AutoStart=0;" << "\n";
		writer << "StartMinimized=0;" << "\n";
		writer << "CloseToSystemtray=0;" << "\n";
		writer << "AutoUpdate=0;" << "\n";
		writer << "WidgetEnabled=0;" << "\n";
		writer << "CheckTempEvery=1000;" << "\n" << "\n";

		writer << "[Log settings]" << "\n";
		writer << "Log=0;" << "\n";
		writer << "LogTime=5000;" << "\n" << "\n";

		writer << "[Customized screen]" << "\n";
		writer << "Lettertype=Times New Roman;" << "\n";
		writer << "fontSize=10;" << "\n";
		writer << "HighTemperatureAbove=70;" << "\n";
		writer << "DegreeCelsius=1;" << "\n";
		writer << "Language=English;" << "\n";
		writer << "Time=24;" << "\n" << "\n";

		writer << "[Screen position]" << "\n";
		writer << "XPositionScreen=;" << "\n";
		writer << "YPositionScreen=;" << "\n";
		writer << "WidthScreen=;" << "\n";
		writer << "HeightScreen=;" << "\n" << "\n";

		file.close();
		return true;
	}

	return false;
}

bool Settings::readFile()
{
	QFile file("Thermos.ini");

	if (!file.exists() && !file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		makeSettingFile();
	}

	//Open file if file exists
	if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream fileStream(&file);
		QString line = fileStream.readLine();

		//Loop to end of the file
		while (!line.isNull())
		{
			//Check autostart is enabled
			if(line.startsWith("AutoStart=") && line.endsWith(";"))
			{
				line = line.remove("AutoStart=");
				line = line.remove(";");

				if(line == "1")
				{
					AutoStart= true;
				}

				else
				{
					AutoStart = false;
				}
			}

			//Check start minimized is checked
			else if(line.startsWith("StartMinimized=") && line.endsWith(";"))
			{
				line = line.remove("StartMinimized=");
				line = line.remove(";");

				if(line == "1")
				{
					StartMinimized= true;
				}

				else
				{
					StartMinimized = false;
				}
			}

			//Check close to system tray is checked
			else if(line.startsWith("CloseToSystemtray=") && line.endsWith(";"))
			{
				line = line.remove("CloseToSystemtray=");
				line = line.remove(";");

				if(line == "0")
				{
					CloseToSystemtray= false;
				}

				else
				{
					CloseToSystemtray = true;
				}
			}

			//Check Auto update is checked
			else if(line.startsWith("AutoUpdate=") && line.endsWith(";"))
			{
				line = line.remove("AutoUpdate=");
				line = line.remove(";");

				if(line == "0")
				{
					AutoUpdate= false;
				}

				else
				{
					AutoUpdate = true;
				}
			}

			//Check widget is checked
			else if(line.startsWith("WidgetEnabled=") && line.endsWith(";"))
			{
				line = line.remove("WidgetEnabled=");
				line = line.remove(";");

				if(line == "1")
				{
					WidgetEnabled= true;
				}

				else
				{
					WidgetEnabled = false;
				}
			}

			//Check check time
			else if(line.startsWith("CheckTempEvery=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("CheckTempEvery=");
				line = line.remove(";");

				CheckTempEvery = line.toInt(&succes, 10);

				if(!succes)
				{
					CheckTempEvery = 1000;
				}

			}

			//Check log is enabled
			else if(line.startsWith("Log=") && line.endsWith(";"))
			{
				line = line.remove("Log=");
				line = line.remove(";");

				if(line == "0")
				{
					Log= false;
				}

				else
				{
					Log = true;
				}

			}

			//Check logtime
			else if(line.startsWith("LogTime=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("LogTime=");
				line = line.remove(";");

				LogTime = line.toInt(&succes, 10);

				if(!succes)
				{
					LogTime = 5000;
				}

			}

			//Check lettertype
			else if(line.startsWith("Lettertype=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("Lettertype=");
				line = line.remove(";");

				Lettertype = line;

			}

			//Check Font size
			else if(line.startsWith("fontSize=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("fontSize=");
				line = line.remove(";");

				fontSize = line.toInt(&succes, 10);

				if(!succes)
				{
					fontSize = 12;
				}

			}

			//Check high temperature
			else if(line.startsWith("HighTemperatureAbove=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("HighTemperatureAbove=");
				line = line.remove(";");

				HighTemperatureAbove = line.toInt(&succes, 10);

				if(!succes)
				{
					HighTemperatureAbove = 70;
				}

			}

			//If program in °C or °F
			else if(line.startsWith("DegreeCelsius=") && line.endsWith(";"))
			{
				line = line.remove("DegreeCelsius=");
				line = line.remove(";");

				if(line == "0")
				{
					DegreeCelsius= false;
				}

				else
				{
					DegreeCelsius = true;
				}

			}

			//Check language
			else if(line.startsWith("Language=") && line.endsWith(";"))
			{
				line = line.remove("Language=");
				line = line.remove(";");

				Language = line;

			}

			//Check time
			else if(line.startsWith("Time=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("Time=");
				line = line.remove(";");

				time = line.toInt(&succes, 10);

				if(!succes)
				{
					time = 24;
				}

			}

			//Check x position mainWindow
			else if(line.startsWith("XPositionScreen=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("XPositionScreen=");
				line = line.remove(";");

				XPositionScreen = line.toInt(&succes, 10);

				if(!succes)
				{
					XPositionScreen =   100;
				}

			}

			//Check y position mainWindow
			else if(line.startsWith("YPositionScreen=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("YPositionScreen=");
				line = line.remove(";");

				YPositionScreen = line.toInt(&succes, 10);

				if(!succes)
				{
					YPositionScreen =   100;
				}

			}

			//Check width mainWindow
			else if(line.startsWith("WidthScreen=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("WidthScreen=");
				line = line.remove(";");

				WidthScreen = line.toInt(&succes, 10);

				if(!succes)
				{
					WidthScreen =   100;
				}

			}

			//Check height mainWindow
			else if(line.startsWith("HeightScreen=") && line.endsWith(";"))
			{
				bool succes;
				line = line.remove("HeightScreen=");
				line = line.remove(";");

				HeightScreen = line.toInt(&succes, 10);

				if(!succes)
				{
					HeightScreen =   100;
				}

			}
			line = fileStream.readLine();
		}
	}

	return true;    
}

void Settings::saveSettings(bool autoStartSetting, bool startMinimizedSetting, bool closeToSystemTraySetting, bool autoUpdateSetting, bool widgetEnabledSetting, bool logEnabledSetting, bool degreesSetting, 
	QString LanguageSetting, QString timeSetting, QString LettertypeSetting, int checkTempTimeSetting, int logTimeSetting, int fontSizeSetting, int HighTemperatureAboveSetting)
{
	AutoStart = autoStartSetting;
	StartMinimized = startMinimizedSetting;
	CloseToSystemtray = closeToSystemTraySetting;
	AutoUpdate = autoStartSetting;
	WidgetEnabled = widgetEnabledSetting;
	Log = logEnabledSetting;
	DegreeCelsius = degreesSetting;

	CheckTempEvery = checkTempTimeSetting;
	LogTime = logTimeSetting;
	fontSize = fontSizeSetting;
	HighTemperatureAbove = HighTemperatureAboveSetting;
	time = timeSetting.toInt();

	Language = LanguageSetting;
	Lettertype = LettertypeSetting;

	saveToFile();
}

void Settings::changeScreenGeometrySettings(int windowWidth, int windowHeight, int XPosition, int YPosition)
{
	XPositionScreen = XPosition;
	YPositionScreen = YPosition;
	WidthScreen =windowWidth;
	HeightScreen =windowHeight;

	saveToFile();
}

void Settings::saveToFile()
{
	QFile file("Thermos.ini");

	//Save new settings to settingsfile
	if (file.exists() && file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream writer(&file);
		writer << "[Thermos settings]" << "\n";

		if(AutoStart)
		{
			writer << "AutoStart=1;" << "\n";
		}
		else
		{
			writer << "AutoStart=0;" << "\n";
		}

		if(StartMinimized)
		{
			writer << "StartMinimized=1;" << "\n";
		}
		else
		{
			writer << "StartMinimized=0;" << "\n";
		}

		if(CloseToSystemtray)
		{
			writer << "CloseToSystemtray=1;" << "\n";
		}
		else
		{
			writer << "CloseToSystemtray=0;" << "\n";
		}

		if(AutoUpdate)
		{
			writer << "AutoUpdate=1;" << "\n";
		}
		else
		{
			writer << "AutoUpdate=0;" << "\n";
		}

		if(WidgetEnabled)
		{
			writer << "WidgetEnabled=1;" << "\n";
		}
		else
		{
			writer << "WidgetEnabled=0;" << "\n";
		}

		writer << "CheckTempEvery=" << CheckTempEvery <<";" << "\n" << "\n";

		writer << "[Log settings]" << "\n";

		if(Log)
		{
			writer << "Log=1;" << "\n";
		}

		else
		{
			writer << "Log=0;" << "\n";
		}
		writer << "LogTime=" << LogTime << ";" << "\n" << "\n";

		writer << "[Customized screen]" << "\n";
		writer << "Lettertype=" << Lettertype <<";" << "\n";
		writer << "fontSize=" << fontSize << ";" << "\n";
		writer << "HighTemperatureAbove=" << HighTemperatureAbove <<";" << "\n";

		if(DegreeCelsius)
		{
			writer << "DegreeCelsius=1;" << "\n";
		}

		else
		{
			writer << "DegreeCelsius=0;" << "\n";
		}

		writer << "Language=" << Language <<";" << "\n";
		writer << "Time=" << time <<";" << "\n" << "\n";

		writer << "[Screen position]" << "\n";
		writer << "XPositionScreen=" << XPositionScreen << ";\n";
		writer << "YPositionScreen="<< YPositionScreen << ";\n";
		writer << "WidthScreen=" << WidthScreen << ";\n";
		writer << "HeightScreen=" << HeightScreen <<";\n" << "\n";

		file.close();
	}
}

bool Settings::getAutoStart()			{return AutoStart;}
bool Settings::getStartMinimized()		{return StartMinimized;}
bool Settings::getCloseToSystemtray()	{return CloseToSystemtray;}
bool Settings::getAutoUpdate()			{return AutoUpdate;}
bool Settings::getWidgetEnabled()		{return WidgetEnabled;}
bool Settings::getLog()					{return Log;}
bool Settings::getDegreeCelsius()		{return DegreeCelsius;}
int Settings::getCheckTempEvery()		{return CheckTempEvery;}
int Settings::getLogTime()				{return LogTime;}
int Settings::getfontSize()	{return fontSize;}
int Settings::getHighTemperatureAbove()	{return HighTemperatureAbove;}
int Settings::getTime()					{return time;}
int Settings::getXPositionScreen()		{return XPositionScreen;}
int Settings::getYPositionScreen()		{return YPositionScreen;}
int Settings::getWidthScreen()			{return WidthScreen;}
int Settings::getHeightScreen()			{return HeightScreen;}

QString Settings::getLanguage()
{
	return Language;
}

QString Settings::getLettertype()
{
	return Lettertype;
}

bool Settings::settingsChanged()
{
	return changed;
}

void Settings::changeChanged(bool changed)
{
	this->changed = changed;
}