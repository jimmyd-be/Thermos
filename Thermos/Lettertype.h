#pragma once


#include "Defines.h"
 #include <QDesktopServices>
#include <QFontDatabase>

class Lettertype
{
public:
	Lettertype(void);
	virtual ~Lettertype(void);

	QStringList getFonts();
	bool fontAvailable(QString);
};

