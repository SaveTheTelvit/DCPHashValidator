QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

VERSION = 1.2.0.2
QMAKE_TARGET_COMPANY = Eugene Dokuchaev
QMAKE_TARGET_PRODUCT = DCP Hash Validator
QMAKE_TARGET_COPYRIGHT = Eugene Dokuchaev

win32: RC_ICONS = $$PWD/icon.ico

SOURCES += \
    dynamicdeletelabel.cpp \
    errorelement.cpp \
    filehashinfo.cpp \
    hashcalculator.cpp \
    hashcalculatorelement.cpp \
    horizontalcontainer.cpp \
    iconbutton.cpp \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    verticalscrollbox.cpp \
    vscrollboxelement.cpp \
    xmlfilereadermodule.cpp

HEADERS += \
    connectioncontroller.h \
    dynamicdeletelabel.h \
    errorelement.h \
    filehashinfo.h \
    hashcalculator.h \
    hashcalculatorelement.h \
    horizontalcontainer.h \
    iconbutton.h \
    mainwindow.h \
    settings.h \
    structs.h \
    verticalscrollbox.h \
    vscrollboxelement.h \
    xmlfilereadermodule.h

FORMS += \
    mainwindow.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
