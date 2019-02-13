//-----------------------------------------------------------------
// MainWindow Object
// C++ Source - MainWindow.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Mainwindow.h"

//-----------------------------------------------------------------
// MainWindow methods
//-----------------------------------------------------------------
MainWindow::MainWindow(Language * lan, Settings * set,Hardware * hardware, QWidget *parent, Qt::WFlags flags): QMainWindow(parent, flags)
{	 
	this->gpuCount = 0;
	this->CPUTab = 0;

	this->menuBar = 0;
	this->fileMenu = 0;
	this->optionMenu = 0;
	this->helpMenu = 0;

	this->exitAction = 0;
	this->settingAction = 0;
	this->openLogAction = 0;
	this->closeToTrayAction = 0;
	this->helpAction = 0;
	this->checkUpdateAction = 0;
	this->aboutAction = 0;

	this->about = 0;
	this->settingWindow = 0;
	this->language = 0;
	this->settings = 0;
	this->hardware = 0;

	this->tabs = 0;
	this->CPUTab = 0;
	this->GPUTab = 0;
	this->memoryTab = 0;
	this->hdTab = 0;

	this->horizontalCpuTabLayout = 0;
	this->showGraph = 0;

	this->generalWidgetVerticalLayout = 0;
	this->infoHorizontalLayout = 0;
	this->logoHorizontalLayout = 0;
	this->totalMemoryVerticalLayout = 0;
	this->freeMemoryVerticalLayout = 0;
	this->usedMemeoryVerticalLayout = 0;
	this->totalMemoryLabel = 0;
	this->totalMemoryField = 0;
	this->freeMemoryLabel = 0;
	this->freeMemoryField = 0;
	this->usedMemoryLabel = 0;
	this->usedMemoryField = 0;
	this->imageLabel = 0;
	this->imageGPULabel = 0;

	this->generalGpuWidgetHorizontalLayout = 0;
	this->toolbox = 0;
	this->gpuCount = 0;
	this->hdCount = 0;

	this->verticalCpuTabLayout = 0;
	this->horizontalCpuLayout = 0;
	this->verticalCpuNameLayout = 0;
	this->verticalCpuSpeedLayout = 0;
	this->verticalCpuTotalCoresLayout = 0;
	this->verticalCpuTotalLogicalLayout = 0;
	this->verticalCpuLoadLayout = 0;

	this->cpuNameLabel = 0;
	this->cpuSpeedLabel = 0;
	this->cpuTotalCoreLabel = 0;
	this->cpuTotalLogicalLabel = 0;
	this->cpuLoadLabel = 0;

	this->cpuNameField = 0;
	this->cpuSpeedField = 0;
	this->cpuTotalCoresField = 0;
	this->cpuTotalLogicalField = 0;
	this->cpuLoadField = 0;

	this->icon = 0;

	this->generalHdWidgeHorizontalLayout = 0;

	this->HdToolbox = 0;

	this->imageHdLabel = 0;
	this->imageCPULabel = 0;

	language = lan;
	settings = set;

	//Set window icon
	icon = new QIcon();
	icon->addFile("logo/thermos.png");
	setWindowIcon(*icon);

	//Set position and size window
	setGeometry(settings->getXPositionScreen(), settings->getYPositionScreen(), settings->getWidthScreen(), settings->getHeightScreen());

	createWindow();
	this->hardware = hardware;
	this->hardware->addObserver(this);
}

MainWindow::~MainWindow()
{
	hardware->deleteObserver(this);

	this->geometry();
	settings->changeScreenGeometrySettings(geometry().width(), geometry().height(), geometry().x(), geometry().y());
	deleteMemoryTab();
	deleteHdLayout();
	deleteHdTab();
	deleteGPUAMDLayout();
	deleteGPULayout();
	deleteGPUTab();
	deleteCPUTab();
	deleteTabs();
	deleteActions();
	deleteMenuBar();

	delete icon;
}

void MainWindow::createWindow()
{
	createActions();
	createMenuBar();
	createTabs();
}

void MainWindow::createMenuBar()
{
	//Create all menu items and put hem on the menubar
	menuBar = new QMenuBar(this);
	fileMenu = new QMenu(language->gettranslation("File"));
	optionMenu = new QMenu(language->gettranslation("Option"));
	helpMenu = new QMenu(language->gettranslation("Help"));

	fileMenu->addAction(openLogAction);
	fileMenu->addAction(closeToTrayAction);
	fileMenu->addAction(exitAction);

	optionMenu->addAction(settingAction);

	helpMenu->addAction(helpAction);
	helpMenu->addAction(checkUpdateAction);
	helpMenu->addAction(aboutAction);

	this->setMenuBar(menuBar);

	menuBar->addMenu(fileMenu);
	menuBar->addMenu(optionMenu);
	menuBar->addMenu(helpMenu);
}

void MainWindow::createActions()
{
	//Create actions and connect them to the buttons
	exitAction = new QAction(language->gettranslation("Exit"), this);
	exitAction->setShortcut(QKeySequence::Quit);
	connect(exitAction, SIGNAL(triggered()), this , SLOT(programClose()));

	settingAction = new QAction(language->gettranslation("Settings"),this);
	connect(settingAction, SIGNAL(triggered()), this , SLOT(setting()));

	openLogAction = new QAction(language->gettranslation("Open log directory"),this);
	connect(openLogAction, SIGNAL(triggered()), this , SLOT(openLogDir()));

	closeToTrayAction = new QAction(language->gettranslation("Close to system tray"),this);
	connect(closeToTrayAction, SIGNAL(triggered()), this , SLOT(closetoSystemTray()));

	helpAction = new QAction(language->gettranslation("Help"),this);
	helpAction->setShortcut(QKeySequence::HelpContents);
	connect(helpAction, SIGNAL(triggered()), this , SLOT(openHelp()));

	checkUpdateAction = new QAction(language->gettranslation("Check for updates"),this);
	connect(checkUpdateAction, SIGNAL(triggered()), this , SLOT(checkUpdate()));

	aboutAction = new QAction(language->gettranslation("About"),this);
	connect(aboutAction, SIGNAL(triggered()), this , SLOT(openAbout()));

}

