//mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();

    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);

    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("10.24.10.111");
    ui->lineEdit_Port->setText("8520");

}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}

void MainWindow::on_pushButton_Connect_clicked()
{

    if(ui->pushButton_Connect->text() == "连接")
    {
        QString IP;
        int port;

        //获取IP地址
        IP = ui->lineEdit_IP->text();
        //获取端口号
        port = ui->lineEdit_Port->text().toInt();

        //取消已有的连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP, port);

        //等待连接成功
        if(!socket->waitForConnected(30000))
        {
            qDebug() << "Connection failed!";
            return;
        }
        qDebug() << "Connect successfully!";

        //发送按键使能
        ui->pushButton_Send->setEnabled(true);
        //修改按键文字
        ui->pushButton_Connect->setText("断开连接");
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->pushButton_Connect->setText("连接");
        ui->pushButton_Send->setEnabled(false);
    }
}

void MainWindow::on_pushButton_Send_clicked()
{

    char la=0xff;
    QString send= ui->textEdit_Send->toPlainText();
    qDebug() << "Send: " << send.toUtf8();
     //获取文本框内容并以ASCII码形式发送

    //socket->write(ui->textEdit_Send->toPlainText().toLatin1());
    //旧代码中文会乱码，所以将消息以utf-8格式发送
//    toStdString().c_str()
    socket->write(send.toUtf8()+la);
    socket->flush();
}

void MainWindow::socket_Read_Data()
{

    QByteArray buffer;
    //读取缓冲区数据

    QByteArray buffer1;
    int index=0;


    buffer = socket->readAll();

    QString temp="汉字";
    QByteArray t=temp.toLocal8Bit();

    QString test=QString::fromLocal8Bit(t.data());
    qDebug()<<"test:"<<test;

    for(int i=2;i<buffer.size();i++){
         buffer1[index++]=buffer[i];
    }
    if(!buffer.isEmpty())
    {
        QString str;
        //将收到的utf-8格式转换回String
        str=QString::fromLocal8Bit(buffer.data());
//         str=QString::fromUtf8(buffer.data());
//        str=QString(buffer1);
        qDebug()<<"rec:"<<str;
        //刷新显示
        ui->textEdit_Recv->clear();
        ui->textEdit_Recv->setText(str);

    }
}

void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    //修改按键文字
    ui->pushButton_Connect->setText("连接");
    qDebug() << "Disconnected!";
}

