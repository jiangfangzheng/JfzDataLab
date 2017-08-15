// Copyright:	Jfz
// Author:		Sandeepin
// Date:		2017-07-23
// Description:	蒋方正文件读取类

#include "JIO.h"
#include <qDebug>
#include <QFile>

JIO::JIO()
{
}

JIO::~JIO()
{
}

// 读取普通文本到字符串列表
QStringList JIO::readFile(QString fileName)
{
	// 输出变量
	QStringList out;
	// 打开文件
	QFile f(fileName);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug("[JIO::readFile(QString fileName)] 打开失败！");
		return out;
	}
	QTextStream txtInput(&f);
	// 每行写入QStringList
	while (!txtInput.atEnd())
	{
		QString lineStr = txtInput.readLine();
		out.append(lineStr);
	}
	f.close();
	return out;
}

// 读取csv到2维字符串列表
QList<QList<QString>> JIO::readCsv(QString fileName, QString fuhao)
{
	// 输出变量
	QList<QList<QString>> out;
	// 先读取每行
	QStringList outRow = readFile(fileName);
	// 切片放入二维List
	for (auto& e : outRow)
	{
		out.append(e.split(fuhao));
	}
	return out;
}

// 读取csv分别保存为项目名列表、时间列表、mat
mat JIO::readCsv(QString fileName, QStringList& itemName, QStringList& timeName)
{
	// 先读数据
	QList<QList<QString>> outList = readCsv(fileName);
	// itemName赋值
	for (int i = 0; i < outList[0].size(); ++i)
	{
		itemName.append(outList[0][i]);
	}
	// timeName赋值
	for (int i = 1; i < outList.size(); ++i)
	{
		timeName.append(outList[i][0]);
	}
	// mat赋值
	unsigned int row = outList.size() - 1;
	unsigned int col = outList[0].size() - 1;
	mat out(row, col);
	for (unsigned int i = 0; i < row; ++i)
	{
		for (unsigned int j = 0; j < col; ++j)
		{
			out(i, j) = outList[i + 1][j + 1].toDouble();
		}
	}
	return out;
}

// 读取mat到2维double列表
QList<QList<double>> JIO::readMat(QString fileName)
{
	// 输出变量
	QList<QList<double>> out;
	// 先读取每行
	QStringList outRow = readFile(fileName);
	// 转成double切片放入二维List
	for (auto& e : outRow)
	{
		QList<QString> strList = e.split(",");
		QList<double> dList;
		for (auto& d : strList)
		{
			dList.append(d.toDouble());
		}
		out.append(dList);
	}
	return out;
}

// 读取mat到2维Armadillo mat
mat JIO::readAMat(QString fileName)
{
	// 读取数据到2维double列表
	QList<QList<double>> outList = readMat(fileName);
	// outList赋值到mat
	unsigned int row = outList.size();
	unsigned int col = 0;
	if (row > 0)
		col = outList[0].size();
	if (row == 0 || col == 0)
	{
		qDebug("[JIO::readMat(QString fileName)] 数据为空！");
		mat out(1, 1);
		out.fill(0);
		return out;
	}
	mat out(row, col);
	for (unsigned int i = 0; i < row; ++i)
		for (unsigned int j = 0; j < col; ++j)
			out(i, j) = outList[i][j];
	return out;
}

bool JIO::save(QString fileName, QString text)
{
	QFile f(fileName);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "[JIO::save] Open failed.";
		return false;
	}
	QTextStream txtOutput(&f);
	txtOutput << text;
	f.close();
	return true;
}

bool JIO::save(QString fileName, QStringList textList)
{
	QFile f(fileName);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "[JIO::save(QString fileName, QStringList textList)] Open failed.";
		return false;
	}
	QTextStream txtOutput(&f);
	for (auto e : textList)
	{
		txtOutput << e << "\n";
	}
	f.close();
	return true;
}

bool JIO::save(QString fileName, mat matData)
{
	// 写文件
	QFile f(fileName);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "[JIO::save(QString fileName, QStringList textList)] Open failed.";
		return false;
	}
	QTextStream txtOutput(&f);
	for (unsigned int i = 0; i < matData.n_rows; ++i)
	{
		QString tmp;
		for (unsigned int j = 0; j < matData.n_cols; ++j)
		{
			QString value;
			if (j + 1 != matData.n_cols)
				value.sprintf("%.3lf,", matData(i, j));
			else
				value.sprintf("%.3lf", matData(i, j));
			tmp += value;
		}
		txtOutput << tmp << "\n";
	}
	f.close();
	return true;
}

