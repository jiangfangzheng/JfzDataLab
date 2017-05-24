#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Qt带的界面
//	a.setStyle(QStyleFactory::create("Fusion"));
//	QStringList keyList = QStyleFactory::keys();
//	for(int i = 0; i < keyList.length(); i++)
//	{
//		qDebug(keyList.at(i).toUtf8());
//	}

	// 设置全局字体
	qApp->setFont(QFont("Microsoft YaHei", 9));
	MainWindow w;
	w.show();

	return a.exec();
}
