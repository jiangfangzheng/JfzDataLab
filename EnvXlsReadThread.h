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
private:
    QStringList EnvFileNameList;
    Ui::MainWindow *ui;
};

#endif // ENVXLSREADTHREAD_H
