#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include "skins/skins.h"
#include "tools/jfzlib.h"
#include "tools/JIO.h"
#include "tools/JSQL.h"
#include "algorithm/datatostand.h"
#include "algorithm/correlations.h"
#include "algorithm/datadiagnosis.h"
#include "algorithm/LinearRegression.h"
#include "algorithm/DataToSQL.h"
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
	qssName << "系统" << "黑色" << "灰黑色" << "灰色" << "浅灰色" << "深灰色" << "银色" << "淡蓝色" << "蓝色"<< "PS黑";
	ui->comboBox_skin->addItems(qssName);
	ui->comboBox_skin->setCurrentIndex(6);// 个人最喜欢银，默认
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
		ui->label_msg->setText("电类处理中！");
		// 【0】计时开始
		QTime time;time.start();
		// 【1】获取CH通道对应文件
		QStringList CH = get_DS18B20_csv(dir);
		// 【2】数据标准化
		QStringList Time;
		Stand_DS18B20(CH, MatDS18B20, Time);// 各通道数据转Mat并排序
		// 【3】保存文件
		int b = saveStandData("Data-DS18B20",DataName_DS18B20,Time, MatDS18B20);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( 0 == b )
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
		ui->label_msg->setText("CCD处理中！");
		// 【0】计时开始
		QTime time;time.start();
		// 【1】获取CH通道对应文件
		QStringList CH = get_CCD_csv(dir);
		// 【2】数据标准化
		QStringList Time;
		Stand_CCD(CH, MatCCD, Time);
		// 【3】保存文件
		int b = saveStandData("Data-CCD",DataName_CCD,Time, MatCCD);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b == 0 )
			ui->label_msg->setText("CCD位移保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
}

// FBG温度标准化
void MainWindow::on_pushButton_FBGT_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open FBGT Directory"), workspacePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		ui->label_msg->setText("FBG温度处理中！");
		// 【0】计时开始
		QTime time;time.start();
		// 【1】获取CH通道对应文件
		QStringList CH = get_FBGT_csv(dir);
		// 【2】数据标准化
		QStringList Time;
		Stand_FBGT(CH, MatFBGT, Time);
		// 【3】保存文件
		int b = saveStandData("Data-FBGTemperature",DataName_FBGT,Time, MatFBGT);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b == 0 )
			ui->label_msg->setText("FBG温度保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
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
		emit sendMsg("FBG温度处理中！");
		// 【0】计时开始
		QTime time;time.start();
		// 【1】获取CH通道对应文件
		QStringList CH = get_FBGT_csv(dir);
		// 【2】数据标准化
		QStringList Time;
		Stand_FBGT_Fix(CH, MatFBGT, Time);
		// 【3】保存文件
		int b = saveStandData("Data-FBGTemperature",DataName_FBGT,Time, MatFBGT);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b == 0)
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
		ui->label_msg->setText("FBG应力处理中！");
		// 【0】计时开始
		QTime time;time.start();
		// 【1】获取CH通道对应文件
		QStringList CH = get_FBGS_csv(dir);
		// 【2】数据标准化
		QStringList Time;
		Stand_FBGS(CH, MatFBGS, Time);// 各通道数据转Mat并排序
		// 【3】保存文件
		int b = saveStandData("Data-FBGStress",DataName_FBGS,Time, MatFBGS);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b == 0 )
			ui->label_msg->setText("FBG应力保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
}

// 环境温度标准化
void MainWindow::on_pushButton_ENV_clicked()
{
	ui->label_msg->setText("...");
	ui->progressBar->show();
//	ui->pushButton_ENV->setEnabled(false);
	EnvFileNameList = QFileDialog::getOpenFileNames(this, tr("打开环境温度数据"), workspacePath, tr("textfile(*.xls);"));
	if(EnvFileNameList.size() != 4)
	{
		emit sendMsg("文件状态：未载入");
		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个文件！", QMessageBox::Yes, QMessageBox::Yes);
//		ui->pushButton_ENV->setEnabled(true);
	}
	else
	{
		// 开启线程处理xls文件，防止界面卡死
		EnvXlsReadThread *readxls = new EnvXlsReadThread(EnvFileNameList);
		connect(readxls, &EnvXlsReadThread::sendMsg, this, &MainWindow::showMsg);
		connect(readxls, &EnvXlsReadThread::sendProgressBar, this, &MainWindow::updateProgressBar);
		readxls->start();
	}
}

