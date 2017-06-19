#include "LinearRegression.h"
#include <QDebug>
#include "tools/JMat.h"
#include "tools/JIO.h"

#include <armadillo>
using namespace arma;

// 功能：释放内存。
// dat：矩阵数组；
// d：临时变量；
// count：方程元数；
// 返回：void
void FreeData(double **dat, double *d, int count)
{
	int i;
	free(d);
	for (i = 0; i < count; i++)
		free(dat[i]);
	free(dat);
}

// 功能：解线性方程。
// data[count*(count+1)]：矩阵数组；
// count：方程元数；
// Answer[count]：求解数组 。
// 返回：0求解成功，-1无解或者无穷解
int LinearEquations(double *data, int count, double *Answer)
{
	int j, m, n;
	double tmp, **dat, *d = data;
	dat = (double**)malloc(count * sizeof(double*));
	for (m = 0; m < count; m++, d += (count + 1))
	{
		dat[m] = (double*)malloc((count + 1) * sizeof(double));
		memcpy(dat[m], d, (count + 1) * sizeof(double));
	}
	d = (double*)malloc((count + 1) * sizeof(double));
	for (m = 0; m < count - 1; m++)
	{
		// 如果主对角线元素为0，行交换
		for (n = m + 1; n < count && dat[m][m] == 0.0; n++)
		{
			if (dat[n][m] != 0.0)
			{
				memcpy(d, dat[m], (count + 1) * sizeof(double));
				memcpy(dat[m], dat[n], (count + 1) * sizeof(double));
				memcpy(dat[n], d, (count + 1) * sizeof(double));
			}
		}
		// 行交换后，主对角线元素仍然为0，无解，返回-1
		if (dat[m][m] == 0.0)
		{
			FreeData(dat, d, count);
			return -1;
		}
		// 消元
		for (n = m + 1; n < count; n++)
		{
			tmp = dat[n][m] / dat[m][m];
			for (j = m; j <= count; j++)
				dat[n][j] -= tmp * dat[m][j];
		}
	}
	for (j = 0; j < count; j++)
		d[j] = 0.0;
	// 求得count - 1的元
	Answer[count - 1] = dat[count - 1][count] / dat[count - 1][count - 1];
	// 逐行代入求各元
	for (m = count - 2; m >= 0; m--)
	{
		for (j = count - 1; j > m; j--)
			d[m] += Answer[j] * dat[m][j];
		Answer[m] = (dat[m][count] - d[m]) / dat[m][m];
	}
	FreeData(dat, d, count);
	return 0;
}

// 功能：求多元回归方程：Y = B0 + B1X1 + B2X2 + ...BnXn
// data[rows*cols]二维数组；X1i,X2i,...Xni,Yi (i=0 to rows-1)
// rows：数据行数；
// cols数据列数；
// Answer[cols]：返回回归系数数组(B0,B1...Bn)
// SquarePoor[4]：返回方差分析指标: 回归平方和，剩余平方和，回归平方差，剩余平方差
// 返回：0求解成功，-1错误
int MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor)
{
	int m, n, i, count = cols - 1;
	double *dat, *p, a, b;
	if (data == 0 || Answer == 0 || rows < 2 || cols < 2)
		return -1;
	dat = (double*)malloc(cols * (cols + 1) * sizeof(double));
	dat[0] = (double)rows;
	for (n = 0; n < count; n++)                     // n = 0 to cols - 2
	{
		a = b = 0.0;
		for (p = data + n, m = 0; m < rows; m++, p += cols)
		{
			a += *p;
			b += (*p * *p);
		}
		dat[n + 1] = a;                              // dat[0, n+1] = Sum(Xn)
		dat[(n + 1) * (cols + 1)] = a;               // dat[n+1, 0] = Sum(Xn)
		dat[(n + 1) * (cols + 1) + n + 1] = b;       // dat[n+1,n+1] = Sum(Xn * Xn)
		for (i = n + 1; i < count; i++)             // i = n+1 to cols - 2
		{
			for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
				a += (p[n] * p[i]);
			dat[(n + 1) * (cols + 1) + i + 1] = a;   // dat[n+1, i+1] = Sum(Xn * Xi)
			dat[(i + 1) * (cols + 1) + n + 1] = a;   // dat[i+1, n+1] = Sum(Xn * Xi)
		}
	}
	for (b = 0.0, m = 0, p = data + n; m < rows; m++, p += cols)
		b += *p;
	dat[cols] = b;                                   // dat[0, cols] = Sum(Y)
	for (n = 0; n < count; n++)
	{
		for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
			a += (p[n] * p[count]);
		dat[(n + 1) * (cols + 1) + cols] = a;        // dat[n+1, cols] = Sum(Xn * Y)
	}
	n = LinearEquations(dat, cols, Answer);          // 计算方程式
	// 方差分析
	if (n == 0 && SquarePoor)
	{
		b = b / rows;                                // b = Y的平均值
		SquarePoor[0] = SquarePoor[1] = 0.0;
		p = data;
		for (m = 0; m < rows; m++, p++)
		{
			for (i = 1, a = Answer[0]; i < cols; i++, p++)
				a += (*p * Answer[i]);               // a = Ym的估计值
			SquarePoor[0] += ((a - b) * (a - b));    // U(回归平方和)
			SquarePoor[1] += ((*p - a) * (*p - a));  // Q(剩余平方和)(*p = Ym)
		}
		SquarePoor[2] = SquarePoor[0] / count;       // 回归方差
		if (rows - cols > 0.0)
			SquarePoor[3] = SquarePoor[1] / (rows - cols); // 剩余方差
		else
			SquarePoor[3] = 0.0;
	}
	free(dat);
	return n;
}

