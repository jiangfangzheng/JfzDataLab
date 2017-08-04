#include "DataSQLThread.h"
#include "algorithm/DataProcessing.h"
#include "tools/JIO.h"
#include <QDir>
#include <QTime>


DataSQLThread::DataSQLThread()
{
	dir = "";
	dataType = "";
	processingType = "";
}

DataSQLThread::~DataSQLThread()
{
	// 请求终止
	qDebug()<<"[~DataSQLThread]";
	requestInterruption();
	quit();
	wait();
}


DataSQLThread::DataSQLThread(QString dir_, JSQL *jsql_, QString dataType_, QString processingType_):
	dir(dir_),
	jsql(jsql_),
	dataType(dataType_),
	processingType(processingType_)
{
}

void DataSQLThread::setQueryMsg(QString tableName, QString startTime, QString endTime, QString itemName, int itemNum, bool isStr, bool isOri)
{
	this->tableName = tableName;
	this->startTime = startTime;
	this->endTime = endTime;
	this->itemName = itemName;
	this->itemNum = itemNum;
	this->isStr = isStr;
	this->isOri = isOri;
}

bool DataSQLThread::CCDtoSQL(QString &dir)
{
	bool b = false;
	// 【1】递归遍历子目录，得到所有*.csv文件路径
	QFileInfoList file_list = GetFileList(dir);
	QStringList AllFileName;
	for (int i = 0; i < file_list.size(); ++i)
	{
		QFileInfo fileInfo = file_list.at(i);
		AllFileName.append(fileInfo.filePath());
	}
	// 【2】向SQL中插入数据
	for(auto filename : AllFileName)
	{
		b = CCDtoMYSQL(filename);
	}
	return b;
}

bool DataSQLThread::DS1toSQL(QString &dir_)
{
	// 【1】递归遍历子目录，得到所有*.csv文件路径
	// 遍历目录
	QDir dir(dir_);
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	QStringList AllDirsName;
	for(int i = 0; i != folder_list.size(); i++)
	{
		 QString name = folder_list.at(i).absoluteFilePath();
		 AllDirsName.append(name);
	}

	// 【2】遍历目录中的文件、向SQL中插入数据
	bool b = false;
	for(int i=0;i<AllDirsName.size();++i)
	{
		QStringList DS18B20ChannelNames = DS18B20Dir2FileName(AllDirsName[i]);
		b = DS18B20toMYSQL(DS18B20ChannelNames);
	}
	return b;
}

bool DataSQLThread::FBGtoSQL(QString &dir_)
{
	// 【1】递归遍历子目录，得到所有*.csv文件路径
	// 遍历目录
	QDir dir(dir_);
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	QStringList AllDirsName;
	for(int i = 0; i != folder_list.size(); i++)
	{
		 QString name = folder_list.at(i).absoluteFilePath();
		 AllDirsName.append(name);
	}

	// 【2】遍历目录中的文件、向SQL中插入数据
	bool b = false;
	for(int i=0;i<AllDirsName.size();++i)
	{
		QStringList FBGChannelNames = FBGDir2FileName(AllDirsName[i]);
		qDebug()<<FBGChannelNames[i];
		b = FBGtoMYSQL(FBGChannelNames);
	}
	return b;
}

bool DataSQLThread::ENVtoSQL(QString &dir)
{
	// 【1】递归遍历子目录，得到所有*.csv文件路径
	QFileInfoList file_list = GetFileList(dir); // 递归遍历子目录
	QStringList AllFileName;
	for (int i = 0; i < file_list.size(); ++i)
	{
		QFileInfo fileInfo = file_list.at(i);
		AllFileName.append(fileInfo.filePath());
//			qDebug()<<QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName());
//			qDebug()<<QString("%1").arg(fileInfo.filePath());
	}

	// 【2】向SQL中插入数据
	bool b = false;
	for(auto filename : AllFileName)
	{
		b = ENVtoMYSQL(filename);
	}
	return b;
}

bool DataSQLThread::CNCtoSQL(QString &dir)
{
	// 【1】递归遍历子目录，得到所有*.csv文件路径
	QFileInfoList file_list = GetFileList(dir); // 递归遍历子目录
	QStringList AllFileName;
	for (int i = 0; i < file_list.size(); ++i)
	{
		QFileInfo fileInfo = file_list.at(i);
		AllFileName.append(fileInfo.filePath());
	}

	// 【2】向SQL中插入数据
	bool b = false;
	for(auto filename : AllFileName)
	{
		b = CNCtoMYSQL(filename);
	}
	return b;
}