// FBG波长转温度
void MainWindow::on_pushButton_LoadFBGT_clicked()
{
	standFBGFileName = QFileDialog::getOpenFileName(this, tr("打开标准FBG波长"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!standFBGFileName.isEmpty())
	{
		ui->label_msg->setText("正在转换...");
		if(!standFBGFileName.isEmpty())
		{
			// 【0】计时开始
			QTime time;time.start();
			// 【1】FBG波长转温度
			QStringList XLabelName;
			mat FBGWave = FBGtoTEMP(standFBGFileName, XLabelName);
			// 【2】保存文件
			QString Date = standFBGFileName.right(33).left(10);
			int b = saveStandDataNoTimeFix("Data-FBGtoTEMP",DataName_FBGT,Date,XLabelName, FBGWave);
			// 【4】计时结束
			QString timecost = QString::number(time.elapsed()/1000.0);
			if( b == 0 )
				ui->label_msg->setText("波长转温度成功!花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
			else
				ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
		}
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
	// 需要的变量
	QStringList DataName1;
	QStringList DataName2;
	QStringList Time;
	// 【0】计时开始
	QTime time;time.start();
	// 【1】相关性计算
	mat corMatneed = CorrelationAnalysis(correlationFileName1, correlationFileName2, DataName1, DataName2, Time);
	// 【2】保存文件
	bool b = saveCorrelationAnalysisCSV(correlationFileName1, correlationFileName2, DataName1, DataName2, corMatneed);
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
	ui->label_msg->setText("");
	QString inputFile = QFileDialog::getOpenFileName(this, tr("打开多元线性回归数据源"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!inputFile.isEmpty())
	{
		ui->label_msg->setText("正在计算...");
		// 【0】计时开始
		QTime time;time.start();
		// 【1】载入\计算 (多元线性回归核心算法)
		QString strOutCsv;
		QString strOut = QLinearRegression(inputFile, strOutCsv);
//		qDebug()<<strOut;
		// 【2】保存文件
		QString outFileName = inputFile.right(inputFile.size() - inputFile.lastIndexOf('/')-1);
		outFileName = outFileName.left(outFileName.lastIndexOf('.'));
		outFileName += "-LRparameters.csv";
//		qDebug()<<"outFileName"<<outFileName;
		bool b = JIO::save(outFileName,strOutCsv);
		// 保存C语言风格的数组
		QString CStyleArrayStr{"double Answerx[cols]={"};
		CStyleArrayStr += strOutCsv + "};";
		outFileName.replace(QRegExp("\\.csv"), ".cpp");
		JIO::save("Code_"+outFileName,CStyleArrayStr);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if(b)
			ui->label_msg->setText("计算完毕！时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span> "+strOut);
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
	else
		ui->label_msg->setText("未选择文件！");
}

// 选择模型
void MainWindow::on_pushButton_SelectModel_clicked()
{
	ModelMatQList.clear();
	QString ModelFile = QFileDialog::getOpenFileName(this, tr("打开模型"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
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
			QList<double> predictedValueList;
			double predictedValue=0;
			for(int i=0; i<DataFileMat.size(); ++i)
			{
				for(int j=0; j<ModelMatQList.size(); ++j)
				{
					if(j == 0)
						predictedValue = ModelMatQList[0];
					else
					{
						predictedValue+= ModelMatQList[j]*DataFileMat[i][j-1];
					}
				}
				predictedValueList.append(predictedValue);
			}
	//		qDebug()<<"predictedValueLists "<<predictedValueList.size();
	//		qDebug()<<"predictedValueList0 "<<predictedValueList[0];
	//		qDebug()<<"predictedValueList1 "<<predictedValueList[1];
	//		qDebug()<<"predictedValueList2 "<<predictedValueList[2];

			for(int i=0; i<DataFileMat.size(); ++i)
			{
				DataFileMat[i].append(predictedValueList[i]);
			}
			// 提取文件名
			QString outFileName = DataFile.right(DataFile.size() - DataFile.lastIndexOf('/')-1);
			outFileName = outFileName.left(outFileName.lastIndexOf('.'));
			outFileName += "-predictedResult.csv";
			JIO::save(outFileName, DataFileMat);
			// 【4】计时结束
			QString timecost = QString::number(time.elapsed()/1000.0);
			emit sendMsg("预测完成！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		}
	}
}



/**************************************** 第2页 功能 ****************************************/

// 数据概览-载入数据
void MainWindow::on_pushButton_clicked()
{
	ui->textEdit->clear();
	// 载入文件
	QString InputFileName_str = QFileDialog::getOpenFileName(this,"载入数据", workspacePath,"textfile(*.csv);;All file(*.*)");
	qDebug()<<InputFileName_str;  // 文件名

	if(!InputFileName_str.isEmpty())
	{
		// 需要变量
		QStringList DataName; // 数据各项名
		QString strMatRow;
		QString strMatCol;
		QString strTitle;
		QString strMatSum;
		QString strMatArit;
		QString strMatMax;
		QString strMatMin;
		// 【0】计时开始
		QTime time;time.start();
		// 【1】数据分析
		mat Mat = DataAnalysis(InputFileName_str, DataName, strMatRow, strMatCol, strTitle, strMatSum, strMatArit, strMatMax, strMatMin);
		// 【2】数据概览-处理
		ui->label_2->setText("数据行列：" + strMatRow+ " x "+ strMatCol );
		ui->label_4->setText("数据名字：" + strTitle );
		ui->label_5->setText("数据总和：" + strMatSum );
		ui->label_6->setText("数据平均：" + strMatArit );
		ui->label_7->setText("数据最大：" + strMatMax );
		ui->label_8->setText("数据最小：" + strMatMin );
		// 【3】数据诊断
		bool b = DataDiagnosis(InputFileName_str, Mat, DataName, ui);
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

// 数据小处理-初始值为0
void MainWindow::on_pushButton_DataZero_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("初始值为0算法"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】初始值为0算法
		mat inputMat = JIO::readAMat(fileName);
		for(unsigned int j=0; j<inputMat.n_cols; ++j) // 列
		{
			double oneLineValue = inputMat(0,j);
			for(unsigned int i=0; i<inputMat.n_rows; ++i) // 遍历行，减第一行的值
			{
				inputMat(i,j) = inputMat(i,j) - oneLineValue;
			}
		}
		// 【2】保存文件
		QString saveFileName = "Zero_" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
		bool b = JIO::save(saveFileName, inputMat);
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
		mat inputMat = JIO::readAMat(fileName);
		for(unsigned int j=0; j<inputMat.n_cols; ++j) // 列 0列到n列
		{
			for(int i=inputMat.n_rows-1; i>0; --i) // 行n行到1行-倒序
			{
				inputMat(i,j) = inputMat(i,j) - inputMat(i-1,j);
			}
		}
		// 【2】保存文件
		QString saveFileName = "Delta_" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
		bool b = JIO::save(saveFileName, inputMat);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("增量化 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 抽样取点函数
QList<int> sampling(int nowNum, int needNum)
 {
	QList<int> outQList;
	// 压缩
	if(nowNum >= needNum)
	{
		double bilv = needNum*1.0 / nowNum;
		double *data = new double[nowNum];
		for (int i = 0; i<nowNum; ++i)
		{
			data[i] = floor(i*bilv);
		}

		int value = -1;
		for (int i = 0; i<nowNum; ++i)
		{
			if (data[i] != value)
			{
				outQList.append(i);
				value = data[i];
			}
		}
		delete data;
	}
	// 拉伸
	if(nowNum < needNum)
	{
		double bilv = nowNum*1.0 / needNum;
		double *data = new double[needNum];
		for (int i = 0; i<needNum; ++i)
		{
			data[i] = floor(i*bilv);
			outQList.append(data[i]);
		}
		delete data;
	}

	return outQList;
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
		mat inputMat = JIO::readAMat(fileName);
		int nowNum = inputMat.n_rows;
		int needNum = ui->lineEdit_DataSampling->text().toInt();
		QList<int> needLine = sampling(nowNum, needNum);
		mat out(needLine.size(),inputMat.n_cols);
		out.fill(0);
		for(unsigned int j=0; j<inputMat.n_cols; ++j) // 列 0列到n列
		{
			for(int i=0; i<needLine.size(); ++i) // 行
			{
				out(i,j) = inputMat(needLine[i],j);
			}
		}
		// 【2】保存文件
		QString saveFileName;
		if(nowNum >= needNum)
			saveFileName = "Minus_" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
		else
			saveFileName = "Add_" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);

		bool b = JIO::save(saveFileName, out);
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
		mat inputMat = JIO::readAMat(fileName);
		for(unsigned int j=0; j<inputMat.n_cols; ++j) // 列 0列到n列
		{
			for(unsigned int i=1; i<inputMat.n_rows; ++i) // 行
			{
				double value = inputMat(0,j);
				inputMat(0,j) = (maxNum - value) >  (value - minNum) ? minNum:maxNum;
				// 超出范围执行
				if(inputMat(i,j) > maxNum || inputMat(i,j) < minNum)
				{
					// 确保上一个不超出范围
					if(inputMat(i-1,j) > minNum && inputMat(i-1,j) < maxNum )
						inputMat(i,j) = inputMat(i-1,j);
					else
					{
						double value = inputMat(i,j);
						inputMat(i,j) = (maxNum - value) >  (value - minNum) ? minNum:maxNum;
					}
				}
			}
		}
		// 【2】保存文件
		QString saveFileName = "Clean_" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
		bool b = JIO::save(saveFileName, inputMat);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("数据清洗 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 功能: 按窗口大小寻找某一序列的 最大值\最小值\平均值 集合
// 参数: 输入向量, 窗口大小, 输出最大值向量, 输出最小值向量
bool findTrendByWindow(QList<double> &input, int window, QList<double> &outMax, QList<double> &outMin, QList<double> &outAvg)
{
	double max = -100000000;
	double min = 100000000;
	double sum = 0;
	for (int i = 0; i<input.size(); i = i + window)
	{
		int cnt = 0;
		for (int j = 0; j< window; j++)
		{
			if ((i + j) >= input.size())
			{
				cnt = j;
				break;
			}
			if (input[i + j] >max) max = input[i + j];
			if (input[i + j] <min) min = input[i + j];
			sum += input[i + j];
			cnt = j + 1;
		}
		outMax.append(max);
		outMin.append(min);
		outAvg.append(sum / cnt);
		//cout<<max<<" ";
		//cout<<min<<" ";
		max = -100000000;
		min = 100000000;
		sum = 0;
	}
	return true;
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
		mat inputMat = JIO::readAMat(fileName);
		mat matMaxOut;
		mat matMinOut;
		mat matAvgOut;
		for(unsigned int j=0; j<inputMat.n_cols; ++j) // 列 0列到n列
		{
			QList<double> input;
			QList<double> outMax;
			QList<double> outMin;
			QList<double> outAvg;
			// 得到QList输入
			for(unsigned int i=1; i<inputMat.n_rows; ++i) // 行
			{
				input.append(inputMat(i,j));
			}
			// 加窗滤波
			findTrendByWindow(input, window, outMax, outMin, outAvg);
			// 写入mat
			mat matMax(outMax.size(),1);
			mat matMin(outMin.size(),1);
			mat matAvg(outAvg.size(),1);
			for(unsigned int i=0; i<matMax.n_rows;++i)
			{
				matMax(i,0) = outMax[i];
				matMin(i,0) = outMin[i];
				matAvg(i,0) = outAvg[i];
			}
			// 合入大mat
			matMaxOut = join_rows(matMaxOut, matMax);
			matMinOut = join_rows(matMinOut, matMin);
			matAvgOut = join_rows(matAvgOut, matAvg);
		}
		// 【2】保存文件
		QString saveFileName1 = "TrendMax_" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
		QString saveFileName2 = "TrendMin_" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
		QString saveFileName3 = "TrendAvg_" + fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
		bool b = JIO::save(saveFileName1, matMaxOut);
		b = JIO::save(saveFileName2, matMinOut);
		b = JIO::save(saveFileName3, matAvgOut);
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("趋势预测 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

// 虚拟映射-数据通道映射为虚拟标准通道-FBG温度
void MainWindow::on_pushButton_VirtualMap_T_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("虚拟映射-FBG温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	// 【0】计时开始
	QTime time;time.start();
	bool b = false;

	for(QString fileName: fileNameList)
	{
		if(!fileName.isEmpty())
		{
			// 【1】虚拟映射算法
			QStringList itemName;
			QStringList timeName;
			mat inputMat = JIO::readCsv(fileName, itemName, timeName);
			mat outputMat;
			for(int i=0; i<219; ++i) // FBG温度219个
			{
				outputMat = join_rows(outputMat, inputMat.col( VirtualMap_FBGT_Index[i] ));
			}
			// 【2】保存文件
			QString saveFileName = getFileName(fileName);
			saveFileName = saveFileName.left(10)+"_FBGT_Wave.csv";
			b = JIO::save(saveFileName, VirtualMap_FBGT_Now, timeName, outputMat);

		}
	}

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
	bool b = false;

	for(QString fileName: fileNameList)
	{
		if(!fileName.isEmpty())
		{
			// 【1】虚拟映射算法
			QStringList itemName;
			QStringList timeName;
			mat inputMat = JIO::readCsv(fileName, itemName, timeName);
			mat outputMat;
			for(int i=0; i<90; ++i) // FBG应力90个
			{
				outputMat = join_rows(outputMat, inputMat.col( VirtualMap_FBGS_Index[i] ));
			}
			// 【2】保存文件
			QString saveFileName = getFileName(fileName);
			saveFileName = saveFileName.left(10)+"_FBGS_Wave.csv";
			b = JIO::save(saveFileName, VirtualMap_FBGS_Now, timeName, outputMat);
		}
	}

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
	bool b = false;

	for(QString fileName: fileNameList)
	{
		if(!fileName.isEmpty())
		{
			// 【1】虚拟映射算法
			QStringList itemName;
			QStringList timeName;
			mat inputMat = JIO::readCsv(fileName, itemName, timeName);
			// 【2】保存文件
			QString saveFileName = getFileName(fileName);
			saveFileName = saveFileName.left(10)+"_DS18_Temp.csv";
			b = JIO::save(saveFileName, VirtualMap_DS18_Now, timeName, inputMat);
		}

	}

	// 【4】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		ui->label_msg->setText("虚拟映射 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
}

// 数据小处理-按天拆分
void MainWindow::on_pushButton_SplitByDate_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("虚拟映射-电类温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!fileName.isEmpty())
	{
		// 【0】计时开始
		QTime time;time.start();
		// 【1】虚拟映射算法
		QStringList inputList = JIO::readFile(fileName);
		QString itemName = inputList[0];
		qDebug()<< "itemName" << itemName;
		// 得到日期集合
		QSet<QString> dateSet;
		for(auto e: inputList)
		{
			if(e.left(4) != "Time") // 排除第一行
				dateSet.insert(e.left(10));
		}

		// 【2】保存文件-保存"日期个数"个文件
		bool b = true;
		for(QString dateName: dateSet)
		{
			qDebug()<< dateName;
			QString saveName = dateName+ "_" + getFileName(fileName);
			QFile f(saveName);
			qDebug()<< saveName;
			if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				qDebug() << "[on_pushButton_SplitByDate_clicked] Open failed.";
				b = false;
			}
			QTextStream txtOutput(&f);
			txtOutput << itemName << "\n";
			for (auto e : inputList)
			{
				if(e.left(10) == dateName)
					txtOutput << e.right(e.size() -11) << "\n"; // 去掉日期
			}
			f.close();
		}

		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("按天拆分 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
	}
}

double VirtualCalibrationWave[219] = {1535.867297,1537.898297,1539.888297,1541.932297,1543.881297,1545.874297,1547.866297,1549.865297,1551.858297,1553.894297,1555.881297,1557.893297,1559.860297,1561.834297,1563.778297,1535.808297,1537.832297,1539.819297,1541.861297,1543.839297,1545.809297,1547.795297,1549.811297,1551.792297,1553.795297,1555.860297,1557.874297,1559.894297,1562.029297,1564.040297,1535.815297,1537.827297,1539.808297,1541.838297,1543.817297,1545.785297,1547.790297,1549.762297,1551.890297,1553.870297,1555.909297,1557.923297,1559.911297,1561.912297,1563.887297,1535.827297,1537.807297,1539.781297,1541.826297,1543.809297,1545.787297,1547.780297,1549.829297,1551.969297,1553.993297,1555.947297,1557.783297,1559.803297,1561.900297,1563.805297,1547.761297,1545.786297,1543.788297,1541.881297,1539.835297,1537.848297,1535.841297,1549.802297,1551.869297,1553.960297,1555.991297,1558.023297,1559.904297,1561.915297,1563.855297,1549.939297,1547.909297,1545.902297,1543.949297,1541.955297,1539.918297,1537.958297,1535.895297,1551.846297,1553.848297,1555.213297,1557.860297,1559.810297,1561.885297,1563.800297,1537.812297,1540.913297,1543.840297,1546.784297,1537.779297,1540.773297,1543.799297,1546.730297,1537.809297,1540.797297,1543.833297,1546.793297,1536.790297,1539.761297,1542.678297,1545.750297,1548.723297,1551.821297,1554.698297,1557.708297,1560.744297,1564.198297,1537.858297,1540.838297,1543.730297,1546.779297,1549.710297,1552.768297,1555.864297,1558.796297,1561.764297,1565.784297,1537.820297,1540.742297,1543.675297,1546.747297,1549.676297,1552.742297,1555.818297,1558.698297,1561.761297,1536.780297,1539.734297,1542.724297,1545.775297,1548.729297,1551.877297,1554.742297,1557.755297,1560.724297,1563.722297,1537.849297,1540.793297,1543.727297,1546.775297,1549.831297,1552.720297,1555.860297,1558.760297,1561.718297,1565.773297,1536.825297,1539.834297,1542.708297,1545.727297,1548.845297,1551.839297,1554.708297,1557.712297,1560.745297,1563.705297,1537.781297,1540.792297,1543.697297,1546.728297,1549.697297,1552.745297,1555.815297,1558.726297,1561.754297,1565.731297,1531.952654,1535.208654,1538.214654,1541.188654,1544.226654,1547.039654,1550.096654,1553.297654,1556.012654,1559.115654,1547.147654,1553.152654,1556.172654,1558.995654,1556.524654,1559.640654,1533.068654,1536.184654,1537.912654,1541.676654,1544.517654,1547.761654,1550.465654,1553.851654,1532.840654,1535.916654,1539.192654,1541.856654,1544.839654,1547.867654,1551.151654,1553.933654,1557.041654,1560.122654,1532.031654,1535.036654,1538.058654,1540.987654,1560.525654,1557.618654,1532.885654,1536.390654,1539.320654,1542.156654,1545.346654,1548.191654,1551.463654,1555.809654};
double VirtualCalibrationWave_201706[219] = {1535.904935,1537.922935,1539.914935,1541.995935,1543.939935,1545.929935,1547.917935,1549.916935,1551.879935,1553.920935,1555.913935,1557.932935,1559.930935,1561.887935,1563.856935,1535.783935,1537.795935,1539.763935,1541.805935,1543.773935,1545.736935,1547.697935,1549.711935,1551.760935,1553.772935,1555.841935,1557.852935,1559.863935,1561.968935,1563.999935,1535.852935,1537.867935,1539.850935,1541.882935,1543.860935,1545.827935,1547.825935,1549.812935,1551.853935,1553.842935,1555.902935,1557.921935,1559.905935,1561.909935,1563.913935,1535.791935,1537.761935,1539.746935,1541.788935,1543.745935,1545.730935,1547.735935,1549.788935,1552.028935,1554.061935,1555.993935,1557.867935,1559.806935,1561.855935,1563.784935,1547.716935,1545.720935,1543.722935,1541.811935,1539.771935,1537.780935,1535.763935,1549.749935,1551.824935,1553.993935,1555.987935,1558.002935,1559.960935,1561.964935,1563.892935,1549.960935,1547.924935,1545.928935,1543.978935,1541.981935,1539.946935,1537.987935,1535.928935,1551.842935,1553.897935,1555.927935,1557.933935,1559.824935,1561.886935,1563.786935,1557.851935,1559.829935,1561.837935,1563.828935,1550.810935,1552.817935,1554.887935,1556.910935,1543.797935,1545.859935,1547.875935,1549.796935,1536.743935,1540.147935,1542.658935,1545.708935,1548.680935,1551.780935,1554.632935,1557.651935,1560.703935,1564.232935,1537.753935,1540.718935,1543.647935,1546.681935,1549.628935,1552.695935,1555.798935,1558.743935,1561.680935,1565.721935,1537.769935,1540.692935,1543.631935,1546.709935,1549.651935,1552.693935,1555.738935,1558.603935,1561.651935,1536.721935,1539.699935,1542.679935,1545.708935,1548.706935,1551.839935,1554.688935,1557.713935,1560.654935,1563.670935,1537.821935,1540.737935,1543.674935,1546.708935,1549.791935,1552.672935,1555.814935,1558.717935,1561.664935,1565.729935,1536.788935,1539.784935,1542.675935,1545.679935,1548.782935,1551.798935,1554.594935,1557.643935,1560.681935,1563.663935,1537.735935,1540.747935,1543.651935,1546.684935,1549.648935,1552.670935,1555.756935,1558.674935,1561.698935,1565.608935,1531.866373,1535.110373,1538.124373,1541.071373,1544.176373,1546.921373,1549.987373,1553.191373,1555.912373,1558.908373,1547.044373,1553.051373,1556.093373,1558.908373,1556.423373,1559.579373,1532.957373,1536.057373,1537.853373,1541.592373,1544.438373,1547.669373,1550.410373,1553.785373,1532.746373,1535.805373,1539.120373,1541.770373,1544.770373,1547.781373,1551.103373,1553.864373,1556.931373,1560.063373,1531.926373,1534.915373,1537.948373,1540.873373,1560.314373,1557.431373,1532.791373,1536.333373,1539.324373,1542.104373,1545.254373,1548.088373,1551.417373,1554.201373};
// 虚拟映射-映射的FBG温度传感器转温度值
void MainWindow::on_pushButton_VirtualFBGtoTEMP_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("FBG转温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	// 【0】计时开始
	QTime time;time.start();
	bool b = false;

	for(QString fileName: fileNameList)
	{
		if(!fileName.isEmpty())
		{
			// 【1】FBG转温度算法
			QStringList itemName;
			QStringList timeName;
			mat inputMat = JIO::readCsv(fileName, itemName, timeName);
			for(unsigned int j=0; j<inputMat.n_cols;++j)
			{
				for(unsigned int i=0; i<inputMat.n_rows;++i)
				{
					double value;
					if(j<=170) // 0.0105 其它
					{
						value = 1.0/0.0105*(inputMat(i,j) - VirtualCalibrationWave_201706[j]);
						inputMat(i,j) = value;
					}
					if(j>170) // 0.0400 立柱
					{
						value = 1.0/0.0400*(inputMat(i,j) - VirtualCalibrationWave_201706[j]);
						inputMat(i,j) = value;
					}
				}
			}
			// 【2】保存文件
			QString saveFileName = getFileName(fileName);
			saveFileName = saveFileName.left(10)+"_FBGT_Temp.csv";
			QString itemNameStr = itemName.join(",");
			b = JIO::save(saveFileName, itemNameStr, timeName, inputMat);
		}
	}

	// 【4】计时结束
	QString timecost = QString::number(time.elapsed()/1000.0);
	if( b )
		ui->label_msg->setText("映射FBG转温度 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	else
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");
}

// 应力换算2017-02-27第一次次数据-以此为基准
double FBGS_Wave[90] = {1536.538,1538.491,1540.669,1543.024,1545.057,1547.014,1549.062,1551.084,1552.100,1554.442,1556.414,1558.423,1560.364,1562.381,1564.149,1536.524,1538.377,1540.426,1542.449,1544.438,1546.725,1548.604,1552.087,1552.087,1554.266,1556.450,1558.222,1560.152,1562.522,1564.342,1536.611,1538.430,1540.360,1542.215,1544.126,1546.174,1548.263,1550.272,1552.234,1554.399,1556.324,1558.156,1560.258,1562.358,1564.019,1536.208,1538.193,1540.164,1542.220,1544.249,1546.220,1547.654,1550.319,1552.137,1554.136,1555.328,1558.223,1560.197,1561.972,1564.187,1548.145,1546.272,1544.277,1542.351,1540.403,1538.269,1536.357,1550.186,1552.186,1554.295,1556.523,1559.289,1560.399,1562.205,1564.002,0.000,0.000,0.000,0.000,0.000,0.000,1548.492,1550.196,1552.032,1554.376,1556.247,1558.303,1560.322,1562.776,1564.310};
double FBGT_Wave[90] = {1536.001,1538.032,1540.022,1542.066,1544.015,1546.008,1548,1549.999,1551.992,1554.028,1556.015,1558.027,1559.994,1561.968,1563.912,1535.942,1537.966,1539.953,1541.995,1543.973,1545.943,1547.929,1549.945,1551.926,1553.929,1555.994,1558.008,1560.028,1562.163,1564.174,1535.949,1537.961,1539.942,1541.972,1543.951,1545.919,1547.924,1549.896,1552.024,1554.004,1556.043,1558.057,1560.045,1562.046,1564.021,1535.961,1537.941,1539.915,1541.96,1543.943,1545.921,1547.914,1549.963,1552.103,1554.127,1556.081,1557.917,1559.937,1562.034,1563.939,1547.895,1545.92,1543.922,1542.015,1539.969,1537.982,1535.975,1549.936,1552.003,1554.094,1556.125,1558.157,1560.038,1562.049,1563.989,1550.073,1548.043,1546.036,1544.083,1542.089,1540.052,1538.092,1536.029,1551.98,1553.982,1555.347,1557.994,1559.944,1562.019,1563.934};
// 虚拟映射-映射的FBG应力传感器转应力
void MainWindow::on_pushButton_VirtualFBGtoSTRESS_clicked()
{
	QStringList fileNameList = QFileDialog::getOpenFileNames(this, tr("FBG转温度"), workspacePath, tr("textfile(*.csv*);;Allfile(*.*)"));
	// 【0】计时开始
	QTime time;time.start();
	bool b = false;

	if(fileNameList.size() !=2 )
	{
		emit sendMsg("文件状态：未载入");
		QMessageBox::critical(NULL, "注意", "请选择配套的应力、温度2个文件！", QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		QString fileNameTemp;
		QString fileNameStress;
		for(QString fileName: fileNameList)
		{
			if(fileName.right(10) == "T_Wave.csv")
				fileNameTemp = fileName;
			if(fileName.right(10) == "S_Wave.csv")
				fileNameStress = fileName;
		}

		if(!fileNameTemp.isEmpty() && !fileNameStress.isEmpty() )
		{
			//【1】应力换算算法-两个数组
			QStringList itemNameT;
			QStringList timeNameT;
			mat inputTempALL = JIO::readCsv(fileNameTemp, itemNameT, timeNameT);

			QStringList itemNameS;
			QStringList timeNameS;
			mat inputStress = JIO::readCsv(fileNameStress, itemNameS, timeNameS);

			// 套入公式
			for(unsigned int j=0; j<inputStress.n_cols;++j)
			{
				for(unsigned int i=0; i<inputStress.n_rows;++i)
				{
					// 核心公式
					inputStress(i,j) = ((inputStress(i,j)-FBGS_Wave[j])/FBGS_Wave[j]-0.920863*(inputTempALL(i,j)-FBGT_Wave[j])/FBGT_Wave[j])/0.776*1000000;
				}
			}
			// 【2】保存文件
			QString saveFileName = getFileName(fileNameStress);
			saveFileName = saveFileName.left(10)+"_FBGS_Stress.csv";
			QString itemNameStr = itemNameS.join(",");
			b = JIO::save(saveFileName, itemNameStr, timeNameS, inputStress);
		}

		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b )
			ui->label_msg->setText("映射FBG转应力 处理成功! <span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'处理失败！</span>");

	}

}
