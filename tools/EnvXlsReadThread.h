#ifndef ENVXLSREADTHREAD_H
#define ENVXLSREADTHREAD_H
#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"


class EnvXlsReadThread : public QThread
{
	Q_OBJECT
public:
    EnvXlsReadThread();
	EnvXlsReadThread(QStringList EnvFileNameList);
	void run();
private:
	QStringList EnvFileNameList;
signals:
	sendMsg(QString msg);
};

#endif // ENVXLSREADTHREAD_H
