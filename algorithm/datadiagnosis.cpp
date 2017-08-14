#include "datadiagnosis.h"
#include "tools/JIO.h"
#include <QFile>
#include <QTextStream>

//mat DataAnalysis(QString &fileName, QStringList &itemName, QStringList &timeName, QString &matSum, QString &matArithmeticMean, QString &matMax, QString &matMin)
//{
//	// 需要的数据的定义
//	int InputFileRow = 0 ; // 文件行
//	int InputFileCol = 0 ; // 文件列
//	QString Title; // 文件内容名字（表名字）
//	QStringList itemName;
//	QStringList XLabelName; // X标签名

//	// 读取文件内容

//	// Debug信息
//	qDebug()<< "Armadillo 版本: "<< QString::fromStdString( arma_version::as_string() ) ;

//	// 与矩阵计算相关的定义
//	double MatMax = ;
//	double MatMin = ;
//	double MatSum = ;
//	double MatArithmeticMean = ;

//	// 数据概览-输出
//	matSum=
//	matArithmeticMean= ;
//	matMax=;
//	matMin=;


//	return input;
//}

//bool DataDiagnosis(QString &InputFileName_str, mat &Mat, QStringList &DataName, Ui::MainWindow *ui)
//{
//	ui->textEdit->append("【文件名】" + getFileName(InputFileName_str));

//	// 数据基本信息-处理
//	mat colMat = sum(Mat); // 每一列的总和
//	mat maxMat = max(Mat); // 每一列的最大值
//	mat minMat = min(Mat); // 每一列的最大值

//	for(unsigned int i=0; i< Mat.n_cols ; i++)
//	{
//		QString text;
//		float avg = colMat(i)*1.0/(Mat.n_rows); // 算术平均值
//		string strDataName(DataName[i].toStdString());
//		text.sprintf("%s  和:%-15.4f  最大值:%-15.4f  最小值:%-15.4f  算术平均值:%-15.4f", strDataName.c_str(), colMat(i), maxMat(i), minMat(i), avg);

//		qDebug()<<avg;
//		if(InputFileName_str.right(6).left(2) == "20" || InputFileName_str.right(6).left(2) == "MP" ) // 电类和转好的FBG温度判断
//		{
//			if(maxMat(i)-minMat(i) < 15 &&  avg > 0 && avg < 50)
//				ui->textEdit->append(text);
//			else
//			{
//				if( fabs(avg)<0.00001 )
//				{
//					QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
//					ui->textEdit->append(text2 + "<span style='color:blue'>数据没有</span>");// 标蓝
//				}
//				else
//				{
//					QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
//					ui->textEdit->append(text2 + "<span style='color:red'>数据异常</span>");// 标红
//				}
//			}
//		}
//		else if(InputFileName_str.right(6).left(2) == "ss" || InputFileName_str.right(6).left(2) == "re" ) // FBG波长数据判断
//		{
//			if(maxMat(i)-minMat(i) < 3 && avg!=0 )
//				ui->textEdit->append(text);
//			else
//			{
//				if( fabs(avg)<0.00001 )
//				{
//					QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
//					ui->textEdit->append(text2 + "<span style='color:blue'>数据没有</span>");// 标蓝
//				}
//				else
//				{
//					QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
//					ui->textEdit->append(text2 + "<span style='color:red'>数据异常</span>");// 标红
//				}
//			}
//		}
//		else if(InputFileName_str.right(6).left(2) == "CD") // CCD数据判断
//		{
//			//qDebug()<<"fabs(avg)"<<fabs(avg);
//			if( fabs(avg)<0.2 )
//				ui->textEdit->append(text);
//			else
//			{
//				QString text2 = text.replace(QRegExp("\\ "), "&nbsp;");
//				ui->textEdit->append(text2 + "<span style='color:red'>偏移过大</span>");// 标红
//			}
//		}
//		else // 没有特殊说明，不诊断直接显示
//		{
//			ui->textEdit->append(text);
//		}

//	}
//	 数据基本信息-求相关系数
//	 求矩阵A与B的相关系数矩阵。矩阵A(B)的每一列为一个变量，每一行为一组观测值。矩阵C(i,j)表示矩阵A的第i个变量与矩阵B的第j个变量的相关系数。
//		mat corMat = cor(Mat);
//		//        qDebug()<< corMat.n_rows<<" "<< corMat.n_cols;
//		QString corStr = "";
//		ui->textEdit->append("\n每列数据间相关系数：");
//		for(unsigned int i=0; i< corMat.n_rows ; i++)
//		{
//			for(unsigned int j=0; j< corMat.n_cols ; j++)
//			{
//				QString standStr;
//				standStr.sprintf("%8.4f", corMat(i,j));
//				corStr += standStr + " ";
//			}
//			ui->textEdit->append(corStr);
//			corStr = "";
//		}
//		qDebug()<< "poi";
//	return true;
//}
