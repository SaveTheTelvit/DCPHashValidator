QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dynamicdeletelabel.cpp \
    errorelement.cpp \
    filehashinfo.cpp \
    hashcalculator.cpp \
    hashcalculatorelement.cpp \
    horizontalcontainer.cpp \
    main.cpp \
    mainwindow.cpp \
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
    mainwindow.h \
    structs.h \
    verticalscrollbox.h \
    vscrollboxelement.h \
    xmlfilereadermodule.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
