//-----------------------------------------------------------------
// MainWindow Object
// C++ Header - MainWindow.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "AboutWindow.h"
#include "SettingWindow.h"
#include "Update.h"
#include "Observer.h"
#include "Hardware.h"
#include <QProcess>

//-----------------------------------------------------------------
// MainWindow Class
//-----------------------------------------------------------------
class MainWindow : public QMainWindow, public Observer
{
	Q_OBJECT

public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	MainWindow(Language *, Settings *, Hardware *, QWidget *parent = 0, Qt::WFlags flags = 0);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~MainWindow();

	//---------------------------
	// General Methods
	//---------------------------
	void createWindow();
	void setMemory(int, int, int);
	void setGPU(string * , int *, int *, int *, int*, int);
	void setGPU(int, int *, int *, int *, int*, int *, int *, int *, int*, float *, string *, string *);
	void setCPU(int, int, int, int, string);
	void setHardDisk(vector<string>, vector<string>, vector<unsigned __int64>, vector<unsigned __int64>, vector<unsigned __int64>);

	private slots:
		//--------------------------
		// Private slots methods
		//--------------------------
		void programClose();
		void setting();
		void openLogDir();
		void closetoSystemTray();
		void openHelp();
		void checkUpdate();
		void openAbout();
		void closeEvent(QCloseEvent *event);

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void createMenuBar();
	void createActions();
	void createTabs();
	void createCpuTab();
	void createGpuTab();
	void createMemoryTab();
	void createHdTab();

	void deleteMenuBar();
	void deleteActions();
	void deleteTabs();
	void deleteMemoryTab();
	void deleteGPUTab();
	void deleteGPULayout();
	void deleteCPUTab();
	void deleteHdLayout();
	void deleteHdTab();
	void deleteGPUAMDLayout();

	//--------------------------
	// Datamembers
	//--------------------------
	QMenuBar * menuBar;
	QMenu * fileMenu;
	QMenu * optionMenu;
	QMenu * helpMenu;

	QAction * exitAction;
	QAction * settingAction;
	QAction * openLogAction;
	QAction * closeToTrayAction;
	QAction * helpAction;
	QAction * checkUpdateAction;
	QAction * aboutAction;

	AboutWindow * about;
	SettingWindow * settingWindow;
	Language * language;
	Settings * settings;

	QTabWidget * tabs;
	QDialog * CPUTab;
	QDialog * GPUTab;
	QDialog * memoryTab;
	QDialog * hdTab;

	QBoxLayout * horizontalCpuTabLayout;
	QPushButton * showGraph;

	QBoxLayout * generalWidgetVerticalLayout;
	QBoxLayout * infoHorizontalLayout;
	QBoxLayout * logoHorizontalLayout;
	QBoxLayout * totalMemoryVerticalLayout;
	QBoxLayout * freeMemoryVerticalLayout;
	QBoxLayout * usedMemeoryVerticalLayout;
	QLabel * totalMemoryLabel;
	QLineEdit * totalMemoryField;
	QLabel * freeMemoryLabel;
	QLineEdit * freeMemoryField;
	QLabel * usedMemoryLabel;
	QLineEdit * usedMemoryField;
	QLabel * imageLabel;
	QLabel * imageGPULabel;

	QBoxLayout *generalGpuWidgetHorizontalLayout;
	QToolBox * toolbox;
	Hardware * hardware;
	int gpuCount;
	int hdCount;

	vector<QWidget *> gpuWidgets;
	vector<QBoxLayout *> verticalLayout;
	vector<QBoxLayout *>horizontalTotalCores;
	vector<QBoxLayout *> horizontalGpuMemory;
	vector<QBoxLayout *> horizontalGpuTemp;
	vector<QBoxLayout *> horizontalGpuUse;
	vector<QLineEdit *> gpuTotalCoreField;
	vector<QLineEdit *> gpuTotalMemoryField;
	vector<QLineEdit *> gpuTemperatureField;
	vector<QLineEdit *> gpuUseField;
	vector<QLabel*> gpuTotalCoreLabel;
	vector<QLabel*> gpuTotalMemoryLabel;
	vector<QLabel*>gpuTemperatureLabel;
	vector<QLabel*> gpuUseLabel;

	QBoxLayout * verticalCpuTabLayout;
	QBoxLayout * horizontalCpuLayout;
	QBoxLayout * verticalCpuNameLayout;
	QBoxLayout * verticalCpuSpeedLayout;
	QBoxLayout * verticalCpuTotalCoresLayout;
	QBoxLayout * verticalCpuTotalLogicalLayout;
	QBoxLayout * verticalCpuLoadLayout;

	QLabel * cpuNameLabel;
	QLabel * cpuSpeedLabel;
	QLabel * cpuTotalCoreLabel;
	QLabel * cpuTotalLogicalLabel;
	QLabel * cpuLoadLabel;

	QLineEdit * cpuNameField;
	QLineEdit * cpuSpeedField;
	QLineEdit * cpuTotalCoresField;
	QLineEdit * cpuTotalLogicalField;
	QLineEdit * cpuLoadField;

	QIcon * icon;

	QBoxLayout * generalHdWidgeHorizontalLayout;

	QToolBox * HdToolbox;
	vector<QBoxLayout *> verticalHdLayout;

	vector<QBoxLayout *> horizontalHdCapacity;
	vector<QBoxLayout *> horizontalHdFreeSpace;
	vector<QBoxLayout *> horizontalHdUsedSpace;

	vector<QLineEdit *> hdCapacityField;
	vector<QLineEdit *> hdFreeSpaceField;
	vector<QLineEdit *> hdUsedSpaceField;

	vector<QWidget *> hdWidgets;

	vector<QLabel *> hdCapacityLabel;
	vector<QLabel *> hdFreeSpaceLabel;
	vector<QLabel *> hdUsedLabelLabel;
	QLabel * imageHdLabel;
	QLabel * imageCPULabel;

	vector<QLabel *> gpuMaxFanSpeedLabel;
	vector<QLabel *> gpuMinFanSpeedLabel;
	vector<QLabel *> gpuFanSpeedLabel;
	vector<QLabel *> gpuClockSpeedLabel;
	vector<QLabel *> gpuMemoryClockLabel;
	vector<QLabel *> gpuVoltageLabel;
	vector<QLabel *> gpuMemoryTypeLabel;

	vector<QLineEdit *> gpuMaxFanSpeedField;
	vector<QLineEdit *> gpuMinFanSpeedField;
	vector<QLineEdit *> gpuFanSpeedField;
	vector<QLineEdit *> gpuClockSpeedField;
	vector<QLineEdit *> gpuMemoryClockField;
	vector<QLineEdit *> gpuVoltageField;
	vector<QLineEdit *> gpuMemoryTypeField;

	vector<QBoxLayout *> horizontalGpuMaxFanSpeed;
	vector<QBoxLayout *> horizontalGpuMinFanSpeed;
	vector<QBoxLayout *> horizontalGpuFanSpeed;
	vector<QBoxLayout *> horizontalGpuClockSpeed;
	vector<QBoxLayout *> horizontalGpuMemoryClock;
	vector<QBoxLayout *> horizontalGpuVoltage;
	vector<QBoxLayout *> horizontalGpuMemoryType;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	MainWindow(const MainWindow& t);
	MainWindow& operator=(const MainWindow& t);
};