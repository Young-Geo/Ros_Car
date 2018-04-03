TEMPLATE = subdirs

SUBDIRS += \
    src/basis \
    src/node_motion \
    src/node_imu    \
    src/node_ladar  \
    src/node_map    \
    src/node_test01 \
    src/node_test02

CONFIG += ordered
