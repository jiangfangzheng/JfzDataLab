#include "DataProcessingThread.h"
#include <QDebug>
#include <QTime>

DataProcessingThread::DataProcessingThread()
{
	fileName1 = "";
	fileName2 = "";
}

DataProcessingThread::~DataProcessingThread()
{
	// 请求终止
	qDebug()<<"[~DataProcessingThread]";
	requestInterruption();
	quit();
	wait();
}

DataProcessingThread::DataProcessingThread(QString fileName1, std::function<bool (QString)> fun)
{
	this->fileName1 = fileName1;
	fun1 = fun;
	nameCnt = 1;
}

DataProcessingThread::DataProcessingThread(QString fileName1, QString fileName2, bool (*fun)(QString, QString))
{
	this->fileName1 = fileName1;
	this->fileName2 = fileName2;
	fun2 = fun;
	nameCnt = 2;
}

DataProcessingThread::DataProcessingThread(QStringList fileNameList, bool (*fun)(QStringList))
{
	this->fileNameList = fileNameList;
	funList = fun;
	nameCnt = 3;
}

void DataProcessingThread::run()
{
	qDebug()<<"[DataProcessingThread start] nameCnt:" << nameCnt;
	QTime time;time.start();
	emit sendProgressBar(1, 100);
	bool b = false;

	switch (nameCnt)
	{
	case 1:
		b = fun1(fileName1);
		break;
	case 2:
		b = fun2(fileName1, fileName2);
		break;
	case 3:
		b = funList(fileNameList);
		break;
	default:
		break;
	}

	emit sendProgressBar(99, 100);
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		emit sendMsg("处理成功！ <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		emit sendMsg("<span style='color: rgb(255, 0, 0);'处理失败！</span>");

	emit sendProgressBar(100, 100);
	qDebug()<<"[DataProcessingThread end]";
}
