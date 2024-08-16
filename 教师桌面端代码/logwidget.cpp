#include "logwidget.h"
#include "ui_logwidget.h"

#include <QGraphicsDropShadowEffect>  //等待研究
#include <QString>

LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWidget)
{
    mainWidget = NULL;
    LogRegStatus = "register";
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/imgPool/logo.png"));
    //--------------先注册登录再主页
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QGraphicsDropShadowEffect *shadow  = new QGraphicsDropShadowEffect();
    //设置圆角
    shadow->setBlurRadius(15);
    shadow->setColor(Qt::black);
    //设置偏移量
    shadow->setOffset(0, 0);
    ui->logwidget->setGraphicsEffect(shadow);
    //show loginBtn,hide registerBtn
    ui->registerBtn->hide();
    // connect 3个信号槽
    // 触发重置按钮的信号槽连接
    //connect(ui->btn_clear,SIGNAL(clicked()),this,SLOT(btn_clear_clicked()));
    // 触发登录按钮的信号槽连接
    //connect(ui->login,SIGNAL(clicked()),this,SLOT(on_login_clicked()));
    // 发出信号后关闭登录窗口的信号槽连接
    //connect(this,SIGNAL(close_window()),this,SLOT(close()));
    //-------------------------------

}

LogWidget::~LogWidget()
{
    if(mainWidget!=NULL) delete mainWidget;
    delete ui;
}
//叉 按钮
void LogWidget::on_CloseBtn_clicked()
{
    this->close();
}
//登录按钮
void LogWidget::on_login_clicked()
{
    //QString ID ="";
    //QString password = "12345";
    //QString getID = ui->UserName->text();
    //QString getPwd = ui->PassWord->text();
    //if(ID==getID&&password==getPwd){
    //    emit(login());
    //    emit(close_window());
    //    MainWindow *mainWidget = new MainWindow;
    //    mainWidget->show();
    //    this->close();
    //}
    //else return;
//    mainWidget = new MainWindow;
//    this->close();
//    mainWidget->show();

    netRegister(2);
}

//注册按钮
void LogWidget::on_registerBtn_clicked()
{
    netRegister(1);
}

//点击登录注册转换按钮转换登录注册界面
void LogWidget::on_switchBtn_clicked()
{
    QString getText = ui->switchBtn->text();
    if(LogRegStatus=="login"){
        qDebug()<<getText;
        //netRegister(1);
        change2login();
    }else if(LogRegStatus=="register"){
        qDebug()<<getText;
        change2register();
        netRegister(0);

    }
}

//登录 注册界面转换工具函数
void LogWidget::change2login(){
    ui->PassWord->clear();
    ui->UserName->clear();
    ui->login->show();
    ui->registerBtn->hide();
    ui->switchBtn->setText("注册");
    LogRegStatus = "register";
}
void LogWidget::change2register(){
    ui->PassWord->clear();
    ui->UserName->clear();
    ui->login->hide();
    ui->registerBtn->show();
    ui->switchBtn->setText("登录");
    LogRegStatus = "login";
}

///tmp/mysql.sock
//------------network--------------
void LogWidget::netRegister(int opt){
    //opt == 0 : get new userID for register
    //    == 1 : register with userID & passwd just input
    //    == 2 : login
    option = opt;
    qDebug()<<option;
    QJsonDocument document;
    QJsonObject obj;
    QString serverurl = "http://43.138.243.228/cyberpodium/PC_interface/Register.php";
    if(option == 0){
        obj.insert("userID", 100000000);
        obj.insert("password", "nothing");
    }else if(option == 1||option == 2){
        obj.insert("userID", ui->UserName->text().toInt());
        obj.insert("password", ui->PassWord->text());
        if(option == 2) serverurl = "http://43.138.243.228/cyberpodium/PC_interface/Login.php";
    }
    mNetworkManager = new QNetworkAccessManager(this);
    QNetworkRequest _quest;
    _quest.setUrl((QUrl(serverurl)));
    _quest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    document.setObject(obj);
    QByteArray _postData = document.toJson(QJsonDocument::Compact);//以Json字符串的方式传参
    mReply = mNetworkManager->post(_quest,_postData);
    //============
    // 处理响应
    QObject::connect(mReply, &QNetworkReply::finished, [&]() {
        if (mReply->error() == QNetworkReply::NoError) {
            // 读取响应数据
            QByteArray responseData = mReply->readAll();
            QString response = QString::fromUtf8(responseData);
            qDebug() << "Server Response: " << response;
            // 解析 JSON 数据
            QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
            if (!jsonDocument.isNull()) {
                if (jsonDocument.isObject()) {
                    QJsonObject jsonObject = jsonDocument.object();
                    // 处理响应数据
                    QString responsemsg = jsonObject.value("message").toString();
                    int userID = jsonObject.value("userID").toInt();
                    QString userIDstr = QString::number(userID);

                    qDebug()<<"userID: "<<userIDstr;
                    if(option == 0){
                        //get new userID
                        ui->UserName->setText(userIDstr);
                    }else if(option == 1){
                        //register
                        if(responsemsg == "register success"){
                            //注册成功

                            change2login();
                        }else if(responsemsg == "ID exist already"){
                            //注册失败--账号已存在
                        }else{
                            //网络问题
                        }
                    }else if(option == 2){
                        //login
                        if(responsemsg == "login success"){
                            //登录成功
                            mainWidget = new MainWindow(nullptr,userIDstr);
                            this->close();
                            mainWidget->show();

                        }else if(responsemsg == "ID exist already"){
                            //登录失败
                            //change2login();
                        }
                    }
                }
            } else {
                qDebug() << "Failed to parse JSON response";
            }
        } else {
            // 处理请求错误
            qDebug() << "Network Error: " << mReply->errorString();
        }
        // 释放资源
        mReply->deleteLater();
        option = -1;
    });
}
