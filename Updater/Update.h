//-----------------------------------------------------------------
// Update Object
// C++ Header - Update.h - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QHttp>
#include <QEventLoop>
#include <QCoreApplication>
#include <QThread>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QString>
#include <vector>
#include <QTextStream>
#include <Qdir>
#include <QProcess>
#include <QApplication>

using namespace std;

//-----------------------------------------------------------------
// Update Class
//-----------------------------------------------------------------
class Update: public QThread
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Update(QString);

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Update(void);

	//---------------------------
	// General Methods
	//---------------------------
	QString getUrl();
	QString dirName;
	void run();

private:
	//---------------------------
	// Private Methods
	//---------------------------
	QFile * downloadUpdate(QString);
	void readFile();
	void copySettingFile();
	void copyLogFiles();

	//--------------------------
	// Datamembers
	//--------------------------
	QFile * file;
	QString url;
	vector<QString> files;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Update(const Update& t);
	Update& operator=(const Update& t);
};