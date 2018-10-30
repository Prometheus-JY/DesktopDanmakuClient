#include "socketthread.h"

//SocketThread::SocketThread(QObject *parent)
//    : QThread(parent)
//    , m_TcpSocket(NULL)
//    , m_isThreadStopped(false)
//    , m_isConected(false)
//{
//}

SocketThread::SocketThread()
    : QObject()
    , m_TcpSocket(NULL)
    , m_isThreadStopped(false)
    , m_isConected(false)
{
}

void SocketThread::run(QString ip, int port)
{
    while (!m_isThreadStopped)
    {
        //检测客户端 socket指针是否为空
        if (!m_TcpSocket)
        {
            m_TcpSocket = new QTcpSocket(this);
            connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(onReadMsg()));
            connect(m_TcpSocket, SIGNAL(connected()), this, SLOT(onConnect()));
            connect(m_TcpSocket, SIGNAL(disconnected()), this, SLOT(onDisConnect()));
        }
        if (!m_isConected)
        {
            m_TcpSocket->connectToHost(ip, port);
            //等待连接。。。延时三秒，三秒内连不上返回false
            m_isConected = m_TcpSocket->waitForConnected(5000);
        }
        if (!m_isConected)
        {
            continue;
        }
        m_TcpSocket->waitForReadyRead(5000);
    }
}



void SocketThread::sendData(QByteArray msg){
    if(m_isThreadStopped==false && m_isConected==true){
        if(m_TcpSocket->write(msg)>0){
            return;
        }
    }
    return;
}


void SocketThread::onConnect()
{
    QString iamteacher = "#iamtheteacher!";
    if(m_TcpSocket->write(iamteacher.toLocal8Bit())>0){
        emit socketConnected();
        return;
    }

}

void SocketThread::onDisConnect()
{
    //socket一旦断开则自动进入这个槽函数
    //通过把 m_isOkConect 设为false，在socket线程的run函数中将会重新连接主机
    m_isConected = false;
    emit socketDisconnected();
}

void SocketThread::onReadMsg()
{
    while (m_TcpSocket->bytesAvailable() > 0)
    {
        m_datagram.clear();
        m_datagram.resize(m_TcpSocket->bytesAvailable());
        m_TcpSocket->read(m_datagram.data(), m_datagram.size());
        QString string = QString::fromLocal8Bit(m_datagram);
        emit dataReady(string);
    }
}


