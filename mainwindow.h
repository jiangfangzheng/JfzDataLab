#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QFileDialog>      //引用文件浏览对话框类
#include <QDebug>
#include <QAxObject>
#include <QMessageBox>
#include <QTime>
#include <QUrl>
#include <math.h>
#include "tools/EnvXlsReadThread.h"
#include "plugins/qcustomplot.h"
#include "systemTray.h"

class QNetworkReply;
class QNetworkAccessManager;

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
private:
	Ui::MainWindow *ui;

// 托盘相关
private:
	SystemTray * myTray; //自定义系统托盘
private slots:
	void ShowWindow(); //处理还原操作
	void SystemTrayActivated(QSystemTrayIcon::ActivationReason reason); //处理点击托盘操作

// 数据处理相关
private slots:
    void on_pushButton_DS18B20_clicked();
    void on_pushButton_CCD_clicked();
    void on_pushButton_FBGT_clicked();
    void on_pushButton_FBGS_clicked();

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_ENV_clicked();

    void on_pushButton_LoadFBGT_clicked();

//    void on_pushButton_wave2temp_clicked();

    void on_pushButton_File1_clicked();

    void on_pushButton_File2_clicked();

    void on_pushButton_covresult_clicked();

	void on_pushButton_LoadFBGT_ALL_clicked();

	void on_comboBox_skin_currentIndexChanged(const QString &arg1);

	void on_pushButton_LinearRegression_clicked();

	void on_pushButton_LoadPlotData_clicked();

	void on_comboBox_LoadPlotList_currentTextChanged(const QString &arg1);

	void on_pushButton_CCDinSQL_clicked();

	void on_pushButton_FBGinSQL_clicked();

	void on_pushButton_DS18BinSQL_clicked();

	void on_pushButton_ENVinSQL_clicked();

	void on_pushButton_CNCinSQL_clicked();

private:
	// 工作区路径
	QString workspacePath;
	// Skins
	QMap<QString, QString> mapStyle;
	void initSkins();
	// Data
    mat MatDS18B20;
    mat MatCCD;
    mat MatFBGT;
    mat MatFBGS;
    mat MatENV;
    QStringList EnvFileNameList;
    QString standFBGFileName;// 标准FBG温度波长文件
    QString correlationFileName1;
    QString correlationFileName2;
	QList<QList<QString>> plotDataStrList; // 绘图载入原始数据
	QList<double> ModelMatQList; // 模型参数文件
	QString DataBaseType; // 数据库类型
	// JPlot
	void JfzPlot(QVector<double> MatData, QString PicName, int TuNum, QColor Colorstyle, double ymax, double ymin);
	// 历史数据库下载
	QNetworkAccessManager *managerDatabase;
	QNetworkReply         *replyDatabase;
	QUrl                  urlDatabase;
	QFile                 *fileDatabase;
	QProcess              runDatabaseProcess;
	QNetworkAccessManager *managerCheckDatabaseUpdate;
	void startRequest(QUrl url);

// 最下方显示信息标签
signals:
	sendMsg(QString msg);
private slots:
	void showMsg(QString msg);
	void on_pushButton_SelectModel_clicked();
	void on_pushButton_SelectData_clicked();
	void on_pushButton_OutFromSQL_clicked();
	void on_pushButton_UpdateSQL_clicked();
	// 历史数据库下载
	void httpFinished();
	void httpReadyRead();
	void updateDataReadProgress(qint64, qint64);
	void runDatabaseProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void checkDatabaseUpdateFinished(QNetworkReply *);
};

#endif // MAINWINDOW_H
