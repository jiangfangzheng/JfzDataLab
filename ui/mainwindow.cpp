﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "skins/skins.h"
#include "tools/JIO.h"
#include "tools/JSQL.h"
#include "tools/DataProcessingThread.h"
#include "tools/DataSQLThread.h"
#include "algorithm/DataProcessing.h"
#include "plugins/qcustomplot.h"
#include "SetUI.h"
#include <QtNetwork>

// 保存文件路径
QString saveFilePath;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	// 工作区\存文件 路径
	workspacePath = QDir::currentPath() + "/workspace/";
	saveFilePath =  "E:/[Data]/output/";
	// 数据处理类型 csv（0）  mat（1）
	dataProType = 0;
	// 初始化Skin
	this->initSkins();
	// 初始化qcustomplot
	ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	// 初始化托盘
	myTray = new SystemTray(this);
	connect(myTray, SIGNAL(showWidget()), this, SLOT(ShowWindow()));//关联信号和槽函数
	connect(myTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(SystemTrayActivated(QSystemTrayIcon::ActivationReason)));
	// 最下方-消息提醒
	connect(this, SIGNAL(sendMsg(QString)), this, SLOT(showMsg(QString)));
	// 最下方-进度条
	ui->progressBar->hide();
	connect(this, &MainWindow::sendProgressBar, this, &MainWindow::updateProgressBar);
	// 历史数据库下载相关
	managerDatabase = new QNetworkAccessManager(this);
	ui->progressBar_UpdateDatabase->hide();
	connect(&runDatabaseProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(runDatabaseProcessFinished(int, QProcess::ExitStatus)));
	managerCheckDatabaseUpdate = new QNetworkAccessManager(this);
	connect(managerCheckDatabaseUpdate, &QNetworkAccessManager::finished, this, &MainWindow::checkDatabaseUpdateFinished);
	// tableWidget默认隐藏;
//	ui->tableWidget->hide();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showMsg(QString msg)
{
	ui->label_msg->setText(msg);
	qDebug()<<"[showMsg]:"<<msg;
}

// 初始化皮肤
void MainWindow::initSkins()
{
	// 对应的主题样式
	mapStyle["系统"]   = QString(":/qss/sys.css");
	mapStyle["黑色"]   = QString(":/qss/black.css");
	mapStyle["灰黑色"] = QString(":/qss/brown.css");
	mapStyle["灰色"]   = QString(":/qss/gray.css");
	mapStyle["浅灰色"] = QString(":/qss/lightgray.css");
	mapStyle["深灰色"] = QString(":/qss/darkgray.css");
	mapStyle["银色"]   = QString(":/qss/silvery.css");
	mapStyle["淡蓝色"] = QString(":/qss/blue.css");
	mapStyle["蓝色"]   = QString(":/qss/dev.css");
	mapStyle["PS黑"]   = QString(":/qss/psblack.css");
	// 下拉框选择
	QStringList qssName;
//	qssName << "系统" << "黑色" << "灰黑色" << "灰色" << "浅灰色" << "深灰色" << "银色" << "淡蓝色" << "蓝色"<< "PS黑";
	qssName << "系统" << "银色" << "淡蓝色";
	ui->comboBox_skin->addItems(qssName);
	ui->comboBox_skin->setCurrentIndex(1);// 个人最喜欢银，默认
	// 直接赋值
	QString qssFile = mapStyle["银色"];
	Skins::setStyle(qssFile);
}

// 皮肤切换
void MainWindow::on_comboBox_skin_currentIndexChanged(const QString &arg1)
{
	QString qssName = arg1.trimmed();
	QString qssFile = mapStyle[qssName];
	if (qssFile.trimmed() == "")
		qssFile = ":/qss/sys.css";
	Skins::setStyle(qssFile);
}

// 托盘相关
void MainWindow::ShowWindow()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
}

// 托盘菜单
void MainWindow::SystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
	case QSystemTrayIcon::Trigger:
	{
		ShowWindow();
		break;
	}
	case QSystemTrayIcon::DoubleClick:
	{
		ShowWindow();
		break;
	}
	default:
		break;
	}
}

// 最下方更新进度条
void MainWindow::updateProgressBar(qint64 bytesRead, qint64 totalBytes)
{
	qDebug()<<"[Enter updateProgressBar]" << bytesRead << " "<< totalBytes;
	if(bytesRead > 0)
		ui->progressBar->show();
//	if(bytesRead == totalBytes)
//		ui->progressBar->hide();

	ui->progressBar->setMaximum(totalBytes);
	ui->progressBar->setValue(bytesRead);
}

