#ifndef JFZLIB_H
#define JFZLIB_H

#include "mainwindow.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

int JfzFileReadRow(QString FileName);
int JfzFileReadCol(QString FileName);
int JfzReadCSVToMat(QString FileName, double* OutMat, int RowNum, int ColNum);
mat JfzReadCSVToAllMat(QString FileName,int &InputFileRow,int &InputFileCol, QStringList &DataName, QStringList &XLabelName);
int saveStandData(QString Title, QString DataName, QStringList Time, mat Mat);
int saveStandDataNoTimeFix(QString Title, QString DataName, QString Date, QStringList Time, mat Mat);
int readEnvXlsFile(QString FileName, QMap<QString,float> &map);

#endif // JFZLIB_H
