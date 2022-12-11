#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onNewConnection();
    void onReadyRead();
    void onStateChanged(QAbstractSocket::SocketState state);
    void onNewFileConnection();
    void onReadyFileRead();

private:
    Ui::MainWindow *ui;
    QTcpServer server;
    QTcpServer fileServer;
    QList<QTcpSocket*> sockets;
    QList<QTcpSocket*> fileSockets;
};

#endif // MAINWINDOW_H
