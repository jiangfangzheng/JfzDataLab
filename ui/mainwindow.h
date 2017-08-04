#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

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

// 【自己写的】
private:
	//自定义系统托盘
	SystemTray * myTray;
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

	QString correlationFileName1;
	QString correlationFileName2;
	QString ModelFile; // 模型文件

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

signals:
	// 最下方显示信息标签
	void sendMsg(QString msg);
	void sendProgressBar(qint64 already, qint64 total);

private slots:
	// 托盘相关
	void ShowWindow(); //处理还原操作
	void SystemTrayActivated(QSystemTrayIcon::ActivationReason reason); //处理点击托盘操作
	// 最下方进度条
	void updateProgressBar(qint64, qint64);
	// 历史数据库下载
	void httpFinished();
	void httpReadyRead();
	void updateDataReadProgress(qint64, qint64);
	void runDatabaseProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void checkDatabaseUpdateFinished(QNetworkReply *);
	// 显示信息
	void showMsg(QString msg);


// 【系统生成的】
private slots:
	void on_pushButton_DS18B20_clicked();
	void on_pushButton_CCD_clicked();
	void on_pushButton_FBGS_clicked();
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();
	void on_pushButton_ENV_clicked();
	void on_pushButton_LoadFBGT_clicked();
	void on_pushButton_File1_clicked();
	void on_pushButton_File2_clicked();
	void on_pushButton_covresult_clicked();
	void on_pushButton_LoadFBGT_ALL_clicked();
	void on_comboBox_skin_currentIndexChanged(const	QString	&arg1);
	void on_pushButton_LinearRegression_clicked();
	void on_pushButton_LoadPlotData_clicked();
	void on_comboBox_LoadPlotList_currentTextChanged(const QString &arg1);
	void on_pushButton_CCDinSQL_clicked();
	void on_pushButton_FBGinSQL_clicked();
	void on_pushButton_DS18BinSQL_clicked();
	void on_pushButton_ENVinSQL_clicked();
	void on_pushButton_CNCinSQL_clicked();
	void on_pushButton_SelectModel_clicked();
	void on_pushButton_SelectData_clicked();
	void on_pushButton_OutFromSQL_clicked();
	void on_pushButton_UpdateSQL_clicked();
	void on_pushButton_DataZero_clicked();
	void on_pushButton_DataDelta_clicked();
	void on_pushButton_DataSampling_clicked();
	void on_pushButton_DataClean_clicked();
	void on_pushButton_DataTendency_clicked();
	void on_pushButton_VirtualMap_T_clicked();
	void on_pushButton_SplitByDate_clicked();
	void on_pushButton_VirtualMap_S_clicked();
	void on_pushButton_VirtualMap_DS18_clicked();
	void on_pushButton_VirtualFBGtoTEMP_clicked();
	void on_pushButton_VirtualFBGtoSTRESS_clicked();
	void on_pushButton_OriDS18toVirtual_clicked();
	void on_pushButton_OriENVtoVirtual_clicked();
	void on_pushButton_OriFBGtoVirtual_clicked();
	void on_pushButton_CSVmerge_clicked();
};

#endif // MAINWINDOW_H
