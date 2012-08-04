//-----------------------------------------------------------------
// Language Object
// C++ Source - Language.cpp - version v1.0 (2012-04-30)
// Thermos v1.0.0
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Language.h"

//-----------------------------------------------------------------
// Update methods
//-----------------------------------------------------------------
Language::Language(QString languageName)
{
	//Make 2 dimension dynamic array
	sizeList = 10;
	translations = new QString * [sizeList];

	for(int i = 0; i < sizeList; i++)
	{
		translations[i] = new QString[2];
	}

	readTranslationFile(findLanguadeFile(languageName));
}

Language::~Language(void)
{
	//Delete 2 dimension dynamic array
	for( int i = 0 ; i < sizeList ; i++ )
	{
		delete [] translations[i] ;
	}

	delete [] translations ;
}

QString Language::gettranslation(QString word)
{
	//Search in array to correct translateion
	for(int i = 0; i < sizeList; i++)
	{
		//If translation is found return it
		if(translations[i][0] == word)
		{
			return translations[i][1];
		}
	}

	//If no translation is found return word
	return word;
}

QString Language::findLanguadeFile(QString languageName)
{
	vector<QString> files = getLanguageFiles();
	QString line ="";

	//Check every language file
	for (int i=0; i<files.size(); i++) {

		QFile* file = new QFile("languages/" + files[i]);

		//Open language file
		file->open(QIODevice::ReadOnly | QIODevice::Text);

		//Open readstream
		QTextStream * input = new QTextStream(file);
		line = input->readLine();

		//If file is opened and stream is not at last line
		while(!line.isNull() && file->isOpen())
		{
			//get language line in file
			if(line.startsWith("\"Language:") && line.endsWith("\\n\""))
			{
				line.remove("\"Language:");
				line.remove("\\n\"");
				line.remove(" ");

				//If file == languageName
				if(line == languageName)
				{
					file->close();
					delete file;
					delete input;
					//return founded file
					return files[i];
				}
			}
			line = input->readLine();
		}

		file->close();
		delete input;
		delete file;
	}

	//If no file is found return empty string
	return "";
}

bool Language::checkLanguageFile(QString fileName)
{
	bool projectControle = false;
	bool languageControle = false;
	bool wordControle = true;

	QString previous = "msgstr";

	QFile* file = new QFile("languages/" + fileName);
	QString line = "";

	//Open languagefile and stream
	file->open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream * input = new QTextStream(file);

	if(file->isOpen())
	{
		line = input->readLine();

		//read every line in the file
		while(!line.isNull() && wordControle)
		{
			//Check if project set to Thermos
			if((line.contains("Project-Id-Version:") && line.contains("Thermos")))
			{
				projectControle = true;				
			}

			if(projectControle)
			{
				//Check if good language is set
				if(line.startsWith("\"Language:") && line.contains("\\n\""))
				{
					line.remove("\"Language: ");
					line.remove("\\n\"");

					if(line.size() != 2)
					{
						languageControle = true;
					}

				}

				else if(languageControle)
				{
					//File has msgid for word that must be translated
					if(line.startsWith("msgid"))
					{
						if(previous == "msgstr")
						{
							if(line.count("\"") != 2)
							{
								wordControle = false;
							}
							previous = "msgid";

						}
						else
						{
							wordControle = false;
						}
					}

					//translation of word must start with msgstr
					else if(line.startsWith("msgstr"))
					{
						if(previous == "msgid")
						{
							if(line.count("\"") != 2)
							{
								wordControle = false;
							}
							previous = "msgstr";
						}
						else
						{
							wordControle = false;
						}
					}
				}
			}
			line = input->readLine();
		}}

	file->close();
	delete file;
	delete input;

	//If file is corrupt or not a translation file return false
	if(!(projectControle && languageControle && wordControle))
	{
		return false;
	}
	else
	{
		return true;
	}
}

vector<QString> Language::getLanguageFiles()
{
	vector<QString> files;
	QDir directory(QDir::currentPath() + "/languages");

	if(directory.exists())
	{
		//Get all files in the languages directory
		directory.setFilter(QDir::Files);
		QFileInfoList list = directory.entryInfoList();

		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);

			//If file is a correct translation file add them to the vector
			if((fileInfo.fileName().endsWith(".po")) && checkLanguageFile(fileInfo.fileName()))
			{
				files.push_back(fileInfo.fileName());
			}
		}
	}
	return files;

}

vector<QString> Language::getLanguageNames()
{
	vector<QString> languages;
	vector<QString> files = getLanguageFiles();
	QString line ="";

	for (int i=0; i<files.size(); i++)
	{
		QFile* file = new QFile("languages/" + files[i]);

		//Open file for reading
		file->open(QIODevice::ReadOnly | QIODevice::Text);

		QTextStream * input = new QTextStream(file);
		line = input->readLine();

		//read every line of file
		while(!line.isNull() && file->isOpen())
		{
			// If language is set in the file push the data to the vector
			if(line.startsWith("\"Language:") && line.endsWith("\\n\""))
			{
				line.remove("\"Language:");
				line.remove("\\n\"");
				line.remove(" ");
				languages.push_back(line);
			}
			line = input->readLine();
		}

		file->close();
		delete file;
		delete input;
	}

	//English don't has a translation file but it's a settings chose
	languages.push_back("English");
	return languages;

}

bool Language::readTranslationFile(QString filename)
{
	if(checkLanguageFile(filename))
	{
		//Open file and stream
		QFile* file = new QFile("languages/" + filename);
		QString line ="";
		int wordsFind = 0;
		file->open(QIODevice::ReadOnly | QIODevice::Text);

		QTextStream * input = new QTextStream(file);

		if(file->isOpen())
		{
			line = input->readLine();

			//read all lines of the opened file
			while(!line.isNull())
			{
				if(line.startsWith("msgid"))
				{
					//If array is to small resize it
					if(sizeList-1 == wordsFind)
					{
						resize();
					}
					translations[wordsFind][0] = line.section("\"",1, 1);
				}

				else if(line.startsWith("msgstr"))
				{
					translations[wordsFind][1] = line.section("\"",1, 1);
					wordsFind++;
				}

				line = input->readLine();
			}
		}
		else
		{
			file->close();
			delete input;
			return false;
		}

		file->close();
		delete input;
		delete file;
	}

	return true;
}

void Language::changeLanguage(QString languageName)
{
	//Delete translationarray
	sizeList = 10;

	for(int i = 0; i < sizeList; i++)
	{
		delete[] translations[i];
	}

	delete[] translations;

	//Create new array for new translation
	translations = new QString * [sizeList];

	for(int i = 0; i < sizeList; i++)
	{
		translations[i] = new QString[2];
	}

	readTranslationFile(findLanguadeFile(languageName));
}

void Language::resize()
{
	int previous = sizeList;
	sizeList += 5;

	//Set data to new helparray
	QString ** helpArray = translations;

	//make new array with more space
	translations = new QString * [sizeList];

	//Set all data from helpArray to new array
	for(int i = 0; i < sizeList; i++)
	{
		translations[i] = new QString[2];

		if(i < previous)
		{

			for(int j =0; j < 2; j++)
			{
				translations[i][j] = helpArray[i][j];
			}
			delete [] helpArray[i];
		}
	}

	delete [] helpArray;
}