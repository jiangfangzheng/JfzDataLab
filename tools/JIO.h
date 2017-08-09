#pragma once

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

#include <QStringList>
#include <armadillo>
using namespace arma;

class JIO
{
public:
	JIO();
	~JIO();
	// 读取
	static QStringList           readFile(QString fileName);	// 读取普通文本到字符串列表
	static QList<QList<QString>> readCsv(QString fileName, QString fuhao = ",");		// 读取csv到2维字符串列表
	static mat                   readCsv(QString fileName, QStringList &itemName, QStringList &timeName);	// 读取csv分别保存为项目名列表、时间列表、mat
	static QList<QList<double>>  readMat(QString fileName);		// 读取mat到2维double列表
	static mat                   readAMat(QString fileName);	// 读取mat到2维Armadillo mat
	// 写入
	static bool save(QString fileName, QString text);
	static bool save(QString fileName, QStringList textList);
	static bool save(QString fileName, mat matData);
	static bool save(QString fileName, QList<QList<double>> matData);
	static bool save(QString fileName, QString itemName, QStringList timeName, mat matData);
	static bool save(QString fileName, QStringList itemName, QStringList timeName, mat matData);
	// 显示
	static void show(QStringList input);
	static void show(QList<QList<QString>> input);
	static void show(QList<QList<double>> input);
	static void show(QList<double> input);
	static void show(mat input);
	// 测试
	static void test();
};

