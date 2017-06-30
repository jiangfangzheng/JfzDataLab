//
// Created by Sandeepin on 2017/5/16 0016.
//
#include "tictoc.h"
#include <cstdio>
#include <time.h>

clock_t start, finish;

void tic()
{
    start = clock();
}

void toc()
{
    finish = clock();
    double duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("Time:%7.3f s\n", duration);
}

