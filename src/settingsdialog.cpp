/***************************************************************************
 *   Copyright (C) 2008 by Peter Holk                                     *
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


#include "settingsdialog.h"
#include <QMessageBox>
#include <QMenu>
#include <QApplication>

SettingsDialog::SettingsDialog(QWidget* parent, Qt::WFlags fl)
	: QDialog( parent, fl ), Ui::settingsDialog()
{
	setupUi(this);

	contextMenu = new QMenu(this);
	selectAllAction = new QAction(tr("Select &all"), this);
	selectNoneAction = new QAction(tr("Select &none"), this);
	contextMenu->addAction(selectAllAction);
	contextMenu->addAction(selectNoneAction);

	settings = new QSettings("ZIMA-Construction", "ZIMA-PTC-Cleaner", this);
	settings->beginGroup("filters");

	QStringList filters = settings->childKeys();
	QString str;
	foreach (str, filters)
	{
		QListWidgetItem *item = new QListWidgetItem(str, listFilter);
		bool check = settings->value(str, false).toBool();
		item->setCheckState(check?Qt::Checked:Qt::Unchecked);
	}
	settings->endGroup();

	delOld = settings->value("removeOldFiles", true).toBool();
	delFilters = settings->value("removeFiltered", true).toBool();
	checkOldFiles->setCheckState(delOld ? Qt::Checked : Qt::Unchecked);
	checkFilters->setCheckState(delFilters ? Qt::Checked : Qt::Unchecked);

	connect(btnClose, SIGNAL(clicked()), this, SLOT(accept()));
	connect(btnAdd, SIGNAL(clicked()), this, SLOT(addFilter()));
	connect(btnRemove, SIGNAL(clicked()), this, SLOT(removeFilter()));
	connect(listFilter, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkFilter(QListWidgetItem*)));
	connect(listFilter, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
	connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAllFilters()));
	connect(selectNoneAction, SIGNAL(triggered()), this, SLOT(deselectAllFilters()));
	connect(checkOldFiles, SIGNAL(stateChanged(int)), this, SLOT(methodCheckChanged(int)));
	connect(checkFilters, SIGNAL(stateChanged(int)), this, SLOT(methodCheckChanged(int)));

#ifdef Q_OS_WIN32
	contextMenuSettings = new QSettings("HKEY_CLASSES_ROOT\\Directory\\shell", QSettings::NativeFormat);
	connect(enableSystemContextMenu, SIGNAL(toggled(bool)), this, SLOT(enableSystemContextMenuChanged(bool)));

	bool enabled = settings->value("enableDirectoryContextMenu", true).toBool();
	enableSystemContextMenu->setChecked(enabled);

	if( enabled )
		enableSystemContextMenuChanged(true);
#else
	enableSystemContextMenu->setVisible(false);
#endif
}

SettingsDialog::~SettingsDialog()
{
}

#ifdef Q_OS_WIN32
void SettingsDialog::enableSystemContextMenuChanged(bool checked)
{
	if( checked )
	{
		contextMenuSettings->setValue("ZIMA-PTC-Cleaner/.", tr("Clean with ZIMA-PTC-Cleaner"));
		contextMenuSettings->setValue("ZIMA-PTC-Cleaner/command/.", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\")) + " \"%1\"");
	} else {
		contextMenuSettings->remove("ZIMA-PTC-Cleaner");
	}
}
#endif

void SettingsDialog::addFilter()
{
	if (textFilter->text().isEmpty())return;
	QListWidgetItem *item = new QListWidgetItem(textFilter->text());
	item->setCheckState(Qt::Checked);
	listFilter->addItem(item);

	settings->beginGroup("filters");
	settings->setValue(textFilter->text(), true);
	settings->endGroup();

	textFilter->setText("");
	textFilter->setFocus();

	anythingChanged = true;
}

void SettingsDialog::removeFilter()
{
	settings->beginGroup("filters");
	settings->remove(listFilter->currentItem()->text());
	settings->endGroup();
	delete listFilter->takeItem(listFilter->currentRow());

	anythingChanged = true;
}

void SettingsDialog::checkFilter(QListWidgetItem *item)
{
	settings->beginGroup("filters");
	settings->setValue(item->text(), item->checkState() == Qt::Checked);
	settings->endGroup();

	anythingChanged = true;
}

void SettingsDialog::methodCheckChanged(int)
{
	delOld = (checkOldFiles->checkState() == Qt::Checked);
	delFilters = (checkFilters->checkState() == Qt::Checked);

	settings->setValue("removeOldFiles", delOld);
	settings->setValue("removeFiltered", delFilters);

	anythingChanged = true;
}

QStringList SettingsDialog::getFilters()
{
	QStringList sl;
	for (int i=0;i<listFilter->count();i++)
	{
		if (listFilter->item(i)->checkState() == Qt::Checked)
			sl << listFilter->item(i)->text();
	}
	return sl;
}

void SettingsDialog::selectAllFilters()
{
	for (int i=0;i<listFilter->count();i++)
		listFilter->item(i)->setCheckState(Qt::Checked);
}

void SettingsDialog::deselectAllFilters()
{
	for (int i=0;i<listFilter->count();i++)
		listFilter->item(i)->setCheckState(Qt::Unchecked);
}

void SettingsDialog::showContextMenu(const QPoint &pos)
{
	contextMenu->popup(listFilter->mapToGlobal(pos));
}

bool SettingsDialog::exitWhenDone()
{
	return settings->value("ExitWhenDone", false).toBool();
}

void SettingsDialog::setExitWhenDone(bool exit)
{
	settings->setValue("ExitWhenDone", exit);
}
