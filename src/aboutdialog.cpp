/***************************************************************************                                                *
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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "zima-ptc-cleaner.h"
#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

	connect(ui->aboutText, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated(QString)));

	ui->aboutText->setText(QString(
			"<html><head><style>"
			"li{list-style-type:none;}"
			"</style></head><body>"
		)
		+ tr(
			"<h1>ZIMA-PTC-Cleaner</h1>"
			"<p class=\"version\">%1</p>"
			"<p>ZIMA-PTC-Cleaner was developed by <a href=\"http://www.zima-construction.cz/\">ZIMA-Construction</a> "
			"and is released under the <a href=\"http://www.gnu.org/\">GNU/GPLv2</a> license."
			"</p>"
			"<h4>Authors:</h4>"
	    ).arg(VERSION)
	    + QString(
			"<ul><li>Jakub Skokan &lt;<a href=\"mailto:aither@havefun.cz\">aither@havefun.cz</a>&gt;</li>"
			"<li>Aleš Kocur &lt;<a href=\"mailto:kafe@havefun.cz\">kafe@havefun.cz</a>&gt;</li>"
			"<li>Peter Holák &lt;<a href=\"mailto:forrondur@gmail.com\">forrondur@gmail.com</a>&gt;</li></ul>"
	    ) + QString("</body></html")
	);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::linkActivated(QString url)
{
	QDesktopServices::openUrl( QUrl(url) );
}
