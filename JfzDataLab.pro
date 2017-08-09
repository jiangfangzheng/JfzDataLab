#-------------------------------------------------
#
# Project created by QtCreator 2017-03-26T10:21:18
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CFLAGS += -std=c99
QT += axcontainer
QT += printsupport
QT += sql
QT += network

TARGET = JfzDataLab
TEMPLATE = app
RC_ICONS = skins/jfz.ico
VERSION = 1.0.0.0
QMAKE_TARGET_PRODUCT = "JfzDataLab"
QMAKE_TARGET_COMPANY = "Sandeepin"
QMAKE_TARGET_DESCRIPTION = "Jfz's DataLab"
QMAKE_TARGET_COPYRIGHT = "Copyright JFZ"

INCLUDEPATH+= D:/Library/Armadillo/include \

LIBS += D:\Library\Armadillo\lib_win32\blas_win32_MT.lib    \
        D:\Library\Armadillo\lib_win32\lapack_win32_MT.lib  \
        D:\Library\Armadillo\lib_win64\blas_win64_MT.lib    \
        D:\Library\Armadillo\lib_win64\lapack_win64_MT.lib  \

SOURCES += main.cpp\
    tools/EnvXlsReadThread.cpp \
    ui/mainwindow.cpp \
    algorithm/datadiagnosis.cpp \
    plugins/qcustomplot.cpp \
    ui/systemTray.cpp \
    tools/JSQL.cpp \
    algorithm/DataProcessing.cpp \
    tools/JIO.cpp \
    tools/DataProcessingThread.cpp \
    tools/DataSQLThread.cpp \
    ui/About.cpp

HEADERS  += ui/mainwindow.h \
    tools/EnvXlsReadThread.h \
    skins/skins.h \
    algorithm/datadiagnosis.h \
    plugins/qcustomplot.h \
    ui/systemTray.h \
    tools/JSQL.h \
    tools/log.h \
    algorithm/DataProcessing.h \
    tools/JIO.h \
    tools/DataProcessingThread.h \
    tools/DataSQLThread.h \
    ui/About.h

FORMS    += mainwindow.ui \
    ui/About.ui

RESOURCES += \
    skins/skins.qrc
