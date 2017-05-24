#include "algorithm/datatostand.h"

vector<double> standWave08 = {1535.91,1537.892,1539.88,1541.928,1543.865,1545.865,1547.871,1549.925,1552.03,1554.058,1555.991,1557.835,1559.858,1561.989,1563.91};
vector<double> standWave09 = {1549.888,1551.923,1554.003,1556.046,1558.072,1559.968,1561.973,1563.921};
vector<double> standWave10 = {1551.908,1553.899,1555.256,1557.897,1559.862,1561.929,1563.863};
vector<double> standWave11 = {1535.919,1537.952,1539.937,1541.986,1543.934,1545.927,1547.917,1549.915,1551.867,1553.893,1555.976,1557.97,1559.93,1561.868,1563.824};
vector<double> standWave12 = {1535.883,1537.887,1539.876,1541.944,1543.916,1545.887,1547.862,1549.875,1551.882,1553.864,1555.933,1557.929,1559.979,1562.067,1564.086};
vector<double> standWave13 = {1535.887,1537.895,1539.878,1541.909,1543.891,1545.846,1547.85,1549.849,1551.913,1553.89,1555.283,1557.986,1559.922,1561.974,1563.944};
vector<double> standWave14 = {1535.917,1537.936,1539.924,1541.964,1543.869,1545.864,1547.86};
vector<double> standWave15 = {1535.953,1538.018,1539.972,1542.003,1544.001,1545.952,1547.968,1550.004};
vector<double> standWave16 = {1537.916,1540.989,1543.933,1546.884,1549.983,1552.842,1556.886,1559.866};
vector<double> standWave17 = {1537.891,1540.868,1543.919,1546.874,1549.865,1552.852,1556.838,1559.845};
vector<double> standWave18 = {1537.88,1540.867,1543.895,1546.834,1550.129,1552.898,1556.887,1559.87};
vector<double> standWave19 = {1536.88,1539.851,1542.783,1545.85,1548.81,1551.925,1554.8,1557.806,1560.847,1563.788};
vector<double> standWave20 = {1537.927,1540.9,1543.824,1546.868,1549.795,1552.848,1555.959,1558.893,1561.833,1565.857};
vector<double> standWave21 = {1533.28,1536.782,1539.724,1542.544,1545.746,1548.583,1551.862};
vector<double> standWave22 = {1533.231,1536.3,1539.579,1542.234,1545.224,1548.25,1551.529,1554.316,1557.424,1560.507};
vector<double> standWave23 = {1532.419,1535.423,1538.44,1541.37};
vector<double> standWave24 = {1532.338,1535.593,1538.606,1541.579,1544.62,1547.436,1550.48,1553.68,1556.401,1559.5};
vector<double> standWave25 = {1533.465,1536.576,1538.311,1542.081,1544.913,1548.159,1550.868,1554.26,1556.928,1560.047};
vector<double> standWave26 = {1547.537,1553.547,1556.568,1559.39};
vector<double> standWave27 = {1537.936,1540.857,1543.784,1546.859,1549.785,1552.862,1555.947,1558.827,1561.923};
vector<double> standWave28 = {1536.868,1539.832,1542.826,1545.86,1548.829,1551.972,1554.835,1557.869,1560.815,1563.835};
vector<double> standWave29 = {1537.944,1540.885,1543.825,1546.883,1549.931,1552.827,1555.965,1558.862,1561.873,1565.898};
vector<double> standWave30 = {1536.901,1539.926,1542.797,1545.828,1548.942,1551.957,1554.805,1557.825,1560.848,1563.82};
vector<double> standWave31 = {1537.88,1540.9,1543.795,1546.836,1549.805,1552.842,1555.919,1558.828,1561.864,1565.845};
vector<double> standWave32 = {1555.018,1558.218,1561.126};

// 波长修复算法，剔除多余的、非标定段的波长，实在没有给0
bool WaveFix(vector<double> &input, vector<double> &standWave, double output[])
{
	int index = 0;
	for (auto i = standWave.begin(); i != standWave.end(); ++i)
	{
		for (auto j = input.begin(); j != input.end(); ++j)
		{
			// 找波长比标定时数据大的，并且差值小于1.1（100多度以内）
			if ((*j - *i) >= 0 && (*j - *i) <= 1.1)
			{
				output[index] = *j; // 这个*j就放在index处
				break;
			}
			else
			{
				output[index] = 0;
			}
		}
		++index;
	}
	return true;
}

