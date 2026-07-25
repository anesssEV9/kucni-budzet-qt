#-------------------------------------------------
#
# Project created by QtCreator 2026-04-15T15:18:42
#
#-------------------------------------------------

QT += core gui sql
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ostalitroskovi.cpp \
    fiksniTroskovi.cpp \
    login.cpp \
    accountsettings.cpp \
    svitroskovi.cpp \
    qcustomplot.cpp \
    grafik.cpp

HEADERS  += mainwindow.h \
    ostalitroskovi.h \
    fiksniTroskovi.h \
    login.h \
    accountsettings.h \
    svitroskovi.h \
    qcustomplot.h \
    grafik.h

FORMS    += mainwindow.ui \
    ostalitroskovi.ui \
    fiksniTroskovi.ui \
    login.ui \
    accountsettings.ui \
    svitroskovi.ui \
    grafik.ui

RESOURCES += \
    ikone.qrc

QT += printsupport

DISTFILES += \
    ../info (2).png