/**************************************** 第1页 功能 ****************************************/
// 电类标准化
void MainWindow::on_pushButton_DS18B20_clicked()
{
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open DS18B20 Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// 电类标准化 standardDS18B20
		DataProcessingThread *dataPro = new DataProcessingThread(dir, standardDS18B20);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// CCD标准化
void MainWindow::on_pushButton_CCD_clicked()
{
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open CCD Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// CCD标准化 standardCCD
		DataProcessingThread *dataPro = new DataProcessingThread(dir, standardCCD);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// FBG温度标准化（预处理-波长修复）
void MainWindow::on_pushButton_LoadFBGT_ALL_clicked()
{
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open FBGT Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// FBG温度标准化（预处理-波长修复） standardFBGT
		DataProcessingThread *dataPro = new DataProcessingThread(dir, standardFBGT);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// FBG应力标准化
void MainWindow::on_pushButton_FBGS_clicked()
{
	// 载入文件夹 ，填"/"跳到根目录， 填“”默认程序位置
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open FBGS Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// FBG应力标准化 standardFBGS
		DataProcessingThread *dataPro = new DataProcessingThread(dir, standardFBGS);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 环境温度标准化
void MainWindow::on_pushButton_ENV_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("打开环境温度数据"), workspacePath, tr("textfile(*.xls);"));
	if(fileNameList.size() != 4)
	{
		emit sendMsg("文件状态：未载入");
		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个文件！", QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		// 环境温度标准化 standardENV
		DataProcessingThread *dataPro = new DataProcessingThread(fileNameList, standardENV);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::sendProgressBar, this, &MainWindow::updateProgressBar);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &DataProcessingThread::deleteLater);
	}
}

// FBG波长转温度
void MainWindow::on_pushButton_LoadFBGT_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开标准FBG波长"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// FBG波长转温度 FBGTtoTEMP
		DataProcessingThread *dataPro = new DataProcessingThread(fileName, FBGTtoTEMP);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 相关性分析-文件1
