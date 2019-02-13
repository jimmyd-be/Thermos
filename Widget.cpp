//-----------------------------------------------------------------
// Widget Object
// C++ Source - WIdget.cpp - version v1.0 (2012-05-12)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Widget.h"


Widget::Widget(Hardware * hardware, Settings * settings)
{
	this->hardware = hardware;
	this->settings = settings;

	hardware->addObserver(this);

	widgetFile = QDir::homePath().append("/AppData/Local/Microsoft/Windows Sidebar/Gadgets/Thermos.gadget/variable.js");
	this->start();
}


Widget::~Widget(void)
{
	if(this->isRunning())
	{
		this->terminate();
	}
	this->hardware->deleteObserver(this);
}

void Widget::run()
{
	exec();
}

int Widget::exec()
{
	//Write every logtime to file
	while(1)
	{
		writeToFile();
		sleep(settings->getCheckTempEvery()/1000);
	}
}

void Widget::setMemory(int total, int free, int used)
{
	memoryText = "";

	//If used memory size is to high change it to TB or GB
	if(used/1024/1024 > 1)
	{
		memoryText.append(QString::number(used/1024/1024));
		memoryText.append(" TB / ");
	}

	else if(used/1024 > 1)
	{
		memoryText.append(QString::number(used/1024));
		memoryText.append(" GB / ");
	}

	else 
	{
		memoryText.append(QString::number(used));
		memoryText.append(" MB / ");
	}

	//If memory size is to high change it to TB or GB
	if(total/1024/1024 > 1)
	{
		memoryText.append(QString::number(total/1024/1024));
		memoryText.append(" TB");
	}

	else if(total/1024 > 1)
	{
		memoryText.append(QString::number(total/1024));
		memoryText.append(" GB");
	}

	else 
	{
		memoryText.append(QString::number(total));
		memoryText.append(" MB");
	}
}

void Widget::setGPU(string * modelNames, int * totalCores, int * totalMemory, int * temperatures, int * coreUse, int gpuCount)
{
	this->gpuName = QString::fromStdString(modelNames[0]);
	this->gpuTemp = temperatures[0];
	this->gpuLoad = QString::number(coreUse[0]);
}

void Widget::setGPU(int totalAdapters, int * temperatures, int * load, int * totalMemory, int* maxFanSpeed, int * minFanSpeed, int * fanSpeed, int * ClockSpeed, int* memoryClock, float * voltage, string * modelName, string * memoryType)
{
	this->gpuName = QString::fromStdString(modelName[0]);
	this->gpuTemp = temperatures[0];
	this->gpuLoad = QString::number(load[0]);
}

void Widget::setCPU(int speed, int totalCores, int logicalCores, int load, string name)
{
	this->cpuName = QString::fromStdString(name);
	this->cpuLoad = QString::number(load);
	this->cpuSpeed = QString::number(speed);
}

void Widget::setHardDisk(vector<string> driveLetter,vector<string> label, vector<unsigned __int64> capacity, vector<unsigned __int64> freeSpace, vector<unsigned __int64> usedSpace)
{
	Hdtext = "";
	totalHardDisks = capacity.size();

	Hdtext.append("\"");

	for (int i=0; i < capacity.size(); i++)
	{
		Hdtext.append(QString::fromStdString(label[i]));
		Hdtext.append(" (");
		Hdtext.append(QString::fromStdString(driveLetter[i]));
		Hdtext.append(")<br />");

		//If used hard disk size is to high change it to TB or GB
		if(usedSpace[i]/1024/1024 > 1)
		{
			Hdtext.append(QString::number(usedSpace[i]/1024/1024));
			Hdtext.append(" TB / ");
		}

		else if(usedSpace[i]/1024 > 1)
		{
			Hdtext.append(QString::number(usedSpace[i]/1024));
			Hdtext.append(" GB / ");
		}

		else 
		{
			Hdtext.append(QString::number(usedSpace[i]));
			Hdtext.append(" MB / ");
		}

		//If capacity hard disk size is to high change it to TB or GB
		if(capacity[i]/1024/1024 > 1)
		{
			Hdtext.append(QString::number(capacity[i]/1024/1024));
			Hdtext.append(" TB<br />");
		}

		else if(capacity[i]/1024 > 1)
		{
			Hdtext.append(QString::number(capacity[i]/1024));
			Hdtext.append(" GB<br />");
		}

		else 
		{
			Hdtext.append(QString::number(capacity[i]));
			Hdtext.append(" MB<br />");
		}
	}
	Hdtext.append("\";\n");
}

void Widget::writeToFile()
{
	QFile widgetFile(widgetFile);
	QString fileText = "";

	//If file is exist make text for writing in file
	if(widgetFile.exists())
	{
		fileText.append("var memory = \"").append(memoryText).append("\";\n");
		fileText.append("var cpuName = \"").append(cpuName).append("\";\n");
		fileText.append("var cpuLoad = \"Usage: ").append(cpuLoad).append(" %\";\n");
		fileText.append("var cpuSpeed = \"Speed: ").append(cpuSpeed).append(" MHz\";\n");

		fileText.append("var gpuLoad = \"Usage: ").append(gpuLoad).append(" %\";\n");
		fileText.append("var gpuName = \"").append(gpuName).append("\";\n");

		fileText.append("var gpuTemp = \"Temperature: ");

		if(!settings->getDegreeCelsius())
		{
			this->gpuTemp = (9/5)*gpuTemp+32;
			fileText.append(QString::number(gpuTemp));
			fileText.append(" &#176;F");
		}
		else
		{
			fileText.append(QString::number(gpuTemp));
			fileText.append(" &#176;C");
		}
		fileText.append("\";\n");

		fileText.append("var hardDisk = ");
		fileText.append(Hdtext);
		fileText.append("var totalHd = ");
		fileText.append(QString::number(totalHardDisks));
		fileText.append(";");

		//if widgetFile is open write text to it
		if (fileText != "" && widgetFile.open(QFile::WriteOnly | QFile::Truncate))
		{
			QTextStream out(&widgetFile);
			out << fileText;
		}
	}
	fileText= "";
}