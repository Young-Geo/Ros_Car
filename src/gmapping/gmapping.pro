TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../basis
INCLUDEPATH += /opt/ros/kinetic/include


TARGET = Gmapping

SOURCES += \
    src/main.cpp \
    src/slam_gmapping.cpp

HEADERS += \
    src/slam_gmapping.h