bool DataSQLThread::DATAfromSQL()
{
	bool b = false;
	// 原始数据导出
	if(isOri == true)
	{
		QStringList out;
		bool b0 = (*jsql).queryData(tableName, startTime, endTime, itemName, itemNum, isStr, out);
		qDebug()<<"查询成功? "<< b0;
		if(b0)
		{
			QString saveFileName;
			startTime = TimeToTimeNum(startTime);
			endTime   = TimeToTimeNum(endTime);
			saveFileName = startTime +"~"+endTime +"_"+tableName+"_OriData.csv";
			qDebug()<<"saveFileName "<<saveFileName;
			b = JIO::save(saveFileName,out);
		}
		else
			b = false;
	}
	// 优化数据导出
	if(isOri == false)
	{
		// 这三类数据不需要优化
		if(tableName == "ccd" || tableName == "environment" || tableName == "cnc")
		{
			QStringList out;
			bool b0 = (*jsql).queryData(tableName, startTime, endTime, itemName, itemNum, isStr, out);
			qDebug()<<"查询成功? "<<b0;
			if(b0)
			{
				QString saveFileName;
				startTime = TimeToTimeNum(startTime);
				endTime   = TimeToTimeNum(endTime);
				saveFileName = startTime +"~"+endTime +"_"+tableName+"_OptData.csv";
				qDebug()<<"saveFileName "<<saveFileName;
				b = JIO::save(saveFileName,out);
			}
			else
				b = false;
		}
		// 电类数据除去最后为零的通道
		if(tableName == "ds18b20")
		{
			QStringList out;
			bool b0 = (*jsql).queryData(tableName, startTime, endTime, itemName, itemNum, isStr, out);
			qDebug()<<"查询成功? "<< b0;
			if(b0)
			{
				// 除去后续无用通道
				for(auto &e: out)
				{
					QStringList templist;
					templist = e.split(",");
					for(int i=0;i<12;++i) // 空余的12个
					{
						templist.removeLast();
					}
					QString joinStr = templist.join(",");
					e = joinStr;
					//				qDebug()<<e;
				}
				QString saveFileName;
				startTime = TimeToTimeNum(startTime);
				endTime   = TimeToTimeNum(endTime);
				saveFileName = startTime +"~"+endTime +"_"+tableName+"_OptData.csv";
				qDebug()<<"saveFileName "<<saveFileName;
				b = JIO::save(saveFileName,out);
			}
			else
				b = false;
		}
		// fbg数据除去没记录的通道
		if(tableName == "fbg")
		{
			QStringList out;
			bool b0 = (*jsql).queryData(tableName, startTime, endTime, itemName, itemNum, isStr, out);
			qDebug()<<"查询成功? "<< b0;
			if(b0)
			{
				// 提取数据转mat矩阵
				out.removeFirst(); // 去除抬头
				mat inputMat(out.size(),640); // FBG矩阵
				inputMat.fill(0.0);
				int rows = 0;
				for(auto &e: out)
				{
					QStringList templist;
					templist = e.split(",");
					for(int i=1; i<641; ++i) // 除去时间第一个，剩下640个
					{
						inputMat(rows,i-1) = templist[i].toDouble();
					}
					rows++;
				}
				// 全部mat数据优化成321个通道
				mat outputMat;
				bool b1 = ALLFBGto321FBG(inputMat, outputMat);
				// 构造输出数据
				QStringList outData;
				rows = 0;
				for(auto &e: out)
				{
					QStringList templist;
					templist = e.split(",");
					outData.append(templist[0]+",");
					for(unsigned int i=0; i<outputMat.n_cols; ++i)
					{
						QString value;
						value.sprintf("%.3lf",outputMat(rows,i));
						if( i+1 != outputMat.n_cols)
							outData[rows] += value + ",";
						else
							outData[rows] += value;
					}
					rows++;
				}
				outData.insert(0,"Time,"+DataName_FBGST);

				// 保存数据
				QString saveFileName;
				startTime = TimeToTimeNum(startTime);
				endTime   = TimeToTimeNum(endTime);
				saveFileName = startTime +"~"+endTime +"_"+tableName+"_OptData.csv";
				qDebug()<<"saveFileName "<<saveFileName;
				b = JIO::save(saveFileName,outData);
				if(b && b1)
					b = true;
			}
			else
				b = false;
		}
	}
	return b;
}

