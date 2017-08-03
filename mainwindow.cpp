#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include "skins/skins.h"
#include "tools/JIO.h"
#include "tools/JSQL.h"
#include "algorithm/DataProcessing.h"
#include "algorithm/DataToSQL.h"
#include "algorithm/datadiagnosis.h"

#include "plugins/qcustomplot.h"


//class JSkin
//{
//public:
//	static void setStyle(const QString &style) {
//		QFile qss(style);
//		qss.open(QFile::ReadOnly);
//		qApp->setStyleSheet(qss.readAll());
//		qss.close();
//	}
//};
//	JSkin::setStyle("white.qss");

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	// 工作区路径
	workspacePath = "./workspace/";
	// Skin
	this->initSkins();
	// init qcustomplot
	ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	// 托盘
	myTray = new SystemTray(this);
	connect(myTray,SIGNAL(showWidget()),this,SLOT(ShowWindow()));//关联信号和槽函数
	connect(myTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(SystemTrayActivated(QSystemTrayIcon::ActivationReason)));
	// 最下方消息提醒
	connect(this, SIGNAL(sendMsg(QString)), this, SLOT(showMsg(QString)));
	// 最下方进度条
	ui->progressBar->hide();
	connect(this, &MainWindow::sendProgressBar, this, &MainWindow::updateProgressBar);
	// 历史数据库下载
	managerDatabase = new QNetworkAccessManager(this);
	ui->progressBar_UpdateDatabase->hide();
	connect(&runDatabaseProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(runDatabaseProcessFinished(int, QProcess::ExitStatus)));
	managerCheckDatabaseUpdate = new QNetworkAccessManager(this);
	connect(managerCheckDatabaseUpdate, &QNetworkAccessManager::finished, this, &MainWindow::checkDatabaseUpdateFinished);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showMsg(QString msg)
{
	qDebug()<<"[信息]:"<<msg;
	ui->label_msg->setText(msg);
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
	ui->progressBar->setMaximum(totalBytes);
	ui->progressBar->setValue(bytesRead);
}

/**************************************** 第1页 功能 ****************************************/
// 电类标准化
void MainWindow::on_pushButton_DS18B20_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open DS18B20 Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】数据标准化
		bool b = standardDS18B20(dir);
		// 【2】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("电类温度保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
}

// CCD标准化
void MainWindow::on_pushButton_CCD_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open CCD Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【2】数据标准化
		bool b = standardCCD(dir);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("CCD位移保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
}

// FBG温度标准化
void MainWindow::on_pushButton_FBGT_clicked()
{
//	ui->label_msg->setText("文件状态：未载入");
//	// 载入文件夹
//	QString dir = QFileDialog::getExistingDirectory(this, tr("Open FBGT Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
//	// 载入成功操作
//	if(!dir.isEmpty())
//	{
//		ui->label_msg->setText("FBG温度处理中！");
//		// 【0】计时开始
//		QTime time;time.start();
//		// 【1】获取CH通道对应文件
//		QStringList CH = get_FBGT_csv(dir);
//		// 【2】数据标准化
//		QStringList Time;
//		Stand_FBGT(CH, MatFBGT, Time);
//		// 【3】保存文件
//		int b = saveStandData("Data-FBGTemperature",DataName_FBGT,Time, MatFBGT);
//		// 【4】计时结束
//		QString timecost = QString::number(time.elapsed()/1000.0);
//		if( b == 0 )
//			ui->label_msg->setText("FBG温度保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
//		else
//			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
//	}
}

// FBG温度标准化（预处理-波长修复）
void MainWindow::on_pushButton_LoadFBGT_ALL_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open FBGT Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【2】数据标准化
		bool b = standardFBGT(dir);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			emit sendMsg("FBG温度保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			emit sendMsg("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
}

// FBG应力标准化
void MainWindow::on_pushButton_FBGS_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	// 载入文件夹 ，填"/"跳到根目录， 填“”默认程序位置
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open FBGS Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【2】数据标准化
		bool b = standardFBGS(dir);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("FBG应力保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
}

