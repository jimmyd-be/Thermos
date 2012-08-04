//-----------------------------------------------------------------
// Update Object
// C++ Source - Update.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "SettingWindow.h"

//-----------------------------------------------------------------
// Update methods
//-----------------------------------------------------------------
SettingWindow::SettingWindow(Language * lan, Settings * se)
{
	language = lan;
	settings = se;
	lettertype = new Lettertype();
	//Change close button bar to only close button 
	setWindowModality(Qt::ApplicationModal);
	setWindowFlags(Qt::WindowTitleHint);

	//Set windowsize
	setFixedSize(350, 400);
	setAttribute( Qt::WA_DeleteOnClose, true );
	createWindow();
}

SettingWindow::~SettingWindow()
{
	deleteWindowLayout();
	delete lettertype;
}


void SettingWindow::createWindow()
{
	//Set window icon and name
	setWindowTitle(language->gettranslation("Settings Thermos"));
	setWindowIcon(QIcon("logo/thermos.png"));

	createGeneraltab();
	createCustomizetab();
	creatWindowLayout();
	createActions();
	setSettings();

	this->show();
}

void SettingWindow::closeEvent(QCloseEvent *event)
{
	this->cancelAction();
}

void SettingWindow::creatWindowLayout()
{
	//Create buttons
	okButton = new QPushButton(language->gettranslation("Ok"));
	cancelButton = new QPushButton(language->gettranslation("Cancel"));
	saveButton = new QPushButton(language->gettranslation("Save"));

	//Create tabs
	tabs = new QTabWidget();
	tabs->setTabPosition(QTabWidget::North);


	//Put items to tab
	tabs->addTab(generaltabWidget, language->gettranslation("General"));
	tabs->addTab(customizeTabWidget, language->gettranslation("Customize"));

	//Set layout of the tabs
	horizontalLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	verticalButtonLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	horizontalLayout->addWidget(tabs);
	verticalButtonLayout->addWidget(okButton);
	verticalButtonLayout->addWidget(cancelButton);
	verticalButtonLayout->addWidget(saveButton);
	horizontalLayout->addLayout(verticalButtonLayout);

	this->setLayout(horizontalLayout);
}

void SettingWindow::createGeneraltab()
{
	//Push all fields and layout to the general tab
	generaltabWidget = new QDialog();
	generalWidgetHorizontalLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	autoStartCheckBox = new QCheckBox(language->gettranslation("Auto start on boot"));
	StartMinimizedCheckBox = new QCheckBox(language->gettranslation("Start minimized"));
	CloseToSystemtrayCheckBox = new QCheckBox(language->gettranslation("Close Thermos to system tray"));
	AutoUpdateCheckBox = new QCheckBox(language->gettranslation("Enable auto update"));
	WidgetEnabledCheckBox = new QCheckBox(language->gettranslation("Enable widget"));
	LogCheckbox = new QCheckBox(language->gettranslation("Enable logging"));

	LanguageLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	LanguageLabel = new QLabel(language->gettranslation("Language"));
	LanguageComboBox = new QComboBox();

	vector<QString> files = language->getLanguageNames();

	for(int i =0; i < files.size(); i++)
	{
		LanguageComboBox->addItem(files[i]);
	}

	LanguageLayout->addWidget(LanguageLabel);
	LanguageLayout->addWidget(LanguageComboBox);

	generalWidgetHorizontalLayout->addWidget(autoStartCheckBox);
	generalWidgetHorizontalLayout->addWidget(StartMinimizedCheckBox);
	generalWidgetHorizontalLayout->addWidget(CloseToSystemtrayCheckBox);
	generalWidgetHorizontalLayout->addWidget(AutoUpdateCheckBox);
	generalWidgetHorizontalLayout->addWidget(WidgetEnabledCheckBox);
	generalWidgetHorizontalLayout->addWidget(LogCheckbox);
	generalWidgetHorizontalLayout->addLayout(LanguageLayout);

	generaltabWidget->setLayout(generalWidgetHorizontalLayout);	
}

