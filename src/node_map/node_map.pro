TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../basis
INCLUDEPATH += /opt/ros/kinetic/include


TARGET = Node_Map

SOURCES += \
    src/main.cpp \
    src/slam_gmapping.cpp \
    src/nodelet.cpp \
    src/replay.cpp

HEADERS += \
    src/slam_gmapping.h \
    src/node_imu.h
