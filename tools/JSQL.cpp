#include "JSQL.h"

JSQL::JSQL(QString HostName_, QString DatabaseName_, QString UserName_ , QString Password_, QString DatabaseType_):HostName(HostName_), DatabaseName(DatabaseName_), UserName(UserName_), Password(Password_), DatabaseType(DatabaseType_)
{
	qDebug()<<"connect Database:";
	qDebug()<<DatabaseType;
	qDebug()<<DatabaseName;
	if(DatabaseType == "QMYSQL")
	{
		db = QSqlDatabase::addDatabase(DatabaseType);
		db.setHostName(HostName);
		db.setDatabaseName(DatabaseName);
		db.setUserName(UserName);
		db.setPassword(Password);
	}
	if(DatabaseType == "QSQLITE")
	{
		//实例 QSqlDatabase
		db = QSqlDatabase::addDatabase(DatabaseType);
		//数据库名称
		db.setDatabaseName(DatabaseName+".db");
	}
	if (!db.open())
		qDebug() << "Failed to connect to Database.";
	else
		qDebug()<<"connect Database success!";
}

JSQL::JSQL():JSQL("localhost","jfz")
{
}

bool JSQL::insertData(QString tableName, QStringList Data)
{
	QSqlQuery q(db);
	QString strData;
	for(auto e:Data)
	{
		strData +="'"+e+"',";
	}
	strData = strData.left(strData.length() - 1);
//	qDebug()<<strData;
	QString strSql = "insert into " + tableName + " values (" + strData + ")";
	return q.exec(strSql);
}


