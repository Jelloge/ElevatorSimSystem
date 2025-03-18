QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    Elevator.cpp \
    Passenger.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Elevator.h \
    Passenger.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    comp3004a3.pro.user

RESOURCES +=