void MainWindow::createTabs()
{
	//Create tabs
	tabs = new QTabWidget();
	tabs->setTabPosition(QTabWidget::North);

	createCpuTab();
	createGpuTab();
	createMemoryTab();
	createHdTab();

	//Add tabitems to tab
	tabs->addTab(CPUTab, language->gettranslation("CPU"));
	tabs->addTab(GPUTab, language->gettranslation("GPU"));
	tabs->addTab(memoryTab, language->gettranslation("Memory"));
	tabs->addTab(hdTab, language->gettranslation("HD"));

	setCentralWidget(tabs);
}

void MainWindow::createCpuTab()
{
	//Create cpu tab layout
	CPUTab = new QDialog();

	imageCPULabel = new QLabel();

	//Create layout that makes window look good
	verticalCpuTabLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	horizontalCpuLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	verticalCpuNameLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	verticalCpuSpeedLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	verticalCpuTotalCoresLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	verticalCpuTotalLogicalLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	verticalCpuLoadLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	//Create labels
	cpuNameLabel = new QLabel(language->gettranslation("Model name"));
	cpuSpeedLabel = new QLabel(language->gettranslation("Frequency"));
	cpuTotalCoreLabel = new QLabel(language->gettranslation("Total cores"));
	cpuTotalLogicalLabel = new QLabel(language->gettranslation("Total logical cores"));
	cpuLoadLabel = new QLabel(language->gettranslation("Load"));

	//Create textfields
	cpuNameField = new QLineEdit();
	cpuSpeedField = new QLineEdit();
	cpuTotalCoresField = new QLineEdit();
	cpuTotalLogicalField = new QLineEdit();
	cpuLoadField = new QLineEdit();

	//Set all textfields to readonly
	cpuNameField->setReadOnly(true);
	cpuSpeedField->setReadOnly(true);
	cpuTotalCoresField->setReadOnly(true);
	cpuTotalLogicalField->setReadOnly(true);
	cpuLoadField->setReadOnly(true);


	//Add the labels and textfields to his layouot
	verticalCpuNameLayout->addWidget(cpuNameLabel);
	verticalCpuNameLayout->addWidget(cpuNameField);

	verticalCpuSpeedLayout->addWidget(cpuSpeedLabel);
	verticalCpuSpeedLayout->addWidget(cpuSpeedField);

	verticalCpuTotalCoresLayout->addWidget(cpuTotalCoreLabel);
	verticalCpuTotalCoresLayout->addWidget(cpuTotalCoresField);

	verticalCpuTotalLogicalLayout->addWidget(cpuTotalLogicalLabel);
	verticalCpuTotalLogicalLayout->addWidget(cpuTotalLogicalField);

	verticalCpuLoadLayout->addWidget(cpuLoadLabel);
	verticalCpuLoadLayout->addWidget(cpuLoadField);

	//Add all layouts to the root layout
	horizontalCpuLayout->addLayout(verticalCpuNameLayout);
	horizontalCpuLayout->addLayout(verticalCpuSpeedLayout);
	horizontalCpuLayout->addLayout(verticalCpuTotalCoresLayout);
	horizontalCpuLayout->addLayout(verticalCpuTotalLogicalLayout);
	horizontalCpuLayout->addLayout(verticalCpuLoadLayout);

	verticalCpuTabLayout->addLayout(horizontalCpuLayout);
	verticalCpuTabLayout->addWidget(imageCPULabel);

	//Put the root layout to the tab
	CPUTab->setLayout(verticalCpuTabLayout);
}

void MainWindow::createGpuTab()
{
	GPUTab = new QDialog();
}

void MainWindow::createMemoryTab()
{
	//Create tab
	memoryTab = new QDialog();

	//Create root layout
	generalWidgetVerticalLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	infoHorizontalLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	logoHorizontalLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	//Set layout tab
	memoryTab->setLayout(generalWidgetVerticalLayout);
	generalWidgetVerticalLayout->addLayout(infoHorizontalLayout);
	generalWidgetVerticalLayout->addLayout(logoHorizontalLayout);

	totalMemoryVerticalLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	freeMemoryVerticalLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	usedMemeoryVerticalLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	//Create total memory labels
	totalMemoryLabel = new QLabel();
	totalMemoryLabel->setText(language->gettranslation("Total memory"));

	//Create total memory field
	totalMemoryField = new QLineEdit();
	totalMemoryField->setReadOnly(true);

	//add label and field to layout
	totalMemoryVerticalLayout->addWidget(totalMemoryLabel);
	totalMemoryVerticalLayout->addWidget(totalMemoryField);

	//Create free memory labels
	freeMemoryLabel = new QLabel();
	freeMemoryLabel->setText(language->gettranslation("Free memory"));

	//Create free memory field
	freeMemoryField = new QLineEdit();
	freeMemoryField->setReadOnly(true);

	//add label and field to layout
	freeMemoryVerticalLayout->addWidget(freeMemoryLabel);
	freeMemoryVerticalLayout->addWidget(freeMemoryField);

	//Create used memory labels
	usedMemoryLabel = new QLabel();
	usedMemoryLabel->setText(language->gettranslation("used memory"));

	//Create used memory field
	usedMemoryField = new QLineEdit();
	usedMemoryField->setReadOnly(true);

	//add label and field to layout
	usedMemeoryVerticalLayout->addWidget(usedMemoryLabel);
	usedMemeoryVerticalLayout->addWidget(usedMemoryField);

	//Add layouts to root layout
	infoHorizontalLayout->addLayout(totalMemoryVerticalLayout);
	infoHorizontalLayout->addLayout(freeMemoryVerticalLayout);
	infoHorizontalLayout->addLayout(usedMemeoryVerticalLayout);

	//Add image to layout
	QPixmap memoryLogo = QPixmap("logo/Memory-logo.png");

	imageLabel = new QLabel();
	imageLabel->setPixmap(memoryLogo.scaledToWidth(150, Qt::SmoothTransformation));

	logoHorizontalLayout->addWidget(imageLabel);

}