void SettingWindow::createCustomizetab()
{
	//Push all fields and layout to the cutomize tab
	customizeTabWidget = new QDialog();

	customizeWidgetHorizontalLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	degreeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	degreesComboBox = new QComboBox();
	degreesComboBox->addItem(language->gettranslation("°celsius"));
	degreesComboBox->addItem(language->gettranslation("°fahrenheit"));

	degreeLabel = new QLabel(language->gettranslation("Show temperature in"));
	degreeLayout->addWidget(degreeLabel);
	degreeLayout->addWidget(degreesComboBox);

	timeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	timeLabel = new QLabel(language->gettranslation("Hours format"));
	timeComboBox = new QComboBox();
	timeComboBox->addItem("12");
	timeComboBox->addItem("24");

	timeLayout->addWidget(timeLabel);
	timeLayout->addWidget(timeComboBox);

	checkTempTimeLayout= new QBoxLayout(QBoxLayout::LeftToRight);
	checkTempTimeLabel= new QLabel(language->gettranslation("Check temperature every"));
	checkTempTimeSpinBox = new QSpinBox();
	checkTempTimeSpinBox->setSuffix(language->gettranslation(" ms"));
	checkTempTimeSpinBox->setMinimum(1);
	checkTempTimeSpinBox->setMaximum(600000);

	checkTempTimeLayout->addWidget(checkTempTimeLabel);
	checkTempTimeLayout->addWidget(checkTempTimeSpinBox);

	logTimeLayout= new QBoxLayout(QBoxLayout::LeftToRight);
	logTimeLabel= new QLabel(language->gettranslation("Log every"));
	logTimeSpinBox = new QSpinBox();
	logTimeSpinBox->setSuffix(language->gettranslation(" ms"));
	logTimeSpinBox->setMinimum(1);
	logTimeSpinBox->setMaximum(600000);

	logTimeLayout->addWidget(logTimeLabel);
	logTimeLayout->addWidget(logTimeSpinBox);

	fontSizeLayout= new QBoxLayout(QBoxLayout::LeftToRight);
	fontSize= new QLabel(language->gettranslation("Font size"));
	fontSizeSpinBox = new QSpinBox();

	fontSizeLayout->addWidget(fontSize);
	fontSizeLayout->addWidget(fontSizeSpinBox);

	HighTemperatureAboveLayout= new QBoxLayout(QBoxLayout::LeftToRight);
	HighTemperatureAboveLabel= new QLabel(language->gettranslation("High temperature above"));
	HighTemperatureAboveSpinBox = new QSpinBox();
	HighTemperatureAboveSpinBox->setMinimum(fontSizeSpinBox->value()+1);

	HighTemperatureAboveLayout->addWidget(HighTemperatureAboveLabel);
	HighTemperatureAboveLayout->addWidget(HighTemperatureAboveSpinBox);

	LettertypeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	LettertypeLabel = new QLabel(language->gettranslation("Lettertype"));
	LettertypeComboBox = new QComboBox();

	QStringList lettertypeList = lettertype->getFonts();

	for(int i =0; i < lettertypeList.size(); i++)
	{

		LettertypeComboBox->addItem(lettertypeList.at(i));
	}

	LettertypeLayout->addWidget(LettertypeLabel);
	LettertypeLayout->addWidget(LettertypeComboBox);

	customizeWidgetHorizontalLayout->addLayout(degreeLayout);
	customizeWidgetHorizontalLayout->addLayout(timeLayout);
	customizeWidgetHorizontalLayout->addLayout(checkTempTimeLayout);
	customizeWidgetHorizontalLayout->addLayout(logTimeLayout);
	customizeWidgetHorizontalLayout->addLayout(HighTemperatureAboveLayout);
	customizeWidgetHorizontalLayout->addLayout(fontSizeLayout);
	customizeWidgetHorizontalLayout->addLayout(LettertypeLayout);
	customizeTabWidget->setLayout(customizeWidgetHorizontalLayout);
}

void SettingWindow::createActions()
{
	//Connect all action to the 3 buttons
	connect(okButton, SIGNAL(clicked()), this , SLOT(okAction()));
	connect(cancelButton, SIGNAL(clicked()), this , SLOT(cancelAction()));
	connect(saveButton, SIGNAL(clicked()), this , SLOT(saveAction()));
}

void SettingWindow::setSettings()
{
	//Set al the settings in the settings window to the current state
	if(settings->getAutoStart())
	{
		autoStartCheckBox->setCheckState(Qt::Checked);
	}
	else
	{
		autoStartCheckBox->setCheckState(Qt::Unchecked);
	}

	if(settings->getStartMinimized())
	{
		StartMinimizedCheckBox->setCheckState(Qt::Checked);
	}
	else
	{
		StartMinimizedCheckBox->setCheckState(Qt::Unchecked);
	}

	if(settings->getCloseToSystemtray())
	{
		CloseToSystemtrayCheckBox->setCheckState(Qt::Checked);
	}
	else
	{
		CloseToSystemtrayCheckBox->setCheckState(Qt::Unchecked);
	}

	if(settings->getAutoUpdate())
	{
		AutoUpdateCheckBox->setCheckState(Qt::Checked);
	}
	else
	{
		AutoUpdateCheckBox->setCheckState(Qt::Unchecked);
	}

	if(settings->getWidgetEnabled())
	{
		WidgetEnabledCheckBox->setCheckState(Qt::Checked);
	}
	else
	{
		WidgetEnabledCheckBox->setCheckState(Qt::Unchecked);
	}

	if(settings->getLog())
	{
		LogCheckbox->setCheckState(Qt::Checked);
	}
	else
	{
		LogCheckbox->setCheckState(Qt::Unchecked);
	}

	if(settings->getDegreeCelsius())
	{
		degreesComboBox->setCurrentIndex(0);
	}
	else
	{
		degreesComboBox->setCurrentIndex(1);
	}

	if(settings->getTime() == 12)
	{
		timeComboBox->setCurrentIndex(0);
	}
	else
	{
		timeComboBox->setCurrentIndex(1);
	}

	LanguageComboBox->setCurrentIndex(LanguageComboBox->findText(settings->getLanguage()));
	LettertypeComboBox->setCurrentIndex(LettertypeComboBox->findText(settings->getLettertype()));

	checkTempTimeSpinBox->setValue(settings->getCheckTempEvery());
	logTimeSpinBox->setValue(settings->getLogTime());
	fontSizeSpinBox->setValue(settings->getfontSize());
	HighTemperatureAboveSpinBox->setValue(settings->getHighTemperatureAbove());
}

