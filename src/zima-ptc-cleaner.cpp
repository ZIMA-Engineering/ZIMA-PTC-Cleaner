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


#include "zima-ptc-cleaner.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QPalette>
#include <QRegularExpression>

Ptcclean::Ptcclean(QWidget* parent)
	: QMainWindow( parent ), Ui::ptccleanWindow()
{
	setupUi(this);

	contextMenu = new QMenu(this);
	selectAllAction = new QAction(tr("Select &all"), this);
	selectNoneAction = new QAction(tr("Select &none"), this);
	contextMenu->addAction(selectAllAction);
	contextMenu->addAction(selectNoneAction);

	aboutDlg = new AboutDialog(this);

	connect(aboutBtn, SIGNAL(clicked()), aboutDlg, SLOT(exec()));

	connect( btnBrowse, SIGNAL(clicked()), this, SLOT(browseDirectory()));
	connect( btnCancelSearch, SIGNAL(clicked()), &dirThread, SLOT(abortBrowsing()));
	connect( &dirThread, SIGNAL(finished()), this, SLOT(browsingFinished()));
	connect( &dirThread, SIGNAL(finalListReady(const QStringList&, const QStringList&)), this, SLOT(addFiles(const QStringList&, const QStringList&)));
	connect( btnSettings, SIGNAL(clicked()), &settingsDlg, SLOT(exec()));
	connect( btnSettings, SIGNAL(clicked()), &settingsDlg, SLOT(resetChangeFlag()));
	connect( &settingsDlg, SIGNAL(finished(int)), this, SLOT(loadFilters()));
	connect( checkSubdirs, SIGNAL(stateChanged(int)), this, SLOT(toggleSubdirs(int)));
	connect( comboDir, SIGNAL(activated(const QString&)), this, SLOT(comboClicked(const QString&)));
	connect( btnSelectAll, SIGNAL(clicked()), this, SLOT(selectAll()));
	connect( btnSelectNone, SIGNAL(clicked()), this, SLOT(selectNone()));
	connect( selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));
	connect( selectNoneAction, SIGNAL(triggered()), this, SLOT(selectNone()));
	connect( listFiles, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
	connect( btnClean, SIGNAL(clicked()), this, SLOT(cleanFiles()));
	connect(exitCheckBox, SIGNAL(toggled(bool)), &settingsDlg, SLOT(setExitWhenDone(bool)));

	exitCheckBox->setChecked(settingsDlg.exitWhenDone());

	listFiles->setStyleSheet("background-image: url(':/gfx/bkg.png');"
				 "background-position: center center;\n"
				 "background-repeat: no-repeat;\n"
				 "background-attachment: fixed;\n"
				 "background-color: " + qApp->palette().color(QPalette::Base).name() + "\n");

	//na zaciatku nacita filtre podla nastavenia
	dirThread.setFilters(settingsDlg.getFilters(), settingsDlg.delOld, settingsDlg.delFilters);
}

Ptcclean::~Ptcclean()
{
}

void Ptcclean::browseDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), currentDir.path());

	if (!dir.isEmpty())
		changeDirectory(dir);
}

void Ptcclean::changeDirectory(const QString &newDir)
{
	if (!QFile::exists(newDir))
	{
		QMessageBox::critical(this, tr("ZIMA-PTC-Cleaner - error"), tr("The path ") + newDir + tr(" does not exist"));
	}
	currentDir.setPath(newDir);
	currentDir.makeAbsolute();

	updateCombo();
	listFiles->clear();

	//pred spustenim prechadzania povypina secky prvky...
	btnCancelSearch->setEnabled(true);
	btnSettings->setEnabled(false);
	checkSubdirs->setEnabled(false);
	btnClean->setEnabled(false);
	btnBrowse->setEnabled(false);
	btnSelectAll->setEnabled(false);
	btnSelectNone->setEnabled(false);
	comboDir->setEnabled(false);

	this->setWindowTitle(tr("ZIMA-PTC-Cleaner - browsing..."));
	dirThread.setDirectory(currentDir);
	dirThread.start();
}

void Ptcclean::browsingFinished()
{
	btnCancelSearch->setEnabled(false);
	btnSettings->setEnabled(true);
	checkSubdirs->setEnabled(true);
	btnClean->setEnabled(true);
	btnBrowse->setEnabled(true);
	btnSelectAll->setEnabled(true);
	btnSelectNone->setEnabled(true);
	comboDir->setEnabled(true);

	this->setWindowTitle(tr("ZIMA-PTC-Cleaner"));
	dirThread.restart();
}