void MainWindow::deleteActions()
{
	delete settingAction;
	delete openLogAction;
	delete closeToTrayAction;
	delete helpAction;
	delete checkUpdateAction;
	delete aboutAction;
	delete exitAction;
}

void MainWindow::deleteMenuBar()
{
	delete fileMenu;
	delete optionMenu;
	delete helpMenu;
	delete menuBar;
}

void MainWindow::deleteTabs()
{
	delete GPUTab;
	delete memoryTab;

	if(CPUTab != 0)
	{
		delete CPUTab;
		CPUTab = 0;
	}

	if(hdTab != 0)
	{
		delete hdTab;
		hdTab = 0;
	}
	delete tabs;
}

void MainWindow::deleteMemoryTab()
{
	delete totalMemoryLabel;
	delete totalMemoryField;
	delete freeMemoryLabel;
	delete freeMemoryField;
	delete usedMemoryLabel;
	delete usedMemoryField;
	delete imageLabel;

	delete totalMemoryVerticalLayout;
	delete freeMemoryVerticalLayout;
	delete usedMemeoryVerticalLayout;
	delete infoHorizontalLayout;
	delete logoHorizontalLayout;

	delete generalWidgetVerticalLayout;
}

void MainWindow::deleteGPUTab()
{
	if(toolbox != 0)
	{
		delete toolbox;
	}

	if(generalGpuWidgetHorizontalLayout != 0)
	{
		delete generalGpuWidgetHorizontalLayout;
	}

}

void MainWindow::programClose()
{
	//Close Thermos
	QApplication::exit(0);
}

void MainWindow::setting()
{
	settingWindow = new SettingWindow(language, settings);

	if(settingWindow->exec() == 0)
	{
		QApplication::exit(-1);
	}
}

void MainWindow::openLogDir()
{
	//Open log directory if exist. If not exist make it and open it
	if(QDir("logs").exists())
	{
		QDesktopServices::openUrl(QUrl("file:///" + QDir("logs").absolutePath(), QUrl::TolerantMode));
	}

	else
	{
		QDir().mkdir("logs");
		QDesktopServices::openUrl(QUrl("file:///" + QDir("logs").absolutePath(), QUrl::TolerantMode));
	}
}

void MainWindow::closetoSystemTray()
{
	QApplication::exit(1);
}

void MainWindow::openHelp()
{
	QUrl dir(QApplication::applicationDirPath().append("/handleiding.pdf"));
	QDesktopServices::openUrl(dir);
}

void MainWindow::checkUpdate()
{
	//Check updates
	Update * update = new Update();
	QString * version = update->checkUpdate();
	bool installSucces = false;

	QMessageBox * message = new QMessageBox();

	//Check version numbers
	if(version[0] == "")
	{
		message->setIcon(QMessageBox::Warning);
		message->setText(language->gettranslation("Could not connect to server"));
	}

	else if(version[0] > BUILD)
	{
		message->addButton(QMessageBox::Ok);
		message->addButton(QMessageBox::Cancel);
		message->setIcon(QMessageBox::Question);
		message->setText(language->gettranslation("Update Thermos to version " + version[0]));
	}
	else
	{
		message->setIcon(QMessageBox::Information);
		message->setText(language->gettranslation("Your version is up to date"));		
	}

	int buttonClicked = message->exec();

	switch (buttonClicked) {
	case QMessageBox::Cancel:
		break;
	case QMessageBox::Ok:

		//Update process will start
		QProcess proces;
		QStringList argumentList;

		argumentList.append(update->getUrl());
		bool succes = proces.startDetached("update.exe", argumentList);

		if(!succes)
		{
			//Update process can't start Thermos remains open
			message->close();
			delete message;
			message = 0;

			message = new QMessageBox();
			message->addButton(QMessageBox::Cancel);
			message->setIcon(QMessageBox::Warning);
			message->setText(language->gettranslation("Couldn't start update program!"));
			message->exec();
		}

		else
		{
			//if update proces is started close Thermos
			exit(0);
		}
		break;
	}

	message->close();
	delete update;
	delete[] version;
	delete message;
}

void MainWindow::openAbout()
{
	about = new AboutWindow(language);
}

void MainWindow::setMemory(int total, int free, int used)
{
	QString totalMemory = "";

	//Convert MB to GB or TB
	if(total/1024/1024 > 1)
	{
		totalMemory=QString::number((float)total/1024/1024);
		totalMemory.append(" TB");
	}

	else if(total/1024 > 1)
	{
		totalMemory=QString::number((float)total/1024);
		totalMemory.append(" GB");
	}

	else 
	{
		totalMemory=QString::number(total);
		totalMemory.append(" MB");
	}


	QString freeMemory = "";

	//Convert MB to GB or TB
	if(free/1024/1024 > 1)
	{
		freeMemory=QString::number((float)free/1024/1024);
		freeMemory.append(" TB");
	}

	else if(free/1024 > 1)
	{
		freeMemory=QString::number((float)free/1024);
		freeMemory.append(" GB");
	}

	else 
	{
		freeMemory=QString::number(free);
		freeMemory.append(" MB");
	}


	QString usedMemory = "";

	//Convert MB to GB or TB
	if(used/1024/1024 > 1)
	{
		usedMemory=QString::number((float)used/1024/1024);
		usedMemory.append(" TB");
	}

	else if(used/1024 > 1)
	{
		usedMemory=QString::number((float)used/1024);
		usedMemory.append(" GB");
	}

	else 
	{
		usedMemory=QString::number(used);
		usedMemory.append(" MB");
	}

	totalMemoryField->setText(totalMemory);
	freeMemoryField->setText(freeMemory);
	usedMemoryField->setText(usedMemory);
}

