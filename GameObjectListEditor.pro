QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editobjectform.cpp \
    filecreatorform.cpp \
    gameobject.cpp \
    labellist.cpp \
    main.cpp \
    mainwindow.cpp \
    property.cpp \
    rwmanager.cpp

HEADERS += \
    editobjectform.h \
    filecreatorform.h \
    gameobject.h \
    labellist.h \
    mainwindow.h \
    property.h \
    rwmanager.h

FORMS += \
    mainwindow.ui

RESOURCES += myqrcfile.qrc

win32:RC_FILE = photosurface.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
