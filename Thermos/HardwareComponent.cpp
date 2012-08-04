//-----------------------------------------------------------------
// HardwareComponent Object
// C++ Source - HardwareComponent.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "HardwareComponent.h"

//-----------------------------------------------------------------
// HardwareComponent methods
//-----------------------------------------------------------------
HardwareComponent::HardwareComponent(void)
{
	this->set = false;
}

HardwareComponent::~HardwareComponent(void)
{
	//Nothing to destroy
}

void HardwareComponent::setObserverList(vector <Observer *>)
{
	//Nothing to do
}

void HardwareComponent::pushDataToWindow()
{
	//Nothing to do
}

bool HardwareComponent::getSet()
{
	return this->set;
}

void HardwareComponent::setSet(bool set)
{
	this->set = set;
}