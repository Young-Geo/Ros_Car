TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../basis
INCLUDEPATH += /opt/ros/kinetic/include


TARGET = Node_Motion

SOURCES += \
    src/main.cpp \
    src/node_motion.cpp \
    src/motion.cpp

HEADERS += \
    src/node_motion.h \
    src/motion.h
