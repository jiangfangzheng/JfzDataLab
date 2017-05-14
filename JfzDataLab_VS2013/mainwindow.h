#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QFileDialog>      //引用文件浏览对话框类
#include <QDebug>
#include <QAxObject>
#include <QMessageBox>
#include <QTime>
#include <math.h>
#include "EnvXlsReadThread.h"


// armadillo矩阵库
#include <armadillo>
using namespace arma;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    friend class EnvXlsReadThread;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_DS18B20_clicked();
    void on_pushButton_CCD_clicked();
    void on_pushButton_FBGT_clicked();
    void on_pushButton_FBGS_clicked();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_ENV_clicked();

    void on_pushButton_LoadFBGT_clicked();

    void on_pushButton_wave2temp_clicked();

    void on_pushButton_File1_clicked();

    void on_pushButton_File2_clicked();

    void on_pushButton_covresult_clicked();

private:
    Ui::MainWindow *ui;
    mat MatDS18B20;
    mat MatCCD;
    mat MatFBGT;
    mat MatFBGS;
    mat MatENV;
    QStringList EnvFileNameList;
    QString standFBGFileName;// 标准FBG温度波长文件
    QString correlationFileName1;
    QString correlationFileName2;
};

#endif // MAINWINDOW_H
