######################################################################
# Automatically generated by qmake (3.0) mar. mai 16 08:55:23 2017
######################################################################

QT       += core websockets
QT       -= gui

TARGET = echoclient
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/mysql-connector-cpp/include
DEPENDPATH += $$PWD/mysql-connector-cpp/include

LIBS += -L$$PWD/mysql-connector-cpp/lib/ -lmysqlcppconn

INCLUDEPATH += $$PWD/mysql-connector-cpp/include
DEPENDPATH += $$PWD/mysql-connector-cpp/include

SOURCES += \
    main.cpp \
    echoclient.cpp \
    database.cpp

HEADERS += \
    echoclient.h \
    database.h

target.path = $$[QT_INSTALL_EXAMPLES]/websockets/echoclient
INSTALLS += target


