#include "upgrade.h"
#include "ui_upgrade.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <QCoreApplication>
#include "includes.h"
#include <QList>

Upgrade::Upgrade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Upgrade)
{
    ui->setupUi(this);
    qRegisterMetaType<can_ymodem_rx_fifo_t>("can_ymodem_rx_fifo_t");

    ymodemFileTransmit = new YmodemFileTransmit();

    //清空接收fifo
    rx_fifo = (can_ymodem_rx_fifo_t*)malloc(sizeof(can_ymodem_rx_fifo_t)+  RT_CAN_YMODEM_DEVICE_SIZE);
    rx_fifo->buffer = (uint8_t*) (rx_fifo + 1);
    memset(rx_fifo->buffer, 0, RT_CAN_YMODEM_DEVICE_SIZE);
    rx_fifo->put_index = 0;
    rx_fifo->get_index = 0;
    rx_fifo->is_full = false;

    transmitButtonStatus = false;
    receiveButtonStatus  = false;
    board_name = 0;
    pkgsize=0;

    ui->btn_send->setDisabled(true);
    ui->file_send_process_show->ensureCursorVisible();//文本自适应移动
    ui->btn_upgrade->setDisabled(true);

    connect(ymodemFileTransmit, SIGNAL(transmitStatus(YmodemFileTransmit::Status)), this, SLOT(transmitStatus(YmodemFileTransmit::Status)));//ymodemFileTransmit->upgrade
    connect(ymodemFileTransmit,&YmodemFileTransmit::send_file,this,&Upgrade::ymodem_can_send);//ymodemFileTransmit->upgrade
    connect(ymodemFileTransmit,&YmodemFileTransmit::transmit_read_request,this,&Upgrade::can_ymodem_rx);//ymodemFileTransmit->upgrade
    connect(ymodemFileTransmit,&YmodemFileTransmit::transmitProgress,this,&Upgrade::transmitProgress);//ymodemFileTransmit->upgrade
}

Upgrade::~Upgrade()
{
    delete ui;
    delete ymodemFileTransmit;
    delete rx_fifo;
}

void Upgrade::on_btn_open_file_clicked()//打开文件并且计算crc32值
{
    //获取文件目录file
    file_location = QFileDialog::getOpenFileName(
                    this, "选择要升级的文件",
                    "/",
                    "所有文件 (*.*);; ");
    if(file_location.isEmpty())
    {
        ui->btn_send->setDisabled(true);
        return;
    }

    ui->file_location->setText(file_location);

    //获取文件信息sendfile
    sendfile=new QFile(file_location);
    if (!sendfile->open(QFile::ReadOnly))
    {
        ui->btn_send->setDisabled(true);
        return;
    }
    ui->btn_send->setEnabled(true);

    //获取文件多少个字节fileSize
    txtcon=sendfile->readAll();
    fileSize=txtcon.size();
    QString file_size = QString("The number is %1").arg(fileSize);
    ui->file_send_process_show->append(file_size + " bytes");
    //计算CRC32值
    crc_value = upgrade_app_crc_cal();
    QString crc_string = QString::number(crc_value, 16);
    ui->file_send_process_show->append("crc32 of file : " +crc_string);

    QByteArray showhex;
    foreach(quint8 chr,txtcon)
    {
        HexString(chr,showhex);
    }

    ui->file_get_show->setPlainText(showhex);//显示二进制值形式显示文件的内容

    rx_fifo->buffer = (uint8_t*) (rx_fifo + 1);
    memset(rx_fifo->buffer, 0, RT_CAN_YMODEM_DEVICE_SIZE);
    rx_fifo->put_index = 0;
    rx_fifo->get_index = 0;
    rx_fifo->is_full = false;
}

void Upgrade::on_btn_upgrade_out_clicked()//退出
{
    this->hide();
}

