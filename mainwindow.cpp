#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "skins/skins.h"
#include "tools/jfzlib.h"
#include "algorithm/datatostand.h"

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

void MainWindow::initSkins()
{
	// 对应的主题样式
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
	qssName << "黑色" << "灰黑色" << "灰色" << "浅灰色" << "深灰色" << "银色" << "淡蓝色" << "蓝色";
//	ui->cboxSkins->addItems(qssName);
//	ui->cboxSkins->setCurrentIndex(5);// 个人最喜欢银，默认
	// 直接赋值
	QString qssFile = mapStyle["银色"];
	Skins::setStyle(qssFile);
}

// 电类标准化
void MainWindow::on_pushButton_DS18B20_clicked()
{
	ui->label_3->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		ui->label_3->setText("电类处理中！");
		QTime time;time.start(); // 计时
		// 指定文件名
		QString CH1 = dir + "/通道1.csv";
		QString CH2 = dir + "/通道2.csv";
		QString CH3 = dir + "/通道3.csv";
		QString CH4 = dir + "/通道4.csv";
		QString CH5 = dir + "/通道5.csv";
		QString CH6 = dir + "/通道6.csv";
		QString CH7 = dir + "/通道7.csv";
		QString  DataName_DS18B20= "CH01-01,CH01-02,CH01-03,CH01-04,CH01-05,CH01-06,CH01-07,CH01-08,CH02-01,CH02-02,CH02-03,CH02-04,CH02-05,CH02-06,CH02-07,CH02-08,CH03-01,CH03-02,CH03-03,CH03-04,CH03-05,CH03-06,CH03-07,CH03-08,CH04-01,CH04-02,CH04-03,CH04-04,CH04-05,CH04-06,CH04-07,CH04-08,CH05-01,CH05-02,CH05-03,CH05-04,CH05-05,CH05-06,CH05-07,CH05-08,CH06-01,CH06-02,CH06-03,CH06-04,CH06-05,CH06-06,CH06-07,CH06-08,CH07-01,CH07-02,CH07-03,CH07-04";
		QStringList Time;
		int a = Stand_DS18B20(CH1, CH2, CH3, CH4, CH5, CH6, CH7, MatDS18B20, Time);// 各通道数据转Mat并排序
		if( a == 0 )
			ui->label_3->setText("拟载入成功！");
		// 保存文件
		int b = saveStandData("Data-DS18B20",DataName_DS18B20,Time, MatDS18B20);
		if( b == 0 )
		{
			QString timecost = QString::number(time.elapsed()/1000.0);
			ui->label_3->setText("电类温度保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		}
	}
}

// CCD标准化
void MainWindow::on_pushButton_CCD_clicked()
{
	ui->label_3->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		ui->label_3->setText("CCD处理中！");
		QTime time;time.start(); // 计时
		// 指定文件名
		QString CH1 = dir + "/CCD数据值.csv";
		QString  DataName_CCD= "X,Y,Z";
		QStringList Time;
		int a = Stand_CCD(CH1, MatCCD, Time);// 各通道数据转Mat、得到时间数组
		if( a == 0 )
			ui->label_3->setText("拟载入成功！");
		// 保存文件
		int b = saveStandData("Data-CCD",DataName_CCD,Time, MatCCD);
		if( b == 0 )
		{
			QString timecost = QString::number(time.elapsed()/1000.0);
			ui->label_3->setText("CCD位移保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		}
	}
}

// FBG温度标准化
void MainWindow::on_pushButton_FBGT_clicked()
{
	ui->label_3->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		ui->label_3->setText("FBG温度处理中！");
		QTime time;time.start(); // 计时
		// 指定文件名
		QString CH08 = dir + "/通道8.csv";
		QString CH09 = dir + "/通道9.csv";
		QString CH10 = dir + "/通道10.csv";
		QString CH11 = dir + "/通道11.csv";
		QString CH12 = dir + "/通道12.csv";
		QString CH13 = dir + "/通道13.csv";
		QString CH14 = dir + "/通道14.csv";
		QString CH15 = dir + "/通道15.csv";
		QString CH16 = dir + "/通道16.csv";
		QString CH17 = dir + "/通道17.csv";
		QString CH18 = dir + "/通道18.csv";
		QString CH19 = dir + "/通道19.csv";
		QString CH20 = dir + "/通道20.csv";
		QString CH21 = dir + "/通道21.csv";
		QString CH22 = dir + "/通道22.csv";
		QString CH23 = dir + "/通道23.csv";
		QString CH24 = dir + "/通道24.csv";
		QString CH25 = dir + "/通道25.csv";
		QString CH26 = dir + "/通道26.csv";
		QString CH27 = dir + "/通道27.csv";
		QString CH28 = dir + "/通道28.csv";
		QString CH29 = dir + "/通道29.csv";
		QString CH30 = dir + "/通道30.csv";
		QString CH31 = dir + "/通道31.csv";
		QString CH32 = dir + "/通道32.csv";

		QString  DataName_FBGT= "CH08-01,CH08-02,CH08-03,CH08-04,CH08-05,CH08-06,CH08-07,CH08-08,CH08-09,CH08-10,CH08-11,CH08-12,CH08-13,CH08-14,CH08-15,CH09-01,CH09-02,CH09-03,CH09-04,CH09-05,CH09-06,CH09-07,CH09-08,CH10-01,CH10-02,CH10-03,CH10-04,CH10-05,CH10-06,CH10-07,CH11-01,CH11-02,CH11-03,CH11-04,CH11-05,CH11-06,CH11-07,CH11-08,CH11-09,CH11-10,CH11-11,CH11-12,CH11-13,CH11-14,CH11-15,CH12-01,CH12-02,CH12-03,CH12-04,CH12-05,CH12-06,CH12-07,CH12-08,CH12-09,CH12-10,CH12-11,CH12-12,CH12-13,CH12-14,CH12-15,CH13-01,CH13-02,CH13-03,CH13-04,CH13-05,CH13-06,CH13-07,CH13-08,CH13-09,CH13-10,CH13-11,CH13-12,CH13-13,CH13-14,CH13-15,CH14-01,CH14-02,CH14-03,CH14-04,CH14-05,CH14-06,CH14-07,CH15-01,CH15-02,CH15-03,CH15-04,CH15-05,CH15-06,CH15-07,CH15-08,CH16-01,CH16-02,CH16-03,CH16-04,CH16-05,CH16-06,CH16-07,CH16-08,CH17-01,CH17-02,CH17-03,CH17-04,CH17-05,CH17-06,CH17-07,CH17-08,CH18-01,CH18-02,CH18-03,CH18-04,CH18-05,CH18-06,CH18-07,CH18-08,CH19-01,CH19-02,CH19-03,CH19-04,CH19-05,CH19-06,CH19-07,CH19-08,CH19-09,CH19-10,CH20-01,CH20-02,CH20-03,CH20-04,CH20-05,CH20-06,CH20-07,CH20-08,CH20-09,CH20-10,CH21-01,CH21-02,CH21-03,CH21-04,CH21-05,CH21-06,CH21-07,CH22-01,CH22-02,CH22-03,CH22-04,CH22-05,CH22-06,CH22-07,CH22-08,CH22-09,CH22-10,CH23-01,CH23-02,CH23-03,CH23-04,CH24-01,CH24-02,CH24-03,CH24-04,CH24-05,CH24-06,CH24-07,CH24-08,CH24-09,CH24-10,CH25-01,CH25-02,CH25-03,CH25-04,CH25-05,CH25-06,CH25-07,CH25-08,CH25-09,CH25-10,CH26-01,CH26-02,CH26-03,CH26-04,CH27-01,CH27-02,CH27-03,CH27-04,CH27-05,CH27-06,CH27-07,CH27-08,CH27-09,CH28-01,CH28-02,CH28-03,CH28-04,CH28-05,CH28-06,CH28-07,CH28-08,CH28-09,CH28-10,CH29-01,CH29-02,CH29-03,CH29-04,CH29-05,CH29-06,CH29-07,CH29-08,CH29-09,CH29-10,CH30-01,CH30-02,CH30-03,CH30-04,CH30-05,CH30-06,CH30-07,CH30-08,CH30-09,CH30-10,CH31-01,CH31-02,CH31-03,CH31-04,CH31-05,CH31-06,CH31-07,CH31-08,CH31-09,CH31-10,CH32-01,CH32-02,CH32-03";
		QStringList Time;
		int a = Stand_FBGT( CH08,  CH09,  CH10,  CH11,  CH12,  CH13,  CH14,  CH15,  CH16, \
							CH17,  CH18,  CH19,  CH20,  CH21,  CH22,  CH23,  CH24,  CH25, \
							CH26,  CH27,  CH28,  CH29,  CH30,  CH31,  CH32, MatFBGT, Time);
		if( a == 0 )
			ui->label_3->setText("拟载入成功！");
		// 保存文件
		int b = saveStandData("Data-FBGTemperature",DataName_FBGT,Time, MatFBGT);
		if( b == 0 )
		{
			QString timecost = QString::number(time.elapsed()/1000.0);
			ui->label_3->setText("FBG温度保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		}
	}
}

// FBG应力标准化
void MainWindow::on_pushButton_FBGS_clicked()
{
	ui->label_3->setText("文件状态：未载入");
	// 载入文件夹 ，填"/"跳到根目录， 填“” 默认程序位置
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		ui->label_3->setText("FBG应力处理中！");
		QTime time;time.start(); // 计时
		// 指定文件名
		QString CH01 = dir + "/通道1.csv";
		QString CH02 = dir + "/通道2.csv";
		QString CH03 = dir + "/通道3.csv";
		QString CH04 = dir + "/通道4.csv";
		QString CH05 = dir + "/通道5.csv";
		QString CH06 = dir + "/通道6.csv";
		QString CH07 = dir + "/通道7.csv";

		QString  DataName_FBGS= "CH01-01,CH01-02,CH01-03,CH01-04,CH01-05,CH01-06,CH01-07,CH01-08,CH01-09,CH01-10,CH01-11,CH01-12,CH01-13,CH01-14,CH01-15,CH02-01,CH02-02,CH02-03,CH02-04,CH02-05,CH02-06,CH02-07,CH02-08,CH03-01,CH03-02,CH03-03,CH03-04,CH03-05,CH03-06,CH03-07,CH03-08,CH03-09,CH03-10,CH03-11,CH03-12,CH03-13,CH03-14,CH03-15,CH04-01,CH04-02,CH04-03,CH04-04,CH04-05,CH04-06,CH04-07,CH04-08,CH04-09,CH04-10,CH04-11,CH04-12,CH04-13,CH04-14,CH04-15,CH05-01,CH05-02,CH05-03,CH05-04,CH05-05,CH05-06,CH05-07,CH05-08,CH05-09,CH05-10,CH05-11,CH05-12,CH05-13,CH05-14,CH05-15,CH06-01,CH06-02,CH06-03,CH06-04,CH06-05,CH06-06,CH06-07,CH06-08,CH06-09,CH06-10,CH06-11,CH06-12,CH06-13,CH06-14,CH06-15,CH07-01,CH07-02,CH07-03,CH07-04,CH07-05,CH07-06,CH07-07";
		QStringList Time;
		int a = Stand_FBGS(CH01, CH02, CH03, CH04, CH05, CH06, CH07, MatFBGS, Time);// 各通道数据转Mat并排序
		if( a == 0 )
			ui->label_3->setText("拟载入成功！");
		// 保存文件
		int b = saveStandData("Data-FBGStress",DataName_FBGS,Time, MatFBGS);
		if( b == 0 )
		{
			QString timecost = QString::number(time.elapsed()/1000.0);
			ui->label_3->setText("FBG应力保存成功！花费时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		}
	}
}

// 环境温度标准化
void MainWindow::on_pushButton_ENV_clicked()
{
	ui->label_3->setText("...");
	ui->pushButton_ENV->setEnabled(false);
	EnvFileNameList = QFileDialog::getOpenFileNames(this, tr("打开环境温度数据"), " ", tr("textfile(*.xls);"));
	if(EnvFileNameList.size() != 4)
	{
		ui->label_3->setText("请选择环境温度4个文件！");
		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个文件！", QMessageBox::Yes, QMessageBox::Yes);
		ui->pushButton_ENV->setEnabled(true);
	}
	else
	{
		ui->label_3->setText("<span style='color: rgb(255, 0, 0);'>正在开启线程处理xls文件...</span>");
		EnvXlsReadThread *readxls = new EnvXlsReadThread(EnvFileNameList, ui);
		readxls->start();

	}
}

// 数据预处理-波长修复
void MainWindow::on_pushButton_LoadFBGT_ALL_clicked()
{
	ui->label_10->setText("文件状态：未载入");
	// 载入文件夹
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory")," ",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	// 载入成功操作
	if(!dir.isEmpty())
	{
		ui->label_10->setText("FBG温度波长修复中！");
		QTime time;time.start(); // 计时
		// 指定文件名
		QString CH08 = dir + "/通道8.csv";
		QString CH09 = dir + "/通道9.csv";
		QString CH10 = dir + "/通道10.csv";
		QString CH11 = dir + "/通道11.csv";
		QString CH12 = dir + "/通道12.csv";
		QString CH13 = dir + "/通道13.csv";
		QString CH14 = dir + "/通道14.csv";
		QString CH15 = dir + "/通道15.csv";
		QString CH16 = dir + "/通道16.csv";
		QString CH17 = dir + "/通道17.csv";
		QString CH18 = dir + "/通道18.csv";
		QString CH19 = dir + "/通道19.csv";
		QString CH20 = dir + "/通道20.csv";
		QString CH21 = dir + "/通道21.csv";
		QString CH22 = dir + "/通道22.csv";
		QString CH23 = dir + "/通道23.csv";
		QString CH24 = dir + "/通道24.csv";
		QString CH25 = dir + "/通道25.csv";
		QString CH26 = dir + "/通道26.csv";
		QString CH27 = dir + "/通道27.csv";
		QString CH28 = dir + "/通道28.csv";
		QString CH29 = dir + "/通道29.csv";
		QString CH30 = dir + "/通道30.csv";
		QString CH31 = dir + "/通道31.csv";
		QString CH32 = dir + "/通道32.csv";

		QString  DataName_FBGT= "CH08-01,CH08-02,CH08-03,CH08-04,CH08-05,CH08-06,CH08-07,CH08-08,CH08-09,CH08-10,CH08-11,CH08-12,CH08-13,CH08-14,CH08-15,CH09-01,CH09-02,CH09-03,CH09-04,CH09-05,CH09-06,CH09-07,CH09-08,CH10-01,CH10-02,CH10-03,CH10-04,CH10-05,CH10-06,CH10-07,CH11-01,CH11-02,CH11-03,CH11-04,CH11-05,CH11-06,CH11-07,CH11-08,CH11-09,CH11-10,CH11-11,CH11-12,CH11-13,CH11-14,CH11-15,CH12-01,CH12-02,CH12-03,CH12-04,CH12-05,CH12-06,CH12-07,CH12-08,CH12-09,CH12-10,CH12-11,CH12-12,CH12-13,CH12-14,CH12-15,CH13-01,CH13-02,CH13-03,CH13-04,CH13-05,CH13-06,CH13-07,CH13-08,CH13-09,CH13-10,CH13-11,CH13-12,CH13-13,CH13-14,CH13-15,CH14-01,CH14-02,CH14-03,CH14-04,CH14-05,CH14-06,CH14-07,CH15-01,CH15-02,CH15-03,CH15-04,CH15-05,CH15-06,CH15-07,CH15-08,CH16-01,CH16-02,CH16-03,CH16-04,CH16-05,CH16-06,CH16-07,CH16-08,CH17-01,CH17-02,CH17-03,CH17-04,CH17-05,CH17-06,CH17-07,CH17-08,CH18-01,CH18-02,CH18-03,CH18-04,CH18-05,CH18-06,CH18-07,CH18-08,CH19-01,CH19-02,CH19-03,CH19-04,CH19-05,CH19-06,CH19-07,CH19-08,CH19-09,CH19-10,CH20-01,CH20-02,CH20-03,CH20-04,CH20-05,CH20-06,CH20-07,CH20-08,CH20-09,CH20-10,CH21-01,CH21-02,CH21-03,CH21-04,CH21-05,CH21-06,CH21-07,CH22-01,CH22-02,CH22-03,CH22-04,CH22-05,CH22-06,CH22-07,CH22-08,CH22-09,CH22-10,CH23-01,CH23-02,CH23-03,CH23-04,CH24-01,CH24-02,CH24-03,CH24-04,CH24-05,CH24-06,CH24-07,CH24-08,CH24-09,CH24-10,CH25-01,CH25-02,CH25-03,CH25-04,CH25-05,CH25-06,CH25-07,CH25-08,CH25-09,CH25-10,CH26-01,CH26-02,CH26-03,CH26-04,CH27-01,CH27-02,CH27-03,CH27-04,CH27-05,CH27-06,CH27-07,CH27-08,CH27-09,CH28-01,CH28-02,CH28-03,CH28-04,CH28-05,CH28-06,CH28-07,CH28-08,CH28-09,CH28-10,CH29-01,CH29-02,CH29-03,CH29-04,CH29-05,CH29-06,CH29-07,CH29-08,CH29-09,CH29-10,CH30-01,CH30-02,CH30-03,CH30-04,CH30-05,CH30-06,CH30-07,CH30-08,CH30-09,CH30-10,CH31-01,CH31-02,CH31-03,CH31-04,CH31-05,CH31-06,CH31-07,CH31-08,CH31-09,CH31-10,CH32-01,CH32-02,CH32-03";
		QStringList Time;
		int a = Stand_FBGT_Fix( CH08,  CH09,  CH10,  CH11,  CH12,  CH13,  CH14,  CH15,  CH16, \
							CH17,  CH18,  CH19,  CH20,  CH21,  CH22,  CH23,  CH24,  CH25, \
							CH26,  CH27,  CH28,  CH29,  CH30,  CH31,  CH32, MatFBGT, Time);
		if( a == 0 )
			ui->label_10->setText("拟载入成功！");
		// 保存文件
		int b = saveStandData("Data-FBGTemperature",DataName_FBGT,Time, MatFBGT);
		if( b == 0 )
		{
			QString timecost = QString::number(time.elapsed()/1000.0);
			ui->label_10->setText("FBG波长修复成功！<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
		}
	}
}


// 载入标准化FBG文件
void MainWindow::on_pushButton_LoadFBGT_clicked()
{
	standFBGFileName = QFileDialog::getOpenFileName(this, tr("打开标准FBG波长"), " ", tr("textfile(*.csv*);;Allfile(*.*)"));
	qDebug()<<standFBGFileName;
	if(!standFBGFileName.isEmpty())
	{
		ui->label_9->setText("载入成功！");
		//ui->pushButton_wave2temp->setEnabled(true);
		ui->label_9->setText("正在转换...");
		//double WaveAndTemp[231][2] = {{8.4,1535.91}, {8.5,1537.892}, {8.5,1539.88}, {8.5,1541.928}, {8.6,1543.865}, {8.7,1545.865}, {8.7,1547.871}, {8.9,1549.925}, {8.9,1552.03}, {9,1554.058}, {9.1,1555.991}, {9.1,1557.835}, {9.2,1559.858}, {9.3,1561.989}, {9.3,1563.91}, {8.4,1549.888}, {8.4,1551.923}, {8.5,1554.003}, {8.6,1556.046}, {8.6,1558.072}, {8.6,1559.968}, {8.6,1561.973}, {8.7,1563.921}, {8.5,1551.908}, {8.6,1553.899}, {8.6,1555.256}, {8.6,1557.897}, {8.6,1559.862}, {8.7,1561.929}, {8.7,1563.863}, {8.4,1535.919}, {8.4,1537.952}, {8.4,1539.937}, {8.5,1541.986}, {8.7,1543.934}, {8.8,1545.927}, {8.6,1547.917}, {9.4,1549.915}, {9.4,1551.867}, {9.5,1553.893}, {9.6,1555.976}, {9.7,1557.97}, {9.6,1559.93}, {9.7,1561.868}, {9.6,1563.824}, {8.5,1535.883}, {8.4,1537.887}, {8.4,1539.876}, {8.4,1541.944}, {8.6,1543.916}, {8.6,1545.887}, {8.6,1547.862}, {9.3,1549.875}, {9.4,1551.882}, {9.4,1553.864}, {9.8,1555.933}, {9.6,1557.929}, {9.6,1559.979}, {9.6,1562.067}, {9.7,1564.0866}, {8.4,1535.887}, {8.3,1537.895}, {8.4,1539.878}, {8.5,1541.909}, {8.5,1543.891}, {8.4,1545.846}, {8.5,1547.85}, {9.2,1549.849}, {9.3,1551.913}, {9.3,1553.89}, {9.4,1555.283}, {9.4,1557.986}, {9.5,1559.922}, {9.5,1561.974}, {9.5,1563.944}, {9.4,1535.917}, {9.4,1537.936}, {9.2,1539.924}, {9.2,1541.964}, {9.2,1543.869}, {9.1,1545.864}, {9.1,1547.86}, {9.3,1535.953}, {9.3,1538.018}, {9.2,1539.972}, {9.3,1542.003}, {9.3,1544.001}, {9.3,1545.952}, {9.2,1547.968}, {9.1,1550.004}, {8.6,1556.886}, {8.7,1559.866}, {8.7,1552.842}, {8.8,1549.983}, {8.9,1546.884}, {8.9,1537.916}, {9,1543.933}, {9.1,1540.989}, {8.6,1546.874}, {8.7,1559.845}, {8.7,1556.838}, {8.7,1552.852}, {8.7,1549.865}, {8.7,1540.868}, {8.8,1543.919}, {8.8,1537.891}, {8.6,1546.874}, {8.7,1559.845}, {8.7,1556.838}, {8.7,1552.852}, {8.7,1549.865}, {8.7,1540.868}, {8.8,1543.919}, {8.8,1537.891}, {8.6,1536.88}, {8.8,1539.851}, {8.8,1542.783}, {8.7,1545.85}, {8.7,1548.81}, {8.6,1551.925}, {8.9,1554.8}, {8.8,1557.806}, {8.7,1560.847}, {8.7,1563.7881}, {9.4,1537.927}, {9.1,1540.9}, {9.3,1543.824}, {9.4,1546.868}, {9.2,1549.795}, {9.4,1552.848}, {9.2,1555.959}, {9.2,1558.893}, {9.1,1561.833}, {9.2,1565.857}, {9.8,1533.28}, {9.7,1536.782}, {9.7,1539.724}, {9.4,1542.544}, {9.5,1545.746}, {9.3,1548.583}, {9.4,1551.862}, {8.8,1533.231}, {8.9,1536.3}, {8.8,1539.579}, {8.9,1542.234}, {9.2,1545.224}, {9.2,1548.25}, {9.1,1551.529}, {9.1,1554.316}, {9.5,1557.424}, {9.6,1560.507}, {9.2,1532.419}, {8.8,1535.423}, {8.8,1538.44}, {8.8,1541.37}, {9.5,1532.338}, {9.5,1535.593}, {9.4,1538.606}, {9.2,1541.579}, {9.4,1544.62}, {9.4,1547.436}, {9.5,1550.48}, {9.3,1553.68}, {9.6,1556.401}, {9.5,1559.5}, {9.7,1533.465}, {9.5,1536.576}, {9.4,1538.311}, {9.3,1542.081}, {9.5,1544.913}, {9.2,1548.159}, {9.3,1550.868}, {9.3,1554.26}, {9.4,1556.928}, {9.3,1560.047}, {9.5,1547.537}, {9.4,1553.547}, {9.5,1556.568}, {9.4,1559.39}, {8,1537.936}, {7.9,1540.857}, {8.1,1543.784}, {8.2,1546.859}, {8.3,1549.785}, {8.2,1552.862}, {8.1,1555.947}, {8.1,1558.827}, {8,1561.923}, {8.3,1636.868}, {8.5,1539.832}, {8.7,1542.826}, {8.7,1545.86}, {8.8,1548.829}, {8.8,1551.972}, {8.9,1554.835}, {8.9,1557.869}, {8.9,1560.815}, {8.9,1563.835}, {7.9,1537.944}, {8,1540.885}, {7.9,1543.825}, {8,1546.883}, {8,1549.931}, {8,1552.827}, {8,1555.965}, {8.1,1558.862}, {8,1561.873}, {8,1565.898}, {8.2,1536.901}, {8.3,1539.926}, {8.4,1542.797}, {8.6,1545.828}, {8.7,1548.942}, {8.7,1551.957}, {8.7,1554.805}, {8.5,1557.825}, {8.4,1560.848}, {8.4,1563.82}, {8,1537.88}, {8.1,1540.9}, {8.2,1543.795}, {8.2,1546.836}, {8.2,1549.805}, {8.3,1552.842}, {8.4,1555.919}, {8.5,1558.828}, {8.6,1561.864}, {8.5,1565.845}, {18.4,1555.118}, {18.1,1558.318}, {18.2,1561.226} };
		double WaveAndTemp[231][2]={{9.3,1535.91},{9.3,1537.892},{9.1,1539.88},{9.2,1541.928},{9.1,1543.865},{9,1545.865},{8.9,1547.871},{8.9,1549.925},{8.4,1552.03},{8.5,1554.058},{8.7,1555.991},{8.7,1557.835},{8.6,1559.858},{8.5,1561.989},{8.5,1563.91},{8.4,1549.888},{8.4,1551.923},{8.6,1554.003},{8.7,1556.046},{8.6,1558.072},{8.6,1559.968},{8.5,1561.973},{8.6,1563.921},{8.5,1551.908},{8.6,1553.899},{8.6,1555.256},{8.7,1557.897},{8.6,1559.862},{8.6,1561.929},{8.7,1563.863},{9.6,1535.919},{9.7,1537.952},{9.6,1539.937},{9.7,1541.986},{9.6,1543.934},{9.5,1545.927},{9.4,1547.917},{9.4,1549.915},{8.6,1551.867},{8.8,1553.893},{8.7,1555.976},{8.5,1557.97},{8.4,1559.93},{8.4,1561.868},{8.4,1563.824},{9.7,1535.883},{9.6,1537.887},{9.6,1539.876},{9.6,1541.944},{9.8,1543.916},{9.4,1545.887},{9.4,1547.862},{9.3,1549.875},{8.6,1551.882},{8.6,1553.864},{8.6,1555.933},{8.4,1557.929},{8.4,1559.979},{8.4,1562.067},{8.5,1564.086},{9.5,1535.887},{9.5,1537.895},{9.5,1539.878},{9.4,1541.909},{9.4,1543.891},{9.3,1545.846},{9.3,1547.85},{9.2,1549.849},{8.5,1551.913},{8.4,1553.89},{8.5,1555.283},{8.5,1557.986},{8.4,1559.922},{8.3,1561.974},{8.4,1563.944},{9.4,1535.917},{9.4,1537.936},{9.2,1539.924},{9.2,1541.964},{9.2,1543.869},{9.1,1545.864},{9.1,1547.86},{9.3,1535.953},{9.3,1538.018},{9.2,1539.972},{9.3,1542.003},{9.3,1544.001},{9.3,1545.952},{9.2,1547.968},{9.1,1550.004},{8.9,1537.916},{9.1,1540.989},{9,1543.933},{8.9,1546.884},{8.8,1549.983},{8.7,1552.842},{8.6,1556.886},{8.7,1559.866},{8.8,1537.891},{8.7,1540.868},{8.8,1543.919},{8.6,1546.874},{8.7,1549.865},{8.7,1552.852},{8.7,1556.838},{8.7,1559.845},{8.1,1537.88},{8.1,1540.867},{8.4,1543.895},{8.5,1546.834},{8.6,1550.129},{8.6,1552.898},{8.7,1556.887},{8.6,1559.87},{8.6,1536.88},{8.8,1539.851},{8.8,1542.783},{8.7,1545.85},{8.7,1548.81},{8.6,1551.925},{8.9,1554.8},{8.8,1557.806},{8.7,1560.847},{8.7,1563.788},{9.4,1537.927},{9.1,1540.9},{9.3,1543.824},{9.4,1546.868},{9.2,1549.795},{9.4,1552.848},{9.2,1555.959},{9.2,1558.893},{9.1,1561.833},{9.2,1565.857},{9.8,1533.28},{9.7,1536.782},{9.7,1539.724},{9.4,1542.544},{9.5,1545.746},{9.3,1548.583},{9.4,1551.862},{8.8,1533.231},{8.9,1536.3},{8.8,1539.579},{8.9,1542.234},{9.2,1545.224},{9.2,1548.25},{9.1,1551.529},{9.1,1554.316},{9.5,1557.424},{9.6,1560.507},{9.2,1532.419},{8.8,1535.423},{8.8,1538.44},{8.8,1541.37},{9.5,1532.338},{9.5,1535.593},{9.4,1538.606},{9.2,1541.579},{9.4,1544.62},{9.4,1547.436},{9.5,1550.48},{9.3,1553.68},{9.6,1556.401},{9.5,1559.5},{9.7,1533.465},{9.5,1536.576},{9.4,1538.311},{9.3,1542.081},{9.5,1544.913},{9.2,1548.159},{9.3,1550.868},{9.3,1554.26},{9.4,1556.928},{9.3,1560.047},{9.5,1547.537},{9.4,1553.547},{9.5,1556.568},{9.4,1559.39},{8,1537.936},{7.9,1540.857},{8.1,1543.784},{8.2,1546.859},{8.3,1549.785},{8.2,1552.862},{8.1,1555.947},{8.1,1558.827},{8,1561.923},{8.3,1536.868},{8.5,1539.832},{8.7,1542.826},{8.7,1545.86},{8.8,1548.829},{8.8,1551.972},{8.9,1554.835},{8.9,1557.869},{8.9,1560.815},{8.9,1563.835},{7.9,1537.944},{8,1540.885},{7.9,1543.825},{8,1546.883},{8,1549.931},{8,1552.827},{8,1555.965},{8.1,1558.862},{8,1561.873},{8,1565.898},{8.2,1536.901},{8.3,1539.926},{8.4,1542.797},{8.6,1545.828},{8.7,1548.942},{8.7,1551.957},{8.7,1554.805},{8.5,1557.825},{8.4,1560.848},{8.4,1563.82},{8,1537.88},{8.1,1540.9},{8.2,1543.795},{8.2,1546.836},{8.2,1549.805},{8.3,1552.842},{8.4,1555.919},{8.5,1558.828},{8.6,1561.864},{8.5,1565.845},{8.876,1555.018}, {8.576,1558.218}, {8.676,1561.126}};
		if(!standFBGFileName.isEmpty())
		{
			QTime time;time.start(); // 计时
			qDebug()<<standFBGFileName;
			int InputFileRow;
			int InputFileCol;
			QStringList DataName;
			QStringList XLabelName;
			QString  DataName_FBGT= "CH08-01,CH08-02,CH08-03,CH08-04,CH08-05,CH08-06,CH08-07,CH08-08,CH08-09,CH08-10,CH08-11,CH08-12,CH08-13,CH08-14,CH08-15,CH09-01,CH09-02,CH09-03,CH09-04,CH09-05,CH09-06,CH09-07,CH09-08,CH10-01,CH10-02,CH10-03,CH10-04,CH10-05,CH10-06,CH10-07,CH11-01,CH11-02,CH11-03,CH11-04,CH11-05,CH11-06,CH11-07,CH11-08,CH11-09,CH11-10,CH11-11,CH11-12,CH11-13,CH11-14,CH11-15,CH12-01,CH12-02,CH12-03,CH12-04,CH12-05,CH12-06,CH12-07,CH12-08,CH12-09,CH12-10,CH12-11,CH12-12,CH12-13,CH12-14,CH12-15,CH13-01,CH13-02,CH13-03,CH13-04,CH13-05,CH13-06,CH13-07,CH13-08,CH13-09,CH13-10,CH13-11,CH13-12,CH13-13,CH13-14,CH13-15,CH14-01,CH14-02,CH14-03,CH14-04,CH14-05,CH14-06,CH14-07,CH15-01,CH15-02,CH15-03,CH15-04,CH15-05,CH15-06,CH15-07,CH15-08,CH16-01,CH16-02,CH16-03,CH16-04,CH16-05,CH16-06,CH16-07,CH16-08,CH17-01,CH17-02,CH17-03,CH17-04,CH17-05,CH17-06,CH17-07,CH17-08,CH18-01,CH18-02,CH18-03,CH18-04,CH18-05,CH18-06,CH18-07,CH18-08,CH19-01,CH19-02,CH19-03,CH19-04,CH19-05,CH19-06,CH19-07,CH19-08,CH19-09,CH19-10,CH20-01,CH20-02,CH20-03,CH20-04,CH20-05,CH20-06,CH20-07,CH20-08,CH20-09,CH20-10,CH21-01,CH21-02,CH21-03,CH21-04,CH21-05,CH21-06,CH21-07,CH22-01,CH22-02,CH22-03,CH22-04,CH22-05,CH22-06,CH22-07,CH22-08,CH22-09,CH22-10,CH23-01,CH23-02,CH23-03,CH23-04,CH24-01,CH24-02,CH24-03,CH24-04,CH24-05,CH24-06,CH24-07,CH24-08,CH24-09,CH24-10,CH25-01,CH25-02,CH25-03,CH25-04,CH25-05,CH25-06,CH25-07,CH25-08,CH25-09,CH25-10,CH26-01,CH26-02,CH26-03,CH26-04,CH27-01,CH27-02,CH27-03,CH27-04,CH27-05,CH27-06,CH27-07,CH27-08,CH27-09,CH28-01,CH28-02,CH28-03,CH28-04,CH28-05,CH28-06,CH28-07,CH28-08,CH28-09,CH28-10,CH29-01,CH29-02,CH29-03,CH29-04,CH29-05,CH29-06,CH29-07,CH29-08,CH29-09,CH29-10,CH30-01,CH30-02,CH30-03,CH30-04,CH30-05,CH30-06,CH30-07,CH30-08,CH30-09,CH30-10,CH31-01,CH31-02,CH31-03,CH31-04,CH31-05,CH31-06,CH31-07,CH31-08,CH31-09,CH31-10,CH32-01,CH32-02,CH32-03";
			mat FBGWave = JfzReadCSVToAllMat(standFBGFileName, InputFileRow, InputFileCol, DataName,  XLabelName);
			qDebug()<<FBGWave(0,0);
			// 波长转温度
			for(unsigned int i=0; i< FBGWave.n_rows; i++)
			{
				for(unsigned int j=0; j<FBGWave.n_cols; j++)
				{
					FBGWave(i,j) = (FBGWave(i,j)-WaveAndTemp[j][1])/0.0105+WaveAndTemp[j][0];
				}
			}
			qDebug()<<FBGWave(0,0);
			QString Date = standFBGFileName.right(33).left(10);
			qDebug()<<"Date"<<Date;
			int b = saveStandDataNoTimeFix("Data-FBGtoTEMP",DataName_FBGT,Date,XLabelName, FBGWave);
			if( b == 0 )
			{
				QString timecost = QString::number(time.elapsed()/1000.0);
				ui->label_9->setText("波长转温度成功!:<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
			}

		}
	}
	else
	{
		//ui->pushButton_wave2temp->setEnabled(false);
	}
}

// 波长转温度
//void MainWindow::on_pushButton_wave2temp_clicked()
//{

//}

// 相关性分析-文件1
void MainWindow::on_pushButton_File1_clicked()
{
	correlationFileName1 = QFileDialog::getOpenFileName(this, tr("打开文件1"), " ", tr("textfile(*.csv*);;Allfile(*.*)"));
	qDebug()<<correlationFileName1;
	if(!correlationFileName1.isEmpty() && !correlationFileName2.isEmpty())
	{
		ui->label_11->setText("选了文件1");
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
		ui->label_11->setText("选了文件2");
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
	ui->label_11->setText("计算开始！");
	QTime time;time.start(); // 计时

	// 读取两文件矩阵
	int InputFileRow1;
	int InputFileCol1;
	int InputFileRow2;
	int InputFileCol2;
	QStringList DataName1;
	QStringList DataName2;
	QStringList Time;
	mat File1 = JfzReadCSVToAllMat(correlationFileName1, InputFileRow1, InputFileCol1, DataName1, Time);
	mat File2 = JfzReadCSVToAllMat(correlationFileName2, InputFileRow2, InputFileCol2, DataName2, Time);
	//    qDebug()<<"File1(0,0)"<<File1(0,0);
	//    qDebug()<<"File2(0,0)"<<File2(0,0);

	// 矩阵合并
	mat All_Mat = join_rows(File1, File2);

	// 相关系数
	// 求矩阵A与B的相关系数矩阵。矩阵A(B)的每一列为一个变量，每一行为一组观测值。矩阵C(i,j)表示矩阵A的第i个变量与矩阵B的第j个变量的相关系数。
	mat corMat = cor(All_Mat);
	// 真正需要的相关系数
	mat corMatneed(File1.n_cols, File2.n_cols);
	for(unsigned int i=0; i<corMatneed.n_rows; i++)
	{
		for(unsigned int j=0; j<corMatneed.n_cols; j++)
		{
			corMatneed(i,j) = corMat(i, j+File1.n_cols);
		}
	}

	// 保存文件
	// 构造DataName2str 数据编号 列为数据1编号  行为数据2编号
	QString DataName2str;
	for(int i=0; i<DataName2.size(); i++)
	{
		if(i == DataName2.size()-1)
			DataName2str = DataName2str + DataName2[i];
		else
			DataName2str = DataName2str + DataName2[i] + ",";
	}
	// 提取Date、vs内容字符串
	QStringList Name0List = correlationFileName1.split("/");
	QStringList Name1List = correlationFileName1.split("-");
	QStringList Name2List = correlationFileName2.split("-");
	QStringList Name12List = Name1List[3].split(".");
	QStringList Name22List = Name2List[3].split(".");
	QString Title = "Correlation-" + Name12List[0] + "vs" + Name22List[0];
	QString Date = Name0List[Name0List.size()-1].left(10);
	//    qDebug()<< "Date "<<Date;
	//    qDebug()<< "correlationFileName1 "<<correlationFileName1;
	//    qDebug()<< "Title "<<Title;
	int b = saveStandDataNoTimeFix(Title, DataName2str, Date, DataName1, corMatneed);
	if( b == 0 )
	{
		QString timecost = QString::number(time.elapsed()/1000.0);
		ui->label_11->setText("OK:<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
	}
}

// 载入统计分析源数据
void MainWindow::on_pushButton_clicked()
{
	ui->textEdit->clear();
	// 载入文件
	QString InputFileName_str = QFileDialog::getOpenFileName(this,"open file"," ","textfile(*.csv);;All file(*.*)");
	qDebug()<<InputFileName_str;  // 文件名

	if(!InputFileName_str.isEmpty())
	{
		// 需要的数据的定义
		int InputFileRow = 0 ; // 文件行
		int InputFileCol = 0 ; // 文件列
		QString Title; // 文件内容名字（表名字）
		QStringList DataName; // 数据各项名
		QStringList XLabelName; // X标签名

		// 读取文件内容
		QFile f(InputFileName_str);
		if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){}
		QTextStream txtInput(&f);
		// 读第一行,得到[标题],[数据各项名]
		QString lineStr = txtInput.readLine();
		QStringList tempbar = lineStr.split(",");
		Title = tempbar[0];
		// 读切片个数即为[列数]
		InputFileCol = tempbar.size();
		for(int i = 1; i<tempbar.size(); i++)
		{
			DataName.append(tempbar[i]);// [数据各项名]
		}

		InputFileRow = JfzFileReadRow(InputFileName_str); // [行数]
		mat Mat(InputFileRow-1,InputFileCol-1); // 存数据的矩阵，少一行一列

		int Row = 0; // 行编号
		while(!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			tempbar = lineStr.split(",");
			XLabelName.append(tempbar[0]);// 添加[X标签名]
			for(int i = 1; i<tempbar.size(); i++)
			{
				Mat(Row,i-1) = tempbar[i].toDouble();
			}
			Row++;
		}
		f.close();

		// Debug信息
		qDebug()<< "Armadillo 版本: "<< QString::fromStdString( arma_version::as_string() ) ;
		//    qDebug()<< "行" << InputFileRow << "列" << InputFileCol;
		//    qDebug()<< "标题" << Title;
		//    qDebug()<< "数据各项名" << DataName;
		//    qDebug()<< "X标签名" << XLabelName;

		// 保存输入Mat,预留
		//    Mat.save("Mat.txt", raw_ascii);
		//    qDebug()<< "Mat行" << Mat.n_rows << "Mat列" << Mat.n_cols;
		//    qDebug()<< "矩阵的最大值max(max(Mat)) = " << max(max(Mat));
		//    qDebug()<< "矩阵的最小值min(min(Mat)) = " << min(min(Mat));
		//    qDebug()<< "所有元素的和accu(Mat): " << accu(Mat);
		//    qDebug()<< "平均值: " << (accu(Mat))/(Mat.n_rows * Mat.n_cols);

		// 与矩阵计算相关的定义
		int MatRow = Mat.n_rows;
		int MatCol = Mat.n_cols;
		double MatMax = max(max(Mat));
		double MatMin = min(min(Mat));
		double MatSum = accu(Mat);
		double MatArithmeticMean = (accu(Mat))/(Mat.n_rows * Mat.n_cols);

		// 数据概览-处理
		ui->label_2->setText("数据行列：" + QString::number(MatRow) + " x "+ QString::number(MatCol) );
		ui->label_4->setText("数据名字：" + Title );
		ui->label_5->setText("数据总和：" + QString::number(MatSum) );
		ui->label_6->setText("数据平均：" + QString::number(MatArithmeticMean) );
		ui->label_7->setText("数据最大：" + QString::number(MatMax) );
		ui->label_8->setText("数据最小：" + QString::number(MatMin) );

		// 数据基本信息-处理
		mat colMat = sum(Mat); // 每一列的总和
		mat maxMat = max(Mat); // 每一列的最大值
		mat minMat = min(Mat); // 每一列的最大值

		for(int i=0; i< MatCol ; i++)
		{
			QString text;
			//        = DataName[i] + "   和:" + QString::number(colMat(i)) +
			//                "   最大值:" + QString::number(maxMat(i)) +
			//                "   最小值:" + QString::number(minMat(i)) +
			//                "   算术平均值:" + QString::number( colMat(i)*1.0/(MatRow) );
			float avg = colMat(i)*1.0/(MatRow); // 算术平均值
			string strDataName(DataName[i].toStdString());
			text.sprintf("%s  和:%-15.4f  最大值:%-15.4f  最小值:%-15.4f  算术平均值:%-15.4f", strDataName.c_str(), colMat(i), maxMat(i), minMat(i), avg);

			qDebug()<<avg;
			if(InputFileName_str.right(6).left(2) == "20" || InputFileName_str.right(6).left(2) == "MP" ) // 电类和转好的FBG温度判断
			{
				if(maxMat(i)-minMat(i) < 15 &&  avg > 0 && avg < 50)
					ui->textEdit->append(text);
				else
				{
					if( fabs(avg)<0.00001 )
					{
						QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
						ui->textEdit->append(text2 + "<span style='color:blue'>数据没有</span>");// 标蓝
					}
					else
					{
						QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
						ui->textEdit->append(text2 + "<span style='color:red'>数据异常</span>");// 标红
					}
				}
			}
			if(InputFileName_str.right(6).left(2) == "ss" || InputFileName_str.right(6).left(2) == "re" ) // FBG波长数据判断
			{
				if(maxMat(i)-minMat(i) < 3 && avg!=0 )
					ui->textEdit->append(text);
				else
				{
					if( fabs(avg)<0.00001 )
					{
						QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
						ui->textEdit->append(text2 + "<span style='color:blue'>数据没有</span>");// 标蓝
					}
					else
					{
						QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
						ui->textEdit->append(text2 + "<span style='color:red'>数据异常</span>");// 标红
					}
				}
			}
			if(InputFileName_str.right(6).left(2) == "CD") // CCD数据判断
			{
				//qDebug()<<"fabs(avg)"<<fabs(avg);
				if( fabs(avg)<0.2 )
					ui->textEdit->append(text);
				else
				{
					QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
					ui->textEdit->append(text2 + "<span style='color:red'>偏移过大</span>");// 标红
				}
			}

		}
		// 数据基本信息-求相关系数
		// 求矩阵A与B的相关系数矩阵。矩阵A(B)的每一列为一个变量，每一行为一组观测值。矩阵C(i,j)表示矩阵A的第i个变量与矩阵B的第j个变量的相关系数。
//        mat corMat = cor(Mat);
//        //        qDebug()<< corMat.n_rows<<" "<< corMat.n_cols;
//        QString corStr = "";
//        ui->textEdit->append("\n每列数据间相关系数：");
//        for(unsigned int i=0; i< corMat.n_rows ; i++)
//        {
//            for(unsigned int j=0; j< corMat.n_cols ; j++)
//            {
//                QString standStr;
//                standStr.sprintf("%8.4f", corMat(i,j));
//                corStr += standStr + " ";
//            }
//            ui->textEdit->append(corStr);
//            corStr = "";
//        }
//        qDebug()<< "poi";
	}
}

// 清空全部
void MainWindow::on_pushButton_2_clicked()
{
	ui->textEdit->clear();
	ui->label_2->setText("数据行列：");
	ui->label_4->setText("数据名字：");
	ui->label_5->setText("数据总和：");
	ui->label_6->setText("数据平均：");
	ui->label_7->setText("数据最大：");
	ui->label_8->setText("数据最小：");
}

