//
// Created by Sandeepin on 2017/5/12 0012.
//

#ifndef JFZCPPLIB_JCSV_H
#define JFZCPPLIB_JCSV_H

#include <iostream>
#include <vector>

using namespace std;


class JCSV {
private:
    int row;
    int col;
    string title;
    vector<string> itemName;
    vector<string> timeName;
    vector<vector<double>> mat;

public:
    JCSV();

    JCSV(string fileName);

    JCSV(string title, vector<string> itemName, vector<string> timeName, vector<vector<double>> mat, int row, int col);

    string getTitle();

    vector<string> getItemName();

    vector<string> getTimeName();

    int getRow();

    int getCol();

    vector<vector<double>> getMat();

    bool show();

    bool save(string fileName);

    static void Test();

};


#endif //JFZCPPLIB_JCSV_H
