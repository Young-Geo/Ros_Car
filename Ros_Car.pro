TEMPLATE = subdirs

SUBDIRS += \
    src/basis \
    src/node_motion \
    src/node_imu    \
    src/node_ladar  \
    src/node_map

CONFIG += ordered
