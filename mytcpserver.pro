QT+=sql
QT -= gui
QT += network
QT += sql

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
    main.cpp \
    mytcpserver.cpp\
    serverfunctions.cpp\
    database.cpp\
    relations.cpp

HEADERS += \
    mytcpserver.h\
    serverfunctions.h\
    database.h\
    relations.h

TARGET = tcpserver
