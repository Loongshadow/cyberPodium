#include "classing.h"
#include "ui_classing.h"
#include "logwidget.h"



//-----------------------------------------

QPoint flowBallPos1(100,100);
QPoint flowBallPos2(100,100);

QAction *menuItemBar;

Classing::Classing(QWidget *parent,QString uID,QString cID) :
    QWidget(parent),
    ui(new Ui::Classing)
{
    mywebsocket = new WebSocketClient(nullptr,this,uID,cID);
    userID = uID;
    courseID = cID;
    attendWidget = NULL;
    //LabelLinkList lllhead;
    //LLLhead = &lllhead;
    //LLLrear = &lllhead;
    //LLLpoint = NULL;
    //----------移动按钮---------------------
    QVBoxLayout* layout = new QVBoxLayout(this);
    m_draggableButton = new DraggableToolButton(this);
    m_draggableButton->setFixedSize(100, 100);
    layout->addWidget(m_draggableButton);
    layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    connect(m_draggableButton, &QToolButton::clicked, this, &Classing::on_m_draggableButton_clicked);
    //----------------------------------------
    //-----------主菜单------------------------
    attendWidget = new attendance(nullptr,userID,courseID);
    attendWidget->initData();
    mainMenu = new QMenu(this);
    mainMenu->addAction("下课");
    mainMenu->addAction("考勤");
    mainMenu->addAction("工具");
    menuItemBar=mainMenu->addAction("关闭弹幕");
    connect(mainMenu, &QMenu::triggered, this, &Classing::onMenuItemClicked);
    //
    //mainMenu->setStyleSheet("QMenu { width: 150px;height:50;}QMenu::item { font-size: 14px; }");
    //----------------------------------------

    ui->setupUi(this);


    //--------------transparent/窗口透明-----------
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette = QPalette();
    palette.setColor(QPalette::Background, QColor(0x00,0xFF,0x00,0x00));
    setPalette(palette);
    //---------------------------------------
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //--------------弹幕test------------------//等待改进
    initBrightColors();
    LLLhead = new LabelLinkList;
    LLLrear = LLLhead;
    LLLpoint = NULL;
    msgTime = -1;
    mylastTime[0] = QDateTime::currentDateTime();
    mylastTime[1] = QDateTime::currentDateTime();
    mylastTime[2] = QDateTime::currentDateTime();
    lastPOS[0] = QDesktopWidget().width()+500;
    lastPOS[1] = QDesktopWidget().width()+500;
    lastPOS[2] = QDesktopWidget().width()+500;
    closeBar = false;
    newBarRunning = false;
    initMessage(); //test

    //---------------------------------------

}

Classing::~Classing()
{

    //delete mainWidget;
    if(mainMenu!=NULL) delete mainMenu;
    if(m_draggableButton!=NULL)delete m_draggableButton;
    //----------------------
    if(attendWidget!=NULL) delete attendWidget;
    if(toolbox!=NULL) delete toolbox;
    //--------弹幕--------------
    deleteLabels();
    if(LLLhead!=NULL)delete LLLhead;
    delete ui;
    //websocket
    if(mywebsocket!=NULL) delete mywebsocket;
}

void Classing::on_m_draggableButton_clicked(){

    if(flowBallPos1==flowBallPos2){
        mainMenu->exec(m_draggableButton->mapToGlobal(QPoint(0, m_draggableButton->height())));
        //mainMenu->show();
    }
    else{
        flowBallPos2=flowBallPos1;
    }

}