void MainWindow::deleteGPULayout()
{
	if(gpuWidgets.size() > 0)
	{
	for(int i=0; i< this->gpuCount;i++)
	{
		delete gpuTotalCoreField[i];
		delete gpuTotalMemoryField[i];
		delete gpuTemperatureField[i];
		delete gpuUseField[i];

		delete gpuTotalCoreLabel[i];
		delete gpuTotalMemoryLabel[i];
		delete gpuTemperatureLabel[i];
		delete gpuUseLabel[i];

		delete horizontalTotalCores[i];
		delete horizontalGpuMemory[i];
		delete horizontalGpuTemp[i];
		delete horizontalGpuUse[i];

		delete verticalLayout[i];
		delete gpuWidgets[i];
	}

	if(generalGpuWidgetHorizontalLayout != 0)
	{
		delete generalGpuWidgetHorizontalLayout;
		generalGpuWidgetHorizontalLayout = 0;
	}

	if(toolbox !=0)
	{
		delete toolbox;
		toolbox = 0;
	}

	gpuWidgets.clear();
	verticalLayout.clear();
	horizontalTotalCores.clear();
	horizontalGpuMemory.clear();
	horizontalGpuTemp.clear();
	horizontalGpuUse.clear();
	gpuTotalCoreField.clear();
	gpuTotalMemoryField.clear();
	gpuTemperatureField.clear();
	gpuUseField.clear();
	gpuTotalCoreLabel.clear();
	gpuTotalMemoryLabel.clear();
	gpuTemperatureLabel.clear();
	gpuUseLabel.clear();
	}
}

void MainWindow::setGPU(string * modelNames, int * totalCores, int * totalMemory, int * temperatures, int * coreUse, int gpuCount)
{
	int currentToolBoxIndex= -1;

	if(HdToolbox != 0)
	{
		currentToolBoxIndex = toolbox->currentIndex();
	}

	this->gpuCount = gpuCount;

	//if there is a gpu found
	if(gpuCount != 0)
	{
		//Clear current layout
		deleteGPULayout();

		QString modelNameLogo = QString::fromStdString(modelNames[0]);
		imageGPULabel = new QLabel();

		//Create logo
		QPixmap cpuLogo = QPixmap("logo/nvidia_logo.jpg");
		imageGPULabel->setPixmap(cpuLogo.scaledToWidth(50, Qt::SmoothTransformation));

		//create toolbox and layout
		toolbox = new QToolBox();
		generalGpuWidgetHorizontalLayout = new QBoxLayout(QBoxLayout::LeftToRight);

		//Add toolbox and image to layout
		generalGpuWidgetHorizontalLayout->addWidget(toolbox);
		generalGpuWidgetHorizontalLayout->addWidget(imageGPULabel);

		//Add root layout to tab
		GPUTab->setLayout(generalGpuWidgetHorizontalLayout);

		for(int i =0; i < gpuCount; i++)
		{
			//Create layouts and add them to vectors
			verticalLayout.push_back(new QBoxLayout(QBoxLayout::TopToBottom));
			gpuWidgets.push_back(new QWidget());

			gpuWidgets[i]->setLayout(verticalLayout[i]);

			horizontalTotalCores.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuMemory.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuTemp.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuUse.push_back(new QBoxLayout(QBoxLayout::LeftToRight));

			//Create textfield and add them to vectors
			gpuTotalCoreField.push_back(new QLineEdit());
			gpuTotalMemoryField.push_back(new QLineEdit());
			gpuTemperatureField.push_back(new QLineEdit());
			gpuUseField.push_back(new QLineEdit());

			//Create labels and add them to vectors
			gpuTotalCoreLabel.push_back(new QLabel(language->gettranslation("Total cores: ")));
			gpuTotalMemoryLabel.push_back(new QLabel(language->gettranslation("Total memory: ")));
			gpuTemperatureLabel.push_back(new QLabel(language->gettranslation("Temperature: ")));
			gpuUseLabel.push_back(new QLabel(language->gettranslation("Used: ")));

			QString totalCoresString =QString::number(totalCores[i]);
			QString totalMemoryString =QString::number(totalMemory[i]);
			QString gpuUseString=QString::number(gpuCount);
			totalMemoryString.append(" MB");
			QString temperature = "";

			//Change temperature format
			if(!settings->getDegreeCelsius())
			{
				temperatures[i] = (9/5)*temperatures[i]+32;
				temperature = QString::number(temperatures[i]);
				temperature.append(" °F");
			}
			else
			{
				temperature = QString::number(temperatures[i]);
				temperature.append(" °C");
			}

			gpuUseString.append(" %");

			//Set all data to text field
			gpuUseField[i]->setText(gpuUseString);
			gpuTotalCoreField[i]->setText(totalCoresString);
			gpuTotalMemoryField[i]->setText(totalMemoryString);
			gpuTemperatureField[i]->setText(temperature);

			//Set all text field to readonly
			gpuTotalMemoryField[i]->setReadOnly(true);
			gpuTotalCoreField[i]->setReadOnly(true);
			gpuTemperatureField[i]->setReadOnly(true);
			gpuUseField[i]->setReadOnly(true);

			//Add label and text field tolayout
			horizontalTotalCores[i]->addWidget(gpuTotalCoreLabel[i]);
			horizontalTotalCores[i]->addWidget(gpuTotalCoreField[i]);

			horizontalGpuMemory[i]->addWidget(gpuTotalMemoryLabel[i]);
			horizontalGpuMemory[i]->addWidget(gpuTotalMemoryField[i]);

			horizontalGpuTemp[i]->addWidget(gpuTemperatureLabel[i]);
			horizontalGpuTemp[i]->addWidget(gpuTemperatureField[i]);

			horizontalGpuUse[i]->addWidget(gpuUseLabel[i]);
			horizontalGpuUse[i]->addWidget(gpuUseField[i]);

			verticalLayout[i]->addLayout(horizontalTotalCores[i]);
			verticalLayout[i]->addLayout(horizontalGpuMemory[i]);
			verticalLayout[i]->addLayout(horizontalGpuTemp[i]);
			verticalLayout[i]->addLayout(horizontalGpuUse[i]);

			toolbox->insertItem(i, gpuWidgets[i],  QString::fromStdString(modelNames[0]));
		}
	}

	toolbox->setCurrentIndex(currentToolBoxIndex);
}

