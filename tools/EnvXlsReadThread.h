#ifndef ENVXLSREADTHREAD_H
#define ENVXLSREADTHREAD_H
#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"


class EnvXlsReadThread : public QThread
{
public:
    EnvXlsReadThread();
    EnvXlsReadThread(QStringList EnvFileNameList, Ui::MainWindow* ui);
    void run();
private slots:
	void on_pushButton_wave2temp_clicked();

	void on_pushButton_FBGT_clicked();

private:
    QStringList EnvFileNameList;
    Ui::MainWindow *ui;
};

#endif // ENVXLSREADTHREAD_H
