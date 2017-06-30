#-------------------------------------------------
#
# Project created by QtCreator 2017-03-26T10:21:18
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
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

SOURCES += main.cpp\
    tools/jfzlib.cpp \
    algorithm/datatostand.cpp \
    tools/EnvXlsReadThread.cpp \
    mainwindow.cpp \
    algorithm/correlations.cpp \
    algorithm/datadiagnosis.cpp \
    tools/stringtools.cpp \
    tools/tictoc.cpp \
    algorithm/LinearRegression.cpp \
    plugins/qcustomplot.cpp \
    tools/JIO.cpp \
    systemTray.cpp \
    tools/JSQL.cpp \
    algorithm/DataToSQL.cpp

HEADERS  += mainwindow.h \
    tools/jfzlib.h \
    algorithm/datatostand.h \
    tools/EnvXlsReadThread.h \
    skins/skins.h \
    algorithm/correlations.h \
    algorithm/datadiagnosis.h \
    tools/stringtools.h \
    tools/tictoc.h \
    algorithm/LinearRegression.h \
    plugins/qcustomplot.h \
    tools/JIO.h \
    systemTray.h \
    tools/JSQL.h \
    algorithm/DataToSQL.h \
    tools/log.h

FORMS    += mainwindow.ui

RESOURCES += \
    skins/skins.qrc
