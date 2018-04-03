TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../basis
INCLUDEPATH += /opt/ros/kinetic/include


TARGET = Node_Motion

SOURCES += \
    src/main.cpp \
    src/node_test02.cpp

HEADERS += \
    src/node_test02.h
