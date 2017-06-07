#include "algorithm/datatostand.h"

/* 定死的数据 */
// FBG标定时所用波长
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
// FBG标定波长+标定温度
double WaveAndTemp[231][2]={{9.3,1535.91},{9.3,1537.892},{9.1,1539.88},{9.2,1541.928},{9.1,1543.865},{9,1545.865},{8.9,1547.871},{8.9,1549.925},{8.4,1552.03},{8.5,1554.058},{8.7,1555.991},{8.7,1557.835},{8.6,1559.858},{8.5,1561.989},{8.5,1563.91},{8.4,1549.888},{8.4,1551.923},{8.6,1554.003},{8.7,1556.046},{8.6,1558.072},{8.6,1559.968},{8.5,1561.973},{8.6,1563.921},{8.5,1551.908},{8.6,1553.899},{8.6,1555.256},{8.7,1557.897},{8.6,1559.862},{8.6,1561.929},{8.7,1563.863},{9.6,1535.919},{9.7,1537.952},{9.6,1539.937},{9.7,1541.986},{9.6,1543.934},{9.5,1545.927},{9.4,1547.917},{9.4,1549.915},{8.6,1551.867},{8.8,1553.893},{8.7,1555.976},{8.5,1557.97},{8.4,1559.93},{8.4,1561.868},{8.4,1563.824},{9.7,1535.883},{9.6,1537.887},{9.6,1539.876},{9.6,1541.944},{9.8,1543.916},{9.4,1545.887},{9.4,1547.862},{9.3,1549.875},{8.6,1551.882},{8.6,1553.864},{8.6,1555.933},{8.4,1557.929},{8.4,1559.979},{8.4,1562.067},{8.5,1564.086},{9.5,1535.887},{9.5,1537.895},{9.5,1539.878},{9.4,1541.909},{9.4,1543.891},{9.3,1545.846},{9.3,1547.85},{9.2,1549.849},{8.5,1551.913},{8.4,1553.89},{8.5,1555.283},{8.5,1557.986},{8.4,1559.922},{8.3,1561.974},{8.4,1563.944},{9.4,1535.917},{9.4,1537.936},{9.2,1539.924},{9.2,1541.964},{9.2,1543.869},{9.1,1545.864},{9.1,1547.86},{9.3,1535.953},{9.3,1538.018},{9.2,1539.972},{9.3,1542.003},{9.3,1544.001},{9.3,1545.952},{9.2,1547.968},{9.1,1550.004},{8.9,1537.916},{9.1,1540.989},{9,1543.933},{8.9,1546.884},{8.8,1549.983},{8.7,1552.842},{8.6,1556.886},{8.7,1559.866},{8.8,1537.891},{8.7,1540.868},{8.8,1543.919},{8.6,1546.874},{8.7,1549.865},{8.7,1552.852},{8.7,1556.838},{8.7,1559.845},{8.1,1537.88},{8.1,1540.867},{8.4,1543.895},{8.5,1546.834},{8.6,1550.129},{8.6,1552.898},{8.7,1556.887},{8.6,1559.87},{8.6,1536.88},{8.8,1539.851},{8.8,1542.783},{8.7,1545.85},{8.7,1548.81},{8.6,1551.925},{8.9,1554.8},{8.8,1557.806},{8.7,1560.847},{8.7,1563.788},{9.4,1537.927},{9.1,1540.9},{9.3,1543.824},{9.4,1546.868},{9.2,1549.795},{9.4,1552.848},{9.2,1555.959},{9.2,1558.893},{9.1,1561.833},{9.2,1565.857},{9.8,1533.28},{9.7,1536.782},{9.7,1539.724},{9.4,1542.544},{9.5,1545.746},{9.3,1548.583},{9.4,1551.862},{8.8,1533.231},{8.9,1536.3},{8.8,1539.579},{8.9,1542.234},{9.2,1545.224},{9.2,1548.25},{9.1,1551.529},{9.1,1554.316},{9.5,1557.424},{9.6,1560.507},{9.2,1532.419},{8.8,1535.423},{8.8,1538.44},{8.8,1541.37},{9.5,1532.338},{9.5,1535.593},{9.4,1538.606},{9.2,1541.579},{9.4,1544.62},{9.4,1547.436},{9.5,1550.48},{9.3,1553.68},{9.6,1556.401},{9.5,1559.5},{9.7,1533.465},{9.5,1536.576},{9.4,1538.311},{9.3,1542.081},{9.5,1544.913},{9.2,1548.159},{9.3,1550.868},{9.3,1554.26},{9.4,1556.928},{9.3,1560.047},{9.5,1547.537},{9.4,1553.547},{9.5,1556.568},{9.4,1559.39},{8,1537.936},{7.9,1540.857},{8.1,1543.784},{8.2,1546.859},{8.3,1549.785},{8.2,1552.862},{8.1,1555.947},{8.1,1558.827},{8,1561.923},{8.3,1536.868},{8.5,1539.832},{8.7,1542.826},{8.7,1545.86},{8.8,1548.829},{8.8,1551.972},{8.9,1554.835},{8.9,1557.869},{8.9,1560.815},{8.9,1563.835},{7.9,1537.944},{8,1540.885},{7.9,1543.825},{8,1546.883},{8,1549.931},{8,1552.827},{8,1555.965},{8.1,1558.862},{8,1561.873},{8,1565.898},{8.2,1536.901},{8.3,1539.926},{8.4,1542.797},{8.6,1545.828},{8.7,1548.942},{8.7,1551.957},{8.7,1554.805},{8.5,1557.825},{8.4,1560.848},{8.4,1563.82},{8,1537.88},{8.1,1540.9},{8.2,1543.795},{8.2,1546.836},{8.2,1549.805},{8.3,1552.842},{8.4,1555.919},{8.5,1558.828},{8.6,1561.864},{8.5,1565.845},{8.876,1555.018}, {8.576,1558.218}, {8.676,1561.126}};
// 各数据项名
QString  DataName_DS18B20= "CH01-01,CH01-02,CH01-03,CH01-04,CH01-05,CH01-06,CH01-07,CH01-08,CH02-01,CH02-02,CH02-03,CH02-04,CH02-05,CH02-06,CH02-07,CH02-08,CH03-01,CH03-02,CH03-03,CH03-04,CH03-05,CH03-06,CH03-07,CH03-08,CH04-01,CH04-02,CH04-03,CH04-04,CH04-05,CH04-06,CH04-07,CH04-08,CH05-01,CH05-02,CH05-03,CH05-04,CH05-05,CH05-06,CH05-07,CH05-08,CH06-01,CH06-02,CH06-03,CH06-04,CH06-05,CH06-06,CH06-07,CH06-08,CH07-01,CH07-02,CH07-03,CH07-04";
QString  DataName_FBGT= "CH08-01,CH08-02,CH08-03,CH08-04,CH08-05,CH08-06,CH08-07,CH08-08,CH08-09,CH08-10,CH08-11,CH08-12,CH08-13,CH08-14,CH08-15,CH09-01,CH09-02,CH09-03,CH09-04,CH09-05,CH09-06,CH09-07,CH09-08,CH10-01,CH10-02,CH10-03,CH10-04,CH10-05,CH10-06,CH10-07,CH11-01,CH11-02,CH11-03,CH11-04,CH11-05,CH11-06,CH11-07,CH11-08,CH11-09,CH11-10,CH11-11,CH11-12,CH11-13,CH11-14,CH11-15,CH12-01,CH12-02,CH12-03,CH12-04,CH12-05,CH12-06,CH12-07,CH12-08,CH12-09,CH12-10,CH12-11,CH12-12,CH12-13,CH12-14,CH12-15,CH13-01,CH13-02,CH13-03,CH13-04,CH13-05,CH13-06,CH13-07,CH13-08,CH13-09,CH13-10,CH13-11,CH13-12,CH13-13,CH13-14,CH13-15,CH14-01,CH14-02,CH14-03,CH14-04,CH14-05,CH14-06,CH14-07,CH15-01,CH15-02,CH15-03,CH15-04,CH15-05,CH15-06,CH15-07,CH15-08,CH16-01,CH16-02,CH16-03,CH16-04,CH16-05,CH16-06,CH16-07,CH16-08,CH17-01,CH17-02,CH17-03,CH17-04,CH17-05,CH17-06,CH17-07,CH17-08,CH18-01,CH18-02,CH18-03,CH18-04,CH18-05,CH18-06,CH18-07,CH18-08,CH19-01,CH19-02,CH19-03,CH19-04,CH19-05,CH19-06,CH19-07,CH19-08,CH19-09,CH19-10,CH20-01,CH20-02,CH20-03,CH20-04,CH20-05,CH20-06,CH20-07,CH20-08,CH20-09,CH20-10,CH21-01,CH21-02,CH21-03,CH21-04,CH21-05,CH21-06,CH21-07,CH22-01,CH22-02,CH22-03,CH22-04,CH22-05,CH22-06,CH22-07,CH22-08,CH22-09,CH22-10,CH23-01,CH23-02,CH23-03,CH23-04,CH24-01,CH24-02,CH24-03,CH24-04,CH24-05,CH24-06,CH24-07,CH24-08,CH24-09,CH24-10,CH25-01,CH25-02,CH25-03,CH25-04,CH25-05,CH25-06,CH25-07,CH25-08,CH25-09,CH25-10,CH26-01,CH26-02,CH26-03,CH26-04,CH27-01,CH27-02,CH27-03,CH27-04,CH27-05,CH27-06,CH27-07,CH27-08,CH27-09,CH28-01,CH28-02,CH28-03,CH28-04,CH28-05,CH28-06,CH28-07,CH28-08,CH28-09,CH28-10,CH29-01,CH29-02,CH29-03,CH29-04,CH29-05,CH29-06,CH29-07,CH29-08,CH29-09,CH29-10,CH30-01,CH30-02,CH30-03,CH30-04,CH30-05,CH30-06,CH30-07,CH30-08,CH30-09,CH30-10,CH31-01,CH31-02,CH31-03,CH31-04,CH31-05,CH31-06,CH31-07,CH31-08,CH31-09,CH31-10,CH32-01,CH32-02,CH32-03";
QString  DataName_FBGS= "CH01-01,CH01-02,CH01-03,CH01-04,CH01-05,CH01-06,CH01-07,CH01-08,CH01-09,CH01-10,CH01-11,CH01-12,CH01-13,CH01-14,CH01-15,CH02-01,CH02-02,CH02-03,CH02-04,CH02-05,CH02-06,CH02-07,CH02-08,CH03-01,CH03-02,CH03-03,CH03-04,CH03-05,CH03-06,CH03-07,CH03-08,CH03-09,CH03-10,CH03-11,CH03-12,CH03-13,CH03-14,CH03-15,CH04-01,CH04-02,CH04-03,CH04-04,CH04-05,CH04-06,CH04-07,CH04-08,CH04-09,CH04-10,CH04-11,CH04-12,CH04-13,CH04-14,CH04-15,CH05-01,CH05-02,CH05-03,CH05-04,CH05-05,CH05-06,CH05-07,CH05-08,CH05-09,CH05-10,CH05-11,CH05-12,CH05-13,CH05-14,CH05-15,CH06-01,CH06-02,CH06-03,CH06-04,CH06-05,CH06-06,CH06-07,CH06-08,CH06-09,CH06-10,CH06-11,CH06-12,CH06-13,CH06-14,CH06-15,CH07-01,CH07-02,CH07-03,CH07-04,CH07-05,CH07-06,CH07-07";
QString  DataName_CCD= "X,Y,Z";

