#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent,QString idfromlogin)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    classWidget = NULL;
    inClassWidget = NULL;
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/imgPool/logo.png"));


    userMsgLineReadOnly(); //使不可编辑
    //--------课程列表-------------------
    ui->courselist_wdg->setIconSize(QSize(70,70));
    setCentralWidget(ui->centralWidget);
    //--------network------------
    userID = idfromlogin;
    getUserMsg();
    connect(ui->courselist_wdg, &QListWidget::itemClicked, this, &MainWindow::on_courseList_clicked);


}

MainWindow::~MainWindow()
{
    if(classWidget!=NULL)delete classWidget;
    if(inClassWidget!=NULL)delete inClassWidget;
    delete ui;
}
//选课-> 上课
void MainWindow::on_lesson_btn_clicked()
{
    this->hide();
    classWidget = new Classing(nullptr,userID,currentCourseID);
    classWidget->setWindowFlags(classWidget->windowFlags()|Qt::WindowStaysOnTopHint);
    classWidget->showFullScreen();
    connect(classWidget,SIGNAL(sendsignal_reshowMain()),this,SLOT(show()));
}
//选课，再后续进行操作
void MainWindow::on_courseList_clicked(){
    QString courseMsg = ui->courselist_wdg->currentItem()->data(Qt::DisplayRole).toString();
    ui->select_view->setText(courseMsg);
    currentCourseIndex = ui->courselist_wdg->currentIndex().row();
    currentCourseID = courseIDList[currentCourseIndex];
}
//更改用户信息部件的仅读/可编辑
void MainWindow::userMsgLineReadOnly(){
    if(ui->lineEditNickname->isReadOnly()){
        ui->lineEditNickname->setReadOnly(false);ui->lineEditWeChat->setReadOnly(false);
        ui->lineEditQQ->setReadOnly(false);ui->lineEditEmail->setReadOnly(false);
        ui->lineEditPhone->setReadOnly(false);ui->lineEditOffice->setReadOnly(false);
        ui->textEditRemarks->setReadOnly(false);
        ui->lineEditAccount->setReadOnly(true);
    }else{
        ui->lineEditNickname->setReadOnly(true);ui->lineEditWeChat->setReadOnly(true);
        ui->lineEditQQ->setReadOnly(true);ui->lineEditEmail->setReadOnly(true);
        ui->lineEditPhone->setReadOnly(true);ui->lineEditOffice->setReadOnly(true);
        ui->textEditRemarks->setReadOnly(true);
        ui->lineEditAccount->setReadOnly(true);
    }
}