// 递归遍历子目录
QFileInfoList DataSQLThread::GetFileList(QString path)
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
QStringList DataSQLThread::FBGDir2FileName(QString DirName)
{
	QStringList FBGChannelNames;
	for(int i=1;i<=32;++i)
	{
		FBGChannelNames.append(DirName+"/通道"+QString::number(i)+".csv");
	}
	return FBGChannelNames;
}

// DS18B20目录名转为精确到通道csv文件名
QStringList DataSQLThread::DS18B20Dir2FileName(QString DirName)
{
	QStringList DS18B20ChannelNames;
	for(int i=1;i<=8;++i)
	{
		DS18B20ChannelNames.append(DirName+"/通道"+QString::number(i)+".csv");
	}
	return DS18B20ChannelNames;
}

bool DataSQLThread::CCDtoMYSQL(QString fileName)
{
	QList<QList<QString>> CCDStrList = JIO::readCsv(fileName);
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
		bOut = (*jsql).insertData("ccd",onedata);
		emit sendProgressBar(100.0/CCDStrList.size()*i, 100);
	}
	return bOut;

}

bool DataSQLThread::DS18B20toMYSQL(QStringList DS18B20ChannelNames)
{
	// 获得大Mat
	mat All_Mat = Stand_DS18B20(DS18B20ChannelNames);

	// 格式化时间 由2017-6-7-12:03:14变为2017-6-7 12:03:14
	QList<QList<QString>> DS18B20ChanneMat = JIO::readCsv(DS18B20ChannelNames[0]);
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
		bOut = (*jsql).insertData("ds18b20",onedata);
		emit sendProgressBar(100.0/DS18B20StrList.size()*i, 100);
	}
	return bOut;

}

bool DataSQLThread::FBGtoMYSQL(QStringList FBGChannelNames)
{
	// 获得大Mat
	mat All_Mat = Stand_FBG(FBGChannelNames);

	// 格式化时间 由2017-6-7-12:03:14变为2017-6-7 12:03:14
	QList<QList<QString>> FBGChanneMat = JIO::readCsv(FBGChannelNames[0]);
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
		bOut = (*jsql).insertData("fbg",onedata);
		emit sendProgressBar(100.0/FBGStrList.size()*i, 100);
	}
	return bOut;

}

bool DataSQLThread::ENVtoMYSQL(QString fileName)
{
	QList<QList<QString>> ENVStrList = JIO::readCsv(fileName);
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
		bOut = (*jsql).insertData("environment",onedata);
		emit sendProgressBar(100.0/ENVStrList.size()*i, 100);
	}
	return bOut;
}

bool DataSQLThread::CNCtoMYSQL(QString fileName)
{
	QList<QList<QString>> CNCStrList = JIO::readCsv(fileName);
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
		bOut = (*jsql).insertData("cnc",onedata);
		emit sendProgressBar(100.0/CNCStrList.size()*i, 100);
	}
	return bOut;
}


void DataSQLThread::run()
{
	qDebug()<<"[DataSQLThread start] processingType:" << processingType <<" dataType:"<<dataType;
	QTime time;time.start();
	emit sendProgressBar(1, 100);
	bool b = false;

	// 数据处理-导入
	if(processingType == "in")
	{
		if(dataType == "ccd")
		{
			b = CCDtoSQL(dir);
		}
		if(dataType == "ds18b20")
		{
			b = DS1toSQL(dir);
		}
		if(dataType == "fbg")
		{
			b = FBGtoSQL(dir);
		}
		if(dataType == "environment")
		{
			b = ENVtoSQL(dir);
		}

		if(dataType == "cnc")
		{
			b = CNCtoSQL(dir);
		}
	}

	// 数据处理-导出
	if(processingType == "out")
	{
		b = DATAfromSQL();
	}

	// 数据处理-其它
	if(processingType == "other")
	{

	}

	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		emit sendMsg("处理成功！ <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		emit sendMsg("<span style='color: rgb(255, 0, 0);'>处理失败！</span>");

	emit sendProgressBar(100, 100);
	qDebug()<<"[DataSQLThread end]";
}
