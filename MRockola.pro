TEMPLATE = app
TARGET = MRockola

QT += qml quick widgets sql

SOURCES += main.cpp \
    sources/constants.cpp \
    sources/database.cpp \
    sources/mediaitem.cpp \
    sources/mediaplayer.cpp \
    sources/settings.cpp \
    sources/keyboard.cpp

RESOURCES += qml.qrc \
             images.qrc \
             translations.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = QML

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    sources/constants.h \
    sources/database.h \
    sources/mediaitem.h \
    sources/mediaplayer.h \
    sources/settings.h \
    sources/keyboard.h


# translations
TRANSLATIONS = languages/mrockola_es.ts

#Linux
unix:INCLUDEPATH += /usr/lib64
unix:DEPENDPATH += /usr/lib64
unix:LIBS += -L/usr/lib64/ -lvlc