void MainWindow::setCPU(int speed, int totalCores, int logicalCores, int Load, string name)
{
	QString modelName = QString::fromStdString(name);
	QString cpuSpeed = QString::number(speed);
	cpuSpeed.append(" MHz");

	//Check wich logo must be show
	if(modelName.contains("intel", Qt::CaseInsensitive))
	{
		QPixmap cpuLogo = QPixmap("logo/intel-logo.jpg");
		imageCPULabel->setPixmap(cpuLogo.scaledToWidth(50, Qt::SmoothTransformation));
	}
	else if(modelName.contains("amd", Qt::CaseInsensitive))
	{
		QPixmap cpuLogo = QPixmap("logo/AMD-Logo.jpg.jpg");
		imageCPULabel->setPixmap(cpuLogo.scaledToWidth(50, Qt::SmoothTransformation));
	}

	QString cores = QString::number(totalCores);
	QString logicalCoresString = QString::number(logicalCores);
	QString loadString= QString::number(Load);
	loadString.append(" %");

	//Set all data in textfields
	cpuNameField->setText(modelName);
	cpuSpeedField->setText(cpuSpeed);
	cpuTotalCoresField->setText(cores);
	cpuTotalLogicalField->setText(logicalCoresString);
	cpuLoadField->setText(loadString);
}

void MainWindow::createHdTab()
{
	hdTab = new QDialog();
}

void MainWindow::setHardDisk(vector<string> driveLetter,vector<string> label, vector<unsigned __int64> capacity, vector<unsigned __int64> freeSpace, vector<unsigned __int64> usedSpace)
{
	int currentToolBoxIndex= -1;

	//Get active toolbox
	if(HdToolbox != 0)
	{
		currentToolBoxIndex = HdToolbox->currentIndex();
	}

	deleteHdLayout();
	hdCount = capacity.size();

	HdToolbox = new QToolBox();	
	generalHdWidgeHorizontalLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	//Create logo and add logo to layout
	imageHdLabel = new QLabel();

	QPixmap HdLogo = QPixmap("logo/Hard-disk-logo.png");
	imageHdLabel->setPixmap(HdLogo.scaledToWidth(150, Qt::SmoothTransformation));
	generalHdWidgeHorizontalLayout->addWidget(HdToolbox);

	//Add layout to tab
	hdTab->setLayout(generalHdWidgeHorizontalLayout);

	generalHdWidgeHorizontalLayout->addWidget(imageHdLabel);

	for(int i =0; i < hdCount; i++)
	{
		//Create layouts
		verticalHdLayout.push_back(new QBoxLayout(QBoxLayout::TopToBottom));
		hdWidgets.push_back(new QWidget());

		hdWidgets[i]->setLayout(verticalHdLayout[i]);

		horizontalHdCapacity.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
		horizontalHdFreeSpace.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
		horizontalHdUsedSpace.push_back(new QBoxLayout(QBoxLayout::LeftToRight));

		//Create tect fields and add them to vector
		hdCapacityField.push_back(new QLineEdit());
		hdFreeSpaceField.push_back(new QLineEdit());
		hdUsedSpaceField.push_back(new QLineEdit());

		//create labels and add them to vector
		hdCapacityLabel.push_back(new QLabel(language->gettranslation("Capacity: ")));
		hdFreeSpaceLabel.push_back(new QLabel(language->gettranslation("Free space: ")));
		hdUsedLabelLabel.push_back(new QLabel(language->gettranslation("Used space: ")));

		label[i].append(" (");
		label[i].append(driveLetter[i]);
		label[i].append(")");

		QString capacityString = "";
		QString usedSpaceString = "";
		QString freeSpaceString = "";

		//Convert MB to GB or TB
		if(capacity[i]/1024/1024 > 1)
		{
			capacityString=QString::number((float)capacity[i]/1024/1024);
			capacityString.append(" TB");
		}

		else if(capacity[i]/1024 > 1)
		{
			capacityString=QString::number((float)capacity[i]/1024);
			capacityString.append(" GB");
		}

		else 
		{
			capacityString=QString::number(capacity[i]);
			capacityString.append(" MB");
		}

		//Convert MB to GB or TB
		if(usedSpace[i]/1024/1024 > 1)
		{
			usedSpaceString=QString::number((float)usedSpace[i]/1024/1024);
			usedSpaceString.append(" TB");
		}

		else if(usedSpace[i]/1024 > 1)
		{
			usedSpaceString=QString::number((float)usedSpace[i]/1024);
			usedSpaceString.append(" GB");
		}

		else 
		{
			usedSpaceString=QString::number(usedSpace[i]);
			usedSpaceString.append(" MB");
		}

		//Convert MB to GB or TB
		if(freeSpace[i]/1024/1024 > 1)
		{
			freeSpaceString=QString::number((float)freeSpace[i]/1024/1024);
			freeSpaceString.append(" TB");
		}

		else if(freeSpace[i]/1024 > 1)
		{
			freeSpaceString=QString::number((float)freeSpace[i]/1024);
			freeSpaceString.append(" GB");
		}

		else 
		{
			freeSpaceString=QString::number(freeSpace[i]);
			freeSpaceString.append(" MB");
		}

		//Set data to text field
		hdCapacityField[i]->setText(capacityString);
		hdFreeSpaceField[i]->setText(freeSpaceString);
		hdUsedSpaceField[i]->setText(usedSpaceString);

		//Set text field readonly
		hdCapacityField[i]->setReadOnly(true);
		hdFreeSpaceField[i]->setReadOnly(true);
		hdUsedSpaceField[i]->setReadOnly(true);

		//Add labels an text fields to layouts
		horizontalHdCapacity[i]->addWidget(hdCapacityLabel[i]);
		horizontalHdCapacity[i]->addWidget(hdCapacityField[i]);

		horizontalHdFreeSpace[i]->addWidget(hdFreeSpaceLabel[i]);
		horizontalHdFreeSpace[i]->addWidget(hdFreeSpaceField[i]);

		horizontalHdUsedSpace[i]->addWidget(hdUsedLabelLabel[i]);
		horizontalHdUsedSpace[i]->addWidget(hdUsedSpaceField[i]);

		//Add layouts to root layout
		verticalHdLayout[i]->addLayout(horizontalHdCapacity[i]);
		verticalHdLayout[i]->addLayout(horizontalHdFreeSpace[i]);
		verticalHdLayout[i]->addLayout(horizontalHdUsedSpace[i]);

		HdToolbox->insertItem(i, hdWidgets[i], QString::fromStdString(label[i]));
	}
	HdToolbox->setCurrentIndex(currentToolBoxIndex);
}