bool JIO::save(QString fileName, QList<QList<double>> matData)
{
	// 写文件
	QFile f(fileName);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "[JIO::save(QString fileName, QStringList textList)] Open failed.";
		return false;
	}
	QTextStream txtOutput(&f);
	for (int i = 0; i < matData.size(); ++i)
	{
		QString tmp;
		for (int j = 0; j < matData[0].size(); ++j)
		{
			QString value;
			if (j + 1 != matData[0].size())
				value.sprintf("%.3lf,", matData[i][j]);
			else
				value.sprintf("%.3lf", matData[i][j]);
			tmp += value;
		}
		txtOutput << tmp << "\n";
	}
	f.close();
	return true;
}

bool JIO::save(QString fileName, QString itemName, QStringList timeName, mat matData)
{
	// TODO
	// 小数点3位定死不好
	// 写文件
	QFile f(fileName);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "[JIO::save(QString fileName, QStringList textList)] Open failed.";
		return false;
	}
	QTextStream txtOutput(&f);
	// 写数据抬头
	txtOutput << itemName << "\n";
	// 写时间和mat
	for (unsigned int i = 0; i < matData.n_rows; ++i)
	{
		// 写时间
		txtOutput << timeName.at(i) << ",";
		// 写mat
		QString tmp;
		for (unsigned int j = 0; j < matData.n_cols; ++j)
		{
			QString value;
			if (j + 1 != matData.n_cols)
				value.sprintf("%.3lf,", matData(i, j));
			else
				value.sprintf("%.3lf", matData(i, j));
			tmp += value;
		}
		txtOutput << tmp << "\n";
	}
	f.close();
	return true;
}

bool JIO::save(QString fileName, QStringList itemName, QStringList timeName, mat matData)
{
	// TODO
	// 小数点3位定死不好
	// 写文件
	QFile f(fileName);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "[JIO::save(QString fileName, QStringList textList)] Open failed.";
		return false;
	}
	QTextStream txtOutput(&f);
	// 写数据抬头
	QString itemNameStr = itemName.join(",");
	txtOutput << itemNameStr << "\n";
	// 写时间和mat
	for (unsigned int i = 0; i < matData.n_rows; ++i)
	{
		// 写时间
		txtOutput << timeName.at(i) << ",";
		// 写mat
		QString tmp;
		for (unsigned int j = 0; j < matData.n_cols; ++j)
		{
			QString value;
			if (j + 1 != matData.n_cols)
				value.sprintf("%.3lf,", matData(i, j));
			else
				value.sprintf("%.3lf", matData(i, j));
			tmp += value;
		}
		txtOutput << tmp << "\n";
	}
	f.close();
	return true;
}

void JIO::show(QStringList input)
{
	for (auto& e : input)
	qDebug() << e;
	qDebug() << "";
}

void JIO::show(QList<QList<QString>> input)
{
	for (auto& e : input)
	{
		QString outLine;
		for (auto& f : e)
		{
			outLine += f + " ";
		}
		qDebug() << outLine;
	}
	qDebug() << "";
}

void JIO::show(QList<QList<double>> input)
{
	for (auto& e : input)
	{
		QString outLine;
		for (auto& f : e)
		{
			outLine += QString::number(f) + " ";
		}
		qDebug() << outLine;
	}
	qDebug() << "";
}

void JIO::show(QList<double> input)
{
	QString outLine;
	for (auto& f : input)
	{
		outLine += QString::number(f) + " ";
	}
	qDebug() << outLine;
	qDebug() << "";
}

void JIO::show(mat input)
{
	for (unsigned int i = 0; i < input.n_rows; ++i)
	{
		QString outLine;
		for (unsigned int j = 0; j < input.n_cols; ++j)
		{
			outLine += QString::number(input(i, j)) + " ";
		}
		qDebug() << outLine;
	}
	qDebug() << "";
}

void JIO::test()
{
	qDebug() << "JIO Class Test:";

	qDebug() << "readFile:";
	QStringList strlist1 = JIO::readFile("In01.csv");
	JIO::show(strlist1);

	qDebug() << "readCsv:";
	QList<QList<QString>> strlist2 = JIO::readCsv("In01.csv");
	JIO::show(strlist2);

	qDebug() << "readCsv2:";
	QString fileName = "In01.csv";
	QStringList itemName;
	QStringList timeName;
	mat mat1 = readCsv(fileName, itemName, timeName);
	JIO::show(itemName);
	JIO::show(timeName);
	JIO::show(mat1);

	qDebug() << "readMat:";
	QList<QList<double>> dlist3 = JIO::readMat("In03.mat");
	JIO::show(dlist3);
	qDebug() << "readMat[2][3]:\n" << dlist3[2][3] << "\n";

	qDebug() << "readAMat:";
	mat mat2 = JIO::readAMat("In03.mat");
	JIO::show(mat2);

	JIO::save("mat2.csv", mat2);
	JIO::save("dlist3.csv", dlist3);

	qDebug() << "end";
}
