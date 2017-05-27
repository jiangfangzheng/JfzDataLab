#ifndef CORRELATIONS_H
#define CORRELATIONS_H

// 相关性分析
#include <armadillo>
#include <QString>
#include <QStringList>
#include "tools/jfzlib.h"
using namespace arma;

// 相关性分析，得到相关性矩阵
mat CorrelationAnalysis(const QString &correlationFileName1, const QString &correlationFileName2, QStringList &DataName1, QStringList &DataName2, QStringList &Time);
// 保存相关性分析的结果
bool saveCorrelationAnalysisCSV(const QString &correlationFileName1, const QString &correlationFileName2, QStringList &DataName1, QStringList &DataName2, mat &corMatneed);

#endif // CORRELATIONS_H
