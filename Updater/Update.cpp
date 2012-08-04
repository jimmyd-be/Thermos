//-----------------------------------------------------------------
// Update Object
// C++ Source - Update.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Update.h"

//-----------------------------------------------------------------
// Update methods
//-----------------------------------------------------------------
Update::Update(QString url)
{
	this->url = url;
}

Update::~Update(void)
{
	delete file;
}


QString Update::getUrl()
{
	return this->url;
}


QFile * Update::downloadUpdate(QString urlString)
{
	QDir * dir = new QDir(QCoreApplication::applicationDirPath());
	bool succes = dir->cdUp();
	QDir::setCurrent(dir->absolutePath());

	QDir().mkdir(this->dirName);

	QDir::setCurrent(dir->absolutePath().append("/").append(this->dirName));

	//If string found languages then set current dir to language
	if(urlString.indexOf("languages") != -1)
	{
		if(QDir().exists("languages"))
		{
			QDir::setCurrent(dir->absolutePath().append("/").append(this->dirName).append("/languages"));
		}
		else
		{
			//Create folder if not exist
			QDir().mkdir("languages");
			QDir::setCurrent(dir->absolutePath().append("/").append(this->dirName).append("/languages"));
		}
	}

	//If string found logo then set current dir to language
	else if(urlString.indexOf("logo") != -1)
	{
		if(QDir().exists("logo"))
		{
			QDir::setCurrent(dir->absolutePath().append("/").append(this->dirName).append("/logo"));
		}
		else
		{
			QDir().mkdir("logo");
			QDir::setCurrent(dir->absolutePath().append("/").append(this->dirName).append("/logo"));
		}
	}


	QUrl url (urlString);
	QFileInfo fileInfo(url.path());
	QString fileName=fileInfo.fileName();

	if (QFile::exists(fileName))
	{
		QFile::remove(fileName);
	}

	QFile * file = new QFile(fileName);
	file->setFileName(fileName);

	if (!file->open(QIODevice::WriteOnly))
	{
		delete file;
		file = 0;
	}

	else
	{
		//Start download and open connection
		QHttp http;
		QEventLoop loop;
		QFile file;
		QObject::connect(&http, SIGNAL(done(bool)), &loop, SLOT(quit()));

		file.setFileName(fileName);
		file.open(QIODevice::WriteOnly);

		http.setHost(url.host(), url.port(80));
		http.get(url.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority), 
			&file);

		loop.exec();

	}

	if(file->isOpen())
	{
		file->close();
	}

	QDir::setCurrent(dir->absolutePath().append("/").append(this->dirName));

	QFile::remove(dir->absolutePath().append("/").append(this->dirName).append(".txt"));
	delete dir;

	return file;
}


void Update::readFile()
{
	dirName = file->fileName();

	if(dirName.endsWith(".txt"))
	{
		dirName.remove(".txt");
	}

	//Open file
	if (file->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream stream(file);

		//read all lines of the file
		while (!stream.atEnd()) {
			files.push_back(stream.readLine());
		}

		file->close();
	}
}


void Update::run()
{
	file = downloadUpdate(this->url);
	readFile();

	//Download all founded files
	for(int i =0; i < files.size(); i++)
	{
		file = downloadUpdate(files[i]);
	}

	copySettingFile();
	copyLogFiles();

	//Start new Thermos proccess and kill this progam
	QDir::setCurrent(QCoreApplication::applicationDirPath());

	QProcess * proces= new QProcess();
	QDir * dir = new QDir(QCoreApplication::applicationDirPath());

	QStringList arguments;
	bool succes = false;

	dir->cdUp();
	arguments.append(QCoreApplication::applicationDirPath());

	QDir::setCurrent(dir->absolutePath().append("/").append(dirName));
	QString test = dir->absolutePath().append("/").append(dirName).append("/Thermos.exe");
	succes = proces->startDetached(test, arguments);

	//If new thermos will not start£. Start old thermos and set argument to updateFail
	if(!succes)
	{
		QDir::setCurrent(QCoreApplication::applicationDirPath());
		QStringList arguments;
		arguments.append("updateFail");
		QString process = QCoreApplication::applicationDirPath().append("/Thermos.exe");
		proces->startDetached(process, arguments);
	}

	delete proces;
	delete dir;

	QApplication::exit(0);
}

void Update::copyLogFiles()
{
	//Copy log files from previous Thermos folder to new Thermos folder
	QDir::setCurrent(QCoreApplication::applicationDirPath());

	QDir dir (QCoreApplication::applicationDirPath().append("/logs/"));

	//get all log file
	QStringList logFileList = dir.entryList(QDir::Files);

	//
	for(int i =0; i < logFileList.size(); i++)
	{
		QFile logFile (dir.absolutePath().append("/").append(logFileList.at(i)));


		QDir dir (QCoreApplication::applicationDirPath());

		//Go to destination folder
		dir.cdUp();
		QString  destination= dir.absolutePath().append("/").append(dirName).append("/logs/").append(logFileList.at(i));

		QDir destinationDir (destination.remove(logFileList.at(i)).remove("/logs/"));

		//if folder not exist create one
		if(!destinationDir.exists("logs"))
		{
			destinationDir.mkdir("logs");
		}

		//Copy files to new destination
		logFile.copy(destination.append("/logs/").append(logFileList.at(i)));
	}
}

void Update::copySettingFile()
{
	//Copy settings file from previous Thermos folder to new Thermos folder
	QDir::setCurrent(QCoreApplication::applicationDirPath());

	//Create file
	QFile * settingsFile = new QFile("Thermos.ini");

	//Go to directory of new folder
	QDir * dir = new QDir(QCoreApplication::applicationDirPath());
	dir->cdUp();

	QString test = dir->absolutePath().append("/").append(dirName).append("/Thermos.ini");

	//Copyt file to new destination
	settingsFile->copy(dir->absolutePath().append("/").append(dirName).append("/Thermos.ini"));

	delete dir;
	delete settingsFile;
}