void MainWindow::on_pushButton_File1_clicked()
{
	correlationFileName1 = QFileDialog::getOpenFileName(this, tr("打开文件1"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!correlationFileName1.isEmpty() && !correlationFileName2.isEmpty())
	{
		ui->label_msg->setText("选了文件1");
		ui->pushButton_covresult->setEnabled(true);
	}
	else
	{
		ui->pushButton_covresult->setEnabled(false);
	}
}

// 相关性分析-文件2
void MainWindow::on_pushButton_File2_clicked()
{
	correlationFileName2 = QFileDialog::getOpenFileName(this, tr("打开文件1"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	qDebug()<<correlationFileName2;
	if(!correlationFileName1.isEmpty() && !correlationFileName2.isEmpty())
	{
		ui->label_msg->setText("选了文件2");
		ui->pushButton_covresult->setEnabled(true);
	}
	else
	{
		ui->pushButton_covresult->setEnabled(false);
	}
}

// 相关性分析
void MainWindow::on_pushButton_covresult_clicked()
{
	// 相关性分析 correlationAnalysis
	DataProcessingThread *dataPro = new DataProcessingThread(correlationFileName1, correlationFileName2, correlationAnalysis);
	dataPro->start();
	connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
	connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
}

// 多元线性回归
void MainWindow::on_pushButton_LinearRegression_clicked()
{
	ui->label_msg->setText("未选择文件！");
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开多元线性回归数据源"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 多元线性回归 LinearRegression
		DataProcessingThread *dataPro = new DataProcessingThread(fileName, LinearRegression);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 选择模型
void MainWindow::on_pushButton_SelectModel_clicked()
{
	ModelMatQList.clear();
	ModelFile = QFileDialog::getOpenFileName(this, tr("打开模型"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!ModelFile.isEmpty())
	{
		QList<QList<double>> ModelMat = JIO::readMat(ModelFile);
		for(auto &e:ModelMat[0])
		{
			ModelMatQList.append(e);
		}
		qDebug()<<"ModelMatQList.size() "<<ModelMatQList.size();
		if( ModelMat.size() == 1 && ModelMatQList.size() > 0)
		{
			emit sendMsg("模型载入成功！");
			ui->pushButton_SelectData->setEnabled(true);
		}
		else
		{
			emit sendMsg("模型不正确！");
			ui->pushButton_SelectData->setEnabled(false);
		}
	}
}

// 选择数据（模型数据来预测）
void MainWindow::on_pushButton_SelectData_clicked()
{
	QString DataFile = QFileDialog::getOpenFileName(this, tr("选择来预测的数据"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!DataFile.isEmpty())
	{
		QList<QList<double>> DataFileMat = JIO::readMat(DataFile);
		if(DataFileMat[0].size() != (ModelMatQList.size()-1) )
		{
			emit sendMsg("模型、预测数据不匹配！");
		}
		else
		{
			// 清空比较数据
			ModelMatQList.clear();
			// 计算预测值 LinearRegressionPredict
			DataProcessingThread *dataPro = new DataProcessingThread(ModelFile, DataFile, LinearRegressionPredict);
			dataPro->start();
			connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
			connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
		}
	}
}

// 数据小处理-初始值为0
void MainWindow::on_pushButton_DataZero_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("数据小处理-初始值为0"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 初始值为0算法 dataZero
		DataProcessingThread *dataPro = nullptr;
		if(dataProType == 0)
			dataPro = new DataProcessingThread(fileName, dataZeroCsv);
		if(dataProType == 1)
			dataPro = new DataProcessingThread(fileName, dataZeroMat);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
		connect(dataPro, &DataProcessingThread::sendProgressBar, this, &MainWindow::updateProgressBar);
	}
}

// 数据小处理-增量化（后一个数-前一个数）
void MainWindow::on_pushButton_DataDelta_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("数据小处理-增量化"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 增量化算法 dataDelta
		DataProcessingThread *dataPro = nullptr;
		if(dataProType == 0)
			dataPro = new DataProcessingThread(fileName, dataDeltaCsv);
		if(dataProType == 1)
			dataPro = new DataProcessingThread(fileName, dataDeltaMat);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 数据小处理-压缩拉伸
void MainWindow::on_pushButton_DataSampling_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("数据小处理-压缩拉伸"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		int needNum = ui->lineEdit_DataSampling->text().toInt();
		int nowNum = 0;
		// 参数绑定
		auto dataSampling1 = std::bind(dataSamplingMat, std::placeholders::_1, nowNum, needNum);
		auto dataSampling1Csv = std::bind(dataSamplingCsv, std::placeholders::_1, nowNum, needNum);
		// 压缩拉伸算法 dataSampling
		DataProcessingThread *dataPro = nullptr;
		if(dataProType == 0)
			dataPro = new DataProcessingThread(fileName, dataSampling1Csv);
		if(dataProType == 1)
			dataPro = new DataProcessingThread(fileName, dataSampling1);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 数据小处理-数据清洗(设定阈值内数据，前一个值覆盖后一个值)
void MainWindow::on_pushButton_DataClean_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("数据小处理-数据清洗"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		double maxNum = ui->lineEdit_DataCleanMax->text().toDouble();
		double minNum = ui->lineEdit_DataCleanMin->text().toDouble();
		// 参数绑定
		auto dataClean1 = std::bind(dataCleanMat, std::placeholders::_1, maxNum, minNum);
		auto dataClean1Csv = std::bind(dataCleanCsv, std::placeholders::_1, maxNum, minNum);
		// 数据清洗算法 dataClean
		DataProcessingThread *dataPro = nullptr;
		if(dataProType == 0)
			dataPro = new DataProcessingThread(fileName, dataClean1Csv);
		if(dataProType == 1)
			dataPro = new DataProcessingThread(fileName, dataClean1);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 数据小处理-趋势预测
void MainWindow::on_pushButton_DataTendency_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("数据小处理-趋势预测"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		int window = ui->lineEdit_DataWindow->text().toInt();
		// 参数绑定
		auto dataTendency1 = std::bind(dataTendencyMat, std::placeholders::_1, window);
		auto dataTendency1Csv = std::bind(dataTendencyCsv, std::placeholders::_1, window);
		// 数据清洗算法 dataClean
		DataProcessingThread *dataPro = nullptr;
		if(dataProType == 0)
			dataPro = new DataProcessingThread(fileName, dataTendency1Csv);
		if(dataProType == 1)
			dataPro = new DataProcessingThread(fileName, dataTendency1);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 数据小处理-按天拆分
void MainWindow::on_pushButton_SplitByDate_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("数据小处理-按天拆分"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 按天拆分 dataSplitByDate
		DataProcessingThread *dataPro = new DataProcessingThread(fileName, dataSplitByDateCsv);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 数据小处理-csv合并
void MainWindow::on_pushButton_CSVmerge_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("数据小处理-csv合并"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(fileNameList.size() <=1 )
	{
		QMessageBox::critical(NULL, "注意", "请选择2个或以上数目的文件！", QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		// csv合并 csvMerge
		DataProcessingThread *dataPro = new DataProcessingThread(fileNameList, mergeCsv);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 数据小处理-时间规范化
void MainWindow::on_pushButton_TimeStand_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("数据小处理-时间规范化"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 时间规范化 timeStandCsv
		DataProcessingThread *dataPro = new DataProcessingThread(fileName, timeStandCsv);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 虚拟映射-数据通道映射为虚拟标准通道-FBG温度
void MainWindow::on_pushButton_VirtualMap_T_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("虚拟映射-FBG温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	DataProcessingThread *dataPro = new DataProcessingThread(fileNameList, virtualMapFBGT);
	dataPro->start();
	connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
	connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
}

// 虚拟映射-数据通道映射为虚拟标准通道-FBG应力
void MainWindow::on_pushButton_VirtualMap_S_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("虚拟映射-FBG应力"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	DataProcessingThread *dataPro = new DataProcessingThread(fileNameList, virtualMapFBGS);
	dataPro->start();
	connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
	connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
}

// 虚拟映射-数据通道映射为虚拟标准通道-电类温度
void MainWindow::on_pushButton_VirtualMap_DS18_clicked()
{
	// 电类温度不需要交换位置，只需要改抬头
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("虚拟映射-电类温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	DataProcessingThread *dataPro = new DataProcessingThread(fileNameList, virtualMapDS18);
	dataPro->start();
	connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
	connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
}

// 虚拟映射-映射的FBG温度传感器转温度值
void MainWindow::on_pushButton_VirtualFBGtoTEMP_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("FBG转温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	DataProcessingThread *dataPro = new DataProcessingThread(fileNameList, virtualFBGtoTEMP);
	dataPro->start();
	connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
	connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
}

// 虚拟映射-映射的FBG应力传感器转应力
void MainWindow::on_pushButton_VirtualFBGtoSTRESS_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("FBG转温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	DataProcessingThread *dataPro = new DataProcessingThread(fileNameList, virtualFBGtoSTRESS);
	dataPro->start();
	connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
	connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
}

// 虚拟映射-电类环境温度4个
void MainWindow::on_pushButton_DS18_ENVtoVirtual_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("虚拟映射-电类环境温度4个"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// 电类环境温度4个 OriDS18ENVtoVirtual
		DataProcessingThread *dataPro = new DataProcessingThread(dir, OriDS18ENVtoVirtual);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 一键环境温度数据映射
void MainWindow::on_pushButton_OriENVtoVirtual_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("打开环境温度txt"), workspacePath, tr("textfile(*.txt);"));
	if(fileNameList.size() != 4)
	{
		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个txt文件！", QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		DataProcessingThread *dataPro = new DataProcessingThread(fileNameList, OriENVtoVirtual);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 一键原始电类数据映射
void MainWindow::on_pushButton_OriDS18toVirtual_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open DS18B20 Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		DataProcessingThread *dataPro = new DataProcessingThread(dir, OriDS18toVirtual);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}

// 一键原始FBG数据映射
void MainWindow::on_pushButton_OriFBGtoVirtual_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open FBG Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		DataProcessingThread *dataPro = new DataProcessingThread(dir, OriFBGtoVirtual);
		dataPro->start();
		connect(dataPro, &DataProcessingThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataPro, &DataProcessingThread::finished, dataPro, &QObject::deleteLater);
	}
}


/**************************************** 第2页 功能 ****************************************/

// 数据概览-载入数据
void MainWindow::on_pushButton_clicked()
{
	ui->tableWidget->clearContents();
	// 载入文件
	QString fileName = QFileDialog::getOpenFileName(this,"数据概览-载入数据", workspacePath, "textfile(*.csv);;All file(*.*)");
	if(!fileName.isEmpty())
	{
		// 需要变量
		QString value; // 用于数字转字符串显示
		QStringList itemName;
		QStringList timeName;
		// 【0】计时开始
		QTime time;time.start();

		// 【1】数据概览-总体统计
		mat input = JIO::readCsv(fileName, itemName, timeName);
		qDebug()<<"读文件时间 "<< QString::number(time.elapsed()/1000.0);
		ui->label_2->setText("数据行列：" + QString::number(input.n_rows)+ " x "+ QString::number(input.n_cols) );
		ui->label_4->setText("数据名字：" + fileName.right(fileName.size() - fileName.lastIndexOf("/")-1) );
		ui->label_5->setText("数据总和：" + value.sprintf("%.3lf", accu(input)) );
		ui->label_6->setText("数据平均：" + value.sprintf("%.3lf", (accu(input))/(input.n_rows * input.n_cols)) );
		ui->label_7->setText("数据最大：" + value.sprintf("%.3lf", max(max(input))) );
		ui->label_8->setText("数据最小：" + value.sprintf("%.3lf", min(min(input))) );
		// 【2】数据概览-面向列的统计
		// 数据基本信息-处理
		mat colSumMat = sum(input);			// 每一列的总和
		mat maxMat = max(input);			// 每一列的最大值
		mat minMat = min(input);			// 每一列的最小值
		mat maxAbsMat = max(abs(input));	// 每一列的最大值(绝对值)
		mat minAbsMat = min(abs(input));	// 每一列的最小值(绝对值)
		mat avgMat(1, colSumMat.n_cols);	// 算术平均值
		avgMat.fill(0);
		for(auto i=0; i<avgMat.n_cols; ++i)
		{
			avgMat(0,i) = colSumMat(i)/(input.n_rows);
		}
		// 【3】tableWidget控件显示
		ui->tableWidget->horizontalHeader()->setStretchLastSection(true); // 设置表格是否充满，即行末不留空

		/*QHeaderView* headerView = ui->tableWidget->horizontalHeader();
		headerView->setSectionResizeMode(QHeaderView::Stretch);
		*/
		ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 设置tablewidget等宽
		QStringList header;
		header<<"数据名"<<"和"<<"最大值"<<"最大值(绝对值)"<<"最小值"<<"最小值(绝对值)"<<"算术平均值"<<"数据检查";
		ui->tableWidget->setRowCount(input.n_cols); // 数据项数目的表行
		ui->tableWidget->setColumnCount(header.size());
		ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
		ui->tableWidget->setHorizontalHeaderLabels(header);
		for(auto i=0; i<input.n_cols; ++i )
		{
			QTableWidgetItem *Item1 =new QTableWidgetItem( itemName[i+1]                  );
			//		Item1->setTextAlignment(Qt::AlignHCenter);
			QTableWidgetItem *Item2 =new QTableWidgetItem( value.sprintf("%.3lf",colSumMat(i)) );
			QTableWidgetItem *Item3 =new QTableWidgetItem( value.sprintf("%.3lf",maxMat(i))    );
			QTableWidgetItem *Item4 =new QTableWidgetItem( value.sprintf("%.3lf",maxAbsMat(i)) );
			QTableWidgetItem *Item5 =new QTableWidgetItem( value.sprintf("%.3lf",minMat(i))    );
			QTableWidgetItem *Item6 =new QTableWidgetItem( value.sprintf("%.3lf",minAbsMat(i)) );
			QTableWidgetItem *Item7 =new QTableWidgetItem( value.sprintf("%.3lf",avgMat(i))    );

			// 【4】宏观诊断
			QString diagnosisMsg;
			// 宏观诊断-温度数据
			if(ui->radioButton_TempCheck->isChecked())
			{
				if (maxMat(i) - minMat(i) < 15 && avgMat(i) > 0 && avgMat(i) < 50)
					diagnosisMsg = "正常";
				else
				{
					if (fabs(avgMat(i)) < 0.00001)
					{
						diagnosisMsg = "未采集";
					}
					else
					{
						diagnosisMsg = "异常";
					}
				}
			}
			// 宏观诊断-波长
			else if(ui->radioButton_WaveCheck->isChecked())
			{

				if (maxMat(i) - minMat(i) < 3 && avgMat(i) != 0)
					diagnosisMsg = "正常";
				else
				{
					if (fabs(avgMat(i)) < 0.00001)
					{
						diagnosisMsg = "未采集";
					}
					else
					{
						diagnosisMsg = "异常";
					}
				}
			}
			// 宏观诊断-ccd
			else if(ui->radioButton_CCDCheck->isChecked())
			{

				if (fabs(avgMat(i)) < 0.2)
					diagnosisMsg = "正常";
				else
				{
					diagnosisMsg = "漂移过大";
				}
			}
			else if(ui->radioButton_NoCheck->isChecked())
				diagnosisMsg = "";
			QTableWidgetItem *Item8 =new QTableWidgetItem(diagnosisMsg);
			if(diagnosisMsg != "正常")
			{
				Item8->setBackgroundColor(QColor(255, 255, 205)); // 设置单元格背景颜色
				Item8->setTextColor(QColor(255,0,0));		      // 设置文字颜色
			}

			ui->tableWidget->setItem(i,0,Item1); // 数据名，+1 跳过时间项
			ui->tableWidget->setItem(i,1,Item2); // 和
			ui->tableWidget->setItem(i,2,Item3); // 最大值
			ui->tableWidget->setItem(i,3,Item4); // 最大值(绝对值)
			ui->tableWidget->setItem(i,4,Item5); // 最小值
			ui->tableWidget->setItem(i,5,Item6); // 最小值(绝对值)
			ui->tableWidget->setItem(i,6,Item7); // 算术平均值
			ui->tableWidget->setItem(i,7,Item8); // 数据检查
		}
		ui->tableWidget->show();
		bool b = true;
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			ui->label_msg->setText("分析成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'分析失败！</span>");
	}
}

// 数据概览-清空全部
void MainWindow::on_pushButton_2_clicked()
{
	ui->tableWidget->clearContents();
	ui->label_2->setText("数据行列：");
	ui->label_4->setText("数据名字：");
	ui->label_5->setText("数据总和：");
	ui->label_6->setText("数据平均：");
	ui->label_7->setText("数据最大：");
	ui->label_8->setText("数据最小：");
	ui->label_msg->setText("...");
}

/**************************************** 第3页 功能 ****************************************/

// 封装的绘图函数
void MainWindow::JfzPlot(QVector<double> MatData, QString PicName, int TuNum, QColor Colorstyle, double ymax, double ymin)
{
	//定义两个可变QVector数组存放绘图的坐标数据（X、Y数据量）
	int xysize = MatData.size();
	//分别存放x和y坐标的数据
	QVector<double> x(xysize),y(MatData);
	//    int max = -1000000,min = 1000000;
	for(int i=0;i<xysize;i++)
	{
		x[i] = i;
	}
	//设置属性可缩放，移动等
	ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
	//设置坐标轴标签名称
	ui->customPlot->xAxis->setLabel("X");
	ui->customPlot->yAxis->setLabel("Y");
	ui->customPlot->legend->setVisible(true);		// 图注可见
	//  ui->customPlot->axisRect()->setupFullAxesBox();
	ui->customPlot->addGraph();						//向绘图区域QCustomPlot(从widget提升来的)添加一条曲线
	ui->customPlot->graph(TuNum)->setName(PicName);	//曲线名称
	ui->customPlot->graph(TuNum)->setPen(QPen(Colorstyle));
	ui->customPlot->graph(TuNum)->setData(x,y);		//设置曲线数据
	//设置坐标轴显示范围,否则我们只能看到默认的范围
	ui->customPlot->xAxis->setRange(0,xysize);
	double delta = (ymax-ymin)*0.3;
	ui->customPlot->yAxis->setRange(ymin-delta,ymax+delta);
}

// 载入用于绘图的数据源
void MainWindow::on_pushButton_LoadPlotData_clicked()
{
	ui->label_msg->setText("");
	ui->comboBox_LoadPlotList->clear();
	ui->customPlot->clearGraphs();
	ui->customPlot->replot();
	QString inputFile = QFileDialog::getOpenFileName(this, tr("打开绘图的数据源"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!inputFile.isEmpty())
	{
		// 【1】载入数据
		plotDataStrList = JIO::readCsv(inputFile);
//		JIO::show(strlist);
		// 【2】获取数据项名字
		QList<QString> strListItem;
		for(auto i=1;i<plotDataStrList[0].size();++i)
			strListItem.append(plotDataStrList[0][i]);
		// 【3】写入comboBox、制作矩阵数据
		ui->comboBox_LoadPlotList->addItems(strListItem);
		QVector<double> MatData;
		for(int i=0;i<plotDataStrList.size()-1;++i)
		{
			MatData.append(plotDataStrList[i+1][1].toDouble());
		}
		// 【4】计算y范围，绘制默认的曲线
		auto ymax = max_element(MatData.begin(),MatData.end());
		auto ymin = min_element(MatData.begin(),MatData.end());
		ui->customPlot->clearGraphs();
		JfzPlot(MatData, plotDataStrList[0][1], 0, Qt::red, *ymax, *ymin);
		ui->customPlot->replot();

		ui->label_msg->setText("绘图完成!");
	}
	else
		ui->label_msg->setText("未选择文件！");
}

// 绘图的comboBox改变时事件
void MainWindow::on_comboBox_LoadPlotList_currentTextChanged(const QString &arg1)
{
	ui->label_msg->setText("");
	ui->customPlot->clearGraphs();
	ui->customPlot->replot();
	if(!arg1.isEmpty())
	{
		int ItemIndex = plotDataStrList[0].indexOf(arg1); // 对应图项目下标
		QVector<double> MatData;
		for(int i=0;i<plotDataStrList.size()-1;++i)
		{
			MatData.append(plotDataStrList[i+1][ItemIndex].toDouble());
		}
		auto ymax = max_element(MatData.begin(),MatData.end());
		auto ymin = min_element(MatData.begin(),MatData.end());
		JfzPlot(MatData, plotDataStrList[0][ItemIndex], 0, Qt::red, *ymax, *ymin);
		ui->customPlot->replot();
		ui->label_msg->setText("绘图完成!");
	}
}

/**************************************** 第4页 功能 ****************************************/

// CCD导入数据库
void MainWindow::on_pushButton_CCDinSQL_clicked()
{
	if(ui->radioButton_SQLite->isChecked())
		DataBaseType = "QSQLITE";
	if(ui->radioButton_MySQL->isChecked())
		DataBaseType = "QMYSQL";
	qDebug()<<"Using"<<DataBaseType;
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open CCD Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		// CCD导入数据库
		JSQL *jsql = new JSQL("localhost", "data_wuzhong", "root", "root", DataBaseType);
		DataSQLThread *dataSQL = new DataSQLThread(strDir, jsql, "ccd", "in");
		dataSQL->start();
		connect(dataSQL, &DataSQLThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataSQL, &DataSQLThread::sendProgressBar, this, &MainWindow::updateProgressBar);
		connect(dataSQL, &DataSQLThread::finished, dataSQL, &QObject::deleteLater);
	}
}

// FBG导入数据库
void MainWindow::on_pushButton_FBGinSQL_clicked()
{
	if(ui->radioButton_SQLite->isChecked())
		DataBaseType = "QSQLITE";
	if(ui->radioButton_MySQL->isChecked())
		DataBaseType = "QMYSQL";
	qDebug()<<"Using"<<DataBaseType;
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open FBG Directory"),"/data/fbg/",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		// FBG导入数据库
		JSQL *jsql = new JSQL("localhost", "data_wuzhong", "root", "root", DataBaseType);
		DataSQLThread *dataSQL = new DataSQLThread(strDir, jsql, "fbg", "in");
		dataSQL->start();
		connect(dataSQL, &DataSQLThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataSQL, &DataSQLThread::sendProgressBar, this, &MainWindow::updateProgressBar);
		connect(dataSQL, &DataSQLThread::finished, dataSQL, &QObject::deleteLater);
	}
}

// DS18B20导入数据库
void MainWindow::on_pushButton_DS18BinSQL_clicked()
{
	if(ui->radioButton_SQLite->isChecked())
		DataBaseType = "QSQLITE";
	if(ui->radioButton_MySQL->isChecked())
		DataBaseType = "QMYSQL";
	qDebug()<<"Using"<<DataBaseType;
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open DS18B20 Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		// DS18B20导入数据库
		JSQL *jsql = new JSQL("localhost", "data_wuzhong", "root", "root", DataBaseType);
		DataSQLThread *dataSQL = new DataSQLThread(strDir, jsql, "ds18b20", "in");
		dataSQL->start();
		connect(dataSQL, &DataSQLThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataSQL, &DataSQLThread::sendProgressBar, this, &MainWindow::updateProgressBar);
		connect(dataSQL, &DataSQLThread::finished, dataSQL, &QObject::deleteLater);
	}
}

// 环境温度导入数据库
void MainWindow::on_pushButton_ENVinSQL_clicked()
{
	if(ui->radioButton_SQLite->isChecked())
		DataBaseType = "QSQLITE";
	if(ui->radioButton_MySQL->isChecked())
		DataBaseType = "QMYSQL";
	qDebug()<<"Using"<<DataBaseType;
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open ENV Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		// 环境温度导入数据库
		JSQL *jsql = new JSQL("localhost", "data_wuzhong", "root", "root", DataBaseType);
		DataSQLThread *dataSQL = new DataSQLThread(strDir, jsql, "environment", "in");
		dataSQL->start();
		connect(dataSQL, &DataSQLThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataSQL, &DataSQLThread::sendProgressBar, this, &MainWindow::updateProgressBar);
		connect(dataSQL, &DataSQLThread::finished, dataSQL, &QObject::deleteLater);
	}
}

// 机床CNC内部数据导入数据库
void MainWindow::on_pushButton_CNCinSQL_clicked()
{
	if(ui->radioButton_SQLite->isChecked())
		DataBaseType = "QSQLITE";
	if(ui->radioButton_MySQL->isChecked())
		DataBaseType = "QMYSQL";
	qDebug()<<"Using"<<DataBaseType;
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open CNC Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		// 机床CNC内部数据导入数据库
		JSQL *jsql = new JSQL("localhost", "data_wuzhong", "root", "root", DataBaseType);
		DataSQLThread *dataSQL = new DataSQLThread(strDir, jsql, "cnc", "in");
		dataSQL->start();
		connect(dataSQL, &DataSQLThread::sendMsg, this, &MainWindow::showMsg);
		connect(dataSQL, &DataSQLThread::sendProgressBar, this, &MainWindow::updateProgressBar);
		connect(dataSQL, &DataSQLThread::finished, dataSQL, &QObject::deleteLater);
	}
}

// 数据导出
void MainWindow::on_pushButton_OutFromSQL_clicked()
{
	// 选择的数据库类型
	if(ui->radioButton_SQLite->isChecked())
		DataBaseType = "QSQLITE";
	if(ui->radioButton_MySQL->isChecked())
		DataBaseType = "QMYSQL";

	// 选择的查询数据类型
	QString tableName;
	QString itemName = "Time,";
	int itemNum;
	bool isStr = false;
	if(ui->radioButton_selectDS18B20->isChecked()) {tableName = "ds18b20";     itemName+=DataName_DS18ALL; itemNum=65; isStr=false;}
	if(ui->radioButton_selectCCD    ->isChecked()) {tableName = "ccd";         itemName+=DataName_CCD;     itemNum=4;  isStr=false;}
	if(ui->radioButton_selectENV    ->isChecked()) {tableName = "environment"; itemName+=DataName_ENV;     itemNum=5;  isStr=false;}
	if(ui->radioButton_selectFBG    ->isChecked()) {tableName = "fbg";         itemName+=DataName_FBG;     itemNum=641;isStr=false;}
	if(ui->radioButton_selectCNC    ->isChecked()) {tableName = "cnc";         itemName+=DataName_CNC;     itemNum=17; isStr=true; }

	// 选择的时间-判别
	QString startTime = ui->dateTimeEdit_Start->text();
	QString endTime = ui->dateTimeEdit_End->text();
	if(startTime >= endTime)
	{
		emit sendMsg("时间需要正序查询(开始时间比结束时间早)！");
	}

	//	原始数据、优化数据导出选择
	bool isOri = false;
	if(ui->radioButton_DataOri->isChecked())
		isOri = true;
	if(ui->radioButton_DataOpt->isChecked())
		isOri = false;

	//	开启线程查询
	JSQL *jsql = new JSQL("localhost", "data_wuzhong", "root", "root", DataBaseType);
	connect(jsql, &JSQL::sendProgressBar, this, &MainWindow::updateProgressBar);

	DataSQLThread *dataSQL = new DataSQLThread("no_use", jsql, "no_use", "out");
	dataSQL->setQueryMsg(tableName, startTime, endTime, itemName, itemNum, isStr, isOri);
	dataSQL->start();
	connect(dataSQL, &DataSQLThread::sendMsg, this, &MainWindow::showMsg);
	connect(dataSQL, &DataSQLThread::sendProgressBar, this, &MainWindow::updateProgressBar);
	connect(dataSQL, &DataSQLThread::finished, dataSQL, &QObject::deleteLater);
}

// 本地历史数据库下载开始请求
void MainWindow::startRequest(QUrl url)
{
	qDebug()<<"url "<<url;
	replyDatabase = managerDatabase->get(QNetworkRequest(url));
	connect(replyDatabase, &QNetworkReply::readyRead,        this, &MainWindow::httpReadyRead);
	connect(replyDatabase, &QNetworkReply::downloadProgress, this, &MainWindow::updateDataReadProgress);
	connect(replyDatabase, &QNetworkReply::finished,         this, &MainWindow::httpFinished);
}

// readyRead后开始写文件
void MainWindow::httpReadyRead()
{
	if (fileDatabase) fileDatabase->write(replyDatabase->readAll());
}

// 更新进度条
void MainWindow::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
	ui->progressBar_UpdateDatabase->setMaximum(totalBytes);
	ui->progressBar_UpdateDatabase->setValue(bytesRead);
}

// 下载完关进度条、关文件、关请求
void MainWindow::httpFinished()
{
	ui->progressBar_UpdateDatabase->hide();
	if(fileDatabase) {
		fileDatabase->close();
		delete fileDatabase;
		fileDatabase = nullptr;
	}
	replyDatabase->deleteLater();
	replyDatabase = 0;
	// 解压文件
	QString program = "data_wuzhong.exe";
	QStringList arguments;
	arguments << "";
	runDatabaseProcess.start(program, arguments);
}

// 解包运行结束显示内容
void MainWindow::runDatabaseProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << "runDatabaseProcessFinished: " << endl << exitCode << exitStatus;
	emit sendMsg("<span style='color: rgb(255, 0, 0);'>历史数据库更新完毕!</span>");
}

// 数据库下载完成后做的事情
void MainWindow::checkDatabaseUpdateFinished(QNetworkReply *reply)
{
	QString DatabaseUpdateTime = reply->readAll();
	reply->deleteLater();
	qDebug()<<"DatabaseUpdateTime "<<DatabaseUpdateTime;
	QStringList DatabaseTime = JIO::readFile("Database");
	bool bupdate = DatabaseUpdateTime > DatabaseTime[0];
	qDebug()<<"bupdate "<<bupdate;
	if(bupdate)
	{
		urlDatabase = "http://img.jfz.me/data_wuzhong.exe";
		QFileInfo info(urlDatabase.path());
		QString fileName(info.fileName());
		if (fileName.isEmpty()) fileName = "data_wuzhong.exe";

		fileDatabase = new QFile(fileName);
		if(!fileDatabase->open(QIODevice::WriteOnly))
		{
			delete fileDatabase;
			fileDatabase = nullptr;
			return;
		}

		startRequest(urlDatabase);
		ui->progressBar_UpdateDatabase->setValue(0);
		ui->progressBar_UpdateDatabase->show();
		JIO::save("Database",DatabaseUpdateTime); // 保存现在时间
	}
	else
	{
		emit sendMsg("不需要更新！");
	}
}

// 更新本地历史数据库
void MainWindow::on_pushButton_UpdateSQL_clicked()
{
	managerCheckDatabaseUpdate->get(QNetworkRequest(QUrl("http://lgpfw.jfz.me/soft/JfzDataLabDatabaseUpdate.txt")));
}


void MainWindow::on_radioButton_DataType_CSV_clicked(bool checked)
{
	if(checked)
		dataProType = 0;  // csv
	qDebug()<<"dataProType"<<dataProType;
	// 能用
	ui->pushButton_SplitByDate->setEnabled(true);
	ui->pushButton_CSVmerge->setEnabled(true);
	ui->pushButton_TimeStand->setEnabled(true);
	ui->pushButton_File1->setEnabled(true);
	ui->pushButton_File2->setEnabled(true);
	// 不能用
	ui->pushButton_LinearRegression->setEnabled(false);
}

void MainWindow::on_radioButton_DataType_MAT_clicked(bool checked)
{
	if(checked)
		dataProType = 1; // mat
	qDebug()<<"dataProType"<<dataProType;
	// 能用
	ui->pushButton_LinearRegression->setEnabled(true);
	// 不能用
	ui->pushButton_SplitByDate->setEnabled(false);
	ui->pushButton_CSVmerge->setEnabled(false);
	ui->pushButton_TimeStand->setEnabled(false);
	ui->pushButton_File1->setEnabled(false);
	ui->pushButton_File2->setEnabled(false);

}

// 打开输出目录
void MainWindow::on_pushButton_openOutputDir_clicked()
{
//	QString path=QDir::currentPath();	// 获取程序当前目录
//	path.replace("/","\\");				// 将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
//	qDebug()<<path;
//	QProcess::startDetached("explorer "+path);	// 打开上面获取的目录
	// 另一种方法
	QDesktopServices::openUrl(QUrl("E:/[Data]/output", QUrl::TolerantMode));
}

// 打开工作目录
void MainWindow::on_pushButton_openInputDir_clicked()
{
	QString path = workspacePath;
	path.replace("/","\\");
	qDebug()<<path;
	QProcess::startDetached("explorer " + path);
}
