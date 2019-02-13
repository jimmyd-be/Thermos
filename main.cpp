//-----------------------------------------------------------------
// main Object
// C++ Source - main.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Mainwindow.h"
#include "Language.h"
#include "Settings.h"
#include "SystemTray.h"
#include "Hardware.h"
#include "Logs.h"
#include "Widget.h"
#include <QWaitCondition>
#include <QMutex>

using namespace std;

//-----------------------------------------------------------------
// main methods
//-----------------------------------------------------------------
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString argument = "";
	QStringList argumentList = QApplication::arguments();

	Update * update = new Update();
	Settings * settings = 0;
	MainWindow * window = 0;
	SystemTray * tray = 0;
	Logs * logs = 0;
	Widget * widget = 0;
	Language * language = 0;
	Hardware  * hardware = 0;
	bool updatefailed = false;
	bool succes = false;
	int returnValue = -9999999;

	//Check every argument
	for(int i=0; i < argumentList.size(); i++)
	{
		argument = argumentList.at(i);

		//If autostart then wait 30 seconds to help Windows start faster
		if(argument.contains("autostart"))
		{
			Sleep(30000);
		}

		//If update is failed setupdatefailed true
		else if(argument.contains("updateFail"))
		{
			updatefailed = true;
		}

		//If argument set to a url remove old Thermos version
		else if(argument.contains(":"))
		{
			update->removePreviousVersion(argument);
		}
	}

	settings = new Settings();

	QFont newFont(settings->getLettertype(), settings->getfontSize(), QFont::Normal	, false);
	QApplication::setFont(newFont);

	//If autoupdate is enabled and previous update was not failed, start autoupdate
	if(!updatefailed && settings->getAutoUpdate())
	{
		QString * version = update->checkUpdate();

		//If version is not equal start update proces
		if(version[0] > BUILD)
		{
			QProcess proces;
			QStringList argumentList;

			argumentList.append(update->getUrl());
			succes = proces.startDetached("update.exe", argumentList);
		}

		if(version != 0)
		{
			delete version;
		}
	}

	//If the update proces is not start open Thermos
	if(!succes)
	{
		language = new Language(settings->getLanguage());
		hardware = new Hardware(settings);

		//If start minimized is not set start mainWindow
		if(!settings->getStartMinimized())
		{
			window = new MainWindow(language, settings, hardware);
			window->show();
		}

		//If log in enabled start logging
		if(settings->getLog())
		{
			logs = new Logs(language, settings, hardware);
		}

		//If widget is enabled start widget
		if(settings->getWidgetEnabled())
		{
			widget = new Widget(hardware, settings);
		}

		tray = new SystemTray(hardware, language, settings);


		//Start main loop
		while(returnValue = a.exec() && returnValue != 0)
		{
			QFont newFont(settings->getLettertype(), settings->getfontSize(), QFont::Normal	, false);
			QApplication::setFont(newFont);

			//Is settings are changed restart windows with new settings
			if(settings->settingsChanged())
			{
				if(window != 0)
				{
					window->close();
					delete window;
					window = 0;
				}
				window = new MainWindow(language, settings, hardware);
				window->show();

				delete tray;
				tray = new SystemTray(hardware, language, settings);

				if(logs != 0)
				{
					delete logs;
					logs = 0;
				}

				if(widget !=0)
				{
					delete widget;
					widget = 0;
				}

				if(settings->getWidgetEnabled())
				{
					widget = new Widget(hardware, settings);
				}

				if(settings->getLog())
				{
					logs = new Logs(language, settings, hardware);
				}
				settings->changeChanged(false);
			}

			else if(returnValue == 1)
			{
				//If system tray start mainwindow start mainwindow.
				if(tray != 0 && tray->openMainWindow())
				{
					if(window == 0)
					{
						window = new MainWindow(language, settings, hardware);
						window->show();
					}
					else
					{
						window->activateWindow();
					}
				}
				else
				{
					if(window != 0)
					{
						window->close();
						delete window;
						window = 0;
					}
				}
			}

		}


		//Delete all variables and close Thermos
		if(window != 0)
		{
			delete window;
			window = 0;
		}

		if(logs != 0)
		{
			delete logs;
			logs = 0;
		}

		if(widget !=0)
		{
			delete widget;
		}

		tray->hide();
		delete tray;
		delete hardware;
		delete settings;
		delete language;
		delete update;
	}
}