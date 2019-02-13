//-----------------------------------------------------------------
// SystemTray Object
// C++ Header - SystemTray.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Observer.h"
#include "Hardware.h"
#include "Language.h"
#include "phonon\mediaobject.h"

//-----------------------------------------------------------------
// SystemTray Class
//-----------------------------------------------------------------
class SystemTray: public QSystemTrayIcon,  public Observer
{
	Q_OBJECT
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	SystemTray(Hardware * hardware, Language * lang, Settings * settings);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~SystemTray(void);

	//---------------------------
	// General Methods
	//---------------------------
	void setMemory(int, int, int);
	void setGPU(string * , int *, int *, int *, int*, int);
	void setGPU(int, int *, int *, int *, int*, int *, int *, int *, int*, float *, string *, string *);
	void setCPU(int, int, int, int, string);
	void setHardDisk(vector<string>, vector<string>, vector<unsigned __int64>, vector<unsigned __int64>, vector<unsigned __int64>);
	bool openMainWindow();

private:
	//---------------------------
	// Private Methods
	//---------------------------
	

	//--------------------------
	// Datamembers
	//--------------------------
	Settings * settings;
	QAction * exitAction;
	QAction * logsAction;
	QAction * openAction;
	Language * language;
	QMenu * contextMenu;
	Hardware * hardware;
	bool openWindow;
	QString messagetext;
	QString memoryText;
	QString CPUtext;
	QString HDText;
	QString GPUText;
	int currentMessage;
	Phonon::MediaObject *music;

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
		void openLogDir();
		void open();
		void systemTrayAction(QSystemTrayIcon::ActivationReason);
		void message();
};

