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
SystemTray::SystemTray()
{
	//Set window icon
	this->setIcon(QIcon("logo/thermos.png"));

	//Create contectMenu with 1 item
	contextMenu = new QMenu();
	exitAction = new QAction("Exit", this);

	contextMenu->addAction(exitAction);
	this->setContextMenu(contextMenu);

	//Connect action to slot
	connect(exitAction, SIGNAL(triggered()), this, SLOT(programClose()));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayAction(QSystemTrayIcon::ActivationReason)));

	this->show();
	message();
}

SystemTray::~SystemTray(void)
{
	this->hide();
	delete exitAction;
	delete contextMenu;
}

void SystemTray::systemTrayAction(QSystemTrayIcon::ActivationReason reason)
{
	//If system tray is triggered
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		message();
		break;

	default:
		break;
	}
}


void SystemTray::programClose()
{
	//Close updater
	QApplication::exit(0);
}

void SystemTray::message()
{
	//Open message balloon
	this->showMessage("Thermos updater", "Update in progress!", QSystemTrayIcon::NoIcon, 10000);
}
