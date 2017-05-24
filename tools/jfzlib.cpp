#include "tools/jfzlib.h"

// 寻找行
int JfzFileReadRow(QString FileName)
{
    int RowNum = 0;
    QFile f(FileName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return -1;
    }
    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        txtInput.readLine();
        RowNum++;
    }
    f.close();
    return RowNum;
}

// 寻找列
int JfzFileReadCol(QString FileName)
{
    int ColNum = 0;
    QFile f(FileName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return -1;
    }
    QTextStream txtInput(&f);
    QString lineStr = txtInput.readLine();
    QStringList tempbar = lineStr.split(",");
    ColNum = tempbar.size();
    f.close();
    return ColNum;
}

// 读取csv到数组
int JfzReadCSVToMat(QString FileName, double* OutMat)
{
    QFile f(FileName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         qDebug()<<"Open failed:";
        return -1;
    }
    QTextStream txtInput(&f);
    QString lineStr;
    int j = 0;;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        //qDebug()<<"lineStr:"<<lineStr;
        QStringList tempbar = lineStr.split(",");
        for (int i = 0; i < tempbar.size(); ++i)
        {
            OutMat[j*tempbar.size()+i] = tempbar[i].toDouble();
            //qDebug()<<"OutMat[j*tempbar.size()+i]:"<<OutMat[j*tempbar.size()+i];
        }
        j++;
    }
    f.close();
    return 0;
}

// 读取整个csv,提取行数,列数,数据各项id,X轴标签名(时间),  返回mat
mat JfzReadCSVToAllMat(QString FileName,int &InputFileRow,int &InputFileCol, QStringList &DataName, QStringList &XLabelName)
{
    // 需要的数据的定义
//    int InputFileRow = 0 ; // 文件行
//    int InputFileCol = 0 ; // 文件列
    QString Title; // 文件内容名字（表名字）
//    QStringList DataName; // 数据各项名
//    QStringList XLabelName; // X标签名

    // 读取文件内容
    QFile f(FileName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){}
    QTextStream txtInput(&f);
    // 读第一行,得到[标题],[数据各项名]
    QString lineStr = txtInput.readLine();
    QStringList tempbar = lineStr.split(",");
    Title = tempbar[0];
    // 读切片个数即为[列数]
    InputFileCol = tempbar.size();
    for(int i = 1; i<tempbar.size(); i++)
    {
        DataName.append(tempbar[i]);// [数据各项名]
    }

    InputFileRow = JfzFileReadRow(FileName); // [行数]
    mat Mat(InputFileRow-1,InputFileCol-1); // 存数据的矩阵，少一行一列

    int Row = 0; // 行编号
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        tempbar = lineStr.split(",");
        XLabelName.append(tempbar[0]);// 添加[X标签名]
        for(int i = 1; i<tempbar.size(); i++)
        {
            Mat(Row,i-1) = tempbar[i].toDouble();
            //qDebug()<<"tempbar[i]:"<<tempbar[i]<<" Mat:"<<Mat(Row,i-1);
            //qDebug("%.3lf", Mat(Row,i-1));
        }
        Row++;
    }
    f.close();
    return Mat;
}

int saveStandData(QString Title, QString DataName, QStringList Time, mat Mat)
{
    QString Text = Title + "," + DataName + "\r\n";
    // 写抬头
    QString SaveFileName = Time[0].left(10) + Title + ".csv";
    QFile file(SaveFileName); // 实例 QFile
    file.open(QIODevice::ReadWrite | QIODevice::Append); // 存在打开，不存在创建
    QByteArray str = Text.toUtf8();// 写入内容，这里需要转码，否则报错。
    file.write(str); // 写入内容,这里需要转码，否则报错。
    // 写数据
    Text = "";
    for(unsigned int i=0; i<Mat.n_rows; i++)
    {
        QString MatRowStr;
        for(unsigned int j=0; j<Mat.n_cols; j++)
        {
            QString value;
            value.sprintf("%.3lf", Mat(i,j));
            if(j != Mat.n_cols-1) MatRowStr = MatRowStr + value + ",";
            else MatRowStr = MatRowStr + value + "\r\n";
        }
        Text = Time[i].right(8) + "," +MatRowStr;// 时间【切出】时分秒
        str = Text.toUtf8();
        file.write(str);
    }
    file.close();
    return 0;
}

int saveStandDataNoTimeFix(QString Title, QString DataName, QString Date, QStringList Time, mat Mat)
{
    QString Text = Title + "," + DataName + "\r\n";
    // 写抬头
    QString SaveFileName = Date + Title + ".csv";
    QFile file(SaveFileName); // 实例 QFile
    file.open(QIODevice::ReadWrite | QIODevice::Append); // 存在打开，不存在创建
    QByteArray str = Text.toUtf8();// 写入内容，这里需要转码，否则报错。
    file.write(str); // 写入内容,这里需要转码，否则报错。
    // 写数据
    Text = "";
    for(unsigned int i=0; i<Mat.n_rows; i++)
    {
        QString MatRowStr;
        for(unsigned int j=0; j<Mat.n_cols; j++)
        {
            QString value;
            value.sprintf("%.3lf", Mat(i,j));
            if(j != Mat.n_cols-1) MatRowStr = MatRowStr + value + ",";
            else MatRowStr = MatRowStr + value + "\r\n";
        }
        Text = Time[i] + "," +MatRowStr;// 时间【就是】时分秒
        str = Text.toUtf8();
        file.write(str);
    }
    file.close();
    return 0;
}




// 读取环境温度xls的专用函数
bool readEnvXlsFile(QString FileName, QMap<QString,float> &map)
{
    QAxObject *excel = NULL;
    QAxObject *workbooks = NULL;
    QAxObject *workbook = NULL;
    excel = new QAxObject("Excel.Application");
    if (!excel)
    {
        qDebug() << "EXCEL对象丢失!";
        QMessageBox::critical(NULL, "注意", "读取xls文件需要您电脑安装Excel！", QMessageBox::Yes, QMessageBox::Yes);
        return false;
    }
    excel->dynamicCall("SetVisible(bool)", false);
    workbooks = excel->querySubObject("WorkBooks");
    workbook = workbooks->querySubObject("Open(QString, QVariant)", FileName);
    QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);//打开第一个sheet
    QAxObject * usedrange = worksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intRows = rows->property("Count").toInt();
    int intCols = columns->property("Count").toInt();
    qDebug() << "xls行数："<<intRows;
    qDebug() << "xls列数："<<intCols;

    //    QAxObject * range = worksheet->querySubObject("Cells(int,int)", 13, 2 );
    //    qDebug() << range->property("Value").toString();

    // 批量载入数据
    QString Range = "B13:C" +QString::number(intRows);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();

    for(int i=0; i<= intRows-13; i++)
    {
        QVariantList allEnvDataList_i =  allEnvDataList[i].toList() ;
        //qDebug()<< allEnvDataList_i[0].toString()<< allEnvDataList_i[1].toFloat();
        QString DateM = allEnvDataList_i[0].toString().left(16);// 精确到分钟就行了
        float value = allEnvDataList_i[1].toFloat();
        map.insert(DateM,value);
    }

    // 释放，但是会降低速度
    workbooks->dynamicCall("Close()");
    delete(allEnvData);
    delete(columns);
    delete(rows);
    delete(usedrange);
    delete(worksheet);
    delete(workbook);
    delete(workbooks);
    delete(excel);

    return true;
}
