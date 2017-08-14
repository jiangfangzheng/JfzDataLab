#ifndef SETUI_H
#define SETUI_H

#if _MSC_VER >= 1600
	   #pragma execution_character_set("utf-8")
#endif

#include <QDialog>

namespace Ui {
class SetUI;
}

class SetUI : public QDialog
{
	Q_OBJECT

public:
	explicit SetUI(QWidget *parent = 0);
	~SetUI();

private:
	Ui::SetUI *ui;
};

#endif // SETUI_H
