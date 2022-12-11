#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "packet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(this)
{
    ui->setupUi(this);

    connect(&socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&fileSocket, SIGNAL(readyRead()), this, SLOT(onReadyFileRead()));

    connect(ui->sendButton, SIGNAL(pressed()), this, SLOT(onSendButtonPressed()));
    connect(ui->fileSearchButton, SIGNAL(pressed()), this, SLOT(onFileSearchButtonPressed()));
    connect(ui->fileSendButton, SIGNAL(pressed()), this, SLOT(onFileSendButtonPressed()));
    connect(ui->connectButton, SIGNAL(pressed()), this, SLOT(onConnectButtonPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadyRead()
{
    QByteArray data = socket.readAll();
    qDebug() << QString(data).section("||",1,1);

    if(QString(data).section("||",0,0)=="%%FILE_END%%"){
        char buf[2000];
        char *res = realpath(".", buf);
        if(res){
            QDesktopServices::openUrl(QUrl(buf, QUrl::TolerantMode));
        }
        ui->textEdit->append(QString(data).section("||",1,1));
    }else{
        ui->textEdit->append(QString::fromStdString(data.toStdString()));
    }
}

void MainWindow::onReadyFileRead()
{
      QByteArray data = fileSocket.readAll();
      QString path = "./" + ui->download->text();
      QFile file(path);

      //qDebug() << QString::fromStdString(data.toStdString());
      file.open(QIODevice::ReadWrite);
      file.write(data);
      if(file.isOpen()){
          file.close();
      }
//      if(QString::fromStdString(data.toStdString())== "%%start%%"){
//          qDebug() << "start";
//          file.open(QIODevice::ReadWrite);
//      }else if(QString::fromStdString(data.toStdString())== "%%end%%"){
//          qDebug() << "close";
//          file.close();
//      }else{
//          file.write((QString(data).section("||",1).toUtf8()));
//      }
}



void MainWindow::onSendButtonPressed()
{
    qDebug() << "onSendButtonPressed";
    QString text = ui->lineEdit->text();
    QString nickname = ui->nickname->text();
    text = nickname + " : " + text;
    ui->lineEdit->setText("");

    qDebug() << text;
    socket.write(QByteArray::fromStdString(text.toStdString()));
}

void MainWindow::onFileSearchButtonPressed()
{
    QFileDialog dialog(this);
    QStringList fileNames;

    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    ui->path->setText(fileNames.first());
}


void MainWindow::onFileSendButtonPressed()
{
//    //file start
//    QString text =  "%%start%%";
//    fileSocket.write(QByteArray::fromStdString(text.toStdString()));

    //file send
    QFile file(ui->path->text());
    file.open(QIODevice::ReadWrite);

    int sendSize =0;
    qint64 len=0;
    do
   {
       char buf[4*1024] = {0};
       len = file.read(buf,sizeof(buf));

       qDebug()<<fileSocket.write(buf,len);

       sendSize += len;
       qDebug()<<len<<sendSize;
   }while(len > 0);

   QString str = QString("%1||%2").arg("%%FILE_END%%").arg("File Send Complete");
   socket.write(QByteArray::fromStdString(str.toStdString()));


//    //file end
//    text =  "%%end%%";
//    fileSocket.write(QByteArray::fromStdString(text.toStdString()));
}

void MainWindow::onConnectButtonPressed()
{
    socket.connectToHost(QHostAddress(ui->serverIp->text()), 9999);
    fileSocket.connectToHost(QHostAddress(ui->serverIp->text()), 9998);

    if(socket.state() == QAbstractSocket::ConnectingState &&fileSocket.state() == QAbstractSocket::ConnectingState){
        ui->textEdit->append("Server Connected!!");
    }else{
        ui->textEdit->append("Server Connected Failed.");
    }


}
