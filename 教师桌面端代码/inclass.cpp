#include "inclass.h"
#include "ui_inclass.h"


inClass::inClass(QWidget *parent,QString uID,QString cID) :
    QWidget(parent)
    ,ui(new Ui::inClass)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/imgPool/logo.png"));
    userID = uID;
    courseID = cID;
    ui->lineEditCourseID->setText(cID);
    //初始化 课程信息和考勤记录
    getCourseMsg();
    //初始化 课程信息为只读状态
    alertCourseMsgLineStatu();
    //初始化 课程表为只读状态 编辑功能关闭状态
    ui->tableWidgetAttendance->setEnabled(false);

    //
}

inClass::~inClass()
{
    delete ui;
}

//HTTP post server for courseMsg
//write into blanks
void inClass::getCourseMsg(){
    QString urlapp = "getCourseMsg.php";
    QJsonObject obj;
    obj.insert("msg","needCourseMsg");
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
                    //-------------course msg------------
                    QString myCourseID = courseID;
                    QString myCourseName = jsonObject.value("courseName").toString();
                    QString myCourseTime = jsonObject.value("time").toString();
                    QString myCoursePlace = jsonObject.value("place").toString();
                    QString myCourseRemark = jsonObject.value("remark").toString();
                    ui->lineEditCourseName->setText(myCourseName);
                    ui->lineEditCourseID->setText(myCourseID);
                    ui->lineEditLocation->setText(myCoursePlace);
                    ui->lineEditTime->setText(myCourseTime);
                    ui->textEditNotes->setText(myCourseRemark);

                    mReply->deleteLater();
                    QTimer::singleShot(0, this, [this]() {
                        // 这里是 mReply 被删除后要执行的代码
                        getAttendanceMsg();
                        // ... 其他逻辑 ...
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
        if(mReply!=nullptr)
            mReply->deleteLater();
    });

}

