unix:LIBS+= -lgtest

TARGET=Tests

SOURCES +=main.cpp \
    ../treeGenerator/lsystem.cpp \
    ../treeGenerator/internode.cpp \
    ../treeGenerator/rule.cpp \
    ../treeGenerator/meshbuilder.cpp \
    ../treeGenerator/collisionmanager.cpp \

INCLUDEPATH += ../treeGenerator/include
