//
// Created by Sandeepin on 2017/5/12 0012.
//

#include "JCSV.h"
#include "stringtools.h"
#include "tictoc.h"
#include <fstream>
#include <c++/sstream>

JCSV::JCSV():row(0),col(0),title("")
{
}

JCSV::JCSV(string fileName)
{
    ifstream infile;
    infile.open(fileName, ios::in);
    if (!infile)
    {
        printf("No File!\n");
    }
    char line[200000];
    string text;

    // 读取第一行
    infile.getline(line, sizeof(line));
    stringstream word(line);
    word >> text;
    vector<string> textlist = split(text, ",");
    col = textlist.size();
    for (auto i = textlist.begin(); i != textlist.end(); ++i)
    {
        if (i == textlist.begin())
            title = *i;
        else
            itemName.push_back(*i);
    }
    if ((itemName.size() + 1) != col) cout << "Data Error!";

    // 读取后面的
    int rows = 0;
    vector<double> matvec;
    while (infile.getline(line, sizeof(line)))
    {
        stringstream word2(line);
        word2 >> text;
        vector<string> textlist2 = split(text, ",");
        for (auto i = textlist2.begin(); i != textlist2.end(); ++i)
        {
            if (i == textlist2.begin())
                timeName.push_back(*i);
            else
                matvec.push_back(atof((*i).c_str()));
        }
        rows++;
    }
    row = rows + 1;

    // mat赋值
    int matrow = rows;
    int matcol = col - 1;
    mat.resize(matrow);
    for (int i = 0; i < matrow; ++i)
    {
        for (int j = 0; j < matcol; ++j)
        {
            mat[i].push_back(matvec[matcol * i + j]);
        }
    }

    infile.close();
}

JCSV::JCSV(string title, vector<string> itemName, vector<string> timeName, vector<vector<double>> mat, int matRow, int matCol)
{
    this->title = title;
    this->itemName = itemName;
    this->timeName = timeName;
    this->mat = mat;
    this->row = matRow + 1;
    this->col = matCol + 1;
}

string JCSV::getTitle()
{
    return title;
}

vector<string> JCSV::getItemName()
{
    return itemName;
}

vector<string> JCSV::getTimeName()
{
    return timeName;
}

int JCSV::getRow()
{
    return row;
}

int JCSV::getCol()
{
    return col;
}

vector<vector<double>> JCSV::getMat()
{
    return mat;
}

bool JCSV::show()
{
    if (row == 0 || col == 0)
    {
        printf("        NULL\r\n\r\n");
        return false;
    }

    cout << "Row: " << row << " Col: " << col << endl;
    cout << "Title: " << title << endl;
    cout << "itemName: ";
    for (auto i = itemName.begin(); i != itemName.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl << "timeName: ";
    for (auto i = timeName.begin(); i != timeName.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl << "Mat:" << endl;

    int matRow = row - 1;
    int matCol = col - 1;
    for (int i = 0; i < matRow; ++i)
    {
        for (int j = 0; j < matCol; ++j)
        {
            if (j + 1 == matCol)
            {
                printf("%16.6lf\n", mat[i][j]);
            }
            else
            {
                printf("%16.6lf ", mat[i][j]);
            }
        }
    }
    printf("\n");
    return true;
}


bool JCSV::save(string fileName)
{
    ofstream outfile;
    outfile.open(fileName, ios::app);
    if (!outfile)
    {
        printf("No File!");
        return false;
    }
    if (col == 0 || row == 0)
    {
        outfile << "";
    }
    else
    {
        string lineOne = title + ",";
        for (auto i = begin(itemName); i != end(itemName); ++i)
        {
            if ((i + 1) != end(itemName))
                lineOne += *i + ",";
            else
                lineOne += *i + "\n";
        }
        outfile << lineOne;

        int matRow = row - 1;
        int matCol = col - 1;
        for (int i = 0; i < matRow; ++i)
        {
            string line = timeName[i] + ",";
            for (int j = 0; j < matCol; ++j)
            {
                char str[32];
                sprintf(str, "%0.3lf", mat[i][j]);
                string strString(str);
                if (j + 1 == matCol)
                    line += strString + "\n";
                else
                    line += strString + ",";
            }
            outfile << line;
        }
    }
    outfile.close();
    return true;
}

void JCSV::Test()
{
    JCSV csv1;
    csv1.show();
    csv1.save("csvtest_csv1_out.csv");

    JCSV csv2("input\\csvtest_2879x232.csv");
//    csv2.show();
    tic();
    csv2.save("csvtest_csv2_out.csv");
    toc();

    string title = "JFZ";
    vector<string> itemName = {"poi1", "poi2", "poi3", "poi4", "poi5",};
    vector<string> timeName = {"2017-05-10", "2017-05-11", "2017-05-12"};
    int matrow = 3;
    int matcol = 5;
    vector<vector<double>> mat;
    int a=0;
    for (int i = 0; i < matrow; ++i)
    {
        vector<double> temp(matcol,0);
        mat.push_back(temp);
        for (int j = 0; j < matcol; ++j)
        {
            mat[i][j] = ++a;
        }
    }
    JCSV csv3(title, itemName, timeName, mat, matrow, matcol);
    csv3.show();
    csv3.save("csvtest_csv3_out.csv");
}