// 功能：求多元回归方程的系数：Y = B0 + B1X1 + B2X2 + ...BnXn
// data[rows*cols]二维数组；X1i,X2i,...Xni,Yi (i=0 to rows-1)
// rows：数据行数；cols数据列数；
// Answer[cols]：返回回归系数数组(B0,B1...Bn)
// 返回：0求解成功，-1错误
int MultipleRegressionOnlyB(double *data, int rows, int cols, double *Answer)
{
	int m, n, i, count = cols - 1;
	double *dat, *p, a, b;
	if (data == 0 || Answer == 0 || rows < 2 || cols < 2)
		return -1;
	dat = (double*)malloc(cols * (cols + 1) * sizeof(double));
	dat[0] = (double)rows;
	for (n = 0; n < count; n++)                     // n = 0 to cols - 2
	{
		a = b = 0.0;
		for (p = data + n, m = 0; m < rows; m++, p += cols)
		{
			a += *p;
			b += (*p * *p);
		}
		dat[n + 1] = a;                              // dat[0, n+1] = Sum(Xn)
		dat[(n + 1) * (cols + 1)] = a;               // dat[n+1, 0] = Sum(Xn)
		dat[(n + 1) * (cols + 1) + n + 1] = b;       // dat[n+1,n+1] = Sum(Xn * Xn)
		for (i = n + 1; i < count; i++)             // i = n+1 to cols - 2
		{
			for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
				a += (p[n] * p[i]);
			dat[(n + 1) * (cols + 1) + i + 1] = a;   // dat[n+1, i+1] = Sum(Xn * Xi)
			dat[(i + 1) * (cols + 1) + n + 1] = a;   // dat[i+1, n+1] = Sum(Xn * Xi)
		}
	}
	for (b = 0.0, m = 0, p = data + n; m < rows; m++, p += cols)
		b += *p;
	dat[cols] = b;                                   // dat[0, cols] = Sum(Y)
	for (n = 0; n < count; n++)
	{
		for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
			a += (p[n] * p[count]);
		dat[(n + 1) * (cols + 1) + cols] = a;        // dat[n+1, cols] = Sum(Xn * Y)
	}
	n = LinearEquations(dat, cols, Answer);          // 计算方程式
	free(dat);
	return n;
}

