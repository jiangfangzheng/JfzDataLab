#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "skins/skins.h"
#include "tools/jfzlib.h"
#include "algorithm/datatostand.h"
#include "algorithm/correlations.h"
#include "algorithm/datadiagnosis.h"

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
}

MainWindow::~MainWindow()
{
	delete ui;
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
	// 下拉框选择
	QStringList qssName;
	qssName << "系统" << "黑色" << "灰黑色" << "灰色" << "浅灰色" << "深灰色" << "银色" << "淡蓝色" << "蓝色";
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
		ui->label_msg->setText("FBG温度处理中！");
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
		if( b == 0 )
			ui->label_msg->setText("FBG温度保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		else
			ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'保存失败！</span>");
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
		ui->label_msg->setText("文件状态：未载入");
		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个文件！", QMessageBox::Yes, QMessageBox::Yes);
		ui->pushButton_ENV->setEnabled(true);
	}
	else
	{
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'>正在开启线程处理xls文件...</span>");
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

