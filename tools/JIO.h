#pragma once
#include <QString>
#include <QStringList>

class JIO
{
public:
	JIO();
	~JIO();
	static QStringList FileToStrList(QString fileName);
	static QList<QList<QString>> CsvToStrList(QString fileName);
	static QList<QList<double>> MatToDList(QString fileName);
	static bool save(QString fileName, QString text);
	static bool save(QString fileName, QStringList textList);
	static void show(QStringList input);
	static void show(QList<QList<QString>> input);
	static void show(QList<QList<double>> input);
	static void test();
};

