RESOURCES = resources.qrc
QT += core
QT += gui
QT += widgets
QT += svg

CONFIG += c++11

TARGET = bowlingSolitaire

TEMPLATE = app

SOURCES += main.cpp \
    card.cpp \
    carddeck.cpp \

HEADERS += \
    card.h \
    carddeck.h \


