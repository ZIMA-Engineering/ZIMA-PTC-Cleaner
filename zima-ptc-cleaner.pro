QT += core gui widgets

TARGET = ZIMA-PTC-Cleaner
TEMPLATE = app
CONFIG += c++17

win32:INCLUDEPATH += ../
VPATH += ./src

SOURCES += main.cpp \
    settingsdialog.cpp \
    zima-ptc-cleaner.cpp \
    aboutdialog.cpp

FORMS += settings.ui \
    zima-ptc-cleaner.ui \
    aboutdialog.ui

RESOURCES += zima-ptc-cleaner.qrc

HEADERS += settingsdialog.h \
    zima-ptc-cleaner.h \
    aboutdialog.h

win32:RC_FILE = zima-ptc-cleaner.rc

OTHER_FILES += \
    src/zima-ptc-cleaner.rc \
    README \
    LICENSE

TRANSLATIONS += locale/ZIMA-PTC-Cleaner_sk.ts \
    locale/ZIMA-PTC-Cleaner_cs.ts

CODECFORTR = UTF-8
