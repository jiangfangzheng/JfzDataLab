#ifndef JSQL_H
#define JSQL_H

#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class JSQL
{
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
};

#endif // JSQL_H