// 获取各项数据该读取的csv文件名
QStringList get_DS18B20_csv(const QString &dir)
{
	QString CH1 = dir + "/通道1.csv";
	QString CH2 = dir + "/通道2.csv";
	QString CH3 = dir + "/通道3.csv";
	QString CH4 = dir + "/通道4.csv";
	QString CH5 = dir + "/通道5.csv";
	QString CH6 = dir + "/通道6.csv";
	QString CH7 = dir + "/通道7.csv";
	QStringList CH{CH1,CH2,CH3,CH4,CH5,CH6,CH7};
	return CH;
}

QStringList get_CCD_csv(const QString &dir)
{
	QString CH1 = dir + "/CCD数据值.csv";
	QStringList CH{CH1};
	return CH;
}

QStringList get_FBGT_csv(const QString &dir)
{
	QString CH08 = dir + "/通道8.csv";
	QString CH09 = dir + "/通道9.csv";
	QString CH10 = dir + "/通道10.csv";
	QString CH11 = dir + "/通道11.csv";
	QString CH12 = dir + "/通道12.csv";
	QString CH13 = dir + "/通道13.csv";
	QString CH14 = dir + "/通道14.csv";
	QString CH15 = dir + "/通道15.csv";
	QString CH16 = dir + "/通道16.csv";
	QString CH17 = dir + "/通道17.csv";
	QString CH18 = dir + "/通道18.csv";
	QString CH19 = dir + "/通道19.csv";
	QString CH20 = dir + "/通道20.csv";
	QString CH21 = dir + "/通道21.csv";
	QString CH22 = dir + "/通道22.csv";
	QString CH23 = dir + "/通道23.csv";
	QString CH24 = dir + "/通道24.csv";
	QString CH25 = dir + "/通道25.csv";
	QString CH26 = dir + "/通道26.csv";
	QString CH27 = dir + "/通道27.csv";
	QString CH28 = dir + "/通道28.csv";
	QString CH29 = dir + "/通道29.csv";
	QString CH30 = dir + "/通道30.csv";
	QString CH31 = dir + "/通道31.csv";
	QString CH32 = dir + "/通道32.csv";
	QStringList CH{CH08,CH09,CH10,CH11,CH12,CH13,CH14,CH15,CH16,CH17,CH18,CH19,CH20,CH21,CH22,CH23,CH24,CH25,CH26,CH27,CH28,CH29,CH30,CH31,CH32};
	return CH;
}

