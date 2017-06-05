#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
	// 高DPI适应
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QApplication a(argc, argv);

	// Qt带的界面
	//	a.setStyle(QStyleFactory::create("Fusion"));
	//	QStringList keyList = QStyleFactory::keys();
	//	for(int i = 0; i < keyList.length(); i++)
	//	{
	//		qDebug(keyList.at(i).toUtf8());
	//	}

	// 设置全局字体
#ifdef Q_OS_WIN
	qApp->setFont(QFont("Microsoft YaHei", 9));
#endif

	MainWindow w;
	w.show();

	return a.exec();
}
