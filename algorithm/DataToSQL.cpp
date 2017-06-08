#include "DataToSQL.h"
#include "tools/JIO.h"
#include "algorithm/datatostand.h"
#include <armadillo>
using namespace arma;

// 递归遍历子目录
QFileInfoList GetFileList(QString path)
{
	QDir dir(path);
	QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	for(int i = 0; i != folder_list.size(); i++)
	{
		 QString name = folder_list.at(i).absoluteFilePath();
		 QFileInfoList child_file_list = GetFileList(name);
		 file_list.append(child_file_list);
	}

	return file_list;
}

// FBG目录名转为精确到通道csv文件名
QStringList FBGDir2FileName(QString DirName)
{
	QStringList FBGChannelNames;
	for(int i=1;i<=32;++i)
	{
		FBGChannelNames.append(DirName+"/通道"+QString::number(i)+".csv");
	}
	return FBGChannelNames;
}

// DS18B20目录名转为精确到通道csv文件名
QStringList DS18B20Dir2FileName(QString DirName)
{
	QStringList DS18B20ChannelNames;
	for(int i=1;i<=8;++i)
	{
		DS18B20ChannelNames.append(DirName+"/通道"+QString::number(i)+".csv");
	}
	return DS18B20ChannelNames;
}

bool CCDtoMYSQL(QString fileName,JSQL &jsql)
{
	QList<QList<QString>> CCDStrList = JIO::CsvToStrList(fileName);
	// 格式化时间 由2017-6-7-12:03:14变为2017-6-7 12:03:14
	QStringList Time;
	for(int i=1;i<CCDStrList.size();++i)
	{
		QString tmp = CCDStrList[i][0].left(10) + " "+ CCDStrList[i][0].right(8);
		Time.append(tmp);
	}
	qDebug()<<"Time.size(): "<<Time.size();
	// 写入数据库
	bool bOut = false;
	for(int i=1;i<CCDStrList.size();++i)
	{
		QStringList onedata{Time[i-1],CCDStrList[i][1],CCDStrList[i][2],CCDStrList[i][3]};
		bOut = jsql.insertData("ccd",onedata);
	}
	return bOut;
}

bool FBGtoMYSQL(QStringList FBGChannelNames,JSQL &jsql)
{
	// 获得大Mat
	mat All_Mat = Stand_FBG(FBGChannelNames);

	// 格式化时间 由2017-6-7-12:03:14变为2017-6-7 12:03:14
	QList<QList<QString>> FBGChanneMat = JIO::CsvToStrList(FBGChannelNames[0]);
	QStringList Time;
	for(int i=1;i<FBGChanneMat.size();++i)
	{
		QString tmp = FBGChanneMat[i][0].left(10) + " "+ FBGChanneMat[i][0].right(8);
		Time.append(tmp);
	}
//	qDebug()<<"Time.size(): "<<Time.size();
//	qDebug()<<"Time.size(): "<<Time[0];

	// 制作字符串格式的Mat
	QList<QList<QString>> FBGStrList;
	for(unsigned int i=0;i<All_Mat.n_rows;++i)
	{
		QList<QString> temp;
		for(unsigned  j=0;j<All_Mat.n_cols;++j)
		{
			QString value;
			value.sprintf("%.3lf",All_Mat(i,j));
			temp.append(value);
		}
		FBGStrList.append(temp);
	}

//	qDebug()<<"FBGStrList.size(): "<<FBGStrList.size();
//	qDebug()<<"FBGStrList[0].size(): "<<FBGStrList[0].size();
//	qDebug()<<"FBGStrList[0][0]: "<<FBGStrList[0][0];
//	qDebug()<<"FBGStrList[0][1]: "<<FBGStrList[0][1];

	// 写入数据库
	bool bOut = false;
	for(int i=0;i<FBGStrList.size();++i)
	{
		QStringList onedata{FBGStrList[i]};
		onedata.insert(0,Time[i]);
		//		if(0==i) qDebug()<<onedata;
		bOut = jsql.insertData("fbg",onedata);
	}
	return bOut;
}

