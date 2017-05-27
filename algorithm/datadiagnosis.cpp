#include "datadiagnosis.h"

mat DataAnalysis(QString &InputFileName_str, QStringList &DataName, QString &strMatRow, QString &strMatCol, QString &strTitle, QString &strMatSum, QString &strMatArithmeticMean, QString &strMatMax, QString &strMatMin)
{
	// 需要的数据的定义
	int InputFileRow = 0 ; // 文件行
	int InputFileCol = 0 ; // 文件列
	QString Title; // 文件内容名字（表名字）
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

	// 数据概览-输出
	strMatRow=QString::number(MatRow);
	strMatCol=QString::number(MatCol);
	strTitle=Title;
	strMatSum=QString::number(MatSum);
	strMatArithmeticMean= QString::number(MatArithmeticMean);
	strMatMax=QString::number(MatMax);
	strMatMin=QString::number(MatMin);

	return Mat;
}

bool DataDiagnosis(QString &InputFileName_str, mat &Mat, QStringList &DataName, Ui::MainWindow *ui)
{
	// 数据基本信息-处理
	mat colMat = sum(Mat); // 每一列的总和
	mat maxMat = max(Mat); // 每一列的最大值
	mat minMat = min(Mat); // 每一列的最大值

	for(int i=0; i< Mat.n_cols ; i++)
	{
		QString text;
		float avg = colMat(i)*1.0/(Mat.n_rows); // 算术平均值
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
	return true;
}