void MainWindow::deleteCPUTab()
{
	delete cpuNameLabel;
	delete cpuSpeedLabel;
	delete cpuTotalCoreLabel;
	delete cpuTotalLogicalLabel;
	delete cpuLoadLabel;

	delete cpuNameField;
	delete cpuSpeedField;
	delete cpuTotalCoresField;
	delete cpuTotalLogicalField;
	delete cpuLoadField;

	delete verticalCpuNameLayout;
	delete verticalCpuSpeedLayout;
	delete verticalCpuTotalCoresLayout;
	delete verticalCpuTotalLogicalLayout;
	delete verticalCpuLoadLayout;

	delete imageCPULabel;

	delete horizontalCpuLayout;
	delete verticalCpuTabLayout;
	delete CPUTab;

	CPUTab = 0;
}

void MainWindow::deleteHdLayout()
{
	for(int i=0; i < hdCount; i++)
	{
		delete horizontalHdCapacity[i];
		delete horizontalHdFreeSpace[i];
		delete horizontalHdUsedSpace[i];

		delete hdCapacityField[i];
		delete hdFreeSpaceField[i];
		delete hdUsedSpaceField[i];

		delete hdCapacityLabel[i];
		delete hdFreeSpaceLabel[i];
		delete hdUsedLabelLabel[i];

		delete verticalHdLayout[i];

		delete hdWidgets[i];

	}

	hdCount = 0;

	if(generalHdWidgeHorizontalLayout != 0)
	{
		delete generalHdWidgeHorizontalLayout;
		generalHdWidgeHorizontalLayout = 0;
	}

	if(HdToolbox != 0)
	{
		delete HdToolbox;
		HdToolbox = 0;
	}

	if(imageHdLabel != 0)
	{
		delete imageHdLabel;
		imageHdLabel = 0;
	}

	verticalHdLayout.clear();

	horizontalHdCapacity.clear();
	horizontalHdFreeSpace.clear();
	horizontalHdUsedSpace.clear();

	hdCapacityField.clear();
	hdFreeSpaceField.clear();
	hdUsedSpaceField.clear();

	hdWidgets.clear();

	hdCapacityLabel.clear();
	hdFreeSpaceLabel.clear();
	hdUsedLabelLabel.clear();

}

