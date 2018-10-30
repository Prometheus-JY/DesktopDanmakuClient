#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H
#include <QThread>
#include <QtNetwork>

class SocketThread : public QObject
{
    Q_OBJECT
public:
    //SocketThread(QObject *parent);
    SocketThread();



Q_SIGNALS:
    void dataReady(QString msg);
    void socketConnected();
    void socketDisconnected();

public slots:
    void onConnect();
    void onDisConnect();
    void onReadMsg();
    void sendData(QByteArray msg);
    void run(QString ip, int port);

private:
    QTcpSocket* m_TcpSocket;

    bool m_isThreadStopped;
    bool m_isConected;
    QString m_ip;
    int m_port;
    QByteArray m_datagram;

};

#endif // SOCKETTHREAD_H
