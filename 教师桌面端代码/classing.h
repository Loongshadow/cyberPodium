#ifndef CLASSING_H
#define CLASSING_H

#include <QWidget>
//#include "mainwindow.h"
//--------------------------
#include "attendance.h"
#include "toolbox.h"
//---------------------------
#include <QToolButton>
#include <QMouseEvent>
#include <QMenu>
#include <QDebug>
#include <QVBoxLayout>

//------------弹幕-------------
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QDesktopWidget>
#include <qpalette>
#include <QDateTime>

//-----------------------------
//----websocket connection----
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QtWebSockets/QWebSocket>
#include <QObject>
#include <QWebSocket>


//-----------------------------

extern QPoint flowBallPos1;
extern QPoint flowBallPos2;

namespace Ui {
class Classing;
}

class DraggableToolButton;
struct LabelLinkList;
class WebSocketClient;

class Classing : public QWidget
{
    Q_OBJECT

public:
    explicit Classing(QWidget *parent = nullptr,QString uID="",QString cID="");
    ~Classing();
    QString userID;
    QString courseID;
    //test
    QNetworkAccessManager* mNetworkManager;
    QNetworkReply* mReply;
    QWebSocket *webSocket;
    static void addNewBar(QString msg,Classing *myclassing){
        myclassing->m_messageList.append(msg);
        myclassing->newBarRunning = true;
        myclassing->onTimeOut();
        myclassing->newBarRunning = false;
    }
    //attendance class
private slots:
    void on_m_draggableButton_clicked();
    void onMenuItemClicked(QAction *action);
    void onSignalReshowMain() {
        show();
    }
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//---------弹幕↓------------------

//---------弹幕↑---------------------
    //void on_pushButton_clicked();

    //void on_pushButton_2_clicked();

signals:
    void sendsignal_reshowMain();
private:
    Ui::Classing *ui;
    //MainWindow* mainWidget;

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//---------弹幕↓------------------
    void onTimeOut();
    void startNewBar(){
        closeBar = false;
        m_messageList.append("BARRAGE-START");
        newBarRunning = true;
        onTimeOut();
        newBarRunning = false;
    }
    void stopNewBar(){
        closeBar = true;
    }
    void initMessage();
    void deleteLabels();
    void displayMsgList(){
        for(int i=0;i<m_messageList.count();i++){
            qDebug()<<m_messageList.at(i);
        }
    }
    void initBrightColors();
    QStringList m_messageList;
    LabelLinkList *LLLhead;
    LabelLinkList *LLLrear;
    LabelLinkList *LLLpoint;
    bool newBarRunning;  //run <ontimeout> endless while true
    bool closeBar;       //ban barrage module while true
    int msgTime;
    QDateTime mylastTime[3];
    QDateTime mythisTime[3];
    int lastPOS[3];
    QStringList brightColors;
//---------弹幕↑----------------------
    DraggableToolButton* m_draggableButton;
    QMenu *mainMenu;

    //toolMenu
    //attandanceMenu
//--------------------------------
    attendance* attendWidget;
    QVector<QString> nameList;
    QVector<QString> idList;
    toolBox* toolbox;

//-----------php websocket test------------
    WebSocketClient * mywebsocket;
};


//---------------------------------------------------------------------------------------
//可移动悬浮球
class DraggableToolButton : public QToolButton
{
public:
    DraggableToolButton(QWidget* parent = nullptr) : QToolButton(parent)
    {
        setMouseTracking(true);
    }
protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            // 记录鼠标点击时的位置和窗口位置差值
            //m_dragStartPosition = event->globalPos() - parentWidget()->pos();
            m_dragStartPosition = event->globalPos() - pos();
            flowBallPos1 =  event->globalPos();
        }
        QToolButton::mousePressEvent(event);

    }
    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (event->buttons() & Qt::LeftButton)
        {
            // 移动控件位置
            //parentWidget()->move(event->globalPos() - m_dragStartPosition);
            move(event->globalPos() - m_dragStartPosition);
            flowBallPos2 = event->globalPos();
        }
        QToolButton::mouseMoveEvent(event);
    }
private:

    QPoint m_dragStartPosition;
};
//----------------------------------------------------------
//结构体实现 QLabel 链表
//用于onTimeOut函数中进行动态空间申请之后的挂靠，以便最后用于释放
struct LabelLinkList{
    LabelLinkList(){
        body = NULL;
        next = NULL;
    }
    QLabel* body;
    LabelLinkList *next;
};

//------------websocket client class---------------
class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(QObject *parent = nullptr,Classing* my_classing=nullptr,QString clientID="",QString courseID="")
        : QObject(parent)
    {
        this->myclassing = my_classing;
        connect(&webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
        connect(&webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
        connect(&webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);

        connect(&heartbeatTimer, &QTimer::timeout, this, &WebSocketClient::sendHeartbeat);

        // Connect to server
        QString qurl = "ws://43.138.243.228/ws?clientID=%1&courseID=%2&isTeacher=1";
        qurl = qurl.arg(clientID).arg(courseID);
        webSocket.open(QUrl(qurl));
    }

signals:
    void messageReceived(const QString &message);

public slots:
    void sendMessage(const QString &message)
    {
        webSocket.sendTextMessage(message);
    }
    void closeConnection()
    {
        webSocket.close();
    }
private slots:
    void onConnected()
    {
        qDebug() << "Connected to server";
        // Start heartbeat timer when connected
        heartbeatTimer.start(15000); // 15 seconds
    }

    void onDisconnected()
    {
        qDebug() << "Disconnected from server";
        // Stop heartbeat timer when disconnected
        heartbeatTimer.stop();
    }

    void onTextMessageReceived(const QString &message)
    {
        qDebug() << "Message received:" << message;
        emit messageReceived(message);
        if(myclassing!=NULL&&message!="Hello, heartbeat!")
            Classing::addNewBar(message,myclassing);
    }

    void sendHeartbeat()
    {
        qDebug() << "Sending heartbeat";
        // Send a heartbeat message to the server
        webSocket.sendTextMessage("heartbeat");
    }

private:
    Classing *myclassing;
    QWebSocket webSocket;
    QTimer heartbeatTimer;
};



#endif // CLASSING_H
