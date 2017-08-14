#include "SetUI.h"
#include "ui_SetUI.h"

SetUI::SetUI(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetUI)
{
	ui->setupUi(this);
}

SetUI::~SetUI()
{
	delete ui;
}
