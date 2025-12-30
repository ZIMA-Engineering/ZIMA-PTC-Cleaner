/***************************************************************************
 *   Copyright (C) 2008 by Peter Holák                                     *
 *   forrondur@gmail.com                                                   *
 *                                                                         *
 *   (c) 2011 Jakub Skokan <aither@havefun.cz>                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, get it at                            *
 *   http://www.gnu.org/licenses/gpl2.html                                 *
 ***************************************************************************/


#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDir>
#include "zima-ptc-cleaner.h"

int main( int argc, char ** argv ) {
	QApplication app( argc, argv );

	QTranslator translator;

	QString filename = "ZIMA-PTC-Cleaner_" + QLocale::system().name();
	QStringList paths;

	paths
			<< filename
			<< QApplication::applicationDirPath() + "/" + filename
			<< QApplication::applicationDirPath() + "/locale/" + filename
			<< ("locale/" + filename)
			<< (":/" + filename);

	for (const QString &path : paths)
	{
		if (translator.load(path))
		{
			app.installTranslator(&translator);
			break;
		}
	}

	Ptcclean *dlg = new Ptcclean();
	dlg->show();

	QStringList args = QApplication::arguments();

	//ak neni pri spusteni ziadny parameter, defaultny adresar bude currentPath
	if (args.count() < 2)
	{
		QString path = QDir::currentPath();
		dlg->changeDirectory(path);
	}else{
		QString path;

		for (int i=1; i<args.count(); i++)
		{
			path += args[i];
			if (i != args.count()-1)
				path += " ";
		}
		dlg->changeDirectory(path);
	}
	app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );
	return app.exec();
}
