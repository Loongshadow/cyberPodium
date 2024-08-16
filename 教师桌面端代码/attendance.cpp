#include "attendance.h"
#include "ui_attendance.h"

attendance::attendance(QWidget *parent,QString uID,QString cID) :
    QWidget(parent),
    ui(new Ui::attendance)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/imgPool/logo.png"));
    userID = uID;
    courseID = cID;
    stuNameList.clear();
    stuIDList.clear();
    stuAttList.clear();
}

attendance::~attendance()
{
    delete ui;
}

void attendance::initData(){
    //①首先获取本课程同学名单和考勤历史记录
    if(stuIDList.size()==0){
        QString urlapp = "getCourseAttendance.php";
        QJsonObject obj;
        obj.insert("msg","needHistoryAttendance");
        obj.insert("userID",userID.toInt());
        obj.insert("courseID",courseID.toInt());
        postServer (urlapp,obj);
        //important code for every connet server func
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
                        int index = -1;
                        int testToday = 0;  //0:never test| 1:test->istoday | 2:test->noToday
                        while(true){
                            index++;
                            if(!jsonObject.contains(QString::number(index))){
                                qDebug()<<"index:"<<index;
                                break;
                            }
                            QJsonObject myObj = jsonObject.value(QString::number(index)).toObject();
                            stuIDList.append(myObj["0"].toString());
                            stuNameList.append(myObj["1"].toString());
                            if(testToday==0){
                                QDate currentDate = QDate::currentDate();
                                QDate compareDate = QDate::fromString(myObj["3"].toString(), "yyyy-MM-dd");
                                qDebug()<<"current:"<<currentDate<<" "<<"compare:"<<compareDate<<endl;
                                if(currentDate == compareDate)
                                    testToday = 1;
                                else testToday = 2;
                            }
                            if(testToday == 1){
                                stuAttList.append(myObj["4"].toString().right(1).at(0));
                            }else{
                                stuAttList.append("0");
                            }
                        }
                        mReply->deleteLater();

                        QTimer::singleShot(0, this, [this]() {
                        //②从本课程临时考勤表获取实时考勤信息
                            QString myurlapp = "getCourseCurrentAttendance.php";
                            QJsonObject myobj;
                            myobj.insert("msg","needCurrentAttendance");
                            myobj.insert("userID",userID.toInt());
                            myobj.insert("courseID",courseID.toInt());
                            postServer (myurlapp,myobj);
                            QObject::connect(mReply, &QNetworkReply::finished, [&]() {
                                if (mReply->error() == QNetworkReply::NoError) {
                                    // 读取响应数据
                                    QByteArray responseData = mReply->readAll();
                                    QString response = QString::fromUtf8(responseData);
                                    qDebug() << "Server Response: " << response;
                                    // 解析 JSON 数据
                                    // 获取数据
                                    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);
                                    if (!jsonDocument.isNull()) {
                                        if (jsonDocument.isObject()) {
                                            QJsonObject jsonObject = jsonDocument.object();
                                            // 处理响应数据
                                            int index = -1;
                                            while(true){
                                                index++;
                                                if(!jsonObject.contains(QString::number(index))){
                                                    qDebug()<<"index:"<<index;
                                                    break;
                                                }
                                                QJsonObject myObj = jsonObject.value(QString::number(index)).toObject();
                                                for(int i=0;i<stuIDList.size();i++){
                                                    QString mystuID = myObj["0"].toString();
                                                    if(mystuID == stuIDList[i]){
                                                        if(stuAttList[i]=="0")
                                                            stuAttList[i]=myObj["1"].toString();
                                                        break;
                                                    }
                                                }
                                            }
                                            //画表
                                            drawTable();
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
                            });
                        });
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
        });
    }else{
    //②从本课程临时考勤表获取实时考勤信息
        QString urlapp = "getCourseCurrentAttendance.php";
        QJsonObject obj;
        obj.insert("msg","needCurrentAttendance");
        obj.insert("userID",userID.toInt());
        obj.insert("courseID",courseID.toInt());
        postServer (urlapp,obj);
        //②从本课程临时考勤表获取实时考勤信息
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
                        int index = -1;
                        while(true){
                            index++;
                            if(!jsonObject.contains(QString::number(index))){
                                qDebug()<<"index:"<<index;
                                break;
                            }
                            QJsonObject myObj = jsonObject.value(QString::number(index)).toObject();
                            for(int i=0;i<stuIDList.size();i++){
                                QString mystuID = myObj["0"].toString();
                                if(mystuID == stuIDList[i]){
                                    if(stuAttList[i]=="0")
                                        stuAttList[i]=myObj["1"].toString();
                                    break;
                                }
                            }
                        }
                        //画表
                        drawTable();
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
        });
    }
}

void attendance::postServer(QString urlappend,QJsonObject obj){
    QString serverurl = "http://43.138.243.228/cyberpodium/PC_interface/";
    serverurl.append(urlappend);
    QJsonDocument document;
    mNetworkManager = new QNetworkAccessManager(this);
    QNetworkRequest _quest;
    _quest.setUrl((QUrl(serverurl)));
    _quest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    document.setObject(obj);
    QByteArray _postData = document.toJson(QJsonDocument::Compact);//以Json字符串的方式传参
    mReply = mNetworkManager->post(_quest,_postData);
}

void attendance::drawTable(){
    ui->tableWidgetAttendance->setRowCount(stuIDList.size());
    for(int i=0;i<stuIDList.size();i++){
        for(int j=0;j<3;j++){
            QTableWidgetItem *item = NULL;
            switch (j) {
            case 0:
                item = new QTableWidgetItem(stuNameList[i]);
                break;
            case 1:
                item = new QTableWidgetItem(stuIDList[i]);
                break;
            case 2:
                item = new QTableWidgetItem(stuAttList[i]);
                break;
            }
            ui->tableWidgetAttendance->setItem(i, j, item);
        }
    }
}
