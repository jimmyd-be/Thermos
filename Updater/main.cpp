#include "SystemTray.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	//get all arguments
	QApplication a(argc, argv);
	QString argument = "";
	QStringList argumentList = QApplication::arguments();

	//Loop all arguments
	for(int i=0; i< argumentList.size();i++)
	{
		//if the argument is a link
		if(argumentList.at(i).contains("http://"))
		{
			argument = argumentList.at(i);
		}
	}

	Update * update;

	if(argument.size() != 0)
	{
		//Open system tray and start update
		int returnValue = -1;
		SystemTray * tray = new SystemTray();

		tray->show();

		update = new Update(argument);

		update->start();

		//Main loop
		while((returnValue = a.exec() && returnValue != 0) && !update->isFinished())
		{

		}

		if(!update->isFinished())
		{
			update->terminate();
		}


		delete update;

		tray->hide();
		delete tray;
		tray = 0;
	}
}
