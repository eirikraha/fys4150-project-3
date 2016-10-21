TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    vec3.cpp \
    celestialbody.cpp \
    solarsystem.cpp \
    integrator.cpp

HEADERS += \
    vec3.h \
    celestialbody.h \
    solarsystem.h \
    integrator.h
