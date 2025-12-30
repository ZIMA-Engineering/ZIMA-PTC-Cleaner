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

#ifndef PTCCLEAN_H
#define PTCCLEAN_H

#include <QAction>
#include <QDir>
#include <QMainWindow>
#include <QMenu>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "ui_zima-ptc-cleaner.h"

#define VERSION "1.2.2"

//pri instalacii do nestandardneho adreara toto odkomentovat, aby si to naslo preklady a obrazok
//#define INSTALL_PREFIX  "/usr"

class DirBrowsingThread : public QThread
{
    Q_OBJECT

public:
    DirBrowsingThread();
    ~DirBrowsingThread();

    //tie zamky su zbytocne, bo toto by sa malo volat len ked thread nebezi, ale sak co uz...
    void setDirectory(QDir &newDir)
    { QMutexLocker lock(&mutex); directory = newDir; }
    void setFilters(QStringList newFilters, bool rmOld, bool rmFilters)
    { QMutexLocker lock(&mutex); filters = newFilters; delOld = rmOld; delFilters = rmFilters; }
    void setSubdirs(bool _subdirs)
    { QMutexLocker lock(&mutex); includeSubdirs = _subdirs; }

    void restart();

    static QString fileWithoutExt(QString file);
    static int fileNumber(QString file);

protected:
    void        run();
    void        addDir(QDir dir);

    bool        delOld, delFilters;
    bool        abort;
    bool        includeSubdirs;
    QDir        directory;
    QStringList filters;
    QStringList finalList;
    QStringList dirList;

    QMutex      mutex;

signals:
    void finalListReady(const QStringList&, const QStringList&);

public slots:
    void abortBrowsing();
};

class Ptcclean : public QMainWindow, private Ui::ptccleanWindow
{
    Q_OBJECT

public:
    Ptcclean(QWidget* parent = 0 );
    ~Ptcclean();

    void changeDirectory(const QString &newDir);

public slots:
    void    addFiles(const QStringList &files, const QStringList& dirs);
    void    browseDirectory();
    void    browsingFinished();
    void    loadFilters();
    void    toggleSubdirs(int);
    void    updateCombo();
    void    comboClicked(const QString&);
    void    selectAll();
    void    selectNone();
    void    cleanFiles();
    void    showContextMenu(const QPoint& pos);

protected:
    QDir                currentDir;
    DirBrowsingThread   dirThread;
    SettingsDialog      settingsDlg;
    QMenu               *contextMenu;
    QAction             *selectAllAction;
    QAction             *selectNoneAction;
    AboutDialog         *aboutDlg;
};

#endif
