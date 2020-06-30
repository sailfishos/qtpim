TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS = contacts organizer versit versitorganizer plugins $$PWD/../tests
qtHaveModule(qml): SUBDIRS += imports