void Ptcclean::addFiles(const QStringList &files, const QStringList &dirs)
{
	QStringList::const_iterator dirI = dirs.begin();
	QListWidgetItem *item;

	//zoznam uz vyfiltrovanych suborov od DirBrowsingThreadu
	listFiles->addItems(files);

	for (int i=0; i<listFiles->count(); i++)
	{
		item = listFiles->item(i);
		item->setCheckState(Qt::Checked);
		item->setStatusTip(*dirI); //ku kazdemu sa do statusTipu ulozi adresar v ktorom dany subor je
		++dirI;
	}
}

void Ptcclean::loadFilters()
{
	if (settingsDlg.hasAnythingChanged()) //ak sa nezmenilo nastavenie filtrov nema cenu znovu prechadzat adresare
	{
		dirThread.setFilters(settingsDlg.getFilters(), settingsDlg.delOld, settingsDlg.delFilters);
		changeDirectory(currentDir.path());
	}
}

void Ptcclean::toggleSubdirs(int newState)
{
	dirThread.setSubdirs(newState == Qt::Checked);
	changeDirectory(currentDir.path());
}

//naplni combo adresarmi od sucasneho az po /
void Ptcclean::updateCombo()
{
	QString str = currentDir.path();

	comboDir->clear();
	if (str != "/") {
		comboDir->addItem(str + "/");
	}else{
		comboDir->addItem(str);
		return;
	}
	comboDir->setCurrentIndex(0);

	int pos = str.lastIndexOf('/');
	while (pos != -1)
	{
		str = str.left(pos);
		comboDir->addItem(str + "/");
		pos = str.lastIndexOf('/');
	}
}

void Ptcclean::comboClicked(const QString &text)
{
	changeDirectory(text);
}

void Ptcclean::selectAll()
{
	for (int i=0; i<listFiles->count(); i++)
	{
		listFiles->item(i)->setCheckState(Qt::Checked);
	}
}

void Ptcclean::selectNone()
{
	for (int i=0; i<listFiles->count(); i++)
	{
		listFiles->item(i)->setCheckState(Qt::Unchecked);
	}
}

void Ptcclean::cleanFiles()
{
	QListWidgetItem *item;
	for (int i=0; i<listFiles->count(); i++)
	{
		item = listFiles->item(i);
		if(item->checkState() == Qt::Checked)
		{
			//adresar v ktorom dany subor je je ulozeny v statusTip :)
			QFile::remove(item->statusTip() + item->text());
			delete listFiles->takeItem(i);
			i--;
		}
	}

	if(exitCheckBox->isChecked())
		qApp->quit();
}

void Ptcclean::showContextMenu(const QPoint &pos)
{
	contextMenu->popup(listFiles->mapToGlobal(pos));
}

//---

DirBrowsingThread::DirBrowsingThread()
{
	abort = false;
	includeSubdirs = false;
}

DirBrowsingThread::~DirBrowsingThread()
{
	abort = true;
	wait();
}

QString DirBrowsingThread::fileWithoutExt(QString file)
{
	int pos = file.lastIndexOf('.');
	if (pos == -1)
		return QString();

	return file.left(pos);
}

int DirBrowsingThread::fileNumber(QString file)
{
	int pos = file.lastIndexOf('.');

	if (pos == -1) //ak tam bodka ani neni tak ten subor tam nepatri
		return -1;

	bool ok;
	int num = file.right(file.length()-pos-1).toUInt(&ok);
	if (!ok) //ak posledna pripona neni cislo, tiez tam subor nepatri
	{
		return -1;
	}

	return num;
}

void DirBrowsingThread::abortBrowsing()
{
	mutex.lock();
	abort = true;
	mutex.unlock();
}

void DirBrowsingThread::addDir(QDir dir)
{
	//zoznam vsetkych suborov
	const QStringList sl = dir.entryList(QDir::Files, QDir::Name);
	finalList << sl;

	//vsetkym sa priradi sucasny adresar
	for (int i = 0; i < sl.count(); ++i)
	{
		dirList << dir.path() + "/";
	}

	//kontrola ci sa thread nema zrusit
	mutex.lock();
	if (abort)
	{
		mutex.unlock();
		return;
	}
	mutex.unlock();

	//dalej hlada vsetky adresare v tomto adresari
	//kvoli tomuto to prechadza kazdy adresar dvakrat, tak ale zas tak moc to nevadi...
	if (includeSubdirs)
	{
		QStringList sld = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (const QString &entry : sld)
		{
			QDir newDir(dir.path() + "/" + entry);
			addDir(newDir);
		}
	}
}

