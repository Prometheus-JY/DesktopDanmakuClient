#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <socketthread.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

Q_SIGNALS:
    void socketSendData(QByteArray msg);
    void socketStart(QString ip, int port);

private slots:

    void socket_Disconnected();

    void socket_Connected();

    void socketDataReceive(QString msg);

private:

    SocketThread* socketThread;

    QThread socketContainerThread;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
