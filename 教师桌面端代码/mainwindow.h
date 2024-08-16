#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "classing.h"
#include "inclass.h"

#include <QVector>
#include <QDialog>
//----network----
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct mycourse;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,QString idfromlogin = "");
    ~MainWindow();
    //network
    QNetworkAccessManager* mNetworkManager;
    QNetworkReply* mReply;
    QString userID;
    QString currentCourseID;
    int currentCourseIndex;
    bool startAddNewcourse;
    void postServer(QString,QJsonObject); //public post server tool
private slots:
    void on_lesson_btn_clicked();   //上课
    void on_courseList_clicked();   //选择课程进行操作

    void on_EditMsgBtn_clicked();

    void on_delete_btn_clicked();

    void on_new_btn_clicked();

    void on_enter_btn_clicked();
    void onSignalReshowMain() {
        show();
        // 执行另一个函数
        getUserMsg();
    }
private:
   Ui::MainWindow *ui;
   Classing *classWidget;
   inClass *inClassWidget;
   //LogWidget * log_ui; //登录界面类的对象作为指针
   QStringList courseIDList;
   QVector<mycourse> courseList; //test 到时候换成结构体列表，存储课程名 地点 时间等信息
   void userMsgLineReadOnly();
   //----network---------
   void getUserMsg();    // user msg & course msg
};

struct mycourse{
   QString courseID;
   QString courseName;
   QString courseTime;
   QString courseLoca;
   QString courseBuilder;
   QString courseNotes;
};

#endif // MAINWINDOW_H
