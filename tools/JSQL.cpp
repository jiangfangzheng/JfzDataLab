#include "JSQL.h"
#include <QSqlDriver>

JSQL::JSQL(QString HostName_, QString DatabaseName_, QString UserName_ , QString Password_, QString DatabaseType_):HostName(HostName_), DatabaseName(DatabaseName_), UserName(UserName_), Password(Password_), DatabaseType(DatabaseType_)
{
	qDebug()<<"连接数据库:";
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
		qDebug() << "连接数据库失败！";
	else
		qDebug()<<"连接数据库："<< DatabaseType <<"成功！";
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

bool JSQL::queryData(QString tableName, QString startTime, QString endTime,QString itemName, int itemNum, bool isStr, QStringList &out)
{
	// 输出数据整理
	out.append(itemName); // 抬头
	// 开始查询
	QSqlQuery sql_query;
	sql_query.prepare("SELECT * FROM "+ tableName +" WHERE Time BETWEEN ? AND ?");
	sql_query.addBindValue(startTime);
	sql_query.addBindValue(endTime);
	if(!sql_query.exec())
	{
		qDebug()<<sql_query.lastError();
		return false;
	}
	else
	{
		// 先判断该数据库驱动是否支持QuerySize特性，如果支持，则可以使用size()函数
		// 如果不支持，那么就使用其他方法来获取总行数
		int numRows = 0;
		if (db.driver()->hasFeature(QSqlDriver::QuerySize))
		{
			numRows = sql_query.size();
			qDebug() << "[可以]用“显示查询大小”的特性：" << numRows;
		}
		else
		{
			sql_query.last();
			numRows = sql_query.at() + 1;
			qDebug() << "[不能]用“显示查询大小”的特性："<< numRows;
		}

		int cntProgress = 0;
		while(sql_query.next())
		{
			QString lineData;
			QString value;
			for(int i=0; i<itemNum; ++i)
			{
				if(i==0)
				{
					value = sql_query.value(i).toString();
					value[10] = ' '; // 去掉日期中的T
				}
				else
				{
					if(isStr)
						value = sql_query.value(i).toString();
					else
					{
						double dvalue = sql_query.value(i).toDouble();
						value.sprintf("%.3lf",dvalue);
					}
				}
				lineData += value+ ",";
			}
			lineData = lineData.left(lineData.lastIndexOf(',')); // 去掉最后的,
			out.append(lineData);
//			qDebug() << lineData;
			// 显示进度
			emit sendProgressBar(100.0/numRows*cntProgress, 100);
			cntProgress++;
		}
//		qDebug() << out[0];
//		qDebug() << out[1];
	}
	return true;
}


void JSQL::closeDatabase()
{
	db.close();
}
