#-------------------------------------------------
#
# Project created by QtCreator 2017-11-05T21:02:09
#
#-------------------------------------------------

QT       += core gui widgets multimedia

TARGET = NightNoise
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR=$$PWD/_bin

SOURCES += \
	src/main.cpp \
	src/MainWindow.cpp \
	src/SleepTimerDialog.cpp \
	src/ApplicationGlobal.cpp \
	src/joinpath.cpp \
	src/MySettings.cpp

HEADERS += \
	src/MainWindow.h \
	src/SleepTimerDialog.h \
	src/ApplicationGlobal.h \
	src/joinpath.h \
	src/MySettings.h

FORMS += \
	src/MainWindow.ui \
	src/SleepTimerDialog.ui

RESOURCES += \
	resources.qrc
