#include "AboutUI.h"
#include "ui_AboutUI.h"

AboutUI::AboutUI(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutUI)
{
	ui->setupUi(this);
	ui->label_2->setText("v1.0.0.1 x64");
	ui->label_3->setText("　　异想家数据实验室是一款方便实用的数据处理软件。最初用于武重项目，处理采集的数据，随后不断完善，扩展成一款轻量级的通用数据处理平台。"
						 " \n\n   © 2016 - 2017 开源软件 ");
	ui->label_4->setText("Qt Framework \nQCustomPlot \nArmadillo、Blas、Lapack \n");
	ui->label_5->setText("sandeepin");
	ui->label_6->setText("mother");

}

AboutUI::~AboutUI()
{
	delete ui;
}
