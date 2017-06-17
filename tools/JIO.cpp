#include "JIO.h"
#include <qDebug>
#include <QFile>

JIO::JIO()
{
}

JIO::~JIO()
{
}

QStringList JIO::FileToStrList(QString fileName)
{
	QStringList out;

	QFile f(fileName);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug("Open failed.");
		return out;
	}
	QTextStream txtInput(&f);
	
	while (!txtInput.atEnd())
	{
		QString lineStr = txtInput.readLine();
		out.append(lineStr);
	}
	f.close();

	return out;
}

QList<QList<QString>> JIO::CsvToStrList(QString fileName)
{
	QList<QList<QString>> out;

	QStringList outRow = FileToStrList(fileName);
	for (auto &e: outRow)
	{
		QList<QString> strList = e.split(",");
		out.append(strList);
	}

	return out;
}

QList<QList<double>> JIO::MatToDList(QString fileName)
{
	QList<QList<double>> out;

	QStringList outRow = FileToStrList(fileName);
	for (auto &e : outRow)
	{
		QList<QString> strList = e.split(",");
		QList<double> dList;
		for (auto &d : strList)
		{
			dList.append(d.toDouble());
		}
		out.append(dList);
	}

	return out;
}

bool JIO::save(QString fileName, QString text)
{
	QFile f(fileName);
	if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
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
	if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "[JIO::save(QString fileName, QStringList textList)] Open failed.";
		return false;
	}
	QTextStream txtOutput(&f);
	for(auto e: textList)
	{
		txtOutput << e << "\n";
	}
	f.close();
	return true;
}

void JIO::show(QStringList input)
{
	for (auto &e : input)
		qDebug() << e;
	qDebug() << "";
}

void JIO::show(QList<QList<QString>> input)
{
	for (auto &e:input)
	{
		QString outLine;
		for (auto &f : e)
		{
			outLine += f + " ";
		}
		qDebug() << outLine;
	}
	qDebug() << "";
}

void JIO::show(QList<QList<double>> input)
{
	for (auto &e : input)
	{
		QString outLine;
		for (auto &f : e)
		{
			outLine += QString::number(f) + " ";
		}
		qDebug() << outLine;
	}
	qDebug() << "";
}

void JIO::test()
{
	qDebug() << "JIO Class Test:";

	qDebug() << "FileToStrList:";
	QStringList strlist1 = JIO::FileToStrList("In01.csv");
	JIO::show(strlist1);

	qDebug() << "CsvToStrList:";
	QList<QList<QString>>  strlist2 = JIO::CsvToStrList("In01.csv");
	JIO::show(strlist2);

	qDebug() << "MatToDList:";
	QList<QList<double>>  dlist3 = JIO::MatToDList("In03.mat");
	JIO::show(dlist3);

	qDebug() << "MatToDList[2][3]:\n" << dlist3[2][3];
}
