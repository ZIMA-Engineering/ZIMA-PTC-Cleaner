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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QStringList>
#include "ui_settings.h"

class SettingsDialog : public QDialog, private Ui::settingsDialog
{
  Q_OBJECT

public:
    SettingsDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~SettingsDialog();

    QStringList getFilters();
    bool        hasAnythingChanged() { return anythingChanged; }
    bool exitWhenDone();

    QSettings *settings;
#ifdef Q_OS_WIN32
    QSettings *contextMenuSettings;
#endif

    bool        delOld;
    bool        delFilters;

public slots:
    void addFilter();
    void removeFilter();
    void checkFilter(QListWidgetItem *item);
    void selectAllFilters();
    void deselectAllFilters();
    void resetChangeFlag() { anythingChanged = false; }
    void showContextMenu(const QPoint& pos);
#ifdef Q_OS_WIN32
    void enableSystemContextMenuChanged(bool checked);
#endif
    void methodCheckChanged(int);
    void setExitWhenDone(bool exit);

protected:
    bool anythingChanged;

    QMenu   *contextMenu;
    QAction *selectAllAction;
    QAction *selectNoneAction;
};

#endif
