#ifndef DATAPROCESSINGTHREAD_H
#define DATAPROCESSINGTHREAD_H

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

#include <QThread>
#include <functional>

class DataProcessingThread : public QThread
{
	Q_OBJECT
public:
	DataProcessingThread();
	~DataProcessingThread();
	DataProcessingThread(QString fileName1, std::function<bool (QString)> fun);
	DataProcessingThread(QString fileName1, QString fileName2, bool (*fun)(QString, QString));
	DataProcessingThread(QStringList fileNameList, bool (*fun)(QStringList));
	void run();
private:
	int nameCnt;					// 处理文件参数的个数(支持1~2)
	QString fileName1;				// 处理文件名
	QString fileName2;				// 处理文件名
	QStringList fileNameList;		// 处理文件名
	std::function<bool (QString)> fun1;			// 处理函数1
	bool (*fun2)(QString, QString);	// 处理函数2
	bool (*funList)(QStringList);		// 处理函数(列表)
signals:
	void sendMsg(QString msg);							// 显示消息
	void sendProgressBar(qint64 already, qint64 total); // 显示进度
};

#endif // DATAPROCESSINGTHREAD_H