// 功能：显示计算结果
// dat：二维数组，X1、X2……Y的矩阵；
// Answer[cols]：返回回归系数数组(B0,B1...Bn)
// SquarePoor[4]：返回方差分析指标: 回归平方和，剩余平方和，回归平方差，剩余平方差
// rows：数据行数；
// cols数据列数；
// 返回：0求解成功，-1错误
void Display(double *dat, double *Answer, double *SquarePoor, int rows, int cols)
{
	double v, *p;
	int i, j;
	printf("回归方程式:    Y = %.5lf", Answer[0]);
	for (i = 1; i < cols; i++)
		printf(" + %.5lf*X%d", Answer[i], i);
	printf(" \n");
	printf("回归显著性检验:  \n");
	printf("回归平方和：%12.4lf  回归方差：%12.4lf  \n", SquarePoor[0], SquarePoor[2]);
	printf("剩余平方和：%12.4lf  剩余方差：%12.4lf  \n", SquarePoor[1], SquarePoor[3]);
	printf("离差平方和：%12.4lf  标准误差：%12.4lf  \n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
	printf("F   检  验：%12.4lf  相关系数：%12.4lf  \n", SquarePoor[2] / SquarePoor[3],
		sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
	printf("剩余分析:  \n");
	printf("      观察值      估计值      剩余值    剩余平方  \n");
	for (i = 0, p = dat; i < rows; i++, p++)
	{
		v = Answer[0];
		for (j = 1; j < cols; j++, p++)
			v += *p * Answer[j];
		printf("%12.2lf%12.2lf%12.2lf%12.2lf  \n", *p, v, *p - v, (*p - v) * (*p - v));
	}
	//system("pause");
	printf(" \n");
}

// 功能：显示计算结果，只显示回归系数
// dat：二维数组，X1、X2……Y的矩阵；
// Answer[cols]：输入回归系数数组(B0,B1...Bn)
// rows：数据行数；
// cols数据列数；
// 返回：0求解成功，-1错误
void DisplayOnlyB(double *dat, double *Answer, int rows, int cols)
{
	double v, *p;
	int i, j;
	printf("回归方程:Y = %.5lf", Answer[0]);
	for (i = 1; i < cols; i++)
		printf(" + %.5lf*X%d", Answer[i], i);
	printf(" \n");
	printf("数据对比:  \n");
	printf("      观察值      估计值  \n");
	for (i = 0, p = dat; i < rows; i++, p++)
	{
		v = Answer[0];
		for (j = 1; j < cols; j++, p++)
			v += *p * Answer[j];
		printf("%12.2lf%12.2lf  \n", *p, v);
	}
	//system("pause");
}

// 功能：输出回归方程系数
// data：二维数组，X1、X2……Y的矩阵；
// rows：数据行数；
// cols数据列数；
// Answer[cols]：输入回归系数数组(B0,B1...Bn)
// 返回：0求解成功，-1错误
int PrintCoefficient(double *data, int rows, int cols, double *Answer)
{
	if (MultipleRegressionOnlyB((double*)data, rows, cols, Answer) == 0)
	{
		int i;
		printf("回归方程:");
		printf("Y = %.5lf", Answer[0]);
		for (i = 1; i < cols; i++)
		{
			printf(" + %.5lf*X%d", Answer[i], i);
		}
		printf(" \n");
		return 0;
	}
	else  return -1;
}


QString QLinearRegression(QString FileName,QString &strOutCsv)
{
	QString strOut; // 输出

	// 【1】用C语言版本算-效果还好
	// 载入文件
	QList<QList<double>> matInput = JIO::MatToDList(FileName);
	int matRow = matInput.size();
	int matCol = 0;
	if(matRow > 0)
		matCol = matInput[0].size();
	// 多元线性回归
	double *data = new double[matRow * matCol];
	double Answer[matCol];
	for(int i=0;i<matRow;++i)
	{
		for(int j=0;j<matCol;++j)
		{
			data[i*matCol+j] = matInput[i][j];
		}
	}

	if (MultipleRegressionOnlyB(data, matRow, matCol, Answer) == 0)
	{
		int i;
		int cols = matCol;
		strOut.sprintf("Y = %.5lf", Answer[0]);
		strOutCsv.sprintf("%.5lf,", Answer[0]);
		for (i = 1; i < cols; i++)
		{
			QString temp;
			QString tempCsv;
			temp.sprintf(" + %.5lf*X%d", Answer[i], i);
			tempCsv.sprintf("%.5lf,", Answer[i]);
			strOut += temp;
			strOutCsv += tempCsv;
		}
		strOutCsv = strOutCsv.left(strOutCsv.lastIndexOf(','));
	}


	// 【2】用mat来算的例子(与matlab计算结果相差大，不好)
//	QList<QList<double>> inMat = JIO::MatToDList(FileName);
//	mat X(inMat.size(),inMat[0].size());
//	X.fill(1);
//	for(int i = 0; i<inMat.size(); ++i)
//	{
//		for(int j = 0; j<inMat[0].size()-1; ++j)
//		{
//			X(i,j+1) = inMat[i][j];
//		}
//	}
// //	X.save("X.csv", raw_ascii);
//	mat Y(inMat.size(),1);
//	int &&ycol = inMat[0].size()-1;
//	for(int i=0; i<inMat.size(); ++i)
//	{
//		Y(i,0) = inMat[i][ycol];
//	}
// //	Y.save("Y.csv", raw_ascii);
//	//b = (X'X)^(-1)X'Y 核心公式
//	mat B = inv(X.t()*X)*X.t()*Y;
// //	B.save("B.csv", raw_ascii);

//	// 保存字符串
//	int i;
//	int rows = B.n_rows;
//	strOut.sprintf("Y = %.5lf", B(0,0));
//	strOutCsv.sprintf("%.15lf,", B(0,0));
//	for (i = 1; i < rows; i++)
//	{
//		QString temp;
//		QString tempCsv;
//		temp.sprintf(" + %.5lf*X%d", B(i,0), i);
//		tempCsv.sprintf("%.15lf,", B(i,0));
//		strOut += temp;
//		strOutCsv += tempCsv;
//	}
//	strOutCsv = strOutCsv.left(strOutCsv.lastIndexOf(','));

	return strOut;
}