void Upgrade::on_board_type_currentIndexChanged(const QString &arg1)
{
    ui->file_send_process_show->append(arg1);

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

void Upgrade::on_btn_send_clicked()//发送按键
{
    if(transmitButtonStatus == false)//表示当前没有在传输数据，可进行传输数据
    {
        ymodemFileTransmit->setFileName(file_location);//把文件目录传出去

        if(ymodemFileTransmit->startTransmit() == true)//可以打开串口，打开定时器
        {
            ui->file_send_process_show->append("start to send file");
            transmitButtonStatus = true;//表示正在传输

            ui->btn_open_file->setDisabled(true);//发送的浏览按键不可按下
            ui->btn_send->setText("cancel");//发送的发送按键文本为"取消"
            ui->pb_send->setValue(0);//进度设置为0
            ui->btn_upgrade->setDisabled(true);

            VCI_CAN_READ crc_send_out;
            crc_send_out.ID = CAN_ID_QT_TO_OBOX_FILE;
            crc_send_out.Len = 1;
            crc_send_out.Data[0] = CAN_CMD_FILE_QT_TO_CTRL_FILE_START;
            emit ymodem_can_write(crc_send_out);

            crc_send_out.Len = 5;
            crc_send_out.Data[0] = CAN_CMD_FILE_QT_TO_CTRL_FILE_SLAVE_CRC;
            crc_send_out.Data[1] = crc_value & 0xff;
            crc_send_out.Data[2] = crc_value >> 8;
            crc_send_out.Data[3] = crc_value >> 16;
            crc_send_out.Data[4] = crc_value >> 24;
            emit ymodem_can_write(crc_send_out);
        }
        else
        {
            QMessageBox::warning(this, "failed", "file send failed", "close");
        }
    }
    else
    {
        ymodemFileTransmit->stopTransmit();//停止传输
    }
}

void Upgrade::on_btn_upgrade_clicked()//升级按键
{
    VCI_CAN_READ crc_send_out;
    crc_send_out.ID = CAN_ID_QT_TO_OBOX_FILE;
    crc_send_out.Len = 1;
    crc_send_out.Data[0] = CAN_CMD_FILE_QT_TO_CTRL_FILE_UPGRADE;
    emit ymodem_can_write(crc_send_out);
}

void Upgrade::transmitProgress(int progress)//显示传输进度
{
    ui->pb_send->setValue(progress);
}

void Upgrade::transmitStatus(Ymodem::Status status)
{
    switch(status)
    {
        case YmodemFileTransmit::StatusEstablish:
        {
            break;
        }

        case YmodemFileTransmit::StatusTransmit:
        {
            break;
        }

        case YmodemFileTransmit::StatusFinish:
        {
            transmitButtonStatus = false;

            ui->btn_open_file->setEnabled(true);
            ui->btn_send->setText("send");

            QMessageBox::warning(this, "success", "file send success", "close");

            break;
        }

        case YmodemFileTransmit::StatusAbort:
        {
            transmitButtonStatus = false;

            ui->btn_open_file->setEnabled(true);
            ui->btn_send->setText("send");

            QMessageBox::warning(this, "failed", "file send failed", "close");

            break;
        }

        case YmodemFileTransmit::StatusTimeout:
        {
            transmitButtonStatus = false;

            ui->btn_open_file->setEnabled(true);
            ui->btn_send->setText("send");

            QMessageBox::warning(this, "failed", "file send failed", "close");

            break;
        }

        default:
        {
            transmitButtonStatus = false;

            ui->btn_open_file->setEnabled(true);
            ui->btn_send->setText("send");

            QMessageBox::warning(this, "failed", "file send failed", "close");
        }
    }
}

void Upgrade::ymodem_can_send(VCI_CAN_READ can_write_ymodem)
{
    emit ymodem_can_write(can_write_ymodem);
}

void Upgrade::dev_open_get(bool flag)
{
    dev_open = flag;
}
//把接收的数据转到fifo中
void Upgrade::ymodem_can_get(VCI_CAN_READ can_read_ymodem)
{
    int ch = -1;
    int length = 0;

    while (length < can_read_ymodem.Len)
    {
        ch = can_read_ymodem.Data[length];
        length ++;

        rx_fifo->buffer[rx_fifo->put_index] = ch;

        rx_fifo->put_index += 1;

        if (rx_fifo->put_index >= RT_CAN_YMODEM_DEVICE_SIZE)
        {
            rx_fifo->put_index = 0;
        }

        if (rx_fifo->put_index == rx_fifo->get_index)
        {
            rx_fifo->get_index += 1;
            rx_fifo->is_full = true;
            if (rx_fifo->get_index >= RT_CAN_YMODEM_DEVICE_SIZE)
            {
                rx_fifo->get_index = 0;
            }
        }
    }
    //释放信号量
    emit ymodem_can_read(can_read_ymodem);
}

void Upgrade::upgrade_info_get(VCI_CAN_READ can_read_ymodem)
{
    switch(can_read_ymodem.Data[0])
    {
    case CAN_CMD_FILE_CMD_MASTER:

    break;

    case CAN_CMD_FILE_FILE_MASTER:
    break;

    case CAN_CMD_FILE_QT_TO_CTRL_FILE_FILE:
    {
        VCI_CAN_READ read_buf_to_ymodem;
        read_buf_to_ymodem.Len = can_read_ymodem.Len - 1;
        memcpy(read_buf_to_ymodem.Data,&can_read_ymodem.Data[1],can_read_ymodem.Len - 1);
        ymodem_can_get(read_buf_to_ymodem);
    }
    break;

    case CAN_CMD_FILE_QT_TO_CTRL_FILE_SLAVE_CRC:
        ui->file_send_process_show->append("ctrl_crc32 of file err !");
    break;

    case PROTOCOL_CMD_QT_TO_CTRLBOX_UPGRADE_PER:
    {
        QString per = QString::number(can_read_ymodem.Data[1], 10);
        ui->file_send_process_show->append("ctrlbox upgrade output : " + per + "%");
    }
    break;

    case PROTOCOL_CMD_QT_TO_CTRLBOX_UPGRADE_END:
        ui->btn_upgrade->setEnabled(true);
    break;

    case CAN_CMD_FILE_QT_TO_CTRL_FILE_UPGRADE_SUCC:
        ui->file_send_process_show->append("ctrlbox upgrade succ");
    break;

    case CAN_CMD_FILE_CMD_SLAVE:

    break;

    default:
    break;
    }
}

//读取fifo中的数据
void Upgrade::can_ymodem_rx(uint8_t *data, uint32_t length, uint32_t *len_return)
{
    uint32_t size;
    size = length;

    while (length)
    {
        int ch;

        if ((rx_fifo->get_index == rx_fifo->put_index) && (rx_fifo->is_full == false))
        {
            break;
        }

        /* otherwise there's the data: */
        ch = rx_fifo->buffer[rx_fifo->get_index];
        rx_fifo->get_index += 1;
        if (rx_fifo->get_index >= RT_CAN_YMODEM_DEVICE_SIZE) rx_fifo->get_index = 0;

        if (rx_fifo->is_full == true)
        {
            rx_fifo->is_full = false;
        }

        *data = ch & 0xff;
        data ++;
        length --;
    }
    *len_return = size - length;
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

uint32_t ef_calc_crc32(uint32_t crc, const void *buf, size_t size)
{
    uint32_t crc32_table[] =
    {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
        0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
        0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
        0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
        0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
        0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
        0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
        0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
        0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
        0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
        0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
        0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
        0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
        0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
        0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
        0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
        0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
        0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
        0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
        0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
        0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
        0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
        0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
        0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
        0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
        0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
        0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
        0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
        0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
        0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
        0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
    };
    const uint8_t *p;

    p = (const uint8_t *)buf;
    crc = crc ^ ~0U;

    while (size--) {
        crc = crc32_table[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    }

    return crc ^ ~0U;
}

uint32_t Upgrade::upgrade_app_crc_cal(void)
{
    int cur_size, size_else, size;
    uint32_t buff[32];
    size_t crc_value_get = 0;

    size_else = fileSize % sizeof(buff);

    int fd = sendfile->handle(); //获取文件描述符

    // 移动到文件的第10个字节
    lseek(fd, 0, SEEK_SET);

    for(cur_size = 0; cur_size < (fileSize - size_else); cur_size += sizeof(buff))
    {
        size = read(fd, buff, sizeof(buff));

        if(size < 0)
        {
            ui->file_send_process_show->append(file_location + "read error");
            sendfile->close();
            return 0;
        }

        crc_value_get = ef_calc_crc32(crc_value_get, (uint8_t*)buff, sizeof(buff));
    }

    if( 0 != size_else)
    {
        size = read(fd, buff, size_else);
        crc_value_get = ef_calc_crc32(crc_value_get, (uint8_t*)buff, size_else);

        if(size < 0)
        {
            ui->file_send_process_show->append(file_location + "read error");
            sendfile->close();
            return 0;
        }
    }

    return crc_value_get;
}

