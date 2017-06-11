#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "skins/skins.h"
#include "tools/jfzlib.h"
#include "tools/JMat.h"
#include "tools/JIO.h"
#include "tools/JSQL.h"
#include "algorithm/datatostand.h"
#include "algorithm/correlations.h"
#include "algorithm/datadiagnosis.h"
#include "algorithm/LinearRegression.h"
#include "algorithm/DataToSQL.h"
#include "plugins/qcustomplot.h"

// 数据库类型
#define DATABASETYPE "QMYSQL"
//#define DATABASETYPE "QSQLITE"

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

// 电类标准化
void MainWindow::on_pushButton_DS18B20_clicked()
{
	ui->label_msg->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
	ui->pushButton_ENV->setEnabled(false);
	EnvFileNameList = QFileDialog::getOpenFileNames(this, tr("打开环境温度数据"), " ", tr("textfile(*.xls);"));
	if(EnvFileNameList.size() != 4)
	{
		emit sendMsg("文件状态：未载入");
		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个文件！", QMessageBox::Yes, QMessageBox::Yes);
		ui->pushButton_ENV->setEnabled(true);
	}
	else
	{
		emit sendMsg("<span style='color: rgb(255, 0, 0);'>正在开启线程处理xls文件...</span>");
		// 开启线程处理xls文件，防止界面卡死
		EnvXlsReadThread *readxls = new EnvXlsReadThread(EnvFileNameList, ui);
		readxls->start();
	}
}

// FBG波长转温度
void MainWindow::on_pushButton_LoadFBGT_clicked()
{
	standFBGFileName = QFileDialog::getOpenFileName(this, tr("打开标准FBG波长"), " ", tr("textfile(*.csv*);;Allfile(*.*)"));
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
	correlationFileName1 = QFileDialog::getOpenFileName(this, tr("打开文件1"), " ", tr("textfile(*.csv*);;Allfile(*.*)"));
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
	correlationFileName2 = QFileDialog::getOpenFileName(this, tr("打开文件1"), " ", tr("textfile(*.csv*);;Allfile(*.*)"));
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

// 数据概览-载入数据
void MainWindow::on_pushButton_clicked()
{
	ui->textEdit->clear();
	// 载入文件
	QString InputFileName_str = QFileDialog::getOpenFileName(this,"open file"," ","textfile(*.csv);;All file(*.*)");
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

// 多元线性回归
void MainWindow::on_pushButton_LinearRegression_clicked()
{
	ui->label_msg->setText("");
	QString inputFile = QFileDialog::getOpenFileName(this, tr("打开数据源"), " ", tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!inputFile.isEmpty())
	{
		ui->label_msg->setText("正在计算...");
		// 【0】计时开始
		QTime time;time.start();
		// 【1】载入\计算
		QString strOut = QLinearRegression(inputFile);
		// 【2】保存文件(暂不保存)
		int b=0;
		// 【4】计时结束
		QString timecost = QString::number(time.elapsed()/1000.0);
		if( b == 0 )
			ui->label_msg->setText(strOut +" 花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
	}
	else
		ui->label_msg->setText("未选择文件！");
}

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
	QString inputFile = QFileDialog::getOpenFileName(this, tr("打开数据源"), " ", tr("textfile(*.csv*);;Allfile(*.*)"));
	if(!inputFile.isEmpty())
	{
		// 【1】载入数据
		plotDataStrList = JIO::CsvToStrList(inputFile);
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

// CCD导入数据库
void MainWindow::on_pushButton_CCDinSQL_clicked()
{
	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
		JSQL jsql("localhost","data_wuzhong","root","root",DATABASETYPE);
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
	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/data/fbg/",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
		JSQL jsql("localhost","data_wuzhong","root","root",DATABASETYPE);
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
	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
		JSQL jsql("localhost","data_wuzhong","root","root",DATABASETYPE);
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
	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
		JSQL jsql("localhost","data_wuzhong","root","root",DATABASETYPE);
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
	ui->label_msg->setText("");
	// 载入文件夹
	QString strDir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
		JSQL jsql("localhost","data_wuzhong","root","root",DATABASETYPE);
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
