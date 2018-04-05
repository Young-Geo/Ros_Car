TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../basis
INCLUDEPATH += /opt/ros/kinetic/include


TARGET = Node_Motion

SOURCES += \
    src/main.cpp \
    src/node_control.cpp \
    src/control.cpp \
    src/imu.cpp

HEADERS += \
    src/node_control.h \
    src/control.h   \
    src/imu.h