// DS18B20标准化
int Stand_DS18B20(QString CH1, QString CH2, QString CH3, QString CH4, QString CH5, QString CH6, QString CH7, mat &DS18B20_All_Mat, QStringList &XLabelName)
{
    int InputFileRow=0;
    int InputFileCol=0;
    // 时间
    //QStringList XLabelName;
    QStringList XLabelName2;

    // 传感器ID编号
    QStringList DataNameCH1;
    QStringList DataNameCH2;
    QStringList DataNameCH3;
    QStringList DataNameCH4;
    QStringList DataNameCH5;
    QStringList DataNameCH6;
    QStringList DataNameCH7;

    // 由文件名输出mat、传感器ID编号、时间
    mat DS18B20_CH1_Mat = JfzReadCSVToAllMat(CH1, InputFileRow, InputFileCol, DataNameCH1, XLabelName);
    mat DS18B20_CH2_Mat = JfzReadCSVToAllMat(CH2, InputFileRow, InputFileCol, DataNameCH2, XLabelName2);
    mat DS18B20_CH3_Mat = JfzReadCSVToAllMat(CH3, InputFileRow, InputFileCol, DataNameCH3, XLabelName2);
    mat DS18B20_CH4_Mat = JfzReadCSVToAllMat(CH4, InputFileRow, InputFileCol, DataNameCH4, XLabelName2);
    mat DS18B20_CH5_Mat = JfzReadCSVToAllMat(CH5, InputFileRow, InputFileCol, DataNameCH5, XLabelName2);
    mat DS18B20_CH6_Mat = JfzReadCSVToAllMat(CH6, InputFileRow, InputFileCol, DataNameCH6, XLabelName2);
    mat DS18B20_CH7_Mat = JfzReadCSVToAllMat(CH7, InputFileRow, InputFileCol, DataNameCH7, XLabelName2);

    //    qDebug()<<"InputFileRow"<<InputFileRow;
    //    qDebug()<<"InputFileCol"<<InputFileCol;
    //    qDebug()<<"DataName"<<DataName;
    //    qDebug()<<"XLabelName"<<XLabelName[0];
    //    qDebug()<<"DS18B20_CH1_Mat"<<DS18B20_CH1_Mat(0,0);

    // 初始化排序Mat
    mat DS18B20_CH1_OrderMat(DS18B20_CH1_Mat.n_rows, DS18B20_CH1_Mat.n_cols);
    mat DS18B20_CH2_OrderMat(DS18B20_CH2_Mat.n_rows, DS18B20_CH2_Mat.n_cols);
    mat DS18B20_CH3_OrderMat(DS18B20_CH3_Mat.n_rows, DS18B20_CH3_Mat.n_cols);
    mat DS18B20_CH4_OrderMat(DS18B20_CH4_Mat.n_rows, DS18B20_CH4_Mat.n_cols);
    mat DS18B20_CH5_OrderMat(DS18B20_CH5_Mat.n_rows, DS18B20_CH5_Mat.n_cols);
    mat DS18B20_CH6_OrderMat(DS18B20_CH6_Mat.n_rows, DS18B20_CH6_Mat.n_cols);
    mat DS18B20_CH7_OrderMat(DS18B20_CH7_Mat.n_rows, DS18B20_CH7_Mat.n_cols);

    // 由ID来排序，建立索引规则
    int CH10 = DataNameCH1.indexOf("ID:28ffcdfa001604cb");
    int CH11 = DataNameCH1.indexOf("ID:28ff7eeda0160301");
    int CH12 = DataNameCH1.indexOf("ID:28ffa203011604fc");
    int CH13 = DataNameCH1.indexOf("ID:28ffd7cc0016044b");
    int CH14 = DataNameCH1.indexOf("ID:28ff6ba7a0160589");
    int CH15 = DataNameCH1.indexOf("ID:28ff28f8041603d9");
    int CH16 = DataNameCH1.indexOf("ID:28ff280c01160410");
    int CH17 = DataNameCH1.indexOf("ID:28ff4a2305160375");

    int CH20 = DataNameCH2.indexOf("ID:28ffaf1fb2150108");
    int CH21 = DataNameCH2.indexOf("ID:28ff921db2150167");
    int CH22 = DataNameCH2.indexOf("ID:28ff86eb041603f8");
    int CH23 = DataNameCH2.indexOf("ID:28ffd3dd041603fe");
    int CH24 = DataNameCH2.indexOf("ID:28ff71c100160479");
    int CH25 = DataNameCH2.indexOf("ID:28ffb81cb2150311");
    int CH26 = DataNameCH2.indexOf("ID:28ff6a0c0116047a");
    int CH27 = DataNameCH2.indexOf("ID:28ffa2dd00160443");

    int CH30 = DataNameCH3.indexOf("ID:28ff1d93a01603e3");
    int CH31 = DataNameCH3.indexOf("ID:28ff78bea01603cd");
    int CH32 = DataNameCH3.indexOf("ID:28ff4ec300160495");
    int CH33 = DataNameCH3.indexOf("ID:28ffaddc001604a3");
    int CH34 = DataNameCH3.indexOf("ID:28ffb6e500160474");
    int CH35 = DataNameCH3.indexOf("ID:28ffa3f7001604e5");
    int CH36 = DataNameCH3.indexOf("ID:28ff30f70016041c");
    int CH37 = DataNameCH3.indexOf("ID:28ff95f7001604fd");

    int CH40 = DataNameCH4.indexOf("ID:28ffe6ce00160405");
    int CH41 = DataNameCH4.indexOf("ID:28ff783db215044f");
    int CH42 = DataNameCH4.indexOf("ID:28ff52c600160449");
    int CH43 = DataNameCH4.indexOf("ID:28ffed30b2150334");
    int CH44 = DataNameCH4.indexOf("ID:28ffdcc20016042e");
    int CH45 = DataNameCH4.indexOf("ID:28ffd827b215045f");
    int CH46 = DataNameCH4.indexOf("ID:28ff3425051603b3");
    int CH47 = DataNameCH4.indexOf("ID:28ff247eb2150310");

    int CH50 = DataNameCH5.indexOf("ID:28ff76e500160456");
    int CH51 = DataNameCH5.indexOf("ID:28ffa5c30016043f");
    int CH52 = DataNameCH5.indexOf("ID:28ff2d22b2150195");
    int CH53 = DataNameCH5.indexOf("ID:28ffcd0d01160497");
    int CH54 = DataNameCH5.indexOf("ID:28ff55a7a01605af");
    int CH55 = DataNameCH5.indexOf("ID:28ff56d7001604e1");
    int CH56 = DataNameCH5.indexOf("ID:28ff3ccba0160491");
    int CH57 = DataNameCH5.indexOf("ID:28ff0091a01604f7");

    int CH60 = DataNameCH6.indexOf("ID:28ff80efa01605df");
    int CH61 = DataNameCH6.indexOf("ID:28ffd7c6a0160325");
    int CH62 = DataNameCH6.indexOf("ID:28ff04ada01605ec");
    int CH63 = DataNameCH6.indexOf("ID:28fffdd4a0160582");
    int CH64 = DataNameCH6.indexOf("ID:28ff87caa0160421");
    int CH65 = DataNameCH6.indexOf("ID:28ff1d9da0160475");
    int CH66 = DataNameCH6.indexOf("ID:28fffdada01605b9");
    int CH67 = DataNameCH6.indexOf("ID:28ff44d3a01605b8");

    int CH70 = DataNameCH7.indexOf("ID:28ff538da0160406");
    int CH71 = DataNameCH7.indexOf("ID:28ff387da0160416");
    int CH72 = DataNameCH7.indexOf("ID:28ffcacfa01605fb");
    int CH73 = DataNameCH7.indexOf("ID:28ff16a8a0160592");

    // mat根据ID按列排序
    DS18B20_CH1_OrderMat.col(0) = DS18B20_CH1_Mat.col(CH10);
    DS18B20_CH1_OrderMat.col(1) = DS18B20_CH1_Mat.col(CH11);
    DS18B20_CH1_OrderMat.col(2) = DS18B20_CH1_Mat.col(CH12);
    DS18B20_CH1_OrderMat.col(3) = DS18B20_CH1_Mat.col(CH13);
    DS18B20_CH1_OrderMat.col(4) = DS18B20_CH1_Mat.col(CH14);
    DS18B20_CH1_OrderMat.col(5) = DS18B20_CH1_Mat.col(CH15);
    DS18B20_CH1_OrderMat.col(6) = DS18B20_CH1_Mat.col(CH16);
    DS18B20_CH1_OrderMat.col(7) = DS18B20_CH1_Mat.col(CH17);

    DS18B20_CH2_OrderMat.col(0) = DS18B20_CH2_Mat.col(CH20);
    DS18B20_CH2_OrderMat.col(1) = DS18B20_CH2_Mat.col(CH21);
    DS18B20_CH2_OrderMat.col(2) = DS18B20_CH2_Mat.col(CH22);
    DS18B20_CH2_OrderMat.col(3) = DS18B20_CH2_Mat.col(CH23);
    DS18B20_CH2_OrderMat.col(4) = DS18B20_CH2_Mat.col(CH24);
    DS18B20_CH2_OrderMat.col(5) = DS18B20_CH2_Mat.col(CH25);
    DS18B20_CH2_OrderMat.col(6) = DS18B20_CH2_Mat.col(CH26);
    DS18B20_CH2_OrderMat.col(7) = DS18B20_CH2_Mat.col(CH27);

    DS18B20_CH3_OrderMat.col(0) = DS18B20_CH3_Mat.col(CH30);
    DS18B20_CH3_OrderMat.col(1) = DS18B20_CH3_Mat.col(CH31);
    DS18B20_CH3_OrderMat.col(2) = DS18B20_CH3_Mat.col(CH32);
    DS18B20_CH3_OrderMat.col(3) = DS18B20_CH3_Mat.col(CH33);
    DS18B20_CH3_OrderMat.col(4) = DS18B20_CH3_Mat.col(CH34);
    DS18B20_CH3_OrderMat.col(5) = DS18B20_CH3_Mat.col(CH35);
    DS18B20_CH3_OrderMat.col(6) = DS18B20_CH3_Mat.col(CH36);
    DS18B20_CH3_OrderMat.col(7) = DS18B20_CH3_Mat.col(CH37);

    DS18B20_CH4_OrderMat.col(0) = DS18B20_CH4_Mat.col(CH40);
    DS18B20_CH4_OrderMat.col(1) = DS18B20_CH4_Mat.col(CH41);
    DS18B20_CH4_OrderMat.col(2) = DS18B20_CH4_Mat.col(CH42);
    DS18B20_CH4_OrderMat.col(3) = DS18B20_CH4_Mat.col(CH43);
    DS18B20_CH4_OrderMat.col(4) = DS18B20_CH4_Mat.col(CH44);
    DS18B20_CH4_OrderMat.col(5) = DS18B20_CH4_Mat.col(CH45);
    DS18B20_CH4_OrderMat.col(6) = DS18B20_CH4_Mat.col(CH46);
    DS18B20_CH4_OrderMat.col(7) = DS18B20_CH4_Mat.col(CH47);

    DS18B20_CH5_OrderMat.col(0) = DS18B20_CH5_Mat.col(CH50);
    DS18B20_CH5_OrderMat.col(1) = DS18B20_CH5_Mat.col(CH51);
    DS18B20_CH5_OrderMat.col(2) = DS18B20_CH5_Mat.col(CH52);
    DS18B20_CH5_OrderMat.col(3) = DS18B20_CH5_Mat.col(CH53);
    DS18B20_CH5_OrderMat.col(4) = DS18B20_CH5_Mat.col(CH54);
    DS18B20_CH5_OrderMat.col(5) = DS18B20_CH5_Mat.col(CH55);
    DS18B20_CH5_OrderMat.col(6) = DS18B20_CH5_Mat.col(CH56);
    DS18B20_CH5_OrderMat.col(7) = DS18B20_CH5_Mat.col(CH57);

    DS18B20_CH6_OrderMat.col(0) = DS18B20_CH6_Mat.col(CH60);
    DS18B20_CH6_OrderMat.col(1) = DS18B20_CH6_Mat.col(CH61);
    DS18B20_CH6_OrderMat.col(2) = DS18B20_CH6_Mat.col(CH62);
    DS18B20_CH6_OrderMat.col(3) = DS18B20_CH6_Mat.col(CH63);
    DS18B20_CH6_OrderMat.col(4) = DS18B20_CH6_Mat.col(CH64);
    DS18B20_CH6_OrderMat.col(5) = DS18B20_CH6_Mat.col(CH65);
    DS18B20_CH6_OrderMat.col(6) = DS18B20_CH6_Mat.col(CH66);
    DS18B20_CH6_OrderMat.col(7) = DS18B20_CH6_Mat.col(CH67);

    DS18B20_CH7_OrderMat.col(0) = DS18B20_CH7_Mat.col(CH70);
    DS18B20_CH7_OrderMat.col(1) = DS18B20_CH7_Mat.col(CH71);
    DS18B20_CH7_OrderMat.col(2) = DS18B20_CH7_Mat.col(CH72);
    DS18B20_CH7_OrderMat.col(3) = DS18B20_CH7_Mat.col(CH73);

//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH3_OrderMat(0,0);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH3_OrderMat(0,1);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH3_OrderMat(0,2);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH3_OrderMat(0,3);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH3_OrderMat(0,4);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH3_OrderMat(0,5);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH3_OrderMat(0,6);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH3_OrderMat(0,7);

//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH2_OrderMat(0,0);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH2_OrderMat(0,1);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH2_OrderMat(0,2);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH2_OrderMat(0,3);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH2_OrderMat(0,4);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH2_OrderMat(0,5);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH2_OrderMat(0,6);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH2_OrderMat(0,7);

//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH7_OrderMat(0,0);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH7_OrderMat(0,1);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH7_OrderMat(0,2);
//        qDebug()<<"DS18B20_CH1_OrderMat"<<DS18B20_CH7_OrderMat(0,3);

    // 矩阵合并
    DS18B20_All_Mat = join_rows(DS18B20_CH1_OrderMat, DS18B20_CH2_OrderMat);
    DS18B20_All_Mat = join_rows(DS18B20_All_Mat, DS18B20_CH3_OrderMat);
    DS18B20_All_Mat = join_rows(DS18B20_All_Mat, DS18B20_CH4_OrderMat);
    DS18B20_All_Mat = join_rows(DS18B20_All_Mat, DS18B20_CH5_OrderMat);
    DS18B20_All_Mat = join_rows(DS18B20_All_Mat, DS18B20_CH6_OrderMat);
    DS18B20_All_Mat = join_rows(DS18B20_All_Mat, DS18B20_CH7_OrderMat.cols(0,3));

    qDebug()<<"电类矩阵行列："<<DS18B20_All_Mat.n_rows<<" "<<DS18B20_All_Mat.n_cols;
    qDebug()<<"All_Mat(0,0)、All_Mat(0,0)："<<DS18B20_All_Mat(0,0)<<" "<<DS18B20_All_Mat(0,1);
    qDebug()<<"CH01_Mat(0,0)、CH01_Mat(0,1)："<<DS18B20_CH1_OrderMat(0,0)<<" "<<DS18B20_CH1_OrderMat(0,1);
    return 0;
}

