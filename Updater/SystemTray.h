//-----------------------------------------------------------------
// SystemTray Object
// C++ Header - SystemTray.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QTimer>
#include <QProcess>
#include "Update.h"

//-----------------------------------------------------------------
// SystemTray Class
//-----------------------------------------------------------------
class SystemTray: public QSystemTrayIcon
{
	Q_OBJECT
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	SystemTray();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~SystemTray(void);

private:
	//---------------------------
	// Private Methods
	//---------------------------

	//--------------------------
	// Datamembers
	//--------------------------
	QAction * exitAction;

	QMenu * contextMenu;


	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	SystemTray(const SystemTray& t);
	SystemTray& operator=(const SystemTray& t);


	//--------------------------
	// SLots
	//--------------------------
	private slots:
		void programClose();
		void systemTrayAction(QSystemTrayIcon::ActivationReason);
		void message();
};

