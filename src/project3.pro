TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    vec3.cpp \
    celestialbody.cpp \
    solarsystem.cpp \
    euler.cpp \
    velocityverlet.cpp

HEADERS += \
    vec3.h \
    celestialbody.h \
    solarsystem.h \
    euler.h \
    velocityverlet.h
