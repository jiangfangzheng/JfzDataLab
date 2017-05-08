#include "EnvXlsReadThread.h"
#include <QDebug>
#include <QTime>
#include "jfzlib.h"

EnvXlsReadThread::EnvXlsReadThread()
{

}

EnvXlsReadThread::EnvXlsReadThread(QStringList EnvFileNameList, Ui::MainWindow* ui)
{
    this->EnvFileNameList = EnvFileNameList;
    this->ui = ui;
}


void EnvXlsReadThread::run()
{
   //qDebug()<<EnvFileNameList;
   QTime time;time.start(); // 计时
   // 从xls中读取文件到map
   QMap<QString,float> mapA;
   QMap<QString,float> mapB;
   QMap<QString,float> mapC;
   QMap<QString,float> mapD;

   for(int i=0; i<EnvFileNameList.size(); i++)
   {
       QString FileName = EnvFileNameList[i];
       QString ID = FileName.right(16).left(1); // 提取文件名中的ABCD以区分，例如“WHUT-A_2017-05-02.xls”
       if(ID == "A") readEnvXlsFile(FileName, mapA);
       if(ID == "B") readEnvXlsFile(FileName, mapB);
       if(ID == "C") readEnvXlsFile(FileName, mapC);
       if(ID == "D") readEnvXlsFile(FileName, mapD);
   }
   //        qDebug()<<"mapA.size："<<mapA.size();
   //        qDebug()<<"mapB.size："<<mapB.size();
   //        qDebug()<<"mapC.size："<<mapC.size();
   //        qDebug()<<"mapD.size："<<mapD.size();

   // 找出共有的【最晚开始时间】和【最早结束时间】
   QString startDate;
   QString endDate;
   QMap<QString, float>::const_iterator i;
   for (i = mapA.constBegin(); i != mapA.constEnd(); ++i)
   {
       if( i == mapA.constBegin() )  startDate = i.key();
       if( i+1 == mapA.constEnd() )  endDate   = i.key();
   }
   for (i = mapB.constBegin(); i != mapB.constEnd(); ++i)
   {
       if( i == mapB.constBegin() )  startDate = QString::compare(i.key(),startDate)>=0?i.key():startDate;
       if( i+1 == mapB.constEnd() )  endDate   = QString::compare(i.key(),endDate  )<=0?i.key():endDate;
   }
   for (i = mapC.constBegin(); i != mapC.constEnd(); ++i)
   {
       if( i == mapC.constBegin() )  startDate = QString::compare(i.key(),startDate)>=0?i.key():startDate;
       if( i+1 == mapC.constEnd() )  endDate   = QString::compare(i.key(),endDate  )<=0?i.key():endDate;
   }
   for (i = mapD.constBegin(); i != mapD.constEnd(); ++i)
   {
       if( i == mapD.constBegin() )  startDate = QString::compare(i.key(),startDate)>=0?i.key():startDate;
       if( i+1 == mapD.constEnd() )  endDate   = QString::compare(i.key(),endDate  )<=0?i.key():endDate;
   }
   qDebug() <<"公共开始时间:" <<startDate << " 公共结束时间:" << endDate;

   // 制作整合的数据（Key为时间，value为,分隔4个值）例如："2017-04-25 13:23"、"19.6,19.4,19.3,19.5"
   QMap<QString,QString> mapAll;
   for (i = mapA.constBegin(); i != mapA.constEnd(); ++i)
   {
       if( QString::compare(i.key(),startDate)>=0 && QString::compare(i.key(),endDate)<=0)
       {
           QString value = QString::number(i.value());
           mapAll.insert(i.key(), value);
       }
   }
   for (i = mapB.constBegin(); i != mapB.constEnd(); ++i)
   {
       if( QString::compare(i.key(),startDate)>=0 && QString::compare(i.key(),endDate)<=0)
       {
           QString value = "," + QString::number(i.value());
           mapAll.insert(i.key(), mapAll[i.key()] + value);
       }
   }
   for (i = mapC.constBegin(); i != mapC.constEnd(); ++i)
   {
       if( QString::compare(i.key(),startDate)>=0 && QString::compare(i.key(),endDate)<=0)
       {
           QString value = "," + QString::number(i.value());
           mapAll.insert(i.key(), mapAll[i.key()] + value);
       }
   }
   for (i = mapD.constBegin(); i != mapD.constEnd(); ++i)
   {
       if( QString::compare(i.key(),startDate)>=0 && QString::compare(i.key(),endDate)<=0)
       {
           QString value = "," + QString::number(i.value());
           mapAll.insert(i.key(), mapAll[i.key()] + value);
       }
   }

   // 保存文件
   QString Text = "EnvTemperature,Back,Right,Left,Front\r\n";
   QString SaveFileName = startDate.left(10) +"~" + endDate.left(10) +"-Data-EnvTemperature.csv";
   //QString SaveFileName = "Data-EnvTemperature.csv";
   QFile file(SaveFileName); // 实例 QFile
   file.open(QIODevice::ReadWrite | QIODevice::Append); // 存在打开，不存在创建
   // 写抬头
   QByteArray str = Text.toUtf8();// 写入内容，这里需要转码，否则报错。
   file.write(str); // 写入内容,这里需要转码，否则报错。
   // 写数据
   Text = "";
   QMap<QString, QString>::const_iterator ii;
   for (ii = mapAll.constBegin(); ii != mapAll.constEnd(); ++ii)
   {
       Text = ii.key() + ":00," +ii.value() + "\r\n";// 时间还原到秒
       str = Text.toUtf8();
       file.write(str);
       //qDebug()<< "Key:"<< ii.key() << " Value:" << ii.value();
   }
   file.close();

   // 计算花费时间
   QString timecost = QString::number(time.elapsed()/1000.0);
   qDebug()<<"读取xls时间："<<timecost<<"s";
   ui->label_3->setText("转换完成！读取xls文件总共时间：<span style='color: rgb(255, 0, 0);'>" + timecost + "秒</span>");
   ui->pushButton_ENV->setEnabled(true);
}