// 环境温度标准化
void MainWindow::on_pushButton_ENV_clicked()
{
//	ui->label_msg->setText("...");
//	ui->progressBar->show();
////	ui->pushButton_ENV->setEnabled(false);
//	EnvFileNameList = QFileDialog::getOpenFileNames(this, tr("打开环境温度数据"), workspacePath, tr("textfile(*.xls);"));
//	if(EnvFileNameList.size() != 4)
//	{
//		emit sendMsg("文件状态：未载入");
//		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个文件！", QMessageBox::Yes, QMessageBox::Yes);
////		ui->pushButton_ENV->setEnabled(true);
//	}
//	else
//	{
//		// 开启线程处理xls文件，防止界面卡死
//		EnvXlsReadThread *readxls = new EnvXlsReadThread(EnvFileNameList);
//		connect(readxls, &EnvXlsReadThread::sendMsg, this, &MainWindow::showMsg);
//		connect(readxls, &EnvXlsReadThread::sendProgressBar, this, &MainWindow::updateProgressBar);
//		readxls->start();
//	}
}

// FBG波长转温度
void MainWindow::on_pushButton_LoadFBGT_clicked()
{
	standFBGFileName = QFileDialog::getOpenFileName(this, tr("打开标准FBG波长"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!standFBGFileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】FBG波长转温度
		bool b = FBGTtoTEMP(standFBGFileName);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("波长转温度成功!花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");

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
	ui->label_msg->setText("计算开始！");
	// 【0】计时开始
	QTime time;time.start();
	// 【1】相关性计算
	bool b = correlationAnalysis(correlationFileName1, correlationFileName2);
	// 【3】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if(b)
		ui->label_msg->setText("保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
}

// 多元线性回归
void MainWindow::on_pushButton_LinearRegression_clicked()
{
	ui->label_msg->setText("未选择文件！");
	QString inputFile = QFileDialog::getOpenFileName(this, tr("打开多元线性回归数据源"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!inputFile.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】载入\计算 (多元线性回归核心算法)
		bool b = LinearRegression(inputFile);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			ui->label_msg->setText("计算完毕！时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span> ");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
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
			// 【0】计时开始
			QTime time;time.start();
			// 计算预测值
			bool b = LinearRegressionPredict(ModelFile, DataFile);
			// 【4】计时结束
			QString timecost = QString::number(time.elapsed()/1000.0);
			if(b)
				emit sendMsg("预测完成！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		}
	}
}

// 数据小处理-初始值为0
void MainWindow::on_pushButton_DataZero_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("初始值为0算法"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】初始值为0算法
		bool b = dataZero(fileName);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("初始值为0 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 数据小处理-增量化（后一个数-前一个数）
void MainWindow::on_pushButton_DataDelta_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("增量化算法"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】增量化算法
		bool b = dataDelta(fileName);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("增量化 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 数据小处理-压缩拉伸
void MainWindow::on_pushButton_DataSampling_clicked()
{
//	qDebug()<<sampling(30, 13);
//	qDebug()<<sampling(13, 30);
	QString fileName = QFileDialog::getOpenFileName(this, tr("压缩拉伸算法"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】压缩拉伸算法
		int needNum = ui->lineEdit_DataSampling->text().toInt();
		int nowNum = 0;
		bool b = dataSampling(fileName, nowNum, needNum);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			if(nowNum >= needNum)
				ui->label_msg->setText("压缩 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
			else
				ui->label_msg->setText("拉伸 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 数据小处理-数据清洗(设定阈值内数据，前一个值覆盖后一个值)
void MainWindow::on_pushButton_DataClean_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("数据清洗算法"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】数据清洗算法
		double maxNum = ui->lineEdit_DataCleanMax->text().toDouble();
		double minNum = ui->lineEdit_DataCleanMin->text().toDouble();
		bool b = dataClean(fileName, maxNum, minNum);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("数据清洗 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 数据小处理-趋势预测
void MainWindow::on_pushButton_DataTendency_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("趋势预测算法"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】趋势预测算法
		int window = ui->lineEdit_DataWindow->text().toInt();
		bool b = dataTendency(fileName, window);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("趋势预测 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 数据小处理-按天拆分
void MainWindow::on_pushButton_SplitByDate_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("虚拟映射-电类温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】按天拆分
		bool b = dataSplitByDate(fileName);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("按天拆分 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 数据小处理-csv合并
void MainWindow::on_pushButton_CSVmerge_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("csv合并"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	QTime time;time.start();// 【0】计时开始
	bool b = false;
	if(fileNameList.size() <=1 )
	{
		QMessageBox::critical(NULL, "注意", "请选择2个或以上数目的文件！", QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		// csv合并
		b = csvMerge(fileNameList);
	}

	// 【4】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		ui->label_msg->setText("csv合并 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
}

// 虚拟映射-数据通道映射为虚拟标准通道-FBG温度
void MainWindow::on_pushButton_VirtualMap_T_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("虚拟映射-FBG温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	// 【0】计时开始
	QTime time;time.start();
	// 【0】算法
	bool b = virtualMapFBGT(fileNameList);
	// 【4】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		ui->label_msg->setText("虚拟映射 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
}

// 虚拟映射-数据通道映射为虚拟标准通道-FBG应力
void MainWindow::on_pushButton_VirtualMap_S_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("虚拟映射-FBG应力"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	// 【0】计时开始
	QTime time;time.start();
	bool b = virtualMapFBGS(fileNameList);
	// 【4】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		ui->label_msg->setText("虚拟映射 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
}

// 虚拟映射-数据通道映射为虚拟标准通道-电类温度
void MainWindow::on_pushButton_VirtualMap_DS18_clicked()
{
	// 电类温度不需要交换位置，只需要改抬头
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("虚拟映射-电类温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	// 【0】计时开始
	QTime time;time.start();
	bool b = virtualMapDS18(fileNameList);
	// 【4】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		ui->label_msg->setText("虚拟映射 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
}

// 虚拟映射-映射的FBG温度传感器转温度值
void MainWindow::on_pushButton_VirtualFBGtoTEMP_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("FBG转温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	// 【0】计时开始
	QTime time;time.start();
	bool b = virtualFBGtoTEMP(fileNameList);
	// 【4】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		ui->label_msg->setText("映射FBG转温度 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
}

// 虚拟映射-映射的FBG应力传感器转应力
void MainWindow::on_pushButton_VirtualFBGtoSTRESS_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("FBG转温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	// 【0】计时开始
	QTime time;time.start();
	bool b = virtualFBGtoSTRESS(fileNameList);
	// 【4】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		ui->label_msg->setText("映射FBG转应力 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
}

// 一键环境温度数据映射
void MainWindow::on_pushButton_OriENVtoVirtual_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	EnvFileNameList = QFileDialog::getOpenFileNames(this, tr("打开环境温度txt"), workspacePath, tr("textfile(*.txt);"));
	QTime time;time.start(); // 计时
	bool b = false;
	if(EnvFileNameList.size() != 4)
	{
		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个txt文件！", QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		b = OriENVtoVirtual(EnvFileNameList);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("一键原始电类数据虚拟通道化 成功！ <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
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
		// 【0】计时开始
		QTime time;time.start();
		// 【1】获取CH通道对应文件
		bool b = OriDS18toVirtual(dir);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("一键原始电类数据虚拟通道化 成功！ <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
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
		// 【0】计时开始
		QTime time;time.start();
		// 【1】获取CH通道对应文件
		bool b = OriFBGtoVirtual(dir);
		// 【5】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			emit sendMsg("一键原始FBG数据虚拟通道化 成功！ <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			emit sendMsg("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
}



/**************************************** 第2页 功能 ****************************************/

// 数据概览-载入数据
void MainWindow::on_pushButton_clicked()
{
//	ui->textEdit->clear();
//	// 载入文件
//	QString InputFileName_str = QFileDialog::getOpenFileName(this,"载入数据", workspacePath,"textfile(*.csv);;All file(*.*)");
//	qDebug()<<InputFileName_str;  // 文件名

//	if(!InputFileName_str.isEmpty())
//	{
//		// 需要变量
//		QStringList DataName; // 数据各项名
//		QString strMatRow;
//		QString strMatCol;
//		QString strTitle;
//		QString strMatSum;
//		QString strMatArit;
//		QString strMatMax;
//		QString strMatMin;
//		// 【0】计时开始
//		QTime time;time.start();
//		// 【1】数据分析
//		mat Mat = DataAnalysis(InputFileName_str, DataName, strMatRow, strMatCol, strTitle, strMatSum, strMatArit, strMatMax, strMatMin);
//		// 【2】数据概览-处理
//		ui->label_2->setText("数据行列：" + strMatRow+ " x "+ strMatCol );
//		ui->label_4->setText("数据名字：" + strTitle );
//		ui->label_5->setText("数据总和：" + strMatSum );
//		ui->label_6->setText("数据平均：" + strMatArit );
//		ui->label_7->setText("数据最大：" + strMatMax );
//		ui->label_8->setText("数据最小：" + strMatMin );
//		// 【3】数据诊断
//		bool b = DataDiagnosis(InputFileName_str, Mat, DataName, ui);
//		// 【4】计时结束
//		QString timecost = QString::number(time.elapsed()/1000.0);
//		if(b)
//			ui->label_msg->setText("分析成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
//		else
//			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'分析失败！</span>");
//	}
}

// 数据概览-清空全部
void MainWindow::on_pushButton_2_clicked()
{
	ui->textEdit->clear();
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

	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open CCD Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		QTime time;time.start();
		// 【1】递归遍历子目录，得到所有*.csv文件路径
		QFileInfoList file_list = GetFileList(strDir);
		QStringList AllFileName;
		for (int i = 0; i < file_list.size(); ++i)
		{
			QFileInfo fileInfo = file_list.at(i);
			AllFileName.append(fileInfo.filePath());
//			qDebug()<<QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName());
//			qDebug()<<QString("%1").arg(fileInfo.filePath());
		}

		// 【2】向SQL中插入数据
		JSQL jsql("localhost","data_wuzhong","root","root",DataBaseType);
		bool b = false;
		for(auto filename : AllFileName)
		{
			b = CCDtoMYSQL(filename, jsql);
		}

		// 【3】显示花费时间
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			ui->label_msg->setText("花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'失败！</span>");
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

	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open FBG Directory"),"/data/fbg/",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		QTime time;time.start();
		// 【1】递归遍历子目录，得到所有*.csv文件路径
		// 遍历目录
		QDir dir(strDir);
		QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		QStringList AllDirsName;
		for(int i = 0; i != folder_list.size(); i++)
		{
			 QString name = folder_list.at(i).absoluteFilePath();
			 AllDirsName.append(name);
		}

		// 【2】遍历目录中的文件、向SQL中插入数据
		JSQL jsql("localhost","data_wuzhong","root","root",DataBaseType);
		bool b = false;
		for(int i=0;i<AllDirsName.size();++i)
		{
			QStringList FBGChannelNames = FBGDir2FileName(AllDirsName[i]);
			qDebug()<<FBGChannelNames[i];
			b = FBGtoMYSQL(FBGChannelNames,jsql);
		}

		// 【3】显示花费时间
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			ui->label_msg->setText("花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'失败！</span>");
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

	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open DS18B20 Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		QTime time;time.start();
		// 【1】递归遍历子目录，得到所有*.csv文件路径
		// 遍历目录
		QDir dir(strDir);
		QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		QStringList AllDirsName;
		for(int i = 0; i != folder_list.size(); i++)
		{
			 QString name = folder_list.at(i).absoluteFilePath();
			 AllDirsName.append(name);
		}

		// 【2】遍历目录中的文件、向SQL中插入数据
		JSQL jsql("localhost","data_wuzhong","root","root",DataBaseType);
		bool b = false;
		for(int i=0;i<AllDirsName.size();++i)
		{
			QStringList DS18B20ChannelNames = DS18B20Dir2FileName(AllDirsName[i]);
//			qDebug()<<DS18B20ChannelNames;
			b = DS18B20toMYSQL(DS18B20ChannelNames,jsql);
		}

		// 【3】显示花费时间
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			ui->label_msg->setText("花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'失败！</span>");
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

	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open ENV Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		QTime time;time.start();
		// 【1】递归遍历子目录，得到所有*.csv文件路径
		QFileInfoList file_list = GetFileList(strDir); // 递归遍历子目录
		QStringList AllFileName;
		for (int i = 0; i < file_list.size(); ++i)
		{
			QFileInfo fileInfo = file_list.at(i);
			AllFileName.append(fileInfo.filePath());
//			qDebug()<<QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName());
//			qDebug()<<QString("%1").arg(fileInfo.filePath());
		}

		// 【2】向SQL中插入数据
		JSQL jsql("localhost","data_wuzhong","root","root",DataBaseType);
		bool b = false;
		for(auto filename : AllFileName)
		{
			b = ENVtoMYSQL(filename, jsql);
		}

		// 【3】显示花费时间
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			ui->label_msg->setText("花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'失败！</span>");
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

	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open CNC Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!strDir.isEmpty())
	{
		QTime time;time.start();
		// 【1】递归遍历子目录，得到所有*.csv文件路径
		QFileInfoList file_list = GetFileList(strDir); // 递归遍历子目录
		QStringList AllFileName;
		for (int i = 0; i < file_list.size(); ++i)
		{
			QFileInfo fileInfo = file_list.at(i);
			AllFileName.append(fileInfo.filePath());
		}

		// 【2】向SQL中插入数据
		JSQL jsql("localhost","data_wuzhong","root","root",DataBaseType);
		bool b = false;
		for(auto filename : AllFileName)
		{
			b = CNCtoMYSQL(filename, jsql);
		}

		// 【3】显示花费时间
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			ui->label_msg->setText("花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'失败！</span>");
	}
}

// 数据导出
void MainWindow::on_pushButton_OutFromSQL_clicked()
{
	emit sendMsg("");
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

	// 选择的时间
	QString startTime = ui->dateTimeEdit_Start->text();
	QString endTime = ui->dateTimeEdit_End->text();
	if(startTime>=endTime)
	{
		emit sendMsg("时间需要正序查询(开始时间比结束时间早)！");
		return;
	}

	JSQL jsql("localhost","data_wuzhong","root","root",DataBaseType);
	// 原始数据导出
	if(ui->radioButton_DataOri->isChecked())
	{
		QTime time;time.start();
		QStringList out;
		bool b0 = jsql.queryData(tableName, startTime, endTime, itemName, itemNum, isStr, out);
		qDebug()<<"查询成功? "<<b0;
		if(b0)
		{
		QString saveFileName;
		startTime = TimeToTimeNum(startTime);
		endTime   = TimeToTimeNum(endTime);
		saveFileName = startTime +"~"+endTime +"_"+tableName+"_OriData.csv";
		qDebug()<<"saveFileName "<<saveFileName;
		bool b = JIO::save(saveFileName,out);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			emit sendMsg("查询并保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			emit sendMsg("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
		}
		else
		{
			emit sendMsg("<span style='color: rgb(255, 0, 0);'>数据库查询失败！</span>");
		}
	}

	// 优化数据导出
	if(ui->radioButton_DataOpt->isChecked())
	{
		// 这三类数据不需要优化
		if(tableName == "ccd" || tableName == "environment" || tableName == "cnc")
		{
			QTime time;time.start();
			QStringList out;
			bool b0 = jsql.queryData(tableName, startTime, endTime, itemName, itemNum, isStr, out);
			qDebug()<<"查询成功? "<<b0;
			if(b0)
			{
			QString saveFileName;
			startTime = TimeToTimeNum(startTime);
			endTime   = TimeToTimeNum(endTime);
			saveFileName = startTime +"~"+endTime +"_"+tableName+"_OptData.csv";
			qDebug()<<"saveFileName "<<saveFileName;
			bool b = JIO::save(saveFileName,out);
			// 【4】计时结束
			QString timecost = QString::number(time.elapsed()/1000.0);
			if(b)
				emit sendMsg("查询并保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
			else
				emit sendMsg("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
			}
			else
			{
				emit sendMsg("<span style='color: rgb(255, 0, 0);'>数据库查询失败！</span>");
			}
		}
		// 电类数据除去最后为零的通道
		if(tableName == "ds18b20")
		{
			QTime time;time.start();
			QStringList out;
			bool b0 = jsql.queryData(tableName, startTime, endTime, itemName, itemNum, isStr, out);
			qDebug()<<"查询成功? "<< b0;
			if(b0)
			{
			// 除去后续无用通道
			for(auto &e: out)
			{
				QStringList templist;
				templist = e.split(",");
				for(int i=0;i<12;++i) // 空余的12个
				{
					templist.removeLast();
				}
				QString joinStr = templist.join(",");
				e = joinStr;
//				qDebug()<<e;
			}
			QString saveFileName;
			startTime = TimeToTimeNum(startTime);
			endTime   = TimeToTimeNum(endTime);
			saveFileName = startTime +"~"+endTime +"_"+tableName+"_OptData.csv";
			qDebug()<<"saveFileName "<<saveFileName;
			bool b = JIO::save(saveFileName,out);
			// 【4】计时结束
			QString timecost = QString::number(time.elapsed()/1000.0);
			if(b)
				emit sendMsg("查询并保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
			else
				emit sendMsg("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
			}
			else
			{
				emit sendMsg("<span style='color: rgb(255, 0, 0);'>数据库查询失败！</span>");
			}
		}
		// fbg数据除去没记录的通道
		if(tableName == "fbg")
		{
			QTime time;time.start();
			QStringList out;
			bool b0 = jsql.queryData(tableName, startTime, endTime, itemName, itemNum, isStr, out);
			qDebug()<<"查询成功? "<< b0;
			if(b0)
			{
				// 提取数据转mat矩阵
				out.removeFirst(); // 去除抬头
				mat inputMat(out.size(),640); // FBG矩阵
				inputMat.fill(0.0);
				int rows = 0;
				for(auto &e: out)
				{
					QStringList templist;
					templist = e.split(",");
					for(int i=1; i<641; ++i) // 除去时间第一个，剩下640个
					{
						inputMat(rows,i-1) = templist[i].toDouble();
					}
					rows++;
				}
				// 全部mat数据优化成321个通道
				mat outputMat;
				bool b1 = ALLFBGto321FBG(inputMat, outputMat);
				// 构造输出数据
				QStringList outData;
				rows = 0;
				for(auto &e: out)
				{
					QStringList templist;
					templist = e.split(",");
					outData.append(templist[0]+",");
					for(unsigned int i=0; i<outputMat.n_cols; ++i)
					{
						QString value;
						value.sprintf("%.3lf",outputMat(rows,i));
						if( i+1 != outputMat.n_cols)
							outData[rows] += value + ",";
						else
							outData[rows] += value;
					}
					rows++;
				}
				outData.insert(0,"Time,"+DataName_FBGST);

				// 保存数据
				QString saveFileName;
				startTime = TimeToTimeNum(startTime);
				endTime   = TimeToTimeNum(endTime);
				saveFileName = startTime +"~"+endTime +"_"+tableName+"_OptData.csv";
				qDebug()<<"saveFileName "<<saveFileName;
				bool b = JIO::save(saveFileName,outData);
				// 【4】计时结束
				QString timecost = QString::number(time.elapsed()/1000.0);
				if(b && b1)
					emit sendMsg("查询并保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
				else
					emit sendMsg("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
			}
			else
			{
				emit sendMsg("<span style='color: rgb(255, 0, 0);'>数据库查询失败！</span>");
			}
		}
	}
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
	managerCheckDatabaseUpdate->get(QNetworkRequest(QUrl("http://blog.jfz.me/soft/JfzDataLabDatabaseUpdate.txt")));
}

