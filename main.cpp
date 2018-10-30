#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QTime>
#include <QDebug>
#include <QScreen>
#include <QList>
#include <danmu.h>
#include <QDesktopWidget>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile stylesheet("D:\\projects\\QT\\Danmaku\\style01.qss");
    stylesheet.open(QFile::ReadOnly);
    QString setSheet = stylesheet.readAll();
    a.setStyleSheet(setSheet);
    QFont f("Microsoft YaHei");
    a.setFont(f);

    MainWindow w;
    w.show();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    return a.exec();
}


