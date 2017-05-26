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
	// 载入文件夹 ，填"/"跳到根目录， 填“” 默认程序位置
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
		ui->label_msg->setText("请选择环境温度4个文件！");
		QMessageBox::critical(NULL, "注意", "请选择环境温度的4个文件！", QMessageBox::Yes, QMessageBox::Yes);
		ui->pushButton_ENV->setEnabled(true);
	}
	else
	{
		ui->label_msg->setText("<span style='color: rgb(255, 0, 0);'>正在开启线程处理xls文件...</span>");
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
		ui->label_msg->setText("OK:<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
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

