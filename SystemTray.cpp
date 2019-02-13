//-----------------------------------------------------------------
// SystemTray Object
// C++ Source - SystemTray.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "SystemTray.h"

//-----------------------------------------------------------------
// SystemTray methods
//-----------------------------------------------------------------
SystemTray::SystemTray(Hardware * hardware, Language * lang, Settings * settings)
{
	currentMessage = 1;
	language = lang;
	music = 0;

	this->settings = settings;
	this->hardware = hardware;
	this->hardware->addObserver(this);

	//Set system tray logo
	this->setIcon(QIcon("logo/thermos.png"));

	openWindow = false;
	contextMenu = new QMenu();

	//Make actions
	exitAction = new QAction(lang->gettranslation("Exit"), this);
	logsAction = new QAction(lang->gettranslation("Open log dir"), this);
	openAction = new QAction(lang->gettranslation("Open"), this);

	//Add actions to systemtray menu
	contextMenu->addAction(openAction);
	contextMenu->addAction(logsAction);
	contextMenu->addAction(exitAction);

	this->setContextMenu(contextMenu);

	//Connect the action to a slot (methode in this class)
	connect(exitAction, SIGNAL(triggered()), this, SLOT(programClose()));
	connect(logsAction, SIGNAL(triggered()), this, SLOT(openLogDir()));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
	connect(this, SIGNAL(messageClicked()), this, SLOT(message()));

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayAction(QSystemTrayIcon::ActivationReason)));

	this->show();
}

SystemTray::~SystemTray(void)
{
	this->hide();
	this->hardware->deleteObserver(this);
	delete exitAction;
	delete logsAction;
	delete openAction;
	delete contextMenu;
}

void SystemTray::systemTrayAction(QSystemTrayIcon::ActivationReason reason)
{
	//System tray is triggered
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
		message();
		break;
	case QSystemTrayIcon::DoubleClick:
		open();
		break;

	default:
		break;
	}
}


void SystemTray::programClose()
{
	//Close Thermos
	QApplication::exit(0);
}

void SystemTray::openLogDir()
{
	//If log dir is exist open it
	if(QDir("logs").exists())
	{
		QDesktopServices::openUrl(QUrl("file:///" + QDir("logs").absolutePath(), QUrl::TolerantMode));
	}

	else
	{
		//Make logs dir and open it
		QDir().mkdir("logs");
		QDesktopServices::openUrl(QUrl("file:///" + QDir("logs").absolutePath(), QUrl::TolerantMode));
	}
}

void SystemTray::open()
{
	//Open main window
	openWindow = true;
	QApplication::exit(1);
}

bool SystemTray::openMainWindow()
{
	return openWindow;
}

void SystemTray::setMemory(int total, int free, int used)
{
	//Set memoryText to text that comes in the systemtray message
	memoryText = "";
	memoryText.append(language->gettranslation("Total"));
	memoryText.append(": ");

	//Change MB to GB or TB
	if(total/1024/1024 > 1)
	{
		memoryText.append(QString::number((float)total/1024/1024));
		memoryText.append(" TB\n");
	}

	else if(total/1024 > 1)
	{
		memoryText.append(QString::number((float)total/1024));
		memoryText.append(" GB\n");
	}

	else 
	{
		memoryText.append(QString::number(total));
		memoryText.append(" MB\n");
	}

	memoryText.append(language->gettranslation("Used"));
	memoryText.append(": ");

	//Change MB to GB or TB
	if(used/1024/1024 > 1)
	{
		memoryText.append(QString::number((float)used/1024/1024));
		memoryText.append(" TB\n");
	}

	else if(used/1024 > 1)
	{
		memoryText.append(QString::number((float)used/1024));
		memoryText.append(" GB\n");
	}

	else 
	{
		memoryText.append(QString::number(used));
		memoryText.append(" MB\n");
	}


	memoryText.append(language->gettranslation("Free"));
	memoryText.append(": ");

	//Change MB to GB or TB
	if(free/1024/1024 > 1)
	{
		memoryText.append(QString::number((float)free/1024/1024));
		memoryText.append(" TB\n");
	}

	else if(free/1024 > 1)
	{
		memoryText.append(QString::number((float)free/1024));
		memoryText.append(" GB\n");
	}

	else 
	{
		memoryText.append(QString::number(free));
		memoryText.append(" MB\n");
	}
}

