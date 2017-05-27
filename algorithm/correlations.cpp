#include "correlations.h"

// 相关性分析，得到相关性矩阵
mat CorrelationAnalysis(const QString &correlationFileName1, const QString &correlationFileName2, QStringList &DataName1, QStringList &DataName2, QStringList &Time)
{
	// 读取两文件矩阵
	int InputFileRow1;
	int InputFileCol1;
	int InputFileRow2;
	int InputFileCol2;
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
	return corMatneed;
}

// 保存相关性分析的结果
bool saveCorrelationAnalysisCSV(const QString &correlationFileName1, const QString &correlationFileName2, QStringList &DataName1, QStringList &DataName2, mat &corMatneed)
{
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
	if(b == 0)
		return true;
	else
		return false;
}
