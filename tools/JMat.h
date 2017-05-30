//
// Created by Sandeepin on 2017/5/11 0011.
//

#ifndef JFZCPPLIB_JMAT_H
#define JFZCPPLIB_JMAT_H

#include <string>
#include <c++/vector>

using namespace std;

class JMat {
private:
    int row;
    int col;
    vector<vector<double>> mat;
public:
    JMat();

    JMat(int nrow, int ncol);

	JMat(vector<vector<double>> dmat);

	int getRow();

	int getCol();

	vector<vector<double>> getMat();

    bool show();

    bool load(string FileName);

    bool save(string FileName);

    static void Test();
};


#endif //JFZCPPLIB_JMAT_H
