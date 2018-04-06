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
    src/imu.cpp \
    src/imu/i2c.cpp \
    src/imu/Kalman.cpp \
    src/imu/Matrix.cpp \
    src/imu/Navigation.cpp \
    src/imu/Quaternion.cpp \
    src/imu/svdcmp.cpp \
    src/imu/Vector.cpp

HEADERS += \
    src/node_control.h \
    src/control.h   \
    src/imu.h \
    src/imu/i2c.h \
    src/imu/Kalman.h \
    src/imu/Matrix.h \
    src/imu/Navigation.h \
    src/imu/Quaternion.h \
    src/imu/svdcmp.h \
    src/imu/Vector.h
