#-------------------------------------------------
#
# Project created by QtCreator 2014-05-29T19:49:12
#
#-------------------------------------------------

QT       += core gui network sql multimedia qml quickwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chat_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chatopen.cpp \
    namelist.cpp \
    friendsearch.cpp \
    friendrequest.cpp \
    mytask1.cpp \
    call.cpp \
    audiooutput.cpp \
    audioinput.cpp

HEADERS  += mainwindow.h \
    chatopen.h \
    namelist.h \
    friendsearch.h \
    friendrequest.h \
    mytask1.h \
    call.h \
    audiooutput.h \
    audioinput.h

FORMS    += mainwindow.ui \
    chatopen.ui \
    namelist.ui \
    friendsearch.ui \
    friendrequest.ui \
    call.ui

RESOURCES += \
    rs.qrc

OTHER_FILES +=
win32:RC_ICONS += app.ico
VERSION = 1.2.3.4
QMAKE_TARGET_COMPANY = Alex Coporation
QMAKE_TARGET_COPYRIGHT = Alex Coporation All Right Reserved
#LIBS += -lws2_32
win32:LIBS += -lsetupapi

DISTFILES +=