void SystemTray::setCPU(int speed, int totalCores, int logicalCores, int Load, string name)
{
	CPUtext = "";

	CPUtext.append(language->gettranslation("Name"));
	CPUtext.append(": ");
	CPUtext.append(QString::fromStdString(name));
	CPUtext.append("\n");

	CPUtext.append(language->gettranslation("Speed"));
	CPUtext.append(": ");
	CPUtext.append(QString::number(speed));
	CPUtext.append("MHz\n");

	CPUtext.append(language->gettranslation("Total cores"));
	CPUtext.append(": ");
	CPUtext.append(QString::number(totalCores));
	CPUtext.append("\n");

	CPUtext.append(language->gettranslation("Total logical cores"));
	CPUtext.append(": ");
	CPUtext.append(QString::number(logicalCores));
	CPUtext.append("\n");

	CPUtext.append(language->gettranslation("Load"));
	CPUtext.append(": ");
	CPUtext.append(QString::number(Load));
	CPUtext.append("%\n");
}


void SystemTray::setGPU(string * modelNames, int * totalCores, int * totalMemory, int * temperatures, int * coreUse, int gpuCount)
{
	GPUText = "";
	for(int i =0; i < gpuCount; i++)
	{

		GPUText.append(language->gettranslation("Name: "));
		GPUText.append(QString::fromStdString(modelNames[i]));
		GPUText.append("\n");

		GPUText.append(language->gettranslation("Total cores: "));
		GPUText.append(QString::number(totalCores[i]));
		GPUText.append("\n");

		GPUText.append(language->gettranslation("Total memory: "));
		GPUText.append(QString::number(totalMemory[i]));
		GPUText.append(" MB\n");

		GPUText.append(language->gettranslation("Temperature: "));

		//Change temperature notation
		if(!settings->getDegreeCelsius())
		{
			temperatures[i] = (9/5)*temperatures[i]+32;
			GPUText.append(QString::number(temperatures[i]));
			GPUText.append(" °F\n");
		}
		else
		{
			GPUText.append(QString::number(temperatures[i]));
			GPUText.append(" °C\n");
		}

		GPUText.append(language->gettranslation("Used: "));
		GPUText.append(QString::number(gpuCount));
		GPUText.append(" %\n\n");
	}

}

void SystemTray::setGPU(int totalAdapters, int * temperatures, int * load, int * totalMemory, int* maxFanSpeed, int * minFanSpeed, int * fanSpeed, int * ClockSpeed, int* memoryClock, float * voltage, string * modelName, string * memoryType)
{
	GPUText = "";

	if(totalAdapters != 0)
	{
		for(int i =0; i < totalAdapters; i++)
		{
			GPUText.append(language->gettranslation("Name: "));
			GPUText.append(QString::fromStdString(modelName[i]));
			GPUText.append("\n");


			GPUText.append(language->gettranslation("Total memory: "));
			GPUText.append(QString::number(totalMemory[i]));
			GPUText.append(" MB\n");

			GPUText.append(language->gettranslation("Temperature: "));

			//Change temperature notation
			if(!settings->getDegreeCelsius())
			{
				temperatures[i] = (9/5)*temperatures[i]+32;
				GPUText.append(QString::number(temperatures[i]));
				GPUText.append(" °F\n");
			}
			else
			{
				GPUText.append(QString::number(temperatures[i]));
				GPUText.append(" °C\n");
			}

			GPUText.append(language->gettranslation("Load: "));
			GPUText.append(QString::number(load[i]));
			GPUText.append(" %\n");

			GPUText.append(language->gettranslation("Maximum fan speed: "));
			GPUText.append(QString::number(maxFanSpeed[i]));
			GPUText.append(language->gettranslation(" RPM"));
			GPUText.append("\n");

			GPUText.append(language->gettranslation("Minimum fan speed: "));
			GPUText.append(QString::number(minFanSpeed[i]));
			GPUText.append(language->gettranslation(" RPM"));
			GPUText.append("\n");

			GPUText.append(language->gettranslation("Currect fan speed: "));
			GPUText.append(QString::number(fanSpeed[i]));
			GPUText.append(language->gettranslation(" RPM"));
			GPUText.append("\n");

			GPUText.append(language->gettranslation("Clock speed: "));
			GPUText.append(QString::number(ClockSpeed[i]));
			GPUText.append(language->gettranslation(" MHz"));
			GPUText.append("\n");

			GPUText.append(language->gettranslation("Memory speed: "));
			GPUText.append(QString::number(memoryClock[i]));
			GPUText.append(language->gettranslation(" MHz"));
			GPUText.append("\n");

			GPUText.append(language->gettranslation("Voltage: "));
			GPUText.append(QString::number(voltage[i]));
			GPUText.append(language->gettranslation(" V"));
			GPUText.append("\n");

			GPUText.append(language->gettranslation("Memory type: "));

			GPUText.append(QString::fromStdString(memoryType[i]));
			GPUText.append("\n\n");

			if(temperatures[i] > settings->getHighTemperatureAbove())
			{
				if(music != 0 && music->state() != Phonon::StoppedState)
				{
					music->stop();
					music = 0;
				}
				music =
					Phonon::createPlayer(Phonon::MusicCategory,
					Phonon::MediaSource("alarm.mp3"));
				music->play();
			}

			else if(music != 0 && music->state() != Phonon::StoppedState)
			{
				music->stop();
				music = 0;
			}
		}
	}

}

