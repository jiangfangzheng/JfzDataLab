#ifndef LINREG_H
#define LINREG_H

#include <cstdio>
#include <cmath>   //math
#include <cstdlib> //system
#include <cstring> //memcpy
#include <QString>

void FreeData(double **dat, double *d, int count);
int LinearEquations(double *data, int count, double *Answer);
int MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor);
void Display(double *dat, double *Answer, double *SquarePoor, int rows, int cols);
int MultipleRegressionOnlyB(double *data, int rows, int cols, double *Answer);
void DisplayOnlyB(double *dat, double *Answer, int rows, int cols);
int PrintCoefficient(double *data, int rows, int cols, double *Answer);

QString QLinearRegression(QString FileName);

#endif
