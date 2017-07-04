#ifndef DATATOSTAND_H
#define DATATOSTAND_H

#include "tools/jfzlib.h"

// 数据各项名
extern QString DataName_DS18B20;
extern QString DataName_DS18ALL;
extern QString DataName_FBGT;
extern QString DataName_FBGS;
extern QString DataName_FBG;
extern QString DataName_CCD;
extern QString DataName_ENV;
extern QString DataName_CNC;
extern QString DataName_FBGST;
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
// 将640个FBG通道转为我们需要的321个通道，包括应力、温度
bool ALLFBGto321FBG(mat &inputMat, mat &outputMat);

// 波长转温度算法-T=k(wave-Bwave)+Btemp
mat FBGtoTEMP(QString &standFBGFileName, QStringList &XLabelName);

// 时间工具，2017-06-17 23:17:13转20170617231713
QString TimeToTimeNum(QString Time);

// 虚拟映射
extern QString VirtualMap_FBGT_Now;
extern QString VirtualMap_FBGT_Ori;
extern QString VirtualMap_FBGS_Now;
extern QString VirtualMap_FBGS_Ori;
extern QString VirtualMap_DS18_Now;
extern const int VirtualMap_FBGT_Index[219];
extern const int VirtualMap_FBGS_Index[90];

// 由路径获取文件名
QString getFileName(QString filePath);

#endif // DATATOSTAND_H
