#ifndef DATATOSTAND_H
#define DATATOSTAND_H

#include "jfzlib.h"

int Stand_DS18B20(QString CH1, QString CH2, QString CH3, QString CH4, QString CH5, QString CH6, QString CH7,mat &DS18B20_All_Mat, QStringList &XLabelName);
int Stand_CCD(QString CH1, mat &All_Mat, QStringList &XLabelName);
int Stand_FBGS(QString CH1, QString CH2, QString CH3, QString CH4, QString CH5, QString CH6, QString CH7, mat &All_Mat, QStringList &XLabelName);
int Stand_FBGT(QString CH08, QString CH09, QString CH10, QString CH11, QString CH12, QString CH13, QString CH14, QString CH15, QString CH16, \
               QString CH17, QString CH18, QString CH19, QString CH20, QString CH21, QString CH22, QString CH23, QString CH24, QString CH25, \
               QString CH26, QString CH27, QString CH28, QString CH29, QString CH30, QString CH31, QString CH32, mat &All_Mat, QStringList &XLabelName);

#endif // DATATOSTAND_H