void SystemTray::setHardDisk(vector<string> driveLetter,vector<string> label, vector<unsigned __int64> capacity, vector<unsigned __int64> freeSpace, vector<unsigned __int64> usedSpace)
{
	HDText ="";

	for(int i =1; i < label.size(); i++)
	{
		HDText.append(QString::fromStdString(label[i]));
		HDText.append(" (");
		HDText.append(QString::fromStdString(driveLetter[i]));
		HDText.append(")\n");

		HDText.append(language->gettranslation("Capacity"));
		HDText.append(": ");

		//Change MB to GB or TB
		if(capacity[i]/1024/1024 > 1)
		{
			HDText.append(QString::number(capacity[i]/1024/1024));
			HDText.append(" TB\n");
		}

		else if(capacity[i]/1024 > 1)
		{
			HDText.append(QString::number(capacity[i]/1024));
			HDText.append(" GB\n");
		}

		else 
		{
			HDText.append(QString::number(capacity[i]));
			HDText.append(" MB\n");
		}

		HDText.append(language->gettranslation("Used"));
		HDText.append(": ");

		//Change MB to GB or TB
		if(usedSpace[i]/1024/1024 > 1)
		{
			HDText.append(QString::number(usedSpace[i]/1024/1024));
			HDText.append(" TB\n");
		}

		else if(usedSpace[i]/1024 > 1)
		{
			HDText.append(QString::number(usedSpace[i]/1024));
			HDText.append(" GB\n");
		}

		else 
		{
			HDText.append(QString::number(usedSpace[i]));
			HDText.append(" MB\n");
		}

		HDText.append(language->gettranslation("Free"));
		HDText.append(": ");

		//Change MB to GB or TB
		if(freeSpace[i]/1024/1024 > 1)
		{
			HDText.append(QString::number(freeSpace[i]/1024/1024));
			HDText.append(" TB\n");
		}

		else if(freeSpace[i]/1024 > 1)
		{
			HDText.append(QString::number(freeSpace[i]/1024));
			HDText.append(" GB\n");
		}

		else 
		{
			HDText.append(QString::number(freeSpace[i]));
			HDText.append(" MB\n");
		}

		HDText.append("\n");

	}
}

void SystemTray::message()
{
	//Show message in message balloon
	if(currentMessage == 1)
	{
		this->showMessage(language->gettranslation("Memory"), memoryText, QSystemTrayIcon::NoIcon, 30000);
		currentMessage = 2;
	}

	else if(currentMessage == 2)
	{
		this->showMessage(language->gettranslation("CPU"), CPUtext, QSystemTrayIcon::NoIcon, 30000);
		currentMessage = 3;
	}

	else if(currentMessage == 3)
	{
		this->showMessage(language->gettranslation("GPU"), GPUText, QSystemTrayIcon::NoIcon, 30000);
		currentMessage = 4;
	}

	else
	{
		this->showMessage(language->gettranslation("HD"), HDText, QSystemTrayIcon::NoIcon, 30000);
		currentMessage = 1;
	}
}