void SettingWindow::okAction()
{
	//If ok button is pushed save and reboot
	saveAction();
	this->hide();
	delete this;
}

void SettingWindow::cancelAction()
{
	settings->changeChanged(true);
	this->hide();
	delete this;
}

void SettingWindow::saveAction()
{
	//Save all settings
	bool autoStart = false;
	bool startMinimized = false;
	bool closeToSystemTray  = false;
	bool autoUpdate = false;
	bool widgetEnabled = false;
	bool logEnabled = false;
	bool degreecelsius = false;

	if(autoStartCheckBox->checkState() == Qt::Checked)
	{
		//Put autostart to Windows register
		autoStart = true;
		QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                    QSettings::NativeFormat);

		QString directory = "\"";

		directory.append(QDir::currentPath());
		directory.replace("/", "\\");
		directory.append("\\");
		directory.append(QFileInfo( QCoreApplication::applicationFilePath() ).fileName());
		directory.append("\" /autostart");
		settings.setValue("Thermos", directory);
	}
	else
	{
		//remove autostart from Windows register
		QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                    QSettings::NativeFormat);
		settings.remove("Thermos");
	}

	if(StartMinimizedCheckBox->checkState() == Qt::Checked)
	{
		startMinimized = true;
	}

	if(CloseToSystemtrayCheckBox->checkState() == Qt::Checked)
	{
		closeToSystemTray = true;
	}

	if(AutoUpdateCheckBox->checkState() == Qt::Checked)
	{
		autoUpdate = true;
	}

	if(WidgetEnabledCheckBox->checkState() == Qt::Checked)
	{
		widgetEnabled = true;
	}

	if(LogCheckbox->checkState() == Qt::Checked)
	{
		logEnabled = true;
	}

	if(degreesComboBox->currentIndex() == 0)
	{
		degreecelsius = true;
	}
	
	//Push data to settings class
	settings->saveSettings(autoStart, startMinimized, closeToSystemTray, autoUpdate, widgetEnabled, 
	logEnabled, degreecelsius, LanguageComboBox->currentText(), timeComboBox->currentText(),
	LettertypeComboBox->currentText(), checkTempTimeSpinBox->value(), logTimeSpinBox->value(), fontSizeSpinBox->value(), HighTemperatureAboveSpinBox->value());


	language->changeLanguage(LanguageComboBox->currentText());

	settings->changeChanged(true);
}

void SettingWindow::deleteWindowLayout()
{
	delete autoStartCheckBox;
	delete StartMinimizedCheckBox;
	delete CloseToSystemtrayCheckBox;
	delete AutoUpdateCheckBox;
	delete WidgetEnabledCheckBox;
	delete LogCheckbox;
	delete LanguageComboBox;
	delete degreeLayout;
	delete degreesComboBox;
	delete degreeLabel;
	delete timeLayout;
	delete timeLabel;
	delete timeComboBox;
	delete checkTempTimeLayout;
	delete checkTempTimeLabel;
	delete checkTempTimeSpinBox;
	delete logTimeLayout;
	delete logTimeLabel;
	delete logTimeSpinBox;
	delete fontSizeLayout;
	delete fontSize;
	delete fontSizeSpinBox;
	delete HighTemperatureAboveLayout;
	delete HighTemperatureAboveLabel;
	delete HighTemperatureAboveSpinBox;
	delete LettertypeLayout;
	delete LettertypeLabel;
	delete LettertypeComboBox;	
	delete LanguageLayout;
	delete LanguageLabel;

	delete okButton;
	delete cancelButton;
	delete saveButton;

	delete customizeWidgetHorizontalLayout;
	delete customizeTabWidget;
	delete generalWidgetHorizontalLayout;
	delete generaltabWidget;
	delete tabs;
	delete verticalButtonLayout;
	delete horizontalLayout;
}