QStringList get_FBGS_csv(const QString &dir)
{
	QString CH01 = dir + "/通道1.csv";
	QString CH02 = dir + "/通道2.csv";
	QString CH03 = dir + "/通道3.csv";
	QString CH04 = dir + "/通道4.csv";
	QString CH05 = dir + "/通道5.csv";
	QString CH06 = dir + "/通道6.csv";
	QString CH07 = dir + "/通道7.csv";
	QStringList CH{CH01,CH02,CH03,CH04,CH05,CH06,CH07};
	return CH;
}

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
int Stand_DS18B20(QStringList &CH, mat &DS18B20_All_Mat, QStringList &XLabelName)
{
	QString CH1=CH[0];
	QString CH2=CH[1];
	QString CH3=CH[2];
	QString CH4=CH[3];
	QString CH5=CH[4];
	QString CH6=CH[5];
	QString CH7=CH[6];

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

mat Stand_DS18B20(QStringList &CH)
{
	mat DS18B20_All_Mat;
	QString CH1=CH[0];
	QString CH2=CH[1];
	QString CH3=CH[2];
	QString CH4=CH[3];
	QString CH5=CH[4];
	QString CH6=CH[5];
	QString CH7=CH[6];
	QString CH8=CH[7];

	int InputFileRow=0;
	int InputFileCol=0;
	// 时间
	QStringList XLabelName;
	QStringList XLabelName2;

	// 传感器ID编号
	QStringList DataNameCH1;
	QStringList DataNameCH2;
	QStringList DataNameCH3;
	QStringList DataNameCH4;
	QStringList DataNameCH5;
	QStringList DataNameCH6;
	QStringList DataNameCH7;
	QStringList DataNameCH8;

	// 由文件名输出mat、传感器ID编号、时间
	mat DS18B20_CH1_Mat = JfzReadCSVToAllMat(CH1, InputFileRow, InputFileCol, DataNameCH1, XLabelName);
	mat DS18B20_CH2_Mat = JfzReadCSVToAllMat(CH2, InputFileRow, InputFileCol, DataNameCH2, XLabelName2);
	mat DS18B20_CH3_Mat = JfzReadCSVToAllMat(CH3, InputFileRow, InputFileCol, DataNameCH3, XLabelName2);
	mat DS18B20_CH4_Mat = JfzReadCSVToAllMat(CH4, InputFileRow, InputFileCol, DataNameCH4, XLabelName2);
	mat DS18B20_CH5_Mat = JfzReadCSVToAllMat(CH5, InputFileRow, InputFileCol, DataNameCH5, XLabelName2);
	mat DS18B20_CH6_Mat = JfzReadCSVToAllMat(CH6, InputFileRow, InputFileCol, DataNameCH6, XLabelName2);
	mat DS18B20_CH7_Mat = JfzReadCSVToAllMat(CH7, InputFileRow, InputFileCol, DataNameCH7, XLabelName2);
	mat DS18B20_CH8_Mat = JfzReadCSVToAllMat(CH8, InputFileRow, InputFileCol, DataNameCH8, XLabelName2);

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
	mat DS18B20_CH8_OrderMat(DS18B20_CH8_Mat.n_rows, DS18B20_CH8_Mat.n_cols);

	// 0矩阵
	mat DS18B20_0(DS18B20_CH1_Mat.n_rows,1);
	DS18B20_0.fill(0);
	// 原始矩阵曾一列处理错情况
	DS18B20_CH1_Mat = join_rows(DS18B20_CH1_Mat, DS18B20_0);
	DS18B20_CH2_Mat = join_rows(DS18B20_CH2_Mat, DS18B20_0);
	DS18B20_CH3_Mat = join_rows(DS18B20_CH3_Mat, DS18B20_0);
	DS18B20_CH4_Mat = join_rows(DS18B20_CH4_Mat, DS18B20_0);
	DS18B20_CH5_Mat = join_rows(DS18B20_CH5_Mat, DS18B20_0);
	DS18B20_CH6_Mat = join_rows(DS18B20_CH6_Mat, DS18B20_0);
	DS18B20_CH7_Mat = join_rows(DS18B20_CH7_Mat, DS18B20_0);
	DS18B20_CH8_Mat = join_rows(DS18B20_CH8_Mat, DS18B20_0);

	// 由ID来排序，建立索引规则
	int CH10 = DataNameCH1.indexOf("ID:28ffcdfa001604cb");if(CH10 == -1) CH10=8;
	int CH11 = DataNameCH1.indexOf("ID:28ff7eeda0160301");if(CH11 == -1) CH11=8;
	int CH12 = DataNameCH1.indexOf("ID:28ffa203011604fc");if(CH12 == -1) CH12=8;
	int CH13 = DataNameCH1.indexOf("ID:28ffd7cc0016044b");if(CH13 == -1) CH13=8;
	int CH14 = DataNameCH1.indexOf("ID:28ff6ba7a0160589");if(CH14 == -1) CH14=8;
	int CH15 = DataNameCH1.indexOf("ID:28ff28f8041603d9");if(CH15 == -1) CH15=8;
	int CH16 = DataNameCH1.indexOf("ID:28ff280c01160410");if(CH16 == -1) CH16=8;
	int CH17 = DataNameCH1.indexOf("ID:28ff4a2305160375");if(CH17 == -1) CH17=8;

	int CH20 = DataNameCH2.indexOf("ID:28ffaf1fb2150108");if(CH20 == -1) CH20=8;
	int CH21 = DataNameCH2.indexOf("ID:28ff921db2150167");if(CH21 == -1) CH21=8;
	int CH22 = DataNameCH2.indexOf("ID:28ff86eb041603f8");if(CH22 == -1) CH22=8;
	int CH23 = DataNameCH2.indexOf("ID:28ffd3dd041603fe");if(CH23 == -1) CH23=8;
	int CH24 = DataNameCH2.indexOf("ID:28ff71c100160479");if(CH24 == -1) CH24=8;
	int CH25 = DataNameCH2.indexOf("ID:28ffb81cb2150311");if(CH25 == -1) CH25=8;
	int CH26 = DataNameCH2.indexOf("ID:28ff6a0c0116047a");if(CH26 == -1) CH26=8;
	int CH27 = DataNameCH2.indexOf("ID:28ffa2dd00160443");if(CH27 == -1) CH27=8;

	int CH30 = DataNameCH3.indexOf("ID:28ff1d93a01603e3");if(CH30 == -1) CH30=8;
	int CH31 = DataNameCH3.indexOf("ID:28ff78bea01603cd");if(CH31 == -1) CH31=8;
	int CH32 = DataNameCH3.indexOf("ID:28ff4ec300160495");if(CH32 == -1) CH32=8;
	int CH33 = DataNameCH3.indexOf("ID:28ffaddc001604a3");if(CH33 == -1) CH33=8;
	int CH34 = DataNameCH3.indexOf("ID:28ffb6e500160474");if(CH34 == -1) CH34=8;
	int CH35 = DataNameCH3.indexOf("ID:28ffa3f7001604e5");if(CH35 == -1) CH35=8;
	int CH36 = DataNameCH3.indexOf("ID:28ff30f70016041c");if(CH36 == -1) CH36=8;
	int CH37 = DataNameCH3.indexOf("ID:28ff95f7001604fd");if(CH37 == -1) CH37=8;

	int CH40 = DataNameCH4.indexOf("ID:28ffe6ce00160405");if(CH40 == -1) CH40=8;
	int CH41 = DataNameCH4.indexOf("ID:28ff783db215044f");if(CH41 == -1) CH41=8;
	int CH42 = DataNameCH4.indexOf("ID:28ff52c600160449");if(CH42 == -1) CH42=8;
	int CH43 = DataNameCH4.indexOf("ID:28ffed30b2150334");if(CH43 == -1) CH43=8;
	int CH44 = DataNameCH4.indexOf("ID:28ffdcc20016042e");if(CH44 == -1) CH44=8;
	int CH45 = DataNameCH4.indexOf("ID:28ffd827b215045f");if(CH45 == -1) CH45=8;
	int CH46 = DataNameCH4.indexOf("ID:28ff3425051603b3");if(CH46 == -1) CH46=8;
	int CH47 = DataNameCH4.indexOf("ID:28ff247eb2150310");if(CH47 == -1) CH47=8;

	int CH50 = DataNameCH5.indexOf("ID:28ff76e500160456");if(CH50 == -1) CH50=8;
	int CH51 = DataNameCH5.indexOf("ID:28ffa5c30016043f");if(CH51 == -1) CH51=8;
	int CH52 = DataNameCH5.indexOf("ID:28ff2d22b2150195");if(CH52 == -1) CH52=8;
	int CH53 = DataNameCH5.indexOf("ID:28ffcd0d01160497");if(CH53 == -1) CH53=8;
	int CH54 = DataNameCH5.indexOf("ID:28ff55a7a01605af");if(CH54 == -1) CH54=8;
	int CH55 = DataNameCH5.indexOf("ID:28ff56d7001604e1");if(CH55 == -1) CH55=8;
	int CH56 = DataNameCH5.indexOf("ID:28ff3ccba0160491");if(CH56 == -1) CH56=8;
	int CH57 = DataNameCH5.indexOf("ID:28ff0091a01604f7");if(CH57 == -1) CH57=8;

	int CH60 = DataNameCH6.indexOf("ID:28ff80efa01605df");if(CH60 == -1) CH60=8;
	int CH61 = DataNameCH6.indexOf("ID:28ffd7c6a0160325");if(CH61 == -1) CH61=8;
	int CH62 = DataNameCH6.indexOf("ID:28ff04ada01605ec");if(CH62 == -1) CH62=8;
	int CH63 = DataNameCH6.indexOf("ID:28fffdd4a0160582");if(CH63 == -1) CH63=8;
	int CH64 = DataNameCH6.indexOf("ID:28ff87caa0160421");if(CH64 == -1) CH64=8;
	int CH65 = DataNameCH6.indexOf("ID:28ff1d9da0160475");if(CH65 == -1) CH65=8;
	int CH66 = DataNameCH6.indexOf("ID:28fffdada01605b9");if(CH66 == -1) CH66=8;
	int CH67 = DataNameCH6.indexOf("ID:28ff44d3a01605b8");if(CH67 == -1) CH67=8;

	int CH70 = DataNameCH7.indexOf("ID:28ff538da0160406");if(CH70 == -1) CH70=8;
	int CH71 = DataNameCH7.indexOf("ID:28ff387da0160416");if(CH71 == -1) CH71=8;
	int CH72 = DataNameCH7.indexOf("ID:28ffcacfa01605fb");if(CH72 == -1) CH72=8;
	int CH73 = DataNameCH7.indexOf("ID:28ff16a8a0160592");if(CH73 == -1) CH73=8;


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
	DS18B20_All_Mat = join_rows(DS18B20_All_Mat, DS18B20_CH7_OrderMat);
	DS18B20_All_Mat = join_rows(DS18B20_All_Mat, DS18B20_CH8_OrderMat);

	qDebug()<<"电类矩阵行列："<<DS18B20_All_Mat.n_rows<<" "<<DS18B20_All_Mat.n_cols;
//	qDebug()<<"All_Mat(0,0)、All_Mat(0,0)："<<DS18B20_All_Mat(0,0)<<" "<<DS18B20_All_Mat(0,1);
//	qDebug()<<"CH01_Mat(0,0)、CH01_Mat(0,1)："<<DS18B20_CH1_OrderMat(0,0)<<" "<<DS18B20_CH1_OrderMat(0,1);
	return DS18B20_All_Mat;
}

// CCD标准化
int Stand_CCD(QStringList &CH, mat &All_Mat, QStringList &XLabelName)
{
	QString CH1 = CH[0];
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
int Stand_FBGS(QStringList &CH, mat &All_Mat, QStringList &XLabelName)
{
	QString CH1 = CH[0];
	QString CH2 = CH[1];
	QString CH3 = CH[2];
	QString CH4 = CH[3];
	QString CH5 = CH[4];
	QString CH6 = CH[5];
	QString CH7 = CH[6];

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
int Stand_FBGT(QStringList &CH, mat &All_Mat, QStringList &XLabelName)
{
	QString CH08 = CH[0];
	QString CH09 = CH[1];
	QString CH10 = CH[2];
	QString CH11 = CH[3];
	QString CH12 = CH[4];
	QString CH13 = CH[5];
	QString CH14 = CH[6];
	QString CH15 = CH[7];
	QString CH16 = CH[8];
	QString CH17 = CH[9];
	QString CH18 = CH[10];
	QString CH19 = CH[11];
	QString CH20 = CH[12];
	QString CH21 = CH[13];
	QString CH22 = CH[14];
	QString CH23 = CH[15];
	QString CH24 = CH[16];
	QString CH25 = CH[17];
	QString CH26 = CH[18];
	QString CH27 = CH[19];
	QString CH28 = CH[20];
	QString CH29 = CH[21];
	QString CH30 = CH[22];
	QString CH31 = CH[23];
	QString CH32 = CH[24];

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
int Stand_FBGT_Fix(QStringList &CH, mat &All_Mat, QStringList &XLabelName)
{
	QString CH08 = CH[0];
	QString CH09 = CH[1];
	QString CH10 = CH[2];
	QString CH11 = CH[3];
	QString CH12 = CH[4];
	QString CH13 = CH[5];
	QString CH14 = CH[6];
	QString CH15 = CH[7];
	QString CH16 = CH[8];
	QString CH17 = CH[9];
	QString CH18 = CH[10];
	QString CH19 = CH[11];
	QString CH20 = CH[12];
	QString CH21 = CH[13];
	QString CH22 = CH[14];
	QString CH23 = CH[15];
	QString CH24 = CH[16];
	QString CH25 = CH[17];
	QString CH26 = CH[18];
	QString CH27 = CH[19];
	QString CH28 = CH[20];
	QString CH29 = CH[21];
	QString CH30 = CH[22];
	QString CH31 = CH[23];
	QString CH32 = CH[24];

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

// 波长转温度算法-T=k(wave-Bwave)+Btemp
mat FBGtoTEMP(QString &standFBGFileName, QStringList &XLabelName)
{
	int InputFileRow;
	int InputFileCol;
	QStringList DataName;
	mat FBGWave = JfzReadCSVToAllMat(standFBGFileName, InputFileRow, InputFileCol, DataName,  XLabelName);
	// 波长转温度
	for(unsigned int i=0; i< FBGWave.n_rows; i++)
	{
		for(unsigned int j=0; j<FBGWave.n_cols; j++)
		{
			FBGWave(i,j) = (FBGWave(i,j)-WaveAndTemp[j][1])/0.0105+WaveAndTemp[j][0];
		}
	}
	return FBGWave;
}


// FBG汇总为一个mat
mat Stand_FBG(QStringList &CH)
{
	QString CH1  = CH[ 0];
	QString CH2  = CH[ 1];
	QString CH3  = CH[ 2];
	QString CH4  = CH[ 3];
	QString CH5  = CH[ 4];
	QString CH6  = CH[ 5];
	QString CH7  = CH[ 6];
	QString CH08 = CH[ 7];
	QString CH09 = CH[ 8];
	QString CH10 = CH[ 9];
	QString CH11 = CH[10];
	QString CH12 = CH[11];
	QString CH13 = CH[12];
	QString CH14 = CH[13];
	QString CH15 = CH[14];
	QString CH16 = CH[15];
	QString CH17 = CH[16];
	QString CH18 = CH[17];
	QString CH19 = CH[18];
	QString CH20 = CH[19];
	QString CH21 = CH[20];
	QString CH22 = CH[21];
	QString CH23 = CH[22];
	QString CH24 = CH[23];
	QString CH25 = CH[24];
	QString CH26 = CH[25];
	QString CH27 = CH[26];
	QString CH28 = CH[27];
	QString CH29 = CH[28];
	QString CH30 = CH[29];
	QString CH31 = CH[30];
	QString CH32 = CH[31];

	int InputFileRow=0;
	int InputFileCol=0;
	// 时间
	QStringList XLabelName;
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
	mat All_Mat;
	All_Mat	= join_rows(CH01_Mat, CH02_Mat);
	All_Mat	= join_rows(All_Mat, CH03_Mat);
	All_Mat	= join_rows(All_Mat, CH04_Mat);
	All_Mat	= join_rows(All_Mat, CH05_Mat);
	All_Mat	= join_rows(All_Mat, CH06_Mat);
	All_Mat	= join_rows(All_Mat, CH07_Mat);
	All_Mat	= join_rows(All_Mat, CH08_Mat);
	All_Mat	= join_rows(All_Mat, CH09_Mat);
	All_Mat	= join_rows(All_Mat, CH10_Mat);
	All_Mat	= join_rows(All_Mat, CH11_Mat);
	All_Mat	= join_rows(All_Mat, CH12_Mat);
	All_Mat	= join_rows(All_Mat, CH13_Mat);
	All_Mat	= join_rows(All_Mat, CH14_Mat);
	All_Mat	= join_rows(All_Mat, CH15_Mat);
	All_Mat	= join_rows(All_Mat, CH16_Mat);
	All_Mat	= join_rows(All_Mat, CH17_Mat);
	All_Mat	= join_rows(All_Mat, CH18_Mat);
	All_Mat	= join_rows(All_Mat, CH19_Mat);
	All_Mat	= join_rows(All_Mat, CH20_Mat);
	All_Mat	= join_rows(All_Mat, CH21_Mat);
	All_Mat	= join_rows(All_Mat, CH22_Mat);
	All_Mat	= join_rows(All_Mat, CH23_Mat);
	All_Mat	= join_rows(All_Mat, CH24_Mat);
	All_Mat	= join_rows(All_Mat, CH25_Mat);
	All_Mat	= join_rows(All_Mat, CH26_Mat);
	All_Mat	= join_rows(All_Mat, CH27_Mat);
	All_Mat	= join_rows(All_Mat, CH28_Mat);
	All_Mat	= join_rows(All_Mat, CH29_Mat);
	All_Mat	= join_rows(All_Mat, CH30_Mat);
	All_Mat	= join_rows(All_Mat, CH31_Mat);
	All_Mat	= join_rows(All_Mat, CH32_Mat);

	qDebug()<<"FBG矩阵行列："<<All_Mat.n_rows<<" "<<All_Mat.n_cols;
//	qDebug()<<"All_Mat(0,0)、All_Mat(0,0)："<<All_Mat(0,0)<<" "<<All_Mat(0,1);
//	qDebug()<<"CH01_Mat(0,0)、CH01_Mat(0,1)："<<CH01_Mat(0,0)<<" "<<CH01_Mat(0,1);
	return All_Mat;
}