//network
//edit and save user's msg
//编辑修改用户信息
void MainWindow::on_EditMsgBtn_clicked()
{
    userMsgLineReadOnly();
    if(ui->lineEditNickname->isReadOnly()){ //保存修改信息到服务器
        ui->EditMsgBtn->setText("修改用户信息");
        if(ui->lineEditNickname->text().length()==0){
            qDebug()<<"nickname is null!!!";
            getUserMsg();
            return;
        }
        QString urlapp = "changeUserMsg.php";
        QJsonObject obj;
        obj.insert("msg","changeUserMag");
        obj.insert("ID",userID.toInt());
        obj.insert("Name",ui->lineEditNickname->text());
        obj.insert("Wechat",ui->lineEditWeChat->text());
        obj.insert("QQ",ui->lineEditQQ->text());
        obj.insert("Phone",ui->lineEditPhone->text());
        obj.insert("Office",ui->lineEditOffice->text());
        obj.insert("EMail",ui->lineEditEmail->text());
        obj.insert("Notes",ui->textEditRemarks->toPlainText());
        postServer(urlapp,obj);
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
                        QString changeResult = jsonObject.value("status").toString();
                        if(changeResult=="success"){
                            qDebug()<<"change user msg success!";
                        }else{
                            qDebug()<<"change user msg unsuccessful";
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
        });

    }else{                                  //对用户个人信息进行修改
        ui->EditMsgBtn->setText("保存修改内容");
    }

}
//get user massage & user's courselist when into mainwindow,
void MainWindow::getUserMsg(){
    QString urlapp = "MainWindMsg.php";
    QJsonObject obj;
    obj.insert("msg","needusermsg&coursemsg");
    obj.insert("userID",userID.toInt());
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
                    //-------------user msg--------------
                    QString responsemsg = jsonObject.value("message").toString();
                    QString userID = jsonObject.value("userID").toString();
                    QString userName = jsonObject.value("username").toString();
                    QString userWechat = jsonObject.value("wechat").toString();
                    QString userQQ = jsonObject.value("QQ").toString();
                    QString userMail = jsonObject.value("mail").toString();
                    QString userPhoneNumber = jsonObject.value("telenumber").toString();
                    QString userOffice = jsonObject.value("office").toString();
                    QString userNotes = jsonObject.value("notes").toString();
                    ui->lineEditNickname->setText(userName);
                    ui->lineEditAccount->setText(userID);
                    ui->lineEditWeChat->setText(userWechat);
                    ui->lineEditQQ->setText(userQQ);
                    ui->lineEditEmail->setText(userMail);
                    ui->lineEditPhone->setText(userPhoneNumber);
                    ui->lineEditOffice->setText(userOffice);
                    ui->textEditRemarks->setText(userNotes);
                    //-----------user course-------------------
                    int index = -1;
                    courseList.clear();
                    courseIDList.clear();
                    ui->courselist_wdg->clear();
                    while(true){
                        index++;
                        if(!jsonObject.contains(QString::number(index))){
                            qDebug()<<"index:"<<index;
                            break;
                        }
                        QJsonObject courseObj = jsonObject.value(QString::number(index)).toObject();
                        mycourse course;
                        course.courseID = courseObj["0"].toString(); //courseID
                        courseIDList.append(course.courseID);
                        course.courseName = courseObj.value("1").toString(); //courseName
                        course.courseTime = courseObj.value("2").toString(); //courseTime
                        course.courseLoca = courseObj.value("3").toString(); //courseLocation
                        course.courseBuilder = courseObj.value("4").toString(); //courseBuilder
                        course.courseNotes = courseObj.value("5").toString(); //courseNotes
                        courseList.append(course);
                    }

                    ui->courselist_wdg->addItems(courseIDList);
                    qDebug()<<"item count:"<<ui->courselist_wdg->count();
                    for(int i =0 ; i < ui->courselist_wdg->count() ; i++){
                        QListWidgetItem * item = ui->courselist_wdg->item(i);
                        //QString filename = ":/images/" + fruitList[i].toLower()+  ".png";
                        //item->setIcon(QIcon(filename));
                        QString showData = courseList[i].courseName;
                        showData.append("\t");showData.append(courseList[i].courseTime);
                        showData.append("\t");showData.append(courseList[i].courseLoca);
                        showData.append("\t");showData.append(courseList[i].courseBuilder);
                        showData.append("\t");showData.append(courseList[i].courseNotes);
                        item->setData(Qt::DisplayRole,showData);
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
    });

}

//used for public
//to connect server and post server
void MainWindow::postServer(QString urlappend,QJsonObject obj){
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



//删除所选课程
void MainWindow::on_delete_btn_clicked()
{

    //waiting to code
    //to judge that if you are just a helper?nodelete:continue;
    //show window to make sure delete the course;
    QString urlapp = "changeCourse.php";
    QJsonObject obj;
    obj.insert("courseID",currentCourseID);
    obj.insert("userID",userID.toInt());
    obj.insert("option","deletecourse");
    postServer(urlapp,obj);
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
                    QString deleteresult = jsonObject.value("status").toString();
                    if(deleteresult=="success"){
                        qDebug()<<"delete course success!";
                        mReply->deleteLater();
                        QTimer::singleShot(0, this, [this]() {
                            // 这里是 mReply 被删除后要执行的代码
                            qDebug() << "Operation after mReply deletion";
                            getUserMsg();
                            // ... 其他逻辑 ...
                        });
//                        QListWidgetItem* itemToDelete = ui->courselist_wdg->takeItem(currentCourseIndex);
//                        delete itemToDelete; // 释放 QListWidgetItem 的内存
//                        courseIDList.removeAt(currentCourseIndex);
//                        courseList.removeAt(currentCourseIndex);
                    }else{
                        qDebug()<<"delete course unsuccessful";
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
        if(mReply!=nullptr)
            mReply->deleteLater();
    });

}

