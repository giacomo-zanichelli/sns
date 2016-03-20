#-------------------------------------------------
#
# Project created by QtCreator 2015-12-19T13:30:24
#
#-------------------------------------------------

QT       += core gui network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += static

TARGET = SNS
TEMPLATE = app

include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)


SOURCES += main.cpp\
        mainwindow.cpp \
    person.cpp \
    address.cpp \
    validator.cpp \
    dbpathdialog.cpp \
    excelpathdialog.cpp \
    confirmationdialog.cpp \
    sqlquerymodel.cpp \
    searchdialog.cpp \
    modelmanager.cpp \
    smtp.cpp

HEADERS  += mainwindow.h \
    person.h \
    address.h \
    validator.h \
    dbpathdialog.h \
    excelpathdialog.h \
    confirmationdialog.h \
    sqlquerymodel.h \
    searchdialog.h \
    modelmanager.h \
    smtp.h

FORMS    += \
    mainwindow.ui \
    excelpathdialog.ui \
    dbpathdialog.ui \
    confirmationdialog.ui \
    searchdialog.ui


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../bin/lib/release/ -lPocoNetSSLd.31
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../bin/lib/debug/ -lPocoNetSSLd.31
#else:unix: LIBS += -L$$PWD/../../../../../../bin/lib/ -lPocoNetSSLd.31

#else:unix: LIBS        += -L$$PWD/../../../../../../bin/lib/ -lPocoFoundationd
#else:unix: LIBS        += -L$$PWD/../../../../../../bin/lib/ -lPocoNetd
#else:unix: LIBS        += -L$$PWD/../../../../../../bin/lib/ -lPocoUtild
#else:unix: LIBS        += -L$$PWD/../../../../../../bin/lib/ -lPocoXML

#else:unix: INCLUDEPATH += $$PWD/../../../../../../bin/include
#else:unix: DEPENDPATH += $$PWD/../../../../../../bin/include
#else:unix: INCLUDEPATH += $$PWD/../../../../../../../../usr/local/openssl/include
#else:unix: DEPENDPATH += $$PWD/../../../../../../../../usr/local/openssl/include


LIBS += -L$$PWD/3rdparty/poco/lib/ -lPocoNetSSL.31
LIBS += -L$$PWD/3rdparty/poco/lib/ -lPocoFoundation.31
LIBS += -L$$PWD/3rdparty/poco/lib/ -lPocoNet.31
LIBS += -L$$PWD/3rdparty/poco/lib/ -lPocoUtil.31
LIBS += -L$$PWD/3rdparty/poco/lib/ -lPocoXML.31

INCLUDEPATH += $$PWD/3rdparty/poco/include
DEPENDPATH += $$PWD/3rdparty/poco/include

INCLUDEPATH += $$PWD/3rdparty/openssl/include
DEPENDPATH += $$PWD/3rdparty/openssl/include
