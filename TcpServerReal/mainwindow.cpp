#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "packet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), server(this)
{
    ui->setupUi(this);
    server.listen(QHostAddress::Any, 9999);
    fileServer.listen(QHostAddress::Any, 9998);
    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    connect(&fileServer, SIGNAL(newConnection()), this, SLOT(onNewFileConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewConnection()
{
    qDebug() << "onNewConnection";
    QTcpSocket *clientSocket = server.nextPendingConnection();


    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    sockets.push_back(clientSocket);
}


void MainWindow::onNewFileConnection()
{
    qDebug() << "onFileConnection";
    QTcpSocket *clientFileSocket = fileServer.nextPendingConnection();

    connect(clientFileSocket, SIGNAL(readyRead()), this, SLOT(onReadyFileRead()));
    connect(clientFileSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    fileSockets.push_back(clientFileSocket);
}

void MainWindow::onStateChanged(QAbstractSocket::SocketState state)
{

    qDebug() << "onStateChanged";
    if(state == QAbstractSocket::UnconnectedState){
        QTcpSocket* s = static_cast<QTcpSocket*>(QObject::sender());
        sockets.removeOne(s);
    }
}

void MainWindow::onReadyRead()
{
    qDebug() << "onReadRead";
    QTcpSocket* s = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = s->readAll();

    for(QTcpSocket* socket : sockets){
        socket->write(data);
        ui->textEdit->append(QString::fromStdString(data.toStdString()));
    }

}

void MainWindow::onReadyFileRead()
{
    qDebug() << "onReadFileRead";
    QTcpSocket* s = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = s->readAll();

    for(QTcpSocket* socket : fileSockets){
        socket->write(data);
        //ui->textEdit->append(QString::fromStdString(data.toStdString()));
    }
}


