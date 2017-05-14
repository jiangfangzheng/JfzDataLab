#-------------------------------------------------
#
# Project created by QtCreator 2017-03-26T10:21:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JfzDataLab
TEMPLATE = app
RC_ICONS = poi.ico

INCLUDEPATH+= D:/Library/Armadillo/include \

QT+=axcontainer

LIBS += D:\Library\Armadillo\examples\lib_win64\blas_win64_MT.lib    \
        D:\Library\Armadillo\examples\lib_win64\lapack_win64_MT.lib  \



SOURCES += main.cpp\
        mainwindow.cpp \
    jfzlib.cpp \
    datatostand.cpp \
    EnvXlsReadThread.cpp

HEADERS  += mainwindow.h \
    jfzlib.h \
    datatostand.h \
    EnvXlsReadThread.h

FORMS    += mainwindow.ui