void MainWindow::setGPU(int totalAdapters, int * temperatures, int * load, int * totalMemory, int* maxFanSpeed, int * minFanSpeed, int * fanSpeed, int * ClockSpeed, int* memoryClock, float * voltage, string * modelName, string * memoryType)
{
	int currentToolBoxIndex= -1;

	if(toolbox != 0)
	{
		currentToolBoxIndex = toolbox->currentIndex();
	}

	this->gpuCount = totalAdapters;

	if(gpuCount != 0)
	{
		if(imageGPULabel != 0)
		{
			deleteGPUAMDLayout();
		}

		//Create logo
		QString modelNameLogo = QString::fromStdString(modelName[0]);
		imageGPULabel = new QLabel();

		QPixmap cpuLogo = QPixmap("logo/AMD-GPU-logo.jpg");
		imageGPULabel->setPixmap(cpuLogo.scaledToWidth(50, Qt::SmoothTransformation));

		//Create toolbox and layout
		toolbox = new QToolBox();
		generalGpuWidgetHorizontalLayout = new QBoxLayout(QBoxLayout::LeftToRight);

		//Add toolbox and image to layoout
		generalGpuWidgetHorizontalLayout->addWidget(toolbox);
		generalGpuWidgetHorizontalLayout->addWidget(imageGPULabel);

		//Add root layout to tab
		GPUTab->setLayout(generalGpuWidgetHorizontalLayout);

		for(int i =0; i < gpuCount; i++)
		{
			verticalLayout.push_back(new QBoxLayout(QBoxLayout::TopToBottom));
			gpuWidgets.push_back(new QWidget());

			gpuWidgets[i]->setLayout(verticalLayout[i]);

			//Create layout and add them to vector
			horizontalGpuMemory.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuTemp.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuUse.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuMaxFanSpeed.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuMinFanSpeed.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuFanSpeed.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuClockSpeed.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuMemoryClock.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuVoltage.push_back(new QBoxLayout(QBoxLayout::LeftToRight));
			horizontalGpuMemoryType.push_back(new QBoxLayout(QBoxLayout::LeftToRight));

			//Create text fields
			gpuTotalMemoryField.push_back(new QLineEdit());
			gpuTemperatureField.push_back(new QLineEdit());
			gpuUseField.push_back(new QLineEdit());
			gpuMaxFanSpeedField.push_back(new QLineEdit());
			gpuMinFanSpeedField.push_back(new QLineEdit());
			gpuFanSpeedField.push_back(new QLineEdit());
			gpuClockSpeedField.push_back(new QLineEdit());
			gpuMemoryClockField.push_back(new QLineEdit());
			gpuVoltageField.push_back(new QLineEdit());
			gpuMemoryTypeField.push_back(new QLineEdit());

			//Create label
			gpuTotalMemoryLabel.push_back(new QLabel(language->gettranslation("Total memory: ")));
			gpuTemperatureLabel.push_back(new QLabel(language->gettranslation("Temperature: ")));
			gpuUseLabel.push_back(new QLabel(language->gettranslation("Load: ")));
			gpuMaxFanSpeedLabel.push_back(new QLabel(language->gettranslation("Maximum fan speed: ")));
			gpuMinFanSpeedLabel.push_back(new QLabel(language->gettranslation("Minimum fan speed: ")));
			gpuFanSpeedLabel.push_back(new QLabel(language->gettranslation("Currect fan speed: ")));
			gpuClockSpeedLabel.push_back(new QLabel(language->gettranslation("Clock speed: ")));
			gpuMemoryClockLabel.push_back(new QLabel(language->gettranslation("Memory speed: ")));
			gpuVoltageLabel.push_back(new QLabel(language->gettranslation("Voltage: ")));
			gpuMemoryTypeLabel.push_back(new QLabel(language->gettranslation("Memory type: ")));

			QString totalMemoryString =QString::number(totalMemory[i]);
			QString gpuUseString=QString::number(gpuCount);
			totalMemoryString.append(" MB");
			QString temperature= "";

			//Change temperature format
			if(!settings->getDegreeCelsius())
			{
				temperatures[i] = (9/5)*temperatures[i]+32;
				temperature = QString::number(temperatures[i]);
				temperature.append(" °F");
			}
			else
			{
				temperature = QString::number(temperatures[i]);
				temperature.append(" °C");
			}

			//Add strings to data
			gpuUseString.append(" %");

			QString maxFanSpeedString = QString::number(maxFanSpeed[i]);
			maxFanSpeedString.append(language->gettranslation(" RPM"));

			QString minFanSpeedString = QString::number(minFanSpeed[i]);
			minFanSpeedString.append(language->gettranslation(" RPM"));

			QString FanSpeedString = QString::number(fanSpeed[i]);
			FanSpeedString.append(language->gettranslation(" RPM"));

			QString clockSpeedString = QString::number(ClockSpeed[i]);
			clockSpeedString.append(language->gettranslation(" MHz"));

			QString memoryClockSpeedString = QString::number(memoryClock[i]);
			memoryClockSpeedString.append(language->gettranslation(" MHz"));

			QString voltageString = QString::number(voltage[i]);
			voltageString.append(language->gettranslation(" V"));

			QString memoryTypeString = QString::fromStdString(memoryType[i]);

			//Set data to text fields
			gpuUseField[i]->setText(gpuUseString);
			gpuTotalMemoryField[i]->setText(totalMemoryString);
			gpuTemperatureField[i]->setText(temperature);
			gpuMaxFanSpeedField[i]->setText(maxFanSpeedString);
			gpuMinFanSpeedField[i]->setText(minFanSpeedString);
			gpuFanSpeedField[i]->setText(FanSpeedString);
			gpuClockSpeedField[i]->setText(clockSpeedString);
			gpuMemoryClockField[i]->setText(memoryClockSpeedString);
			gpuVoltageField[i]->setText(voltageString);
			gpuMemoryTypeField[i]->setText(memoryTypeString);

			//Set text field to readonly
			gpuTotalMemoryField[i]->setReadOnly(true);
			gpuTemperatureField[i]->setReadOnly(true);
			gpuUseField[i]->setReadOnly(true);
			gpuMaxFanSpeedField[i]->setReadOnly(true);
			gpuMinFanSpeedField[i]->setReadOnly(true);
			gpuFanSpeedField[i]->setReadOnly(true);
			gpuClockSpeedField[i]->setReadOnly(true);
			gpuMemoryClockField[i]->setReadOnly(true);
			gpuVoltageField[i]->setReadOnly(true);
			gpuMemoryTypeField[i]->setReadOnly(true);

			//Add labels and fields to layouts
			horizontalGpuMemory[i]->addWidget(gpuTotalMemoryLabel[i]);
			horizontalGpuMemory[i]->addWidget(gpuTotalMemoryField[i]);

			horizontalGpuTemp[i]->addWidget(gpuTemperatureLabel[i]);
			horizontalGpuTemp[i]->addWidget(gpuTemperatureField[i]);

			horizontalGpuUse[i]->addWidget(gpuUseLabel[i]);
			horizontalGpuUse[i]->addWidget(gpuUseField[i]);

			horizontalGpuMaxFanSpeed[i]->addWidget(gpuMaxFanSpeedLabel[i]);
			horizontalGpuMaxFanSpeed[i]->addWidget(gpuMaxFanSpeedField[i]);

			horizontalGpuMinFanSpeed[i]->addWidget(gpuMinFanSpeedLabel[i]);
			horizontalGpuMinFanSpeed[i]->addWidget(gpuMinFanSpeedField[i]);

			horizontalGpuFanSpeed[i]->addWidget(gpuFanSpeedLabel[i]);
			horizontalGpuFanSpeed[i]->addWidget(gpuFanSpeedField[i]);

			horizontalGpuClockSpeed[i]->addWidget(gpuClockSpeedLabel[i]);
			horizontalGpuClockSpeed[i]->addWidget(gpuClockSpeedField[i]);

			horizontalGpuMemoryClock[i]->addWidget(gpuMemoryClockLabel[i]);
			horizontalGpuMemoryClock[i]->addWidget(gpuMemoryClockField[i]);

			horizontalGpuVoltage[i]->addWidget(gpuVoltageLabel[i]);
			horizontalGpuVoltage[i]->addWidget(gpuVoltageField[i]);

			horizontalGpuMemoryType[i]->addWidget(gpuMemoryTypeLabel[i]);
			horizontalGpuMemoryType[i]->addWidget(gpuMemoryTypeField[i]);

			//Add layouts to root layout
			verticalLayout[i]->addLayout(horizontalGpuTemp[i]);
			verticalLayout[i]->addLayout(horizontalGpuUse[i]);
			verticalLayout[i]->addLayout(horizontalGpuMemory[i]);
			verticalLayout[i]->addLayout(horizontalGpuMemoryType[i]);
			verticalLayout[i]->addLayout(horizontalGpuMemoryClock[i]);
			verticalLayout[i]->addLayout(horizontalGpuClockSpeed[i]);
			verticalLayout[i]->addLayout(horizontalGpuFanSpeed[i]);
			verticalLayout[i]->addLayout(horizontalGpuMinFanSpeed[i]);
			verticalLayout[i]->addLayout(horizontalGpuMaxFanSpeed[i]);			
			verticalLayout[i]->addLayout(horizontalGpuVoltage[i]);

			toolbox->insertItem(i, gpuWidgets[i],  QString::fromStdString(modelName[i]));
		}
	}

	toolbox->setCurrentIndex(currentToolBoxIndex);
}

