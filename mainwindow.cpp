#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QtDebug>
#include <danmu.h>
#include <QTime>
#include <QScreen>
#include <QList>
#include <QDesktopWidget>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    socketThread = new SocketThread();
    socketThread->moveToThread(&socketContainerThread);
    connect(&socketContainerThread,SIGNAL(finished()),socketThread,SLOT(deleteLater()));

    connect(this, SIGNAL(socketStart(QString,int)),socketThread,SLOT(run(QString,int)));
    connect(socketThread, SIGNAL(dataReady(QString)), this, SLOT(socketDataReceive(QString)));
    connect(socketThread, SIGNAL(socketConnected()), this, SLOT(socket_Connected()));
    connect(socketThread, SIGNAL(socketDisconnected()), this, SLOT(socket_Disconnected()));
    connect(this, SIGNAL(socketSendData(QByteArray)), socketThread, SLOT(sendData(QByteArray)));

    socketContainerThread.start();

    emit socketStart("47.95.216.239", 6974);

    qDebug()<<"连接中";

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::socket_Connected(){
    qDebug() << "已连接";
//    QString iamteacher = "#iamtheteacher!";
//    emit socketSendData(iamteacher.toLocal8Bit());
}

void MainWindow::socket_Disconnected(){
    qDebug() << "已断开";
}

void MainWindow::socketDataReceive(QString msg){
    QRect screenRect;
    QList<QScreen *> screens = QGuiApplication::screens();
    int screenCount = QGuiApplication::screens().count();
    //使用count-1 来实现多显示屏适配
    screenRect = screens.at(screenCount-1)->geometry();

    qDebug() <<"Received message: " <<msg;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(msg.toLocal8Bit(), &jsonError);//解析json对象，看有没有报错


    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("type")) {  // 包含指定的 key
                QJsonValue value = object.value("type");  // 获取指定 key 对应的 value
                if (value.isString()) {  // 判断 value 是否为字符串
                    QString type = value.toString();  // 将 value 转化为字符串

                    //心跳消息
                    if(type=="ping"){
                        //构建心跳回复消息
                        qDebug() << "收到心跳包";
                        QJsonObject pong;
                        pong.insert("type", "pong");
                        QJsonDocument pongDocument;
                        pongDocument.setObject(pong);
                        QByteArray pongByteArray = pongDocument.toJson(QJsonDocument::Compact);
                        emit socketSendData(pongByteArray);

                    }

                    //弹幕消息
                    if(type=="danmu"){
                       QJsonValue contentValue = object.value("content");
                       if (contentValue.isString()) {
                           QString content = contentValue.toString();
                           qDebug() << "收到弹幕消息:" << content;
                           new Danmu(this,content,"White",1,screenRect);
                       }

                    }

                }
            }

        }
    }
}

