#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFileDialog>
#include <QDesktopServices>

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
    void onReadyRead();
    void onSendButtonPressed();
    void onFileSearchButtonPressed();
    void onFileSendButtonPressed();
    void onReadyFileRead();
    void onConnectButtonPressed();

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;
    QTcpSocket fileSocket;
};

#endif // MAINWINDOW_H
