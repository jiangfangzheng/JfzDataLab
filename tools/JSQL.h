#ifndef JSQL_H
#define JSQL_H

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class JSQL:public QObject
{
	Q_OBJECT
private:
	QString HostName;
	QString DatabaseName;
	QString UserName;
	QString Password;
	QString DatabaseType;
	QSqlDatabase db;
public:
	JSQL(QString HostName_, QString DatabaseName_, QString UserName_= "root", QString Password_= "root", QString DatabaseType_ = "QMYSQL");
	JSQL();
	bool insertData(QString tableName, QStringList Data);
	bool queryData(QString tableName, QString startTime, QString endTime,QString itemName, int itemNum, bool isStr, QStringList &out);
	void closeDatabase();
signals:
	void sendMsg(QString msg);							// 显示消息
	void sendProgressBar(qint64 already, qint64 total); // 显示进度
};

#endif // JSQL_H
