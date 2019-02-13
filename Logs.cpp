//-----------------------------------------------------------------
// Logs Object
// C++ Source - Logs.cpp - version v1.0 (2012-05-12)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Logs.h"


Logs::Logs(Language * lan, Settings * set, Hardware * hard)
{
	this->language = lan;
	this->settings = set;
	this->hardware = hard;

	MemoryTotal = 0;
	MemoryFree = 0;
	MemoryUsed  = 0;

	CPUSpeed = 0;
	CPUTotalCores = 0;
	CPULogicalCores = 0;
	CPULoad = 0;
	CPUTemperature = 0;
	CPUName = "";

	GPUModelNames.clear();
	GPUTotalCores.clear();
	GPUTotalMemory.clear();
	GPUTemperatures.clear();
	GPUCoreUse.clear();
	GPUTotalAdapters = 0;

	GPUMaxFanSpeed.clear();
	GPUMinFanSpeed.clear();
	GPUFanSpeed.clear();
	GPUClockSpeed.clear();
	GPUMemoryClock.clear();
	GPUVoltage.clear();
	GPUMemoryType.clear();

	logFile = 0;
	fileText = "";
	firstWrite = true;

	HardDiskDriveLetter.clear();
	HardDiskLabel.clear();
	HardDiskCapacity.clear();
	HardDiskFreeSpace.clear();
	HardDiskUsedSpace.clear();

	hardware->addObserver(this);

	createLogFile();
	this->start();
}

Logs::~Logs(void)
{
	delete logFile;
	this->terminate();
	hardware->deleteObserver(this);
}

void Logs::run()
{
	exec();
}

int Logs::exec()
{
	//Write data to file every logTime
	while(1)
	{
		readFile();

		//If fileText is not empty and the data is set write to file
		if(CPUTotalCores != 0 && fileText != "")
		{
			writedata();
			writeFile();
		}

		//Let sleep this thread and start after logTime
		sleep(settings->getLogTime()/1000);
	}
}

void Logs::createLogFile()
{
	QFile logFile;
	QDate date;
	date = QDate::currentDate();
	QString templateFile = "";
	QString destination = "";
	QString dateString = date.toString("dd-MM-yyyy");
	QTime time;
	time= QTime::currentTime();	

	templateFile.append(QApplication::applicationDirPath()).append("/logs/template/template.htm");
	destination.append(QApplication::applicationDirPath()).append("/logs/");
	destination.append(dateString).append(time.toString("hh.mm.ss.z")).append(".htm");

	//Copy template file to log directory
	bool succes = QFile::copy(templateFile, destination);

	//If copy is succeed change logfile
	if(succes)
	{
		this->logFile = new QFile(destination);
	}
	else
	{
		delete this;
	}
}

void Logs::readFile()
{
	//read all lines in the logfile when it's open
	if (!logFile->open(QIODevice::ReadOnly))
	{
		QTextStream stream(logFile);

		fileText = stream.readAll();

		logFile->close();
	}
}

void Logs::writeFile()
{
	//Write to file if text is not empty and file is open
	if (fileText != "" && logFile->open(QFile::WriteOnly | QFile::Truncate))
	{
		QTextStream out(logFile);
		out << fileText;
		firstWrite = false;
	}
	fileText= "";
}

QString Logs::getCurrentTime()
{
	QTime time;
	time= QTime::currentTime();

	QString timeString = "";

	if(settings->getTime() == 12)
	{
		timeString = time.toString("hh:mm:ss:z AP");
	}

	else
	{
		timeString = time.toString("hh:mm:ss:z");
	}
	return timeString;
}

void Logs::writedata()
{
	//Chenge html/javascript comment in Thermos data
	QDate date = QDate::currentDate();
	QTime time= QTime::currentTime();	

	QString dateString = date.toString("dd-MM-yyyy").append(" ").append(time.toString("hh.mm.ss.z"));
	fileText.replace("<!--datetime-->", dateString);


	if(settings->getDegreeCelsius())
	{
		fileText.replace("/*temperatureFormat*/", "'C'");
	}
	else
	{
		fileText.replace("/*temperatureFormat*/", "'F'");
	}

	if(firstWrite)
	{
		QString timeText = QString("'").append(getCurrentTime()).append("'");
		fileText.replace("/*time*/", timeText.append("/*time*/"));

		fileText.replace("/*CPULoadData*/", QString::number(CPULoad).append("/*CPULoadData*/"));
		fileText.replace("/*CPUTemperature*/", QString::number(CPUTemperature).append("/*CPUTemperature*/"));

		fileText.replace("/*GPULoadData*/", QString::number(GPUCoreUse[0]).append("/*GPULoadData*/"));
		fileText.replace("/*GPUFanSpeedData*/", QString::number(GPUFanSpeed[0]).append("/*GPUFanSpeedData*/"));
		fileText.replace("/*GPUtemperaturedata*/", QString::number(GPUTemperatures[0]).append("/*GPUtemperaturedata*/"));

		fileText.replace("/*MemoryFree*/", QString::number(MemoryFree).append("/*MemoryFree*/"));
		fileText.replace("/*MemoryUsed*/", QString::number(MemoryUsed).append("/*MemoryUsed*/"));
		fileText.replace("/*Memorytotal*/", QString::number(MemoryTotal).append("/*Memorytotal*/"));
	}

	else
	{
		QString timeText = QString(", '").append(getCurrentTime()).append("'");
		fileText.replace("/*time*/", timeText.append("/*time*/"));

		fileText.replace("/*CPULoadData*/", QString(", ").append(QString::number(CPULoad)).append("/*CPULoadData*/"));
		fileText.replace("/*CPUTemperature*/", QString(", ").append(QString::number(CPUTemperature)).append("/*CPUTemperature*/"));

		fileText.replace("/*GPULoadData*/", QString(", ").append(QString::number(GPUCoreUse[0])).append("/*GPULoadData*/"));
		fileText.replace("/*GPUFanSpeedData*/", QString(", ").append(QString::number(GPUFanSpeed[0])).append("/*GPUFanSpeedData*/"));
		fileText.replace("/*GPUtemperaturedata*/", QString(", ").append(QString::number(GPUTemperatures[0])).append("/*GPUtemperaturedata*/"));

		fileText.replace("/*MemoryFree*/", QString(", ").append(QString::number(MemoryFree)).append("/*MemoryFree*/"));
		fileText.replace("/*MemoryUsed*/", QString(", ").append(QString::number(MemoryUsed)).append("/*MemoryUsed*/"));
		fileText.replace("/*Memorytotal*/", QString(", ").append(QString::number(MemoryTotal)).append("/*Memorytotal*/"));
	}

	fileText.replace("<!--CPUNameField-->", QString::fromStdString(CPUName));
	fileText.replace("<!--CPUFrequencyField-->", QString::number(CPUSpeed));
	fileText.replace("<!--CPUTotalCoreField-->", QString::number(CPUTotalCores));
	fileText.replace("<!--CPULogCoreField-->", QString::number(CPULogicalCores));

	if(firstWrite && GPUModelNames.size() != 0)
	{
		fileText.replace("<!--GPUNameField-->", QString::fromStdString(GPUModelNames[0]));
		fileText.replace("<!--GPUTotalMemoryField-->", QString::number(GPUTotalMemory[0]));
	}
}