//新建课程
void MainWindow::on_new_btn_clicked()
{
    //弹窗输入课程信息
    //填信息 | 课程名不能为空
    QString courseName;
    QString time;
    QString place;
    QString note;
    // 创建对话框
    QDialog dialog;
    dialog.setWindowTitle("添加课程");
    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    // 创建标签和输入框
    QLabel *courseLabel = new QLabel("课程:");
    QLineEdit *courseEdit = new QLineEdit();
    layout->addWidget(courseLabel);
    layout->addWidget(courseEdit);

    QLabel *timeLabel = new QLabel("时间:");
    QLineEdit *timeEdit = new QLineEdit();
    layout->addWidget(timeLabel);
    layout->addWidget(timeEdit);

    QLabel *placeLabel = new QLabel("地点:");
    QLineEdit *placeEdit = new QLineEdit();
    layout->addWidget(placeLabel);
    layout->addWidget(placeEdit);

    QLabel *noteLabel = new QLabel("备注:");
    QTextEdit *noteEdit = new QTextEdit();
    layout->addWidget(noteLabel);
    layout->addWidget(noteEdit);

    // 创建提交按钮
    startAddNewcourse = false;
    QPushButton *okButton = new QPushButton("确定");
    connect(okButton, &QPushButton::clicked, [&]() {
        // 在这里处理用户输入的信息
        startAddNewcourse = true;
        courseName = courseEdit->text();
        time = timeEdit->text();
        place = placeEdit->text();
        note = noteEdit->toPlainText();

        if(courseName.length()==0){
            return;
        }
        //关闭对话框
        dialog.accept();
    });
    // 将按钮添加到布局中
    layout->addWidget(okButton);
    // 显示对话框
    dialog.exec();
    //important code for every connet server func
    if(startAddNewcourse){
        //等待成功与否

        QString urlapp = "changeCourse.php";
        QJsonObject obj;
        //obj.insert("courseID",currentCourseID);
        obj.insert("userID",userID.toInt());
        obj.insert("option","addcourse");
        obj.insert("cname",courseName);
        obj.insert("ctime",time);
        obj.insert("clocation",place);
        obj.insert("cbuilder",ui->lineEditNickname->text());
        obj.insert("cnote",note);
        postServer(urlapp,obj);
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
                    QString addresult = jsonObject.value("status").toString();
                    if(addresult=="success"){
                        qDebug()<<"add course success!";
                        mReply->deleteLater();
                        QTimer::singleShot(0, this, [this]() {
                            // 这里是 mReply 被删除后要执行的代码
                            qDebug() << "Operation after mReply deletion";
                            getUserMsg();
                            // ... 其他逻辑 ...
                        });
                        //QListWidgetItem* newitem = new QListWidgetItem;
                        //QString showData = courseName;
//                        showData.append("\t");showData.append(time);
//                        showData.append("\t");showData.append(place);
//                        showData.append("\t");showData.append(ui->lineEditNickname->text());
//                        showData.append("\t");showData.append(note);
//                        //newitem->setData(Qt::DisplayRole,showData);
                        //newitem->setText(showData);
                        //ui->courselist_wdg->addItem(newitem);
                        //ui->courselist_wdg->setCurrentRow(ui->courselist_wdg->count()-1);

                    }else{
                        qDebug()<<"add course unsuccessful";
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
        if(mReply!=nullptr)
            mReply->deleteLater();
        });
    }

}

//选择课程 进入课程
void MainWindow::on_enter_btn_clicked()
{
    this->hide();
    inClassWidget = new inClass(nullptr,userID,currentCourseID);
    inClassWidget->show();
    //classWidget->setWindowFlags(classWidget->windowFlags()|Qt::WindowStaysOnTopHint);
    //classWidget->showFullScreen();
    connect(inClassWidget,SIGNAL(sendsignal_reshowMain()),this,SLOT(onSignalReshowMain()));
}

