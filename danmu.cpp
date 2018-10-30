#include "Danmu.h"

Danmu::Danmu(QWidget * parent,QString text,QString color,int type,QRect rect,QFont danmuFont,double Transparency,int runTime):QLabel(parent){
    DText = text;
    //this->setText(text);        //设置内容
    this->setColor(color);      //设置内容
    this->setType(type);        //设置类型
    this->setQFont(danmuFont);      //弹幕字体
    this->setTransparency(Transparency);        //弹幕透明度
    this->setRunTime(runTime);
    this->setScreenRect(rect);
    QFontMetrics metrics(this->getQFont());
    QPalette palll=QPalette();
    QString DColor = this->getColor();
    anim2 = NULL;

    //颜色字符串转化为特定的颜色
    if(DColor == "White"){
        palll.setColor(QPalette::WindowText,QColor(255,255,246,255));
        this->setQColor(QColor(255,255,246,255));
    }else if(DColor =="Red"){
        palll.setColor(QPalette::WindowText,QColor(231,0,18,255));
        this->setQColor(QColor(231,0,18,255));
    }else if(DColor =="Yellow"){
        palll.setColor(QPalette::WindowText,QColor(254,241,2,255));
        this->setQColor(QColor(254,241,2,255));
    }else if(DColor == "Green"){
        palll.setColor(QPalette::WindowText,QColor(0,152,67,255));
        this->setQColor(QColor(0,152,67,255));
    }else if(DColor == "Blue"){
        palll.setColor(QPalette::WindowText,QColor(0,160,234,255));
        this->setQColor(QColor(0,160,234,255));
    }else if(DColor == "Pink"){
        palll.setColor(QPalette::WindowText,QColor(226,2,127,255));
        this->setQColor(QColor(226,2,127,255));
    }else if(DColor == "Grass"){
        palll.setColor(QPalette::WindowText,QColor(144,195,32,255));
        this->setQColor(QColor(144,195,32,255));
    }else if(DColor == "DBlue"){
        palll.setColor(QPalette::WindowText,QColor(0,46,114,255));
        this->setQColor(QColor(0,46,114,255));
    }else if(DColor == "DYellow"){
        palll.setColor(QPalette::WindowText,QColor(240,171,42,255));
        this->setQColor(QColor(240,171,42,255));
    }else if(DColor =="DPurple"){
        palll.setColor(QPalette::WindowText,QColor(104,58,123,255));
        this->setQColor(QColor(104,58,123,255));
    }else if(DColor == "LBlue"){
        palll.setColor(QPalette::WindowText,QColor(129,193,205,255));
        this->setQColor(QColor(129,193,205,255));
    }else if(DColor =="Brown"){
        palll.setColor(QPalette::WindowText,QColor(149,119,57,255));
        this->setQColor(QColor(149,119,57,255));
    }else{
        palll.setColor(QPalette::WindowText,QColor(255,255,246,255));
        this->setQColor(QColor(255,255,246,255));
    }
    this->setPalette(palll);        //设置调色盘


    this->setFixedHeight(metrics.height()+5);
    this->setFixedWidth(metrics.width(DText)+4);

    int yy = qrand()%rect.height();
    yy = yy<(rect.height()-metrics.height()-5)?(yy):(rect.height()-metrics.height()-5); //判断字体是否显示完全
    yy = rect.y()+yy;
    int xx = rect.width()+200; //屏幕款+随机500
    xx = rect.x() + xx;

    this->move(xx,yy);
    this->setPosX(xx);      //设置弹幕水平的位置
    this->setPosY(yy);       //设置弹幕垂直位置

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);    //设置弹幕为无窗口无工具栏且呆在窗口顶端

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFocusPolicy(Qt::NoFocus);
    this->hide();

    anim2=new QPropertyAnimation(this, "pos");
    anim2->setDuration(this->getRunTime());
    anim2->setStartValue(QPoint(this->getPosX(),this->getPosY()));
    anim2->setEndValue(QPoint(-(this->width()), this->getPosY()));
    anim2->setEasingCurve(QEasingCurve::Linear);

    this->setWindowOpacity(this->getTransparency());
    this->show();
    this->repaint();
    anim2->start();
    connect(anim2,SIGNAL(finished()),this,SLOT(deleteLater()));
}

void Danmu::paintEvent(QPaintEvent *){  //弹幕绘制函数
        QPainter painter(this);     //以弹幕窗口为画布
        painter.save();
        QFontMetrics metrics(this->getQFont());     //获取弹幕字体
        QPainterPath path;      //描绘路径用
        QPen pen(QColor(0, 0, 0, 230));       //自定义画笔的样式，让文字周围有边框
        painter.setRenderHint(QPainter::Antialiasing);
        int penwidth = 4;
        pen.setWidth(penwidth);
        int len = metrics.width(DText);
        int w = this->width();
        int px = (len - w) / 2;
        if(px < 0)
        {
            px = -px;
        }
        int py = (height() - metrics.height()) / 2 + metrics.ascent();
        if(py < 0)
        {
            py = -py;
        }
        path.addText(px+2,py+2,this->getQFont(),DText);     //画字体轮廓
        painter.strokePath(path, pen);
        painter.drawPath(path);
        painter.fillPath(path, QBrush(this->getQColor()));      //用画刷填充
        painter.restore();
}

void Danmu::setScreenRect(QRect screenRect){
    this->screenrect = screenRect;
}

QRect Danmu::getScreenRect(){
    return this->screenrect;
}

Danmu::~Danmu(){
    qDebug()<<"弹幕被析构"<<endl;
}

int Danmu::getPosX(){
    return PosX;
}

int Danmu::getPosY(){
    return PosY;
}

void Danmu::setPosX(int posx){
    this->PosX = posx;
}

void Danmu::setPosY(int posy){
    this->PosY = posy;
}

QString Danmu::getColor(){
    return color;
}

int Danmu::getType(){
    return type;
}

void Danmu::setColor(QString color){
    this->color = color;
}

void Danmu::setType(int type){
    this->type = type;
}

QColor Danmu::getQColor(){
    return qcolor;
}

void Danmu::setQColor(QColor qcolor){
    this->qcolor = qcolor;
}

QFont Danmu::getQFont(){
    return danmuFont;
}

void Danmu::setQFont(QFont danmuFont){
    this->danmuFont = danmuFont;
}

void Danmu::setTransparency(double Transparency){
    this->Transparency = Transparency;
}

double Danmu::getTransparency(){
    return Transparency;
}

void Danmu::setRunTime(int runTime){
    this->runTime = runTime;
}

int Danmu::getRunTime(){
    return this->runTime;
}

QPropertyAnimation * Danmu::getanimation(){
    return anim2;
}

