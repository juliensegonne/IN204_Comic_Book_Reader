# Configuration du projet
QT += widgets

# Nom de votre exécutable
TARGET = image

# Fichiers sources de votre application
SOURCES += main.cpp \
           comic_book.cpp

# Headers de votre application
HEADERS += comic_book.hpp

LIBS += -lMagick++ -lboost_filesystem

QMAKE_CXXFLAGS += -Wno-reorder