bool DS18B20toMYSQL(QStringList DS18B20ChannelNames,JSQL &jsql)
{
	// 获得大Mat
	mat All_Mat = Stand_DS18B20(DS18B20ChannelNames);

	// 格式化时间 由2017-6-7-12:03:14变为2017-6-7 12:03:14
	QList<QList<QString>> DS18B20ChanneMat = JIO::CsvToStrList(DS18B20ChannelNames[0]);
	QStringList Time;
	for(int i=1;i<DS18B20ChanneMat.size();++i)
	{
		QString tmp = DS18B20ChanneMat[i][0].left(10) + " "+ DS18B20ChanneMat[i][0].right(8);
		Time.append(tmp);
	}
//	qDebug()<<"Time.size(): "<<Time.size();
//	qDebug()<<"Time.size(): "<<Time[0];

	// 制作字符串格式的Mat
	QList<QList<QString>> DS18B20StrList;
	for(unsigned int i=0;i<All_Mat.n_rows;++i)
	{
		QList<QString> temp;
		for(unsigned int j=0;j<All_Mat.n_cols;++j)
		{
			QString value;
			value.sprintf("%.3lf",All_Mat(i,j));
			temp.append(value);
		}
		DS18B20StrList.append(temp);
	}

//	qDebug()<<"FBGStrList.size(): "<<DS18B20StrList.size();
//	qDebug()<<"FBGStrList[0].size(): "<<DS18B20StrList[0].size();
//	qDebug()<<"FBGStrList[0][0]: "<<DS18B20StrList[0][0];
//	qDebug()<<"FBGStrList[0][1]: "<<DS18B20StrList[0][1];

	// 写入数据库
	bool bOut = false;
	for(int i=0;i<DS18B20StrList.size();++i)
	{
		QStringList onedata{DS18B20StrList[i]};
		onedata.insert(0,Time[i]);
//		if(0==i) qDebug()<<onedata;
		bOut = jsql.insertData("ds18b20",onedata);
	}
	return bOut;
}

bool ENVtoMYSQL(QString fileName,JSQL &jsql)
{
	QList<QList<QString>> ENVStrList = JIO::CsvToStrList(fileName);
	// 格式化时间 由2017-6-7-12:03:14变为2017-6-7 12:03:14
	QStringList Time;
	for(int i=1;i<ENVStrList.size();++i)
	{
		QString tmp = ENVStrList[i][0].left(10) + " "+ ENVStrList[i][0].right(8);
		Time.append(tmp);
	}
	qDebug()<<"Time.size(): "<<Time.size();
	// 写入数据库
	bool bOut = false;
	for(int i=1;i<ENVStrList.size();++i)
	{
		QStringList onedata{Time[i-1], ENVStrList[i][1], ENVStrList[i][2], ENVStrList[i][3], ENVStrList[i][4]};
		bOut = jsql.insertData("environment",onedata);
	}
	return bOut;
}

bool CNCtoMYSQL(QString fileName,JSQL &jsql)
{
	QList<QList<QString>> CNCStrList = JIO::CsvToStrList(fileName);
	// 格式化时间 由2017-6-7-12:03:14变为2017-6-7 12:03:14
	QStringList Time;
	for(int i=1;i<CNCStrList.size();++i)
	{
		QString tmp = CNCStrList[i][0].left(10) + " "+ CNCStrList[i][0].right(8);
		Time.append(tmp);
	}
	qDebug()<<"Time.size(): "<<Time.size();
	// 写入数据库
	bool bOut = false;
	for(int i=1;i<CNCStrList.size();++i)
	{
		QStringList onedata{Time[i-1], CNCStrList[i][1], CNCStrList[i][2], CNCStrList[i][3], CNCStrList[i][4], CNCStrList[i][5], CNCStrList[i][6], CNCStrList[i][7], CNCStrList[i][8], CNCStrList[i][9], CNCStrList[i][10], CNCStrList[i][11], CNCStrList[i][12], CNCStrList[i][13], CNCStrList[i][14], CNCStrList[i][15], CNCStrList[i][16]};
		bOut = jsql.insertData("cnc",onedata);
	}
	return bOut;
}