void Logs::setMemory(int total, int free, int used)
{
	MemoryTotal = total;
	MemoryFree = free;
	MemoryUsed = used;
}

void Logs::setCPU(int speed, int totalCores, int logicalCores, int Load, int temperature, string name)
{
	this->CPUSpeed = speed;
	this->CPUTotalCores = totalCores;
	this->CPULogicalCores = logicalCores;
	this->CPULoad = Load;
	this->CPUTemperature =  temperature;
	this->CPUName = name;
}


void Logs::setGPU(string * modelNames, int * totalCores, int * totalMemory, int * temperatures, int * coreUse, int gpuCount)
{
	this->GPUModelNames.clear();
	this->GPUTotalCores.clear();
	this->GPUTotalMemory.clear();
	this->GPUTemperatures.clear();
	this->GPUCoreUse.clear();

	this->GPUTotalAdapters = gpuCount;

	for(int i=0; i <gpuCount; i++)
	{
		this->GPUModelNames.push_back(modelNames[i]);
		this->GPUTotalCores.push_back(totalCores[i]);
		this->GPUTotalMemory.push_back(totalMemory[i]);
		this->GPUTemperatures.push_back(temperatures[i]);
		this->GPUCoreUse.push_back(coreUse[i]);
	}
}

void Logs::setGPU(int totalAdapters, int * temperatures, int * load, int * totalMemory, int* maxFanSpeed, int * minFanSpeed, int * fanSpeed, int * ClockSpeed, int* memoryClock, float * voltage, string * modelName, string * memoryType)
{

	this->GPUModelNames.clear();
	this->GPUTotalCores.clear();
	this->GPUTotalMemory.clear();
	this->GPUTemperatures.clear();
	this->GPUCoreUse.clear();

	this->GPUMaxFanSpeed.clear();
	this->GPUMinFanSpeed.clear();
	this->GPUFanSpeed.clear();
	this->GPUClockSpeed.clear();
	this->GPUMemoryClock.clear();
	this->GPUVoltage.clear();
	this->GPUMemoryType.clear();

	this->GPUTotalAdapters = totalAdapters;

	for(int i=0; i <totalAdapters; i++)
	{
		this->GPUTemperatures.push_back(temperatures[i]);
		this->GPUCoreUse.push_back(load[i]);
		this->GPUTotalMemory.push_back(totalMemory[i]);
		this->GPUModelNames.push_back(modelName[i]);
		this->GPUMaxFanSpeed.push_back(maxFanSpeed[i]);
		this->GPUMinFanSpeed.push_back(minFanSpeed[i]);
		this->GPUFanSpeed.push_back(fanSpeed[i]);
		this->GPUClockSpeed.push_back(ClockSpeed[i]);
		this->GPUMemoryClock.push_back(memoryClock[i]);
		this->GPUVoltage.push_back(voltage[i]);
		this->GPUMemoryType.push_back(memoryType[i]);
	}
}

void Logs::setHardDisk(vector<string> driveLetter,vector<string> label, vector<unsigned __int64> capacity, vector<unsigned __int64> freeSpace, vector<unsigned __int64> usedSpace)
{
	this->HardDiskDriveLetter.clear();
	this->HardDiskLabel.clear();
	this->HardDiskCapacity.clear();
	this->HardDiskFreeSpace.clear();
	this->HardDiskUsedSpace.clear();

	this->HardDiskDriveLetter = driveLetter;
	this->HardDiskLabel = label;
	this->HardDiskCapacity = capacity;
	this->HardDiskFreeSpace = freeSpace;
	this->HardDiskUsedSpace = usedSpace;
}