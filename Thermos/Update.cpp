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
Update::Update(void)
{
	url = "";
}

Update::~Update(void)
{
	//nothing to destroy
}

QString* Update::checkUpdate()
{
	QDir::setCurrent(QDir::tempPath());
	QString * returnvalue = new QString[2] ;
	QString url = SERVER + "update.xml";
	QFile * file = this->downloadUpdate(url);

	QDomDocument doc( "xml" );

	//Open file in read only mode
	if(file->open(QIODevice::ReadOnly))
	{
		if(!doc.setContent(file))
		{
			file->close();
		}
		file->close();

		QDomElement root = doc.documentElement();
		QDomNode n = root.firstChild();
		QString parentTag = "";

		//Loop to end of file
		while(!n.isNull())
		{
			QDomElement e = n.toElement();

			//Check XML dom
			if(!e.isNull())
			{
				if(e.tagName() == "version")
				{
					parentTag = "version";
					returnvalue[0] = QString(e.text());
				}

				else if(e.tagName() == "windows" && parentTag == "version")
				{
					parentTag = "windows";
					this->url = e.text();
				}

				else if(e.tagName() == "x32")
				{
					if((OS == "windowsx32" && parentTag == "windows") || (OS == "linuxx32" && parentTag == "linux"))
					{
						returnvalue[1] = QString(e.text());
					}
				}

				else if(e.tagName() == "x64")
				{
					if((OS == "linuxx64" && parentTag == "windows") || (OS == "linuxx64" && parentTag == "linux"))
					{
						returnvalue[1] = QString(e.text());
					}
				}
			}
			n = n.nextSibling();
		}
	}

	file->remove(file->fileName());
	QDir::setCurrent(QCoreApplication::applicationDirPath());
	delete file;
	return returnvalue;
}

QString Update::getUrl()
{
	return this->url;
}

QFile * Update::downloadUpdate(QString urlString)
{
	QDir::setCurrent(QDir::tempPath());

	QUrl url(urlString);
	QFileInfo fileInfo(url.path());
	QString fileName=fileInfo.fileName();
	QFile * file = new QFile(fileName);
	file->setFileName(fileName);

	//If file is not open delete file
	if (!file->open(QIODevice::WriteOnly))
	{
		delete file;
		file = 0;
	}

	//Open connection to server and download file
	else
	{
		QHttp http;
		QEventLoop loop;
		//	QObject::connect(&http, SIGNAL(done(bool)), &loop, SLOT(quit()));

		http.setHost(url.host(), url.port(80));
		http.get(url.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority), file);

		loop.exec();
		file->close();
	}

	QDir::setCurrent(QCoreApplication::applicationDirPath());
	return file;
}

//If update is succeed remove previous Thermos version
void Update::removePreviousVersion(QString argument)
{
	argument.remove("\"");

	QDir directory(argument);
	QStringList filesList = directory.entryList(QDir::Files);
	QString fileName;

	//Delete all files in Thermos directory
	for(int i =0; i < filesList.size(); i++)
	{
		fileName = filesList.at(i);
		directory.remove(fileName);

	}

	directory = argument.append("/languages/");
	filesList = directory.entryList(QDir::Files);

	//Delete all files in language directory
	for(int i =0; i < filesList.size(); i++)
	{  
		fileName = filesList.at(i);
		directory.remove(fileName);

	}

	directory = argument.remove("/languages/");
	directory = argument.append("/logs/");
	filesList = directory.entryList(QDir::Files);

	//Delete all files in Logs directory
	for(int i =0; i < filesList.size(); i++)
	{  
		fileName = filesList.at(i);
		directory.remove(fileName);

	}

	directory = argument.append("template/");
	filesList = directory.entryList(QDir::Files);

	//Delete all files in Template directory
	for(int i =0; i < filesList.size(); i++)
	{  
		fileName = filesList.at(i);
		directory.remove(fileName);

	}

	//Remove directory's
	directory = argument.remove("template/");
	directory.rmdir("template");
	directory = argument.remove("/logs/");

	directory = argument.append("/logo/");
	QString test = directory.absolutePath();
	filesList = directory.entryList(QDir::Files);

	//delete all files in logo folder
	for(int i =0; i < filesList.size(); i++)
	{
		fileName = filesList.at(i);
		directory.remove(fileName);
	}

	directory = argument.remove("/logo/");
	directory = argument;
	filesList = directory.entryList(QDir::Dirs);

	//Remove all directory's in Thermos folder
	for(int i =0; i < filesList.size(); i++)
	{
		fileName = filesList.at(i);
		directory.rmdir(fileName);
	}

	directory.cdUp();

	QStringList list = argument.split("/");
	QString lastDirName = "";

	//Delete previous thermos folder
	if(list.size() > 0)
	{
		lastDirName = list.at(list.size()-1);
		directory.rmdir(lastDirName);
	}
}