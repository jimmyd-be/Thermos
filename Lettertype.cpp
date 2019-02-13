#include "Lettertype.h"


Lettertype::Lettertype(void)
{
}


Lettertype::~Lettertype(void)
{
}


QStringList Lettertype::getFonts()
{
	QFontDatabase font;
	return font.families(QFontDatabase::Any);
}


bool Lettertype::fontAvailable(QString name)
{
	QFontDatabase font;
	QStringList fonts =  font.families(QFontDatabase::Any);
	bool available = false;

	for(int i=0 ; i < fonts.size(); i++)
	{
		if(fonts.at(i) == name)
		{
			available = true;
			break;
		}
	}

	return available;
}