void MainWindow::deleteHdTab()
{
	if(hdTab != 0)
	{
		delete hdTab;
		hdTab = 0;
	}
}

void MainWindow::deleteGPUAMDLayout()
{
	for(int i =0; i < gpuCount; i++)
	{
		delete gpuTotalMemoryField[i];
		gpuTotalMemoryField[i] = 0;
		delete gpuTemperatureField[i];
		delete gpuUseField[i];
		delete gpuMaxFanSpeedField[i];
		delete gpuMinFanSpeedField[i];
		delete gpuFanSpeedField[i];
		delete gpuClockSpeedField[i];
		delete gpuMemoryClockField[i];
		delete gpuVoltageField[i];
		delete gpuMemoryTypeField[i];

		delete gpuTotalMemoryLabel[i];
		delete gpuTemperatureLabel[i];
		delete gpuUseLabel[i];
		delete gpuMaxFanSpeedLabel[i];
		delete gpuMinFanSpeedLabel[i];
		delete gpuFanSpeedLabel[i];
		delete gpuClockSpeedLabel[i];
		delete gpuMemoryClockLabel[i];
		delete gpuVoltageLabel[i];
		delete gpuMemoryTypeLabel[i];

		delete horizontalGpuMemory[i];
		delete horizontalGpuTemp[i];
		delete horizontalGpuUse[i];
		delete horizontalGpuMaxFanSpeed[i];
		delete horizontalGpuMinFanSpeed[i];
		delete horizontalGpuFanSpeed[i];
		delete horizontalGpuClockSpeed[i];
		delete horizontalGpuMemoryClock[i];
		delete horizontalGpuVoltage[i];
		delete horizontalGpuMemoryType[i];

		delete verticalLayout[i];
		delete gpuWidgets[i];
	}
	delete generalGpuWidgetHorizontalLayout;
	delete imageGPULabel;
	delete toolbox;

	generalGpuWidgetHorizontalLayout = 0;
	imageGPULabel = 0;
	toolbox = 0;

	gpuTotalMemoryField.clear();
	gpuTemperatureField.clear();
	gpuUseField.clear();
	gpuMaxFanSpeedField.clear();
	gpuMinFanSpeedField.clear();
	gpuFanSpeedField.clear();
	gpuClockSpeedField.clear();
	gpuMemoryClockField.clear();
	gpuVoltageField.clear();
	gpuMemoryTypeField.clear();

	gpuTotalMemoryLabel.clear();
	gpuTemperatureLabel.clear();
	gpuUseLabel.clear();
	gpuMaxFanSpeedLabel.clear();
	gpuMinFanSpeedLabel.clear();
	gpuFanSpeedLabel.clear();
	gpuClockSpeedLabel.clear();
	gpuMemoryClockLabel.clear();
	gpuVoltageLabel.clear();
	gpuMemoryTypeLabel.clear();

	horizontalGpuMemory.clear();
	horizontalGpuTemp.clear();
	horizontalGpuUse.clear();
	horizontalGpuMaxFanSpeed.clear();
	horizontalGpuMinFanSpeed.clear();
	horizontalGpuFanSpeed.clear();
	horizontalGpuClockSpeed.clear();
	horizontalGpuMemoryClock.clear();
	horizontalGpuVoltage.clear();
	horizontalGpuMemoryType.clear();

	verticalLayout.clear();
	gpuWidgets.clear();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	//Close to system tray or close thermos
	if(settings->getCloseToSystemtray())
	{
		QApplication::exit(1);
	}
	else
	{
		QApplication::exit(0);
	}
}