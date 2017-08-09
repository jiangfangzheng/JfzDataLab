#pragma once

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

#include <QStringList>
#include <armadillo>
using namespace arma;

extern QString saveFilePath;

// 数据各项名
extern QString DataName_DS18B20;
extern QString DataName_DS18ALL;
extern QString DataName_FBGT;
extern QString DataName_FBGS;
extern QString DataName_FBG;
extern QString DataName_CCD;
extern QString DataName_ENV;
extern QString DataName_CNC;
extern QString DataName_FBGST;


// 线性回归 输出: 字符串
QString QLinearRegression(QString FileName, QString &strOutCsv);
// 线性回归 输出: 回归数组
QList<double> QLinearRegression(QString FileName);
// 线性回归 输入: 二维QList 输出: 回归数组
QList<double> QLinearRegression(QList<QList<double>> matInput);


// 代码中用-时间工具（2017-06-17 23:17:13转20170617231713）
QString TimeToTimeNum(QString Time);
// 代码中用-将640个FBG通道转为我们需要的321个通道，包括应力、温度
bool ALLFBGto321FBG(mat &inputMat, mat &outputMat);

// 标准化算法-核心
mat Stand_FBG(QStringList &CH);
mat Stand_DS18B20(QStringList &CH);

// 数据标准化-DS18B20-部署通道排序
bool standardDS18B20(QString dir);
// 数据标准化-CCD-部署通道排序
bool standardCCD(QString dir);
// 数据标准化-FBGT(预处理-波长修复)-部署通道排序
bool standardFBGT(QString dir);
// 数据标准化-FBGS(没有修复)-部署通道排序
bool standardFBGS(QString dir);
// FBG波长转温度(陈标定)
bool FBGTtoTEMP(QString standFBGTFileName);
// 相关性分析-输入2个文件
bool correlationAnalysis(QString correlationFileName1, QString correlationFileName2);
// 多元线性回归-输入文件名
bool LinearRegression(QString inputFile);
// 多元线性回归预测-输入模型、训练数据集
bool LinearRegressionPredict(QString ModelFile, QString DataFile);

// 数据概览-csv文件基本统计分析
bool csvStatisticAnalysis(QString InputFileName_str);

// 数据小处理-初始值为0
bool dataZeroMat(QString fileName);
bool dataZeroCsv(QString fileName);
// 数据小处理-增量化（后一个数-前一个数）
bool dataDeltaMat(QString fileName);
bool dataDeltaCsv(QString fileName);
// 数据小处理-压缩拉伸
bool dataSamplingMat(QString fileName, int &nowNum ,int needNum);
bool dataSamplingCsv(QString fileName, int &nowNum ,int needNum);
// 数据小处理-数据清洗(设定阈值内数据，前一个值覆盖后一个值)
bool dataCleanMat(QString fileName, double maxNum, double minNum);
bool dataCleanCsv(QString fileName, double maxNum, double minNum);
// 数据小处理-趋势预测
bool dataTendencyMat(QString fileName, int window);
bool dataTendencyCsv(QString fileName, int window);
// 数据小处理-按天拆分
bool dataSplitByDateCsv(QString fileName);
// 数据小处理-csv合并
bool mergeCsv(QStringList fileNameList);

// 虚拟映射-数据通道映射为虚拟标准通道-FBG温度
bool virtualMapFBGT(QStringList fileNameList);
// 虚拟映射-数据通道映射为虚拟标准通道-FBG应力
bool virtualMapFBGS(QStringList fileNameList);
// 虚拟映射-数据通道映射为虚拟标准通道-电类温度
bool virtualMapDS18(QStringList fileNameList);
// 虚拟映射-映射的FBG温度传感器转温度值
bool virtualFBGtoTEMP(QStringList fileNameList);
// 虚拟映射-映射的FBG应力传感器转应力
bool virtualFBGtoSTRESS(QStringList fileNameList);
// 虚拟映射-环境温度数据映射-txt版本
bool OriENVtoVirtual(QStringList EnvFileNameList);
// 一键原始电类数据映射
bool OriDS18toVirtual(QString dir);
// 一键原始FBG数据映射
bool OriFBGtoVirtual(QString dir);