//pripravi thread na opatovne spustenie
void DirBrowsingThread::restart()
{
	QMutexLocker lock(&mutex);
	abort = false;
	finalList.clear();
	dirList.clear();
}

void DirBrowsingThread::run()
{
	if (!delOld && !delFilters)
	{
		emit finalListReady(finalList, dirList);
		return;
	}

	addDir(directory);

	mutex.lock();
	if (abort) {
		mutex.unlock();
		return;
	}
	QStringList::iterator   strI;
	QStringList::iterator   dirI = dirList.begin();

	int                     num, bigNum = -1;
	QStringList::iterator   bigItem;
	QStringList::iterator   bigItemDir;
	QString                 lastExt;
	QString                 lastDir;
	QList<QRegularExpression> regExps;

	if (delFilters)
	{
		for (const QString &s : filters)
		{
			regExps << QRegularExpression::fromWildcard(s, Qt::CaseSensitive);
		}
	}

	//neodobera priamo, ale len nastavuje na \n, ktore potom vyhodi, pretoze ak
	//si ukladam iterator na predchadzajuci prvok a neskor ho vymazem tak to blbne...
	//mozno by to slo keby som tie iteratory mazal v opacnom poradi (od konca), ale pre istotu
	//to necham takto...

	bool forceKeepInList = false; //ak vyhovuje aspon jedemu filtru
	bool forceKeepBig = false; //ak to so sucasne najvacsim cislom vyhovovalo filtru (takze sa ma nechat v zozname)

	for (strI = finalList.begin(); strI != finalList.end() ; ++strI)
	{
		forceKeepInList = false; //ak vyhovuje aspon jedemu filtru

		//metoda mazania podla filtrov (wildcards)
		if (delFilters)
		{
			for (const QRegularExpression &r : regExps)
			{
				if (r.match(*strI).hasMatch())
				{
					forceKeepInList = true;
					break;
				}
			}

			if (!forceKeepInList && !delOld) //ak maze len touto metodou (podla filtrov) a nevyhovuje ziadnemu filtru
			{
				*strI = "\n";
				*dirI = "\n";
				++dirI;
				continue;
			}else if (!delOld)
			{
				++dirI;
				continue;
			}
		}

		//metoda mazania starsich verzii suborov (podla cisla poslednej pripony)
		if (delOld)
		{
			//subory aj ked maju rovnaku priponu a meno, budu v jednotlivych adresaroch oddelene
			if ((bigNum != -1) && (lastDir != *dirI))
			{
				if (!forceKeepBig)
				{
					*bigItem = "\n";
					*bigItemDir = "\n";
				}
				bigNum = -1;
			}
			//subory su uz zoradene, takze ak je na dalsom ina pripona, je jasny najvacsi z predchadzajucej skupiny
			if ((bigNum != -1) && (fileWithoutExt(*strI) != lastExt))
			{
				if (!forceKeepBig)
				{
					*bigItem = "\n";
					*bigItemDir = "\n";
				}
				bigNum = -1;
			}

			num = fileNumber(*strI);
			lastExt = fileWithoutExt(*strI);
			lastDir = *dirI;

			//ak posledna pripona neni cislo
			if (!forceKeepInList && (num == -1))
			{
				*strI = "\n";
				*dirI = "\n";
			}
			//este vacsi nez predchadzajuci najvacsi
			if (num > bigNum)
			{
				if (forceKeepInList)
					forceKeepBig = true;
				else
					forceKeepBig = false;
				bigNum = num;
				bigItem = strI;
				bigItemDir = dirI;
			}
		}
		++dirI;
	}

	//no a ten co ostal ako najvacsi z poslednej "skupiny" sa tiez nebude mazat... (iba ze by vyhovoval nejakemu filtru)
	if (!forceKeepBig && (bigNum != -1))
	{
		*bigItem = "\n";
		*bigItemDir = "\n";
	}

	finalList.removeAll("\n");
	dirList.removeAll("\n");
	mutex.unlock();

	emit finalListReady(finalList, dirList);
}
