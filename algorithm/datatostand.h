#ifndef DATATOSTAND_H
#define DATATOSTAND_H

#include "tools/jfzlib.h"

// 数据各项名
extern QString DataName_DS18B20;
extern QString DataName_FBGT;
extern QString DataName_FBGS;
extern QString DataName_CCD;
// FBG标定波长+标定温度
extern double WaveAndTemp[231][2];

// 获取各项数据该读取的csv文件名
QStringList get_DS18B20_csv(const QString &dir);
QStringList get_CCD_csv(const QString &dir);
QStringList get_FBGT_csv(const QString &dir);
QStringList get_FBGS_csv(const QString &dir);

// 标准化算法名
int Stand_DS18B20  (QStringList &CH, mat &DS18B20_All_Mat, QStringList &XLabelName);
int Stand_CCD      (QStringList &CH, mat &All_Mat, QStringList &XLabelName);
int Stand_FBGS     (QStringList &CH, mat &All_Mat, QStringList &XLabelName);
int Stand_FBGT     (QStringList &CH, mat &All_Mat, QStringList &XLabelName);
int Stand_FBGT_Fix (QStringList &CH, mat &All_Mat, QStringList &XLabelName);
mat Stand_FBG(QStringList &CH);
mat Stand_DS18B20(QStringList &CH);

// 波长转温度算法-T=k(wave-Bwave)+Btemp
mat FBGtoTEMP(QString &standFBGFileName, QStringList &XLabelName);

#endif // DATATOSTAND_H
