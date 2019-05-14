#-------------------------------------------------
#
# Project created by QtCreator 2019-05-14T12:24:30
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wizualizacja-pola-wektorowego
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += /../../../Qt/5.12.3/gcc_64/include

LIBS += -L$$OUT_PWD/../../../Qt/5.12.3/gcc_64/lib

TEMPLATE = app

QT += datavisualization

contains(TARGET, qml.*) {
    QT += qml quick
}

CONFIG += c++17

SOURCES += \
        main.cpp \
        scatter.cpp

HEADERS += \
        scatter.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \
    resources.qrc
