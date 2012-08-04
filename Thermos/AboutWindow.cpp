//-----------------------------------------------------------------
// Update Object
// C++ Source - Update.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "AboutWindow.h"

//-----------------------------------------------------------------
// Update methods
//-----------------------------------------------------------------
AboutWindow::AboutWindow(Language * lan)
{
	language = lan;

	//Change closeBar to only close button
	setWindowModality(Qt::ApplicationModal);
	setWindowFlags(Qt::WindowTitleHint);

	//Change window title
	setWindowTitle(language->gettranslation("About Thermos"));

	//set window size
	setFixedSize(400, 200);
	creatWindowLayout();

	setAttribute( Qt::WA_DeleteOnClose, true );
	setWindowIcon(QIcon("logo/thermos.png"));
	this->show();
}

AboutWindow::~AboutWindow()
{
	deleteWindowLayout();
}

void AboutWindow::creatWindowLayout()
{
	//make text for about window
	QString aboutText = language->gettranslation("Programmed by:");
	aboutText.append(" Jimmy Dom\n");
	aboutText.append(language->gettranslation("Version:"));
	aboutText.append(" " + BUILD + "\n\n");
	aboutText.append("Copyright(c) 2011.\n");
	aboutText.append("Jimmy Dom. ");
	aboutText.append(language->gettranslation("All Rights Reserved."));
	aboutText.append("\n\n");
	aboutText.append(language->gettranslation("All other trademarks are the property of their respective owners."));

	//Create close button
	closeButton = new QPushButton(language->gettranslation("Close"));
	closeButton->setFixedSize(100,25);

	//Connect action to closeWindow slot
	connect(closeButton, SIGNAL(clicked()), this , SLOT(closeWindow()));
	text = new QLabel(aboutText);
	text->setWordWrap(true);

	//add image to window
	image = new QLabel();
	pixmap = new QPixmap("logo/thermos.png");
	image->setPixmap(pixmap->scaledToHeight(150, Qt::SmoothTransformation));

	//Create layouts
	horizontalLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	verticalLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	horizontalLayout->addLayout(verticalLayout);

	//Add text, button and image to layout
	verticalLayout->addWidget(image);
	verticalLayout->addWidget(text);
	horizontalLayout->addWidget(closeButton, 0, Qt::AlignHCenter);

	this->setLayout(horizontalLayout);
}

void AboutWindow::deleteWindowLayout()
{
	delete closeButton;
	delete text;
	delete image;
	delete pixmap;
	delete verticalLayout;
	delete horizontalLayout;
}

void AboutWindow::closeWindow()
{
	//Close the current window
	this->close();
	delete this;
}