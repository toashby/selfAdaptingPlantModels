QT -= core gui

CONFIG += c++11 console
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -fopenmp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# where to put the .o files
OBJECTS_DIR=obj

INCLUDEPATH ''= $$system(pwd)/include

SOURCES += main.cpp \
    stb_image.cpp \
    glad.c\
    include/glm/detail/glm.cpp \
    lsystem.cpp \
    linemodel.cpp \
    rule.cpp \
    internode.cpp \
    collisionmanager.cpp \
    meshbuilder.cpp \
    sceneloader.cpp \
    model.cpp

LIBS += -lassimp -fopenmp

LIBS +=-lGLEW -lglfw -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
HEADERS += \
    include/shader_m.h \
    include/camera.h \
    include/model.h \
    include/stb_image.h \
    include/mesh.h \
    include/shader.h \
    include/shader_s.h \
    include/linemodel.h \
    include/lsystem.h \
    include/rule.h \
    include/internode.h \
    include/collisionmanager.h \
    include/meshbuilder.h \
    include/sceneloader.h

OTHER_FILES += shaders/lines.vs shaders/lines.fs \
    shaders/1.model_loading.fs \
    shaders/1.model_loading.vs

DISTFILES += \
    include/imgui/LICENSE.txt

