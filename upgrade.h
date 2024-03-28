#ifndef UPGRADE_H
#define UPGRADE_H

#include <QDialog>
#include <string.h>
#include <QDebug>
#include <QFileDialog>
#include <QTimer>

namespace Ui {
class Upgrade;
}

class Upgrade : public QDialog
{
    Q_OBJECT

public:
    explicit Upgrade(QWidget *parent = nullptr);
    ~Upgrade();

    void HexString(quint8 vhex,QByteArray &vba);
    uint8_t board_name;
    QString file_location;

signals:
    void fileName_send(QString file);

private:
    int fileSize;
    int pkgsize;        //×Ö½Ú
    int sendedByte;
    QFile *sendfile;
    QByteArray txtcon;
    QTimer *sendTimer;

private slots:
    void on_btn_open_file_clicked();

    void on_btn_upgrade_out_clicked();

    void on_board_type_currentIndexChanged(const QString &arg1);

    void on_btn_upgrade_clicked();

    void slt_timCycle();
private:
    Ui::Upgrade *ui;
};

#endif // UPGRADE_H
