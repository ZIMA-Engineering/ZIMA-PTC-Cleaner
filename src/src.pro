SOURCES += main.cpp \
 settingsdialog.cpp \
    zima-ptc-cleaner.cpp \
    aboutdialog.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
 uitools
TARGET = ../bin/ZIMA-PTC-Cleaner

FORMS += \
settings.ui \
    zima-ptc-cleaner.ui \
    aboutdialog.ui
RESOURCES += icons.qrc

HEADERS += \
 settingsdialog.h \
    zima-ptc-cleaner.h \
    aboutdialog.h

win32:RC_FILE = zima-ptc-cleaner.rc

TRANSLATIONS += ZIMA-PTC-Cleaner_sk.ts \
 ZIMA-PTC-Cleaner_cs.ts


zima-ptc-cleaner.path = /usr/bin/

zima-ptc-cleaner_data.path = /usr/share/ptcclean/

zima-ptc-cleaner_data.files += zima-ptc-cleaner_sk.qm \
zima-ptc-cleaner_cs.qm \
bkg.png

OTHER_FILES += \
    zima-ptc-cleaner.rc \
    ../README










