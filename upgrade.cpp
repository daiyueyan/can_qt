#include "upgrade.h"
#include "ui_upgrade.h"

Upgrade::Upgrade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Upgrade)
{
    ui->setupUi(this);
    board_name = 0;
    sendTimer=new QTimer(this);
    pkgsize=0;
    ui->file_send_process_show->ensureCursorVisible();//文本自适应移动
    connect(sendTimer,SIGNAL(timeout()),this,SLOT(slt_timCycle()));
}

Upgrade::~Upgrade()
{
    delete ui;
}

void Upgrade::on_btn_open_file_clicked()//打开文件
{
    QString file = QFileDialog::getOpenFileName(
                    this, "选择要升级的文件",
                    "/",
                    "所有文件 (*.*);; ");
    if(file.isEmpty())
        return;

    ui->file_location->setText(file);
    this->file_location = file;

    sendfile=new QFile(file);
    if (!sendfile->open(QFile::ReadOnly))
    {
        return;
    }

    txtcon=sendfile->readAll();
    fileSize=txtcon.size();
    QString file_size = QString("The number is %1 \n").arg(fileSize);
    ui->file_send_process_show->append(file_size);

    QByteArray showhex;
    foreach(quint8 chr,txtcon)
    {
        HexString(chr,showhex);
    }

    ui->file_get_show->setPlainText(showhex);
}

void Upgrade::on_btn_upgrade_out_clicked()//退出
{
    this->hide();
}

void Upgrade::on_board_type_currentIndexChanged(const QString &arg1)
{
    qDebug()<<"arg1 is " << arg1;
    if(arg1 ==QString::fromLocal8Bit("控制盒"))
    {
        board_name = 0;
    }
    else if(arg1 ==QString::fromLocal8Bit("机头控制板"))
    {
        board_name = 1;
    }
    else if(arg1 ==QString::fromLocal8Bit("屏幕"))
    {
        board_name = 2;
    }
    else
     {}
}

void Upgrade::on_btn_upgrade_clicked()//升级
{
    sendTimer->start(200);
}

void Upgrade::slt_timCycle()
{
    int remainlen=fileSize-sendedByte;

    QString remain = QString("remain = %1 \n").arg(fileSize);
    ui->file_send_process_show->append(remain);

    QByteArray sendBa=txtcon.mid(sendedByte,remainlen);

    //my_port->write(sendBa,remainlen);
    sendedByte+=remainlen;

    int sendpro=sendedByte*100/fileSize;
    ui->pb_send->setValue(sendpro);

    if(sendedByte==fileSize)
    {
        ui->file_send_process_show->append("send end");
        sendTimer->stop();
    }
}


void Upgrade::HexString(quint8 vhex,QByteArray &vba)
{
    quint8 vh=vhex/16;
    quint8 vl=vhex%16;

    if(vh>=0&&vh<=9)    vh=vh+'0';
    else if(vh>=10&&vh<=15) vh=vh+'a'-10;

    if(vl>=0&&vl<=9)    vl=vl+'0';
    else if(vl>=10&&vl<=15) vl=vl+'a'-10;

    vba.append(vh);
    vba.append(vl);
    vba.append(0x20);
}
