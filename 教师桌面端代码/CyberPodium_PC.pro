QT       += core gui
QT       += concurrent
QT       += network
QT       += websockets
RC_FILE  += logo.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    attendance.cpp \
    classing.cpp \
    inclass.cpp \
    logwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    toolbox.cpp

HEADERS += \
    attendance.h \
    classing.h \
    forpublic.h \
    inclass.h \
    logwidget.h \
    mainwindow.h \
    toolbox.h

FORMS += \
    attendance.ui \
    classing.ui \
    inclass.ui \
    logwidget.ui \
    mainwindow.ui \
    toolbox.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc

DISTFILES += \
    logo.rc
