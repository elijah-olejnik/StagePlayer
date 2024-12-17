#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T22:16:32
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StagePlayer
TEMPLATE = app
VERSION = 2.2


SOURCES += main.cpp\
        mainwindow.cpp \
    exitdialog.cpp \
    deck.cpp \
    warningdialog.cpp \
    spinboxdelegate.cpp \
    keybindings.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    nonedittablecolumndelegate.h \
    exitdialog.h \
    deck.h \
    PlayListItemModel.h \
    warningdialog.h \
    wavheader.h \
    spinboxdelegate.h \
    wavheader.h \
    keybindings.h \
    about.h

FORMS    += mainwindow.ui \
    exitdialog.ui \
    warningdialog.ui \
    keybindings.ui \
    about.ui

RESOURCES += images.qrc

RC_FILE += StagePlayer.rc
