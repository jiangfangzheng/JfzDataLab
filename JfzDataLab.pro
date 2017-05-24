#-------------------------------------------------
#
# Project created by QtCreator 2017-03-26T10:21:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JfzDataLab
TEMPLATE = app
RC_ICONS = skins/jfz.ico
QT+=axcontainer

INCLUDEPATH+= D:/Library/Armadillo/include \

LIBS += D:\Library\Armadillo\examples\lib_win64\blas_win64_MT.lib    \
        D:\Library\Armadillo\examples\lib_win64\lapack_win64_MT.lib  \

SOURCES += main.cpp\
    tools/jfzlib.cpp \
    algorithm/datatostand.cpp \
    tools/EnvXlsReadThread.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h \
    tools/jfzlib.h \
    algorithm/datatostand.h \
    tools/EnvXlsReadThread.h \
    skins/skins.h

FORMS    += mainwindow.ui

RESOURCES += \
    skins/skins.qrc
