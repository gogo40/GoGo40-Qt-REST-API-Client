QT += core network
QT -= gui

CONFIG   += c++11

TARGET = rest_api_client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    rest_api_client.cpp \
    qt_rest_api_client.cpp \
    test_rest_api_client.cpp

HEADERS += \
    rest_api_client.h \
    qt_rest_api_client.h

