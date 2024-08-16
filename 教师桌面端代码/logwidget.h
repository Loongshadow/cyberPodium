#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include "mainwindow.h"
//network
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class LogWidget;
}

//QT_BEGIN_NAMESPACE
//namespace Ui { class LogWidget; }
//QT_END_NAMESPACE

class LogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = nullptr);
    ~LogWidget();
    //static MainWindow* mainWidget;

signals:
    //----------登录注册再主页面-----------
    //void login(); //登录主界面信号
    //void close_window(); //关闭登录界面信号

private slots:
    void on_login_clicked();
    //void on_CloseBtn_clicked();
    //void mouseMoveEvent(QMouseEvent *event) override;
    //void mousePressEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;

    void on_switchBtn_clicked();

    void on_registerBtn_clicked();

    void on_CloseBtn_clicked();
private:
    Ui::LogWidget *ui;
    MainWindow *mainWidget;
    QPoint z;
    //tool function
    QString LogRegStatus;
    void change2login();
    void change2register();
    //network
    QNetworkAccessManager* mNetworkManager;
    QNetworkReply* mReply;
    int option; //for register&login :0 getnewID|1: register|2: login
    //void netRequestID();    //
    void netRegister(int);     //注册请求  请求新账号

};

#endif // LOGWIDGET_H
