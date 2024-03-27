#ifndef CANPARAMSETTING_H
#define CANPARAMSETTING_H

#include <QDialog>

namespace Ui {
class CANParamSetting;
}

class CANParamSetting : public QDialog
{
    Q_OBJECT

public:
    explicit CANParamSetting(QWidget *parent = 0);//explicit使构造函数只能显性调用，如果CANParamSetting can = CANParamSetting(1.2)不能CANParamSetting can = 1.2
    ~CANParamSetting();
    int CANtype;//CAN卡类型
    int index;//索引号
    int baundRate;//波特率
    int devicCOM;//端口号

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::CANParamSetting *ui;
};

#endif // CANPARAMSETTING_H
