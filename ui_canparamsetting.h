/********************************************************************************
** Form generated from reading UI file 'canparamsetting.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANPARAMSETTING_H
#define UI_CANPARAMSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CANParamSetting
{
public:
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label;
    QLabel *label_2;
    QComboBox *CANType;
    QComboBox *index;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *baundRate;
    QComboBox *CANCom;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *CANParamSetting)
    {
        if (CANParamSetting->objectName().isEmpty())
            CANParamSetting->setObjectName(QString::fromUtf8("CANParamSetting"));
        CANParamSetting->setEnabled(true);
        CANParamSetting->resize(317, 300);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CANParamSetting->sizePolicy().hasHeightForWidth());
        CANParamSetting->setSizePolicy(sizePolicy);
        CANParamSetting->setMaximumSize(QSize(317, 300));
        gridLayout = new QGridLayout(CANParamSetting);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout->setHorizontalSpacing(30);
        gridLayout->setVerticalSpacing(1);
        gridLayout->setContentsMargins(30, -1, 30, -1);
        label_5 = new QLabel(CANParamSetting);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 0, 0, 1, 2);

        label = new QLabel(CANParamSetting);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(CANParamSetting);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        CANType = new QComboBox(CANParamSetting);
        CANType->addItem(QString());
        CANType->addItem(QString());
        CANType->addItem(QString());
        CANType->addItem(QString());
        CANType->setObjectName(QString::fromUtf8("CANType"));

        gridLayout->addWidget(CANType, 2, 0, 1, 1);

        index = new QComboBox(CANParamSetting);
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->addItem(QString());
        index->setObjectName(QString::fromUtf8("index"));

        gridLayout->addWidget(index, 2, 1, 1, 1);

        label_3 = new QLabel(CANParamSetting);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label_4 = new QLabel(CANParamSetting);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 3, 1, 1, 1);

        baundRate = new QComboBox(CANParamSetting);
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->addItem(QString());
        baundRate->setObjectName(QString::fromUtf8("baundRate"));

        gridLayout->addWidget(baundRate, 4, 0, 1, 1);

        CANCom = new QComboBox(CANParamSetting);
        CANCom->addItem(QString());
        CANCom->addItem(QString());
        CANCom->setObjectName(QString::fromUtf8("CANCom"));

        gridLayout->addWidget(CANCom, 4, 1, 1, 1);

        pushButton = new QPushButton(CANParamSetting);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(true);
        pushButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(pushButton, 5, 0, 1, 1);

        pushButton_2 = new QPushButton(CANParamSetting);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 5, 1, 1, 1);

        gridLayout->setRowStretch(0, 3);
        gridLayout->setRowStretch(1, 1);
        gridLayout->setRowStretch(2, 2);
        gridLayout->setRowStretch(3, 4);
        gridLayout->setRowStretch(4, 2);
        gridLayout->setRowStretch(5, 2);

        retranslateUi(CANParamSetting);

        QMetaObject::connectSlotsByName(CANParamSetting);
    } // setupUi

    void retranslateUi(QDialog *CANParamSetting)
    {
        CANParamSetting->setWindowTitle(QCoreApplication::translate("CANParamSetting", "CANParamSetting", nullptr));
        label_5->setText(QCoreApplication::translate("CANParamSetting", "CAN\345\215\241\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("CANParamSetting", "\350\256\276\345\244\207\347\261\273\345\236\213", nullptr));
        label_2->setText(QCoreApplication::translate("CANParamSetting", "\350\256\276\345\244\207\347\264\242\345\274\225", nullptr));
        CANType->setItemText(0, QCoreApplication::translate("CANParamSetting", "USBCAN-2A", nullptr));
        CANType->setItemText(1, QCoreApplication::translate("CANParamSetting", "USBCAN-2C", nullptr));
        CANType->setItemText(2, QCoreApplication::translate("CANParamSetting", "CANalyst-II", nullptr));
        CANType->setItemText(3, QCoreApplication::translate("CANParamSetting", "USBCAN-E-U", nullptr));

        index->setItemText(0, QCoreApplication::translate("CANParamSetting", "0", nullptr));
        index->setItemText(1, QCoreApplication::translate("CANParamSetting", "1", nullptr));
        index->setItemText(2, QCoreApplication::translate("CANParamSetting", "2", nullptr));
        index->setItemText(3, QCoreApplication::translate("CANParamSetting", "3", nullptr));
        index->setItemText(4, QCoreApplication::translate("CANParamSetting", "4", nullptr));
        index->setItemText(5, QCoreApplication::translate("CANParamSetting", "5", nullptr));
        index->setItemText(6, QCoreApplication::translate("CANParamSetting", "6", nullptr));
        index->setItemText(7, QCoreApplication::translate("CANParamSetting", "7", nullptr));
        index->setItemText(8, QCoreApplication::translate("CANParamSetting", "8", nullptr));
        index->setItemText(9, QCoreApplication::translate("CANParamSetting", "9", nullptr));
        index->setItemText(10, QCoreApplication::translate("CANParamSetting", "10", nullptr));
        index->setItemText(11, QCoreApplication::translate("CANParamSetting", "11", nullptr));
        index->setItemText(12, QCoreApplication::translate("CANParamSetting", "12", nullptr));
        index->setItemText(13, QCoreApplication::translate("CANParamSetting", "13", nullptr));

        label_3->setText(QCoreApplication::translate("CANParamSetting", "\346\263\242\347\211\271\347\216\207", nullptr));
        label_4->setText(QCoreApplication::translate("CANParamSetting", "\351\200\232\351\201\223", nullptr));
        baundRate->setItemText(0, QCoreApplication::translate("CANParamSetting", "10Kbps", nullptr));
        baundRate->setItemText(1, QCoreApplication::translate("CANParamSetting", "20Kbps", nullptr));
        baundRate->setItemText(2, QCoreApplication::translate("CANParamSetting", "40Kbps", nullptr));
        baundRate->setItemText(3, QCoreApplication::translate("CANParamSetting", "50Kbps", nullptr));
        baundRate->setItemText(4, QCoreApplication::translate("CANParamSetting", "80Kbps", nullptr));
        baundRate->setItemText(5, QCoreApplication::translate("CANParamSetting", "100Kbps", nullptr));
        baundRate->setItemText(6, QCoreApplication::translate("CANParamSetting", "125Kbps", nullptr));
        baundRate->setItemText(7, QCoreApplication::translate("CANParamSetting", "200Kbps", nullptr));
        baundRate->setItemText(8, QCoreApplication::translate("CANParamSetting", "250Kbps", nullptr));
        baundRate->setItemText(9, QCoreApplication::translate("CANParamSetting", "400Kbps", nullptr));
        baundRate->setItemText(10, QCoreApplication::translate("CANParamSetting", "500Kbps", nullptr));
        baundRate->setItemText(11, QCoreApplication::translate("CANParamSetting", "666Kbps", nullptr));
        baundRate->setItemText(12, QCoreApplication::translate("CANParamSetting", "800Kbps", nullptr));
        baundRate->setItemText(13, QCoreApplication::translate("CANParamSetting", "1000Kbps", nullptr));
        baundRate->setItemText(14, QCoreApplication::translate("CANParamSetting", "33.33Kbps", nullptr));
        baundRate->setItemText(15, QCoreApplication::translate("CANParamSetting", "66.66Kbps", nullptr));
        baundRate->setItemText(16, QCoreApplication::translate("CANParamSetting", "83.33Kbps", nullptr));

        CANCom->setItemText(0, QCoreApplication::translate("CANParamSetting", "0", nullptr));
        CANCom->setItemText(1, QCoreApplication::translate("CANParamSetting", "1", nullptr));

        pushButton->setText(QCoreApplication::translate("CANParamSetting", "\347\241\256\345\256\232", nullptr));
        pushButton_2->setText(QCoreApplication::translate("CANParamSetting", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CANParamSetting: public Ui_CANParamSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANPARAMSETTING_H