//HTTP POST serve for this course's attendance records
//and write into table
void inClass::getAttendanceMsg(){
    QString urlapp = "getCourseAttendance.php";
    QJsonObject obj;
    obj.insert("msg","needAttendanceMsg");
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
                    //-------------attendance msg------------
                    //清空stuList和ui->table
                    stuList.clear();

                    int index = -1;
                    while(true){
                        index++;
                        if(!jsonObject.contains(QString::number(index))){
                            qDebug()<<"index:"<<index;
                            break;
                        }
                        QJsonObject myObj = jsonObject.value(QString::number(index)).toObject();
                        attendanceItem attitem;
                        attitem.stuID = myObj["0"].toString();
                        attitem.stuName = myObj["1"].toString();
                        attitem.stuAttendanceRecord = myObj["4"].toString();
                        attitem.stuRemark = myObj["5"].toString();

                        stuList.append(attitem);
                    }
                    //写入表格
                    ui->tableWidgetAttendance->setRowCount(stuList.size());
                    for(int i=0;i<stuList.size();i++){
                        for(int j=0;j<4;j++){
                            QTableWidgetItem *item = NULL;
                            switch (j) {
                            case 0:
                                item = new QTableWidgetItem(stuList[i].stuName);
                                break;
                            case 1:
                                item = new QTableWidgetItem(stuList[i].stuID);
                                break;
                            case 2:
                                item = new QTableWidgetItem(stuList[i].stuAttendanceRecord);
                                break;
                            case 3:
                                item = new QTableWidgetItem(stuList[i].stuRemark);
                                break;
                            }

                            ui->tableWidgetAttendance->setItem(i, j, item);
                        }
                    }
                    //QTableWidgetItem *item = new QTableWidgetItem(QString("Row %1, Column %2").arg(i).arg(j));
                    //tableWidget.setItem(i, j, item);
                    //如何把点击与表格项目联系起来

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

//used for public
//to connect server and post server
void inClass::postServer(QString urlappend,QJsonObject obj){
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

//“编辑课程信息”→“保存修改”&课程信息可修改→修改成功&“编辑课程信息”
//编辑修改课程信息
void inClass::on_btnEditCourse_clicked()
{
    if(ui->lineEditCourseName->isReadOnly()){
        //修改为可编辑状态
        alertCourseMsgLineStatu();
        ui->btnEditCourse->setText("保存修改");
    }else{
        //对信息进行保存 并令服务器保存修改
        QString mycourseName = ui->lineEditCourseName->text();
        QString mycourseTime = ui->lineEditTime->text();
        QString mycoursePlace = ui->lineEditLocation->text();
        QString mycourseRemark = ui->textEditNotes->toPlainText();
        qDebug()<<mycourseName<<endl;
        serverSaveCourseMsg(mycourseName,courseID,mycourseTime,mycoursePlace,mycourseRemark);
        //修改为只读权限
        alertCourseMsgLineStatu();
        ui->btnEditCourse->setText("编辑课程信息");
    }
}

void inClass::alertCourseMsgLineStatu(){
    if(ui->lineEditCourseName->isReadOnly()){
        ui->lineEditCourseName->setReadOnly(false);;
        ui->lineEditTime->setReadOnly(false);
        ui->lineEditLocation->setReadOnly(false);
        ui->textEditNotes->setReadOnly(false);
    }else{
        ui->lineEditCourseName->setReadOnly(true);;
        ui->lineEditTime->setReadOnly(true);
        ui->lineEditLocation->setReadOnly(true);
        ui->textEditNotes->setReadOnly(true);
        ui->lineEditCourseID->setReadOnly(true);
    }
}
//服务器保存课程信息
//参数：课程名字 ID 时间 地点 备注
void inClass::serverSaveCourseMsg(QString cN,QString cID,QString cT,QString cP,QString cR){
    QString urlapp = "changeCourse.php";
    QJsonObject obj;
    obj.insert("msg","needCourseMsg");
    obj.insert("userID",userID.toInt());
    obj.insert("courseID",cID.toInt());
    obj.insert("cname",cN);
    obj.insert("ctime",cT);
    obj.insert("clocation",cP);
    obj.insert("cnote",cR);
    obj.insert("option","alertcourse");
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
                    //-------------course msg------------
                    qDebug()<<jsonObject.value("status")<<endl;
                    qDebug()<<jsonObject.value("sql")<<endl;
                    mReply->deleteLater();
                    QTimer::singleShot(0, this, [this]() {
                        // 这里是 mReply 被删除后要执行的代码
                        getCourseMsg();
                        // ... 其他逻辑 ...
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
        if(mReply!=nullptr)
            mReply->deleteLater();
    });
}

//“启动编辑功能”->“编辑备注”→备注可编辑&“保存修改”→保存修改&“编辑备注”
void inClass::on_btnEditRemarks_clicked()
{
    ui->tableWidgetAttendance->setEnabled(true);
    QTableWidgetItem *item = ui->tableWidgetAttendance->item(0, 3);
    if(item!=NULL){
        if(item->flags() & Qt::ItemIsEditable){//若第四列可编辑，则令其不可编辑
            if(ui->btnEditRemarks->text()=="保存修改"){
                serverSaveAttendanceRemark();
            }
            ui->btnEditRemarks->setText("编辑备注");
            for (int row = 0; row < ui->tableWidgetAttendance->rowCount(); ++row) {
                for (int col = 0; col < ui->tableWidgetAttendance->columnCount(); ++col) {
                    QTableWidgetItem *item2 = ui->tableWidgetAttendance->item(row, col);
                    if (item2) {
                        item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
                    }
                }
            }
        }else{//若第四列不可编辑 则令其可编辑
            //清空beforeList 填充beforeList(主要获取第二列stuID 第四列remarks)
            beforesList.clear();
            for (int row = 0; row < ui->tableWidgetAttendance->rowCount(); ++row) {
                attendanceItem attitem;
                // 获取第二列和第四列的单元格项
                QTableWidgetItem *itemCol2 = ui->tableWidgetAttendance->item(row, 1);
                QTableWidgetItem *itemCol4 = ui->tableWidgetAttendance->item(row, 3);
                // 检查项是否存在，并获取其文本或进行其他操作
                if (itemCol2) {
                    attitem.stuID=itemCol2->text();
                }
                if (itemCol4) {
                    attitem.stuRemark=itemCol4->text();
                }
                beforesList.append(attitem);
            }

            ui->btnEditRemarks->setText("保存修改");
            for (int row = 0; row < ui->tableWidgetAttendance->rowCount(); ++row) {
                // 第一、二、三列设置为只读
                for (int col = 0; col < 3; ++col) {
                    QTableWidgetItem *myitem = ui->tableWidgetAttendance->item(row, col);
                    if (myitem) {
                        myitem->setFlags(myitem->flags() & ~Qt::ItemIsEditable);
                    }
                }

                // 第四列设置为可编辑
                QTableWidgetItem *myitem2 = ui->tableWidgetAttendance->item(row, 3);
                if (myitem2) {
                    myitem2->setFlags(myitem2->flags() | Qt::ItemIsEditable);
                }
            }
        }
    }
}

void inClass::serverSaveAttendanceRemark(){
    //清空afterList 初始化afterList
    afterList.clear();
    for (int row = 0; row < ui->tableWidgetAttendance->rowCount(); ++row) {
        attendanceItem attitem;
        // 获取第二列和第四列的单元格项
        QTableWidgetItem *itemCol2 = ui->tableWidgetAttendance->item(row, 1);
        QTableWidgetItem *itemCol4 = ui->tableWidgetAttendance->item(row, 3);
        // 检查项是否存在，并获取其文本或进行其他操作
        if (itemCol2) {
            attitem.stuID=itemCol2->text();
        }
        if (itemCol4) {
            attitem.stuRemark=itemCol4->text();
        }
        afterList.append(attitem);
    }
    //比对beforeList和afterList 判断是否进行了编辑修改
    bool isequal = true;
    for(int i=0;i<afterList.size();i++){
        if(beforesList[i].stuRemark!=afterList[i].stuRemark){
            isequal=false;
            break;
        }
    }
    //若有进行过编辑 则请求服务器对其进行保存
    if(!isequal){
        qDebug()<<"请求服务器保存修改！！！"<<endl;
        QString urlapp = "changeAttendance.php";
        QJsonObject obj;
        obj.insert("msg","updateAttendance");
        obj.insert("userID",userID.toInt());
        obj.insert("courseID",courseID.toInt());
        obj.insert("option","updateRemark");
        //使obj插入 (0,0号学生ID),(1,0号学生备注),(2,1号学生ID),(3,1号学生备注),(4,2号学生ID),(5,2号学生备注)
        for(int i=0;i<afterList.size()*2;i++){
            if((i%2)==0){
                obj.insert(QString::number(i),afterList[(i/2)].stuID);
            }else{
                obj.insert(QString::number(i),afterList[(i/2)].stuRemark);
            }
        }
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
                        //-------------course msg------------
                        qDebug()<<jsonObject.value("lastsql")<<endl;
                        mReply->deleteLater();
                        QTimer::singleShot(0, this, [this]() {
                            // 这里是 mReply 被删除后要执行的代码
                            getCourseMsg();
                            // ... 其他逻辑 ...
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
            if(mReply!=nullptr)
                mReply->deleteLater();
        });
    }
}

