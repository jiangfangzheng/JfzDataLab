﻿#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

#include <QSystemTrayIcon>
#include <QAction>
#include <QApplication>
#include <QMenu>

class SystemTray : public QSystemTrayIcon
{
	Q_OBJECT
public:
	explicit SystemTray(QWidget *parent = 0);
	~SystemTray();
	void addAction();
	void createAction();
private:
	QMenu   *myMenu;			// 托盘菜单
	QAction *setAction;			// 设置
	QAction *restoreWinAction;	// 还原
	QAction *quitAction;		// 退出
	QAction *aboutUsAction;		// 关于我们
	QAction *updateAction;		// 更新
private slots:
	void showAboutUs();
	void showUpdate();
	void showSetUI();
signals:
	void showWidget();
};

#endif // SYSTEMTRAY_H
