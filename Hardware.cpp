//-----------------------------------------------------------------
// Hardware Object
// C++ Source - Hardware.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Hardware.h"

//-----------------------------------------------------------------
// Hardware methods
//-----------------------------------------------------------------
Hardware::Hardware(Settings * settings)
{
	this->settings = settings;

	hardwareList.push_back(new Memory(observerList));
	hardwareList.push_back(new CPU(observerList));
	hardwareList.push_back(new HardDisk(observerList));
	
	AmdGPU * amd = new AmdGPU(observerList);

	//Check GPU adapter
	if(amd->getGrapichCardIsAmd())
	{
		hardwareList.push_back(amd);
	}

	//If no AMd Gpu is found try NVidia
	else
	{
		delete amd;
		amd = 0;

		GPUNvidia * nvidia = new GPUNvidia(observerList);

		if(!nvidia->getGraficCardIsNvidia())
		{
			delete nvidia;
			nvidia = 0;
		}

		else
		{
			hardwareList.push_back(nvidia);
		}
	}
}

Hardware::~Hardware(void)
{
	vector<HardwareComponent *>::iterator iterator;

	//Delete all hardwarecomponenten
	for(int i = 0; i < hardwareList.size(); i++)
	{
		hardwareList[i]->terminate();
		delete hardwareList[i];
	}

	hardwareList.clear();
}

void Hardware::getData()
{
	//Check data in the hardware
	for(int i=0; i< hardwareList.size();i++)
	{
		//If thread isn't running
		if(!hardwareList[i]->isRunning())
		{
			//If data is set push them to GUI
			if(hardwareList[i]->getSet())
			{
				hardwareList[i]->pushDataToWindow();

				hardwareList[i]->setSet(false);
			}
			hardwareList[i]->terminate();

			hardwareList[i]->start();
		}
	}

	//Start timer
	QTimer::singleShot(settings->getCheckTempEvery(), this, SLOT(getData()));
}

void Hardware::addObserver(Observer * observer)
{
	//Add new observer to vector
	observerList.push_back(observer);

	vector<HardwareComponent *>::iterator it;

	//Let every hardware know that there is a new observer
	for(it=hardwareList.begin(); it != hardwareList.end(); it++)
	{
		(*it)->setObserverList(observerList);
	}
 	getData();
}

void Hardware::deleteObserver(Observer * observer)
{
	vector<Observer *>::iterator iterator;

	//Delete observer out of the vector
	for(iterator = observerList.begin(); iterator != observerList.end(); iterator++)
	{
		if(observer == (*iterator))
		{
			observerList.erase(iterator);
			break;
		}
	}

	//Push new vector to hardware class
	for(int i=0; i < hardwareList.size(); i++)
	{
		if(!hardwareList[i]->isFinished())
		{
			hardwareList[i]->terminate();
		}

		hardwareList[i]->setObserverList(observerList);
	}
}