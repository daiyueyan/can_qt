#include "canparamsetting.h"
#include "ui_canparamsetting.h"
#include <QFile>
#include <QDebug>

CANParamSetting::CANParamSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CANParamSetting)
{
    ui->setupUi(this);

    QFile file("canparamsetting.qss");//.qss是定义了一个界面风格的文件
    if( file.open(QFile::ReadOnly))//以只读的方式打开文件
    {
        QString styleSheet = QLatin1String(file.readAll());//读取文件中定义的风格
//        qDebug()<<styleSheet.remove("\r").remove("\n").remove("\t");
        this->setStyleSheet(styleSheet);//设置界面的风格
        file.close();
        qDebug()<<"load success";
    }
    else
        qDebug()<<"load error";
}

CANParamSetting::~CANParamSetting()
{
    delete ui;
}

void CANParamSetting::on_pushButton_clicked()//确定
{
    CANtype = 4;//ui->baundRate->currentIndex();
    index = ui->index->currentIndex();
    baundRate = (int)ui->baundRate->currentText().remove("Kbps").toFloat();
    devicCOM = ui->CANCom->currentIndex();
    this->hide();
}

void CANParamSetting::on_pushButton_2_clicked()//退出
{
    this->hide();
}
