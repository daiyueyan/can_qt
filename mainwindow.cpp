﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sendthreadcan.h"
#include "mymethod.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 *函数名:初始化
 *函数参数:NULL
 *函数作用:NULL
 *函数返回值:NULL
 *备注:NULL
 */
void MainWindow::init()
{
    CANsetting = new CANParamSetting();
    thread_cansend = new SendThreadCan();
    canthread = new CANThread();
    protocolHand = new ProtocolThrend();
    connect(canthread,&CANThread::send_can_info_send,thread_cansend,&SendThreadCan::send_can_info_rev);
    connect(this,&MainWindow::send_can_out_send,thread_cansend,&SendThreadCan::send_can_out_rev);
    connect(canthread,&CANThread::getProtocolData,this,&MainWindow::displayText);
    thread_cansend->start();
}
//在界面上把pushButton右键转为槽后自己生成on_pushButton_clicked函数
void MainWindow::on_pushButton_clicked()//参数设置
{
    CANsetting->show();
}

void MainWindow::on_pushButton_2_clicked()//启动CAN
{
    if(ui->pushButton_2->text() == tr("CAN启动"))
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setText(tr("CAN关闭"));
        canthread->deviceType = CANsetting->CANtype;
        canthread->debicIndex = CANsetting->index;
        canthread->baundRate = CANsetting->baundRate;
        canthread->debicCom = CANsetting->devicCOM;
        bool dev = canthread->openCAN();
        if(dev == true)
             canthread->start();
    }
    else if(ui->pushButton_2->text() == tr("CAN关闭"))
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setText(tr("CAN启动"));
        canthread->stop();
        canthread->closeCAN();
    }
}

void MainWindow::on_pushButton_3_clicked()//发送
{
    int ID = ui->lineEdit->text().toUInt(nullptr,Z_HEX);//ID转换为16位进制
    emit send_can_out_send(ID, ui->textEdit->toPlainText());
}

void MainWindow::displayText(QString info)
{
    ui->textEdit_2->setText(info);
}
