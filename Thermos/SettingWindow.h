//-----------------------------------------------------------------
// SettingWindow Object
// C++ Header - SettingWindow.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Language.h"
#include "Settings.h"
#include<qsettings.h>
#include <QFileInfo>
 #include <QDesktopServices>
#include "Lettertype.h"

//-----------------------------------------------------------------
// SettingWindow Class
//-----------------------------------------------------------------
class SettingWindow: public QDialog
{
	Q_OBJECT

public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	SettingWindow(Language *, Settings *);

	//---------------------------
	// Destructor
	//---------------------------
	~SettingWindow();

	//---------------------------
	// General Methods
	//---------------------------

private slots:
	//---------------------------
	// Private slots Methods
	//---------------------------
	void okAction();
	void cancelAction();
	void saveAction();
	void closeEvent(QCloseEvent *event);

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void creatWindowLayout();
	void deleteWindowLayout();
	void createGeneraltab();
	void createCustomizetab();
	void createActions();
	void translateInterface();
	void setSettings();
	void createWindow();

	//--------------------------
	// Datamembers
	//--------------------------
	Language * language;
	Settings * settings;
	Lettertype * lettertype;

	QPushButton * okButton;
	QPushButton * cancelButton;
	QPushButton * saveButton;

	QBoxLayout * horizontalLayout;
	QBoxLayout * verticalButtonLayout;
	QBoxLayout * generalWidgetHorizontalLayout;
	QBoxLayout * customizeWidgetHorizontalLayout;
	QBoxLayout *LanguageLayout;
	QBoxLayout *degreeLayout;
	QBoxLayout *timeLayout;
	QBoxLayout * checkTempTimeLayout;
	QBoxLayout * fontSizeLayout;
	QBoxLayout * logTimeLayout;
	QBoxLayout * HighTemperatureAboveLayout;
	QBoxLayout *LettertypeLayout;

	QTabWidget * tabs;
	QDialog * generaltabWidget;
	QDialog * customizeTabWidget;

	QCheckBox * autoStartCheckBox;
	QCheckBox * StartMinimizedCheckBox;
	QCheckBox * CloseToSystemtrayCheckBox;
	QCheckBox * AutoUpdateCheckBox;
	QCheckBox * WidgetEnabledCheckBox;
	QCheckBox * LogCheckbox;

	QLabel * LanguageLabel;
	QLabel * degreeLabel;
	QLabel * timeLabel;
	QLabel * checkTempTimeLabel;
	QLabel * logTimeLabel;
	QLabel * fontSize;
	QLabel * HighTemperatureAboveLabel;
	QLabel * LettertypeLabel;

	QComboBox * LanguageComboBox;
	QComboBox * degreesComboBox;
	QComboBox * timeComboBox;
	QComboBox *LettertypeComboBox;

	QSpinBox * checkTempTimeSpinBox;
	QSpinBox * logTimeSpinBox;
	QSpinBox * fontSizeSpinBox;
	QSpinBox * HighTemperatureAboveSpinBox;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	SettingWindow(const SettingWindow& t);
	SettingWindow& operator=(const SettingWindow& t);
};