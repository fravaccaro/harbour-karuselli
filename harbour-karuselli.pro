# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-karuselli

CONFIG += sailfishapp

QML_IMPORT_PATH += $$PWD/qml/modules

SOURCES += \
    src/harbour-karuselli.cpp \
    src/desktopparser.cpp \
    src/appmodel.cpp \
    src/selectedappsmodel.cpp \
    src/partnerstore.cpp \
    src/partnercontroller.cpp

HEADERS += \
    src/appinfo.h \
    src/desktopparser.h \
    src/appmodel.h \
    src/selectedappsmodel.h \
    src/partnerstore.h \
    src/partnercontroller.h

OPAL_FILES = $$files($$PWD/qml/modules/Opal/*, true)

DISTFILES += qml/harbour-karuselli.qml \
    qml/cover/CoverPage.qml \
    qml/components/KaruselliSupportDialog.qml \
    qml/components/MuotoTextLabel.qml \
    qml/components/LabelSpacer.qml \
    qml/pages/MainPage.qml \
    qml/pages/AppPickerPage.qml \
    qml/pages/AboutPage.qml \
    $$OPAL_FILES \
    appicons/86x86/apps/harbour-karuselli.png \
    appicons/108x108/apps/harbour-karuselli.png \
    appicons/128x128/apps/harbour-karuselli.png \
    appicons/172x172/apps/harbour-karuselli.png \
    appicons/256x256/apps/harbour-karuselli.png \
    images/*.png \
    images/*.png.license \
    tools/generate-brand-assets.py \
    rpm/harbour-karuselli.changes.in \
    rpm/harbour-karuselli.changes.run.in \
    rpm/harbour-karuselli.spec \
    translations/*.ts \
    harbour-karuselli.desktop

INSTALLS -= icon
icon.files =

appicons.files = appicons/*
appicons.path = /usr/share/icons/hicolor/

images.files = $$files($$PWD/images/*)
images.path = /usr/share/$$TARGET/images
INSTALLS += appicons images

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# Locale-specific translation files (compiled to .qm on build).
TRANSLATIONS +=  translations/*.ts
