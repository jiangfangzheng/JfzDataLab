/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QGroupBox *groupBox_3;
    QPushButton *pushButton_DS18B20;
    QPushButton *pushButton_CCD;
    QPushButton *pushButton_FBGT;
    QPushButton *pushButton_FBGS;
    QPushButton *pushButton_ENV;
    QLabel *label_3;
    QLabel *label_12;
    QGroupBox *groupBox_4;
    QPushButton *pushButton_LoadFBGT;
    QPushButton *pushButton_wave2temp;
    QLabel *label_9;
    QGroupBox *groupBox_5;
    QPushButton *pushButton_covresult;
    QPushButton *pushButton_File2;
    QPushButton *pushButton_File1;
    QLabel *label_11;
    QWidget *tab;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_8;
    QGroupBox *groupBox_2;
    QTextEdit *textEdit;
    QWidget *tab_2;
    QLabel *label;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1000, 600);
        MainWindow->setMinimumSize(QSize(1000, 600));
        MainWindow->setMaximumSize(QSize(1000, 600));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 1000, 571));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        groupBox_3 = new QGroupBox(tab_3);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 10, 781, 91));
        pushButton_DS18B20 = new QPushButton(groupBox_3);
        pushButton_DS18B20->setObjectName(QStringLiteral("pushButton_DS18B20"));
        pushButton_DS18B20->setGeometry(QRect(10, 30, 111, 28));
        pushButton_CCD = new QPushButton(groupBox_3);
        pushButton_CCD->setObjectName(QStringLiteral("pushButton_CCD"));
        pushButton_CCD->setGeometry(QRect(130, 30, 111, 28));
        pushButton_FBGT = new QPushButton(groupBox_3);
        pushButton_FBGT->setObjectName(QStringLiteral("pushButton_FBGT"));
        pushButton_FBGT->setGeometry(QRect(250, 30, 111, 28));
        pushButton_FBGS = new QPushButton(groupBox_3);
        pushButton_FBGS->setObjectName(QStringLiteral("pushButton_FBGS"));
        pushButton_FBGS->setGeometry(QRect(370, 30, 111, 28));
        pushButton_ENV = new QPushButton(groupBox_3);
        pushButton_ENV->setObjectName(QStringLiteral("pushButton_ENV"));
        pushButton_ENV->setGeometry(QRect(490, 30, 141, 28));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 70, 761, 16));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(640, 20, 121, 41));
        groupBox_4 = new QGroupBox(tab_3);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 110, 381, 61));
        pushButton_LoadFBGT = new QPushButton(groupBox_4);
        pushButton_LoadFBGT->setObjectName(QStringLiteral("pushButton_LoadFBGT"));
        pushButton_LoadFBGT->setGeometry(QRect(10, 20, 111, 28));
        pushButton_wave2temp = new QPushButton(groupBox_4);
        pushButton_wave2temp->setObjectName(QStringLiteral("pushButton_wave2temp"));
        pushButton_wave2temp->setEnabled(false);
        pushButton_wave2temp->setGeometry(QRect(130, 20, 111, 28));
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(250, 30, 121, 16));
        groupBox_5 = new QGroupBox(tab_3);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(400, 110, 391, 61));
        pushButton_covresult = new QPushButton(groupBox_5);
        pushButton_covresult->setObjectName(QStringLiteral("pushButton_covresult"));
        pushButton_covresult->setEnabled(false);
        pushButton_covresult->setGeometry(QRect(190, 20, 81, 28));
        pushButton_File2 = new QPushButton(groupBox_5);
        pushButton_File2->setObjectName(QStringLiteral("pushButton_File2"));
        pushButton_File2->setGeometry(QRect(100, 20, 81, 28));
        pushButton_File1 = new QPushButton(groupBox_5);
        pushButton_File1->setObjectName(QStringLiteral("pushButton_File1"));
        pushButton_File1->setGeometry(QRect(10, 20, 81, 28));
        label_11 = new QLabel(groupBox_5);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(280, 30, 101, 16));
        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 93, 28));
        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 50, 93, 28));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(110, 10, 881, 71));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 661, 39));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(190, 0));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMinimumSize(QSize(190, 0));

        gridLayout->addWidget(label_5, 0, 1, 1, 1);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMinimumSize(QSize(190, 0));

        gridLayout->addWidget(label_7, 0, 2, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 1, 1, 1, 1);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 1, 2, 1, 1);

        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 80, 981, 451));
        textEdit = new QTextEdit(groupBox_2);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setEnabled(true);
        textEdit->setGeometry(QRect(10, 20, 961, 421));
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        textEdit->setFont(font);
        textEdit->setReadOnly(true);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(700, 580, 91, 20));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 23));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\345\274\202\346\203\263\345\256\266\346\225\260\346\215\256\345\256\236\351\252\214\345\256\244", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\346\225\260\346\215\256\346\240\207\345\207\206\345\214\226", 0));
        pushButton_DS18B20->setText(QApplication::translate("MainWindow", "DS18B20(Dir)", 0));
        pushButton_CCD->setText(QApplication::translate("MainWindow", "CCD(Dir)", 0));
        pushButton_FBGT->setText(QApplication::translate("MainWindow", "FBG\346\270\251\345\272\246(Dir)", 0));
        pushButton_FBGS->setText(QApplication::translate("MainWindow", "FBG\345\272\224\345\212\233(Dir)", 0));
        pushButton_ENV->setText(QApplication::translate("MainWindow", "\347\216\257\345\242\203\346\270\251\345\272\246(*.xls)", 0));
        label_3->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\347\212\266\346\200\201\357\274\232\346\234\252\350\275\275\345\205\245", 0));
        label_12->setText(QApplication::translate("MainWindow", "Dir:\351\200\211\346\226\207\344\273\266\345\244\271\n"
"*.xls:\345\244\232\351\200\211xls", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\346\263\242\351\225\277\350\275\254\346\270\251\345\272\246(\344\273\205\344\276\233\345\217\202\350\200\203)", 0));
        pushButton_LoadFBGT->setText(QApplication::translate("MainWindow", "\346\240\207\345\207\206FBG\346\226\207\344\273\266", 0));
        pushButton_wave2temp->setText(QApplication::translate("MainWindow", "\344\270\200\351\224\256\350\275\254\346\215\242", 0));
        label_9->setText(QApplication::translate("MainWindow", "\346\234\252\350\275\275\345\205\245", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "\347\233\270\345\205\263\346\200\247\350\256\241\347\256\227", 0));
        pushButton_covresult->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220\347\273\223\346\236\234", 0));
        pushButton_File2->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\2662", 0));
        pushButton_File1->setText(QApplication::translate("MainWindow", " \346\226\207\344\273\2661", 0));
        label_11->setText(QApplication::translate("MainWindow", "\345\276\205\350\256\241\347\256\227", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\346\225\260\346\215\256\346\240\207\345\207\206\345\214\226", 0));
        pushButton->setText(QApplication::translate("MainWindow", "\350\275\275\345\205\245\346\225\260\346\215\256", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\345\205\250\351\203\250", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\225\260\346\215\256\346\246\202\350\247\210", 0));
        label_2->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\350\241\214\345\210\227\357\274\232", 0));
        label_5->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\346\200\273\345\222\214\357\274\232", 0));
        label_7->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\346\234\200\345\244\247\357\274\232", 0));
        label_4->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\220\215\345\255\227\357\274\232", 0));
        label_6->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\271\263\345\235\207\357\274\232", 0));
        label_8->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\346\234\200\345\260\217\357\274\232", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\237\272\346\234\254\344\277\241\346\201\257", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\346\225\260\346\215\256\346\246\202\350\247\210", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\346\225\260\346\215\256\347\273\230\345\233\276", 0));
        label->setText(QApplication::translate("MainWindow", "\350\222\213\346\226\271\346\255\243\345\210\266\344\275\234", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