//悬浮球菜单 菜单项点击事件
void Classing::onMenuItemClicked(QAction *action) {
    if (action->text() == "下课") {
        mywebsocket->closeConnection();
        this->close();
        emit sendsignal_reshowMain();
        if(attendWidget!=NULL) delete attendWidget;
        stopNewBar();
        deleteLabels();//释放弹幕空间
    } else if (action->text() == "关闭弹幕") {
        stopNewBar();
        initMessage(); //test
        deleteLabels();
        menuItemBar->setText("打开弹幕");
    } else if (action->text() == "打开弹幕"){
        startNewBar();
        initMessage(); //test
        menuItemBar->setText("关闭弹幕");
    }else if (action->text() == "考勤") {
        //this->hide();
        if(attendWidget==nullptr) attendWidget = new attendance(nullptr,userID,courseID);
        attendWidget->initData();
        attendWidget->show();
        //connect(attendWidget,SIGNAL(sendsignal_reshowMain()),this,SLOT(()));


        // 处理 "考勤" 菜单项的逻辑
    } else if (action->text() == "工具") {
        if(nameList.size()==0&&attendWidget!=nullptr){
            if(attendWidget->stuNameList.size()!=0){
                nameList = attendWidget->stuNameList;
                idList = attendWidget->stuIDList;
            }
        }
        toolbox = new toolBox(nullptr,nameList,idList);
        toolbox->show();

    }
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//-------------弹幕-------------------------
void Classing::onTimeOut(){
    //QMetaObject::invokeMethod(this, "performUIOperation", Qt::QueuedConnection);
    if(!closeBar&&newBarRunning&&!m_messageList.isEmpty()){
        QString msg = m_messageList.takeFirst();//取出并弹出
        msgTime++;
        if(msgTime==9999) deleteLabels();
        QLabel *qlabelpoint = new QLabel(this);
        LLLpoint = new LabelLinkList;
        LLLpoint->body = qlabelpoint;
        if(LLLhead==LLLrear) qDebug()<<"here LLLhead == LLLrear";
        if(LLLhead->next==NULL) qDebug()<<"here LLLhead->next is NULL";
        LLLrear->next = LLLpoint;
        LLLrear = LLLrear->next;
        qDebug()<<"HI";

        LLLpoint->body->setMinimumWidth(500);
        //Label 背景透明化/无边框窗口/置顶
        LLLpoint->body->setAttribute(Qt::WA_TranslucentBackground);
        LLLpoint->body->setWindowFlags(LLLpoint->body->windowFlags() | Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);
        //LLLpoint->body->setStyleSheet(QString("color:%1;")
        //                                  .arg(QColor::colorNames().at(QRandomGenerator::global()->bounded(QColor::colorNames().count()))));
        int colorAt = QRandomGenerator::global()->bounded(0,brightColors.count()-1);
        QString randomColor = brightColors.at(colorAt);
        LLLpoint->body->setStyleSheet(QString("color:%1;").arg(randomColor));
        //字体 加粗/字体/像素
        QFont font;
        font.setBold(true);
        font.setFamily(QStringLiteral("幼圆"));
        font.setPixelSize(50);
        LLLpoint->body->setFont(font);
        //随机设置文字 //或要改成顺序播放？
        LLLpoint->body->setText(msg);
        qDebug()<<"displaying: "<<msg;
        //移动起始位置
        int msgTimeMod3 = msgTime%3;
        int randomAddStartPos = QRandomGenerator::global()->bounded(0,200);
        //LLLpoint->body->move(QDesktopWidget().width()+1000,(msgTime%4)*50+10);
        mythisTime[msgTimeMod3] = QDateTime::currentDateTime();
        qint64 timeDiffer = mylastTime[msgTimeMod3].msecsTo(mythisTime[msgTimeMod3]);
        qDebug()<<timeDiffer;

        if(timeDiffer>1000)
            lastPOS[msgTimeMod3] = QDesktopWidget().width()+500+randomAddStartPos;
        else{
            lastPOS[msgTimeMod3] = lastPOS[msgTimeMod3]+(100*msg.length());
        }
        LLLpoint->body->move(lastPOS[msgTimeMod3],(msgTimeMod3)*50+10);
        mylastTime[msgTimeMod3] = mythisTime[msgTimeMod3];
        LLLpoint->body->show();
        QPropertyAnimation *animation = new QPropertyAnimation(LLLpoint->body, "pos", this);
        //每循环一次重新设置字体颜色以及显示内容
        //connect(animation, &QAbstractAnimation::currentLoopChanged, [=]() {
        //    LLLpoint->body->setText(m_messageList.at(QRandomGenerator::global()->bounded(m_messageList.count())));
        //    LLLpoint->body->setStyleSheet(QString("color:%1;")
        //                                     .arg(QColor::colorNames().at(QRandomGenerator::global()->bounded(QColor::colorNames().count()))));
        //});
        //随机起始位置
        //if 逻辑防止弹幕停止在可视屏幕中
        //animation->setStartValue(QPoint(LLLpoint->body->x(), LLLpoint->body->y()));
        //animation->setEndValue(QPoint(LLLpoint->body->x()-10000,  LLLpoint->body->y()));
        int toolEndX = LLLpoint->body->x()-10000;
        int toolBarLen = -(100*msg.length());
        qDebug()<<"变量"<<toolEndX<<" "<<toolBarLen;
            if((toolEndX<toolBarLen)){
            animation->setStartValue(QPoint(LLLpoint->body->x(), LLLpoint->body->y()));
            animation->setEndValue(QPoint(LLLpoint->body->x()-10000,  LLLpoint->body->y()));
        }else{
            animation->setStartValue(QPoint(LLLpoint->body->x()-10000, LLLpoint->body->y()));
            animation->setEndValue(QPoint(LLLpoint->body->x()-20000,  LLLpoint->body->y()));
        }
        //速度：动画在该时间内走完全程-》相同时间下路程长的速度快
        animation->setDuration(40000);
        animation->start();
    }
//    if(msgTime%4==0){
//        int waitTime = QRandomGenerator::global()->bounded(800,1100);
//        QTimer::singleShot(waitTime,this,&Classing::onTimeOut);
//    }else{
//        int waitTime = QRandomGenerator::global()->bounded(300,800);
//        QTimer::singleShot(waitTime,this,&Classing::onTimeOut);
//    }
}

void Classing::initMessage()
{
    qDebug()<<"initmessage ing......";
    QFile file("data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    in.setCodec("UTF-8"); // 设置编码为UTF-8
    while (!in.atEnd())
    {

            QString line = in.readLine();
            m_messageList.append(line);

        if(!closeBar){
            newBarRunning = true;
            onTimeOut();
            newBarRunning = false;
        }
    }
    file.close();
    return;
}

void Classing::initBrightColors(){
//    brightColors << "#FF1493" << "#9400D3" << "#FF00FF" << "#00FF00" << "#00BFFF" << "#FF69B4"
//                 << "#8A2BE2" << "#DC143C" << "#00FF7F" << "#FF8C00" << "#1E90FF" << "#FF00FF"
//                 << "#7CFC00" << "#FF4500" << "#DAA520" << "#228B22" << "#BA55D3" << "#4169E1"
//                 << "#FF6347" << "#FFA500" << "#ADFF2F" << "#40E0D0";
/*<< "#F4F4F4"<< "#c6dafc" << "#fce8b2"<<  "#b7e1cd"<< "#f8f9fa" << "#f5f5f5" << "#e0e0e0" << "#bdc1c6"
 << "#e8eaf6" << "#c5cae9"<< "#ffebee" << "#9e9e9e" << "#616161"<< "#f3e5f5" << "#e1bee7"<< "#ede7f6" << "#d1c4e9" << "#b39ddb"
   << "#e3f2fd" << "#e1f5fe"
*/
    brightColors<< "#FB7299" << "#23ADE5" << "#6dc781"  << "#f4c7c3" << "#e67c73"
                << "#c53929"  << "#7baaf7" << "#4285f4" << "#1a73e8" << "#3367d6"<< "#fce4ec"
                << "#57bb8a" << "#0f9d58" << "#0b8043" << "#f7cb4d"
                << "#f4b400" << "#f09300"
                << "#ffcdd2" << "#ef9a9a" << "#e57373"
                << "#ef5350" << "#f44336" << "#e53935" << "#d32f2f" << "#c62828" << "#b71c1c"
                << "#ff8a80" << "#ff5252" << "#ff1744" << "#d50000"  << "#f8bbd0"
                << "#f48fb1" << "#f06292" << "#ec407a" << "#e91e63" << "#d81b60" << "#c2185b"
                << "#ad1457" << "#880e4f" << "#ff80ab" << "#ff4081" << "#f50057" << "#c51162"
                 << "#ce93d8" << "#ba68c8" << "#ab47bc" << "#9c27b0"
                << "#8e24aa" << "#7b1fa2" << "#6a1b9a" << "#4a148c" << "#ea80fc" << "#e040fb"
                << "#d500f9" << "#aa00ff"  << "#9575cd"
                << "#7e57c2" << "#673ab7" << "#5e35b1" << "#512da8" << "#4527a0" << "#311b92"
                << "#b388ff" << "#7c4dff" << "#651fff" << "#6200ea"
                << "#9fa8da" << "#7986cb" << "#5c6bc0" << "#3f51b5" << "#3949ab" << "#303f9f"
                << "#283593" << "#1a237e" << "#8c9eff" << "#536dfe" << "#3d5afe" << "#304ffe"
                << "#db4437" << "#80deea" << "#00bfa5" << "#00c853"
                 << "#64dd17" << "#aeea00" << "#ffd600" << "#ffab00" << "#ff6500" <<"#dd2c00";
}


void Classing::deleteLabels(){
    if(LLLhead->next==NULL) qDebug()<<"LLLhead->next is null";
    if(LLLhead==LLLrear) qDebug()<<"LLLhead == LLLrear";
    int a=0;
    while(LLLhead->next!=NULL&&LLLhead!=LLLrear){
        a=a+1;
        LLLpoint = LLLhead->next;
        if(LLLpoint==LLLrear) LLLrear = LLLhead;
        LLLhead->next = LLLpoint->next;
        if(LLLpoint->body!=NULL){
            //LLLpoint->body->close();
            delete(LLLpoint->body);
        }
        if(LLLpoint!=NULL)
            delete LLLpoint;
    }
    qDebug()<<"this is a"<<a<<endl;
    if(LLLhead==LLLrear) qDebug()<<"Yeah!!!!";
    m_messageList.clear();
}


//------------------class for floating button-----------------------


//void Classing::on_pushButton_clicked()
//{
//    QString line = ui->lineEdit->text();
//    m_messageList.append(line);
//    if(!closeBar){
//        newBarRunning = true;
//        onTimeOut();
//        newBarRunning = false;
//    }
//}


//void Classing::on_pushButton_2_clicked()
//{

//}

