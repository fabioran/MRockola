TEMPLATE = app
TARGET = MRockola3

QT += quick widgets sql 

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
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14

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
CONFIG +=sdk_no_version_check
#QMAKE_CXXFLAGS += -Wno-deprecated-declarations
#MacOS
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lvlc