// CCD标准化
int Stand_CCD(QString CH1, mat &All_Mat, QStringList &XLabelName)
{
    int InputFileRow=0;
    int InputFileCol=0;
    // 传感器ID编号
    QStringList DataNameCH1;
    // 由文件名输出mat、传感器ID编号、时间
    All_Mat = JfzReadCSVToAllMat(CH1, InputFileRow, InputFileCol, DataNameCH1, XLabelName);
    qDebug()<<"CCD矩阵行列："<<All_Mat.n_rows<<" "<<All_Mat.n_cols;
    qDebug()<<"All_Mat(0,0)、All_Mat(0,0)："<<All_Mat(0,0)<<" "<<All_Mat(0,1);
    return 0;
}

// FBGS应力标准化
int Stand_FBGS(QString CH1, QString CH2, QString CH3, QString CH4, QString CH5, QString CH6, QString CH7, mat &All_Mat, QStringList &XLabelName)
{
    int InputFileRow=0;
    int InputFileCol=0;
    // 时间
    //QStringList XLabelName;
    QStringList XLabelName2;

    // 传感器ID编号
    QStringList DataName;

    // 由文件名输出mat、传感器ID编号、时间
    mat CH01_Mat = JfzReadCSVToAllMat(CH1, InputFileRow, InputFileCol, DataName, XLabelName);
    mat CH02_Mat = JfzReadCSVToAllMat(CH2, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH03_Mat = JfzReadCSVToAllMat(CH3, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH04_Mat = JfzReadCSVToAllMat(CH4, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH05_Mat = JfzReadCSVToAllMat(CH5, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH06_Mat = JfzReadCSVToAllMat(CH6, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH07_Mat = JfzReadCSVToAllMat(CH7, InputFileRow, InputFileCol, DataName, XLabelName2);

    // 矩阵合并
    All_Mat = join_rows(CH01_Mat.cols(0,14), CH02_Mat.cols(0,7));
    All_Mat = join_rows(All_Mat, CH03_Mat.cols(0,14));
    All_Mat = join_rows(All_Mat, CH04_Mat.cols(0,14));
    All_Mat = join_rows(All_Mat, CH05_Mat.cols(0,14));
    All_Mat = join_rows(All_Mat, CH06_Mat.cols(0,14));
    All_Mat = join_rows(All_Mat, CH07_Mat.cols(0,6));

    qDebug()<<"FBG应力矩阵行列："<<All_Mat.n_rows<<" "<<All_Mat.n_cols;
    qDebug()<<"All_Mat(0,0)、All_Mat(0,0)："<<All_Mat(0,0)<<" "<<All_Mat(0,1);
    qDebug()<<"CH01_Mat(0,0)、CH01_Mat(0,1)："<<CH01_Mat(0,0)<<" "<<CH01_Mat(0,1);
    return 0;
}


// FBGT温度标准化
int Stand_FBGT(QString CH08, QString CH09, QString CH10, QString CH11, QString CH12, QString CH13, QString CH14, QString CH15, QString CH16, QString CH17, QString CH18, QString CH19, QString CH20, QString CH21, QString CH22, QString CH23, QString CH24, QString CH25, QString CH26, QString CH27, QString CH28, QString CH29, QString CH30, QString CH31, QString CH32, mat &All_Mat, QStringList &XLabelName)
{
    int InputFileRow=0;
    int InputFileCol=0;
    // 时间
    //QStringList XLabelName;
    QStringList XLabelName2;

    // 传感器ID编号
    QStringList DataName;

    // 由文件名输出mat、传感器ID编号、时间
    mat CH08_Mat = JfzReadCSVToAllMat(CH08, InputFileRow, InputFileCol, DataName, XLabelName);
    mat CH09_Mat = JfzReadCSVToAllMat(CH09, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH10_Mat = JfzReadCSVToAllMat(CH10, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH11_Mat = JfzReadCSVToAllMat(CH11, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH12_Mat = JfzReadCSVToAllMat(CH12, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH13_Mat = JfzReadCSVToAllMat(CH13, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH14_Mat = JfzReadCSVToAllMat(CH14, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH15_Mat = JfzReadCSVToAllMat(CH15, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH16_Mat = JfzReadCSVToAllMat(CH16, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH17_Mat = JfzReadCSVToAllMat(CH17, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH18_Mat = JfzReadCSVToAllMat(CH18, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH19_Mat = JfzReadCSVToAllMat(CH19, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH20_Mat = JfzReadCSVToAllMat(CH20, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH21_Mat = JfzReadCSVToAllMat(CH21, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH22_Mat = JfzReadCSVToAllMat(CH22, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH23_Mat = JfzReadCSVToAllMat(CH23, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH24_Mat = JfzReadCSVToAllMat(CH24, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH25_Mat = JfzReadCSVToAllMat(CH25, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH26_Mat = JfzReadCSVToAllMat(CH26, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH27_Mat = JfzReadCSVToAllMat(CH27, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH28_Mat = JfzReadCSVToAllMat(CH28, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH29_Mat = JfzReadCSVToAllMat(CH29, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH30_Mat = JfzReadCSVToAllMat(CH30, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH31_Mat = JfzReadCSVToAllMat(CH31, InputFileRow, InputFileCol, DataName, XLabelName2);
    mat CH32_Mat = JfzReadCSVToAllMat(CH32, InputFileRow, InputFileCol, DataName, XLabelName2);

    // 矩阵合并
    All_Mat = join_rows(CH08_Mat.cols(0,14), CH09_Mat.cols(0,7));
    All_Mat = join_rows(All_Mat, CH10_Mat.cols(0,6));
    All_Mat = join_rows(All_Mat, CH11_Mat.cols(0,14));
    All_Mat = join_rows(All_Mat, CH12_Mat.cols(0,14));
    All_Mat = join_rows(All_Mat, CH13_Mat.cols(0,14));
    All_Mat = join_rows(All_Mat, CH14_Mat.cols(0,6));
    All_Mat = join_rows(All_Mat, CH15_Mat.cols(0,7));
    All_Mat = join_rows(All_Mat, CH16_Mat.cols(0,7));
    All_Mat = join_rows(All_Mat, CH17_Mat.cols(0,7));
    All_Mat = join_rows(All_Mat, CH18_Mat.cols(0,7));
    All_Mat = join_rows(All_Mat, CH19_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH20_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH21_Mat.cols(0,6));
    All_Mat = join_rows(All_Mat, CH22_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH23_Mat.cols(0,3));
    All_Mat = join_rows(All_Mat, CH24_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH25_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH26_Mat.cols(0,3));
    All_Mat = join_rows(All_Mat, CH27_Mat.cols(0,8));
    All_Mat = join_rows(All_Mat, CH28_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH29_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH30_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH31_Mat.cols(0,9));
    All_Mat = join_rows(All_Mat, CH32_Mat.cols(0,2));

    qDebug()<<"FBG应力矩阵行列："<<All_Mat.n_rows<<" "<<All_Mat.n_cols;
    qDebug()<<"All_Mat(0,0)、All_Mat(0,0)："<<All_Mat(0,0)<<" "<<All_Mat(0,1);
    qDebug()<<"CH01_Mat(0,0)、CH01_Mat(0,1)："<<CH08_Mat(0,0)<<" "<<CH08_Mat(0,1);
    return 0;
}

// 将输入mat按标定波长进行修复
void MatStandWave(mat &CH_Mat, vector<double> &standWave)
{
	for(unsigned int i=0; i<CH_Mat.n_rows;++i)
	{
		vector<double> input;
		for(unsigned int j=0; j<CH_Mat.n_cols;++j)
		{
			input.push_back(CH_Mat(i,j));
		}
		double output[20] = {0};
		WaveFix(input, standWave, output);
		for(unsigned int j=0; j<CH_Mat.n_cols;++j)
		{
			CH_Mat(i,j)=output[j];
		}
	}
}

// FBGT温度标准化-修复波长版
int Stand_FBGT_Fix(QString CH08, QString CH09, QString CH10, QString CH11, QString CH12, QString CH13, QString CH14, QString CH15, QString CH16, QString CH17, QString CH18, QString CH19, QString CH20, QString CH21, QString CH22, QString CH23, QString CH24, QString CH25, QString CH26, QString CH27, QString CH28, QString CH29, QString CH30, QString CH31, QString CH32, mat &All_Mat, QStringList &XLabelName)
{
	int InputFileRow=0;
	int InputFileCol=0;
	// 时间
	//QStringList XLabelName;
	QStringList XLabelName2;

	// 传感器ID编号
	QStringList DataName;

	// 由文件名输出mat、传感器ID编号、时间
	mat CH08_Mat = JfzReadCSVToAllMat(CH08, InputFileRow, InputFileCol, DataName, XLabelName);
	mat CH09_Mat = JfzReadCSVToAllMat(CH09, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH10_Mat = JfzReadCSVToAllMat(CH10, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH11_Mat = JfzReadCSVToAllMat(CH11, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH12_Mat = JfzReadCSVToAllMat(CH12, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH13_Mat = JfzReadCSVToAllMat(CH13, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH14_Mat = JfzReadCSVToAllMat(CH14, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH15_Mat = JfzReadCSVToAllMat(CH15, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH16_Mat = JfzReadCSVToAllMat(CH16, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH17_Mat = JfzReadCSVToAllMat(CH17, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH18_Mat = JfzReadCSVToAllMat(CH18, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH19_Mat = JfzReadCSVToAllMat(CH19, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH20_Mat = JfzReadCSVToAllMat(CH20, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH21_Mat = JfzReadCSVToAllMat(CH21, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH22_Mat = JfzReadCSVToAllMat(CH22, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH23_Mat = JfzReadCSVToAllMat(CH23, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH24_Mat = JfzReadCSVToAllMat(CH24, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH25_Mat = JfzReadCSVToAllMat(CH25, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH26_Mat = JfzReadCSVToAllMat(CH26, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH27_Mat = JfzReadCSVToAllMat(CH27, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH28_Mat = JfzReadCSVToAllMat(CH28, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH29_Mat = JfzReadCSVToAllMat(CH29, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH30_Mat = JfzReadCSVToAllMat(CH30, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH31_Mat = JfzReadCSVToAllMat(CH31, InputFileRow, InputFileCol, DataName, XLabelName2);
	mat CH32_Mat = JfzReadCSVToAllMat(CH32, InputFileRow, InputFileCol, DataName, XLabelName2);

	// 每项波长修复
	MatStandWave(CH08_Mat, standWave08);
	MatStandWave(CH09_Mat, standWave09);
	MatStandWave(CH10_Mat, standWave10);
	MatStandWave(CH11_Mat, standWave11);
	MatStandWave(CH12_Mat, standWave12);
	MatStandWave(CH13_Mat, standWave13);
	MatStandWave(CH14_Mat, standWave14);
	MatStandWave(CH15_Mat, standWave15);
	MatStandWave(CH16_Mat, standWave16);
	MatStandWave(CH17_Mat, standWave17);
	MatStandWave(CH18_Mat, standWave18);
	MatStandWave(CH19_Mat, standWave19);
	MatStandWave(CH20_Mat, standWave20);
	MatStandWave(CH21_Mat, standWave21);
	MatStandWave(CH22_Mat, standWave22);
	MatStandWave(CH23_Mat, standWave23);
	MatStandWave(CH24_Mat, standWave24);
	MatStandWave(CH25_Mat, standWave25);
	MatStandWave(CH26_Mat, standWave26);
	MatStandWave(CH27_Mat, standWave27);
	MatStandWave(CH28_Mat, standWave28);
	MatStandWave(CH29_Mat, standWave29);
	MatStandWave(CH30_Mat, standWave30);
	MatStandWave(CH31_Mat, standWave31);
	MatStandWave(CH32_Mat, standWave32);

	// 矩阵合并
	All_Mat = join_rows(CH08_Mat.cols(0,14), CH09_Mat.cols(0,7));
	All_Mat = join_rows(All_Mat, CH10_Mat.cols(0,6));
	All_Mat = join_rows(All_Mat, CH11_Mat.cols(0,14));
	All_Mat = join_rows(All_Mat, CH12_Mat.cols(0,14));
	All_Mat = join_rows(All_Mat, CH13_Mat.cols(0,14));
	All_Mat = join_rows(All_Mat, CH14_Mat.cols(0,6));
	All_Mat = join_rows(All_Mat, CH15_Mat.cols(0,7));
	All_Mat = join_rows(All_Mat, CH16_Mat.cols(0,7));
	All_Mat = join_rows(All_Mat, CH17_Mat.cols(0,7));
	All_Mat = join_rows(All_Mat, CH18_Mat.cols(0,7));
	All_Mat = join_rows(All_Mat, CH19_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH20_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH21_Mat.cols(0,6));
	All_Mat = join_rows(All_Mat, CH22_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH23_Mat.cols(0,3));
	All_Mat = join_rows(All_Mat, CH24_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH25_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH26_Mat.cols(0,3));
	All_Mat = join_rows(All_Mat, CH27_Mat.cols(0,8));
	All_Mat = join_rows(All_Mat, CH28_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH29_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH30_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH31_Mat.cols(0,9));
	All_Mat = join_rows(All_Mat, CH32_Mat.cols(0,2));

	// 某一列遇到0值用前一行值覆盖
	for(unsigned int j=0; j<All_Mat.n_cols;++j) // 先对列处理
	{
		for(unsigned int i=1; i<All_Mat.n_rows;++i) // 从第二行开始处理
		{
			if(All_Mat(i,j)<0.001 && All_Mat(i-1,j)>0.001)
			{
				// 如果某一列现在值为0，且上一行对应值不为0，上一行值给这一行
				All_Mat(i,j) = All_Mat(i-1,j);
			}
		}
	}
	// 倒序来一轮，防止前面很多行都是0，靠后面的把前面的补全了
	for(unsigned int j=0; j<All_Mat.n_cols;++j) // 先对列处理
	{
		for(int i=All_Mat.n_rows-2; i>=0;--i) // 从倒数第二行开始处理, 并且要处理0行，递减只好用有符号(这种做法不好)
		{
			if(All_Mat(i,j)<0.001 && All_Mat(i+1,j)>0.001)
			{
				All_Mat(i,j) = All_Mat(i+1,j);
			}
		}
	}
	// 来一轮前后差值的，滤除突变点
	for(unsigned int j=0; j<All_Mat.n_cols;++j) // 先对列处理
	{
		for(unsigned int i=1; i<All_Mat.n_rows;++i) // 从第二行开始处理
		{
			if(fabs(All_Mat(i,j)-All_Mat(i-1,j)) >0.01575 ) // 前后波动1.5度以上说明是突变点
			{
				All_Mat(i,j) = All_Mat(i-1,j);
			}
		}
	}

	qDebug()<<"FBG应力矩阵行列："<<All_Mat.n_rows<<" "<<All_Mat.n_cols;
	qDebug()<<"All_Mat(0,0)、All_Mat(0,0)："<<All_Mat(0,0)<<" "<<All_Mat(0,1);
	qDebug()<<"CH01_Mat(0,0)、CH01_Mat(0,1)："<<CH08_Mat(0,0)<<" "<<CH08_Mat(0,1);
	return 0;
}


