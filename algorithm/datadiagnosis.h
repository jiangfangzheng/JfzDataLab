#ifndef DATADIAGNOSIS_H
#define DATADIAGNOSIS_H

#include <armadillo>
#include <QString>
#include <QStringList>
#include "tools/jfzlib.h"
using namespace arma;

mat DataAnalysis(QString &InputFileName_str, QStringList &DataName, QString &strMatRow, QString &strMatCol, QString &strTitle, QString &strMatSum, QString &strMatArithmeticMean, QString &strMatMax, QString &strMatMin);
bool DataDiagnosis(QString &InputFileName_str, mat &Mat, QStringList &DataName, Ui::MainWindow *ui);
#endif // DATADIAGNOSIS_H
