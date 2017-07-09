TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c

LIBS += -lSDL2 -lSDL2_gfx -lSDL2_ttf

QMAKE_CFLAGS += -std=c11
