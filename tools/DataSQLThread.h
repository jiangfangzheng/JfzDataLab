#ifndef DATASQLTHREAD_H
#define DATASQLTHREAD_H

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

#include "tools/JSQL.h"
#include <QThread>
#include <QFileInfoList>

class DataSQLThread : public QThread
{
	Q_OBJECT
public:
	DataSQLThread();
	~DataSQLThread();
	DataSQLThread(QString dir_, JSQL *jsql_, QString dataType_, QString processingType_); // 输入目录，处理数据类型，导入还是导出还是其它 in out other
	void setQueryMsg(QString tableName, QString startTime, QString endTime, QString itemName, int itemNum, bool isStr,bool isOri);
	void run();
private:
	QString dir;
	JSQL *jsql; // 数据库类地址
	QString dataType;
	QString processingType;
	QString tableName;
	QString startTime;
	QString endTime;
	QString itemName;
	int itemNum;
	bool isStr;
	bool isOri;
	// 对外接口
	bool CCDtoSQL(QString &dir);
	bool DS1toSQL(QString &dir);
	bool FBGtoSQL(QString &dir);
	bool ENVtoSQL(QString &dir);
	bool CNCtoSQL(QString &dir);
	bool DATAfromSQL(void);
	// 内部细处理
	QFileInfoList GetFileList(QString path);			// 递归遍历子目录
	QStringList FBGDir2FileName(QString DirName);		// FBG目录名转为精确到通道csv文件名
	QStringList DS18B20Dir2FileName(QString DirName);	// DS18B20目录名转为精确到通道csv文件名
	bool CCDtoMYSQL(QString fileName);
	bool DS18B20toMYSQL(QStringList DS18B20ChannelNames);
	bool FBGtoMYSQL(QStringList FBGChannelNames);
	bool ENVtoMYSQL(QString fileName);
	bool CNCtoMYSQL(QString fileName);

signals:
	void sendMsg(QString msg);							// 显示消息
	void sendProgressBar(qint64 already, qint64 total); // 显示进度
};

#endif // DATASQLTHREAD_H
