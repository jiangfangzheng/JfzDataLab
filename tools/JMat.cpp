//
// Created by Sandeepin on 2017/5/11 0011.
//

#include "JMat.h"
#include "stringtools.h"
#include <iostream>
#include <fstream>
#include <c++/sstream>
#include <QDebug>

JMat::JMat() : row(0), col(0)
{
}

JMat::JMat(int nrow, int ncol) : row(nrow), col(ncol)
{
    for (int i = 0; i < row; ++i)
    {
        vector<double> temp(col, 0);
        mat.push_back(temp);
    }
}

JMat::JMat(vector<vector<double>> dmat) : mat(dmat)
{
	row = mat.size();
	if(row!=0)
		col = mat[0].size();
	else
		col = 0;
}

int JMat::getRow()
{
	return row;
}

int JMat::getCol()
{
	return col;
}

vector<vector<double>> JMat::getMat()
{
	return mat;
}

bool JMat::show()
{
    if (row == 0 || col == 0)
    {
        printf("        NULL\r\n\r\n");
        return false;
    }
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            if (j + 1 == col)
            {
                printf("%16.6lf\r\n", mat[i][j]);
            }
            else
            {
                printf("%16.6lf ", mat[i][j]);
            }
        }
    }
    printf("\r\n");
    return true;
}


bool JMat::load(string FileName)
{
	// 【1】读文件
    // 以读模式打开文件
    ifstream infile;
    infile.open(FileName, ios::in);
    if (!infile)
    {
        printf("No File!");
        return false;
    }
    // 从文件读取数据，并显示它
    char line[200000];
	string text;
    while (infile.getline(line, sizeof(line)))
    {
        stringstream word(line);
        word >> text;
		vector<string> textlist = split(text, ",");
		vector<double> doublelist;
		for(unsigned int i=0;i<textlist.size();++i)
		{
			doublelist.push_back( stod(textlist[i]) );
		}
		// 【2】double矩阵赋值
		mat.push_back(doublelist);
    }
    // 关闭打开的文件
	infile.close();

	// 【3】行列赋值
	row = mat.size();
	if(row != 0)
		col = mat[0].size();
	else
		col = 0;
//	qDebug()<<"row: "<<row<<" "<< col;
	return true;
}

bool JMat::save(string FileName)
{
    // 以写模式打开文件
    ofstream outfile;
    outfile.open(FileName, ios::app);
    if (!outfile)
    {
        printf("No File!");
        return false;
    }
    // 向文件写入用户输入的数据
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            char str[32];
            sprintf(str, "%0.3lf", mat[i][j]);
            outfile << str;
            if (j + 1 == col)
                outfile << "\n";
            else
                outfile << ",";
        }
    }
    // 关闭打开的文件
    outfile.close();
    return true;
}

void JMat::Test()
{
    JMat a;
    a.show();
    a.save("a.txt");

    JMat b(3, 4);
    b.show();
    b.save("b1.txt");
    b.load("input\\load.txt");
    b.show();
    b.save("b2.txt");

    vector<vector<double>> mat;
    int row = 6;
    int col = 2;
    for (int i = 0; i < row; ++i)
    {
        vector<double> temp(col, 0);
        mat.push_back(temp);
        for (int j = 0; j < col; ++j)
        {
            mat[i][j] = (j + 1) * i * (j + 1) * i;
        }
    }

	JMat c(mat);
    c.show();
    c.save("c.txt");

    JMat d(2878, 231);
    d.load("input\\mattest_2878x231.csv");
    d.save("d.csv");

}


