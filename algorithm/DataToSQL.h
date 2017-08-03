#ifndef DATATOSQL_H
#define DATATOSQL_H

#include "tools/JSQL.h"
#include "algorithm/DataProcessing.h"
#include <QFileDialog>
#include <QDebug>


QFileInfoList GetFileList(QString path);

QStringList FBGDir2FileName(QString DirName);
QStringList DS18B20Dir2FileName(QString DirName);

bool CCDtoMYSQL(QString fileName, JSQL &jsql);
bool FBGtoMYSQL(QStringList FBGChannelNames, JSQL &jsql);
bool DS18B20toMYSQL(QStringList DS18B20ChannelNames, JSQL &jsql);
bool ENVtoMYSQL(QString fileName, JSQL &jsql);
bool CNCtoMYSQL(QString fileName,JSQL &jsql);



#endif // DATATOSQL_H
