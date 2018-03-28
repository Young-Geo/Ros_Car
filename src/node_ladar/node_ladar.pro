TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../basis
INCLUDEPATH += /opt/ros/kinetic/include


TARGET = Node_Ladar

SOURCES += \
    src/main.cpp \
    src/node_ladar.cpp \
    src/ladar.cpp

HEADERS += \
    src/node_ladar.h \
    src/ladar.h
