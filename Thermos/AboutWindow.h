//-----------------------------------------------------------------
// AboutWindow Object
// C++ Header - AboutWindow.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Defines.h"
#include "Language.h"

//-----------------------------------------------------------------
// AboutWindow Class
//-----------------------------------------------------------------
class AboutWindow :	public QDialog
{
	Q_OBJECT

public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	AboutWindow(Language *);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~AboutWindow();

	//---------------------------
	// General Methods
	//---------------------------

private slots:
	//---------------------------
	// Private slots Methods
	//---------------------------
	void closeWindow();

private:
	//---------------------------
	// Private Methods
	//---------------------------
	void creatWindowLayout();
	void deleteWindowLayout();

	//--------------------------
	// Datamembers
	//--------------------------
	QPushButton *closeButton;
	QLabel * text;
	QLabel * image;
	QBoxLayout *horizontalLayout;
	QBoxLayout *verticalLayout;
	QPixmap * pixmap;
	Language * language;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	AboutWindow(const AboutWindow& t);
	AboutWindow& operator=(const AboutWindow& t);
};