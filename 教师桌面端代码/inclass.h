#ifndef INCLASS_H
#define INCLASS_H

#include <QWidget>

#include <QVector>
#include <QDialog>
#include <QCloseEvent>
#include <QTimer>
//----network----
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class inClass;
}

struct attendanceItem;
class inClass : public QWidget
{
    Q_OBJECT

public:
    explicit inClass(QWidget *parent = nullptr,QString uID="",QString cID="");
    ~inClass();

    QString userID;
    QString courseID;
private slots:

    void on_btnEditCourse_clicked();

    void on_btnEditRemarks_clicked();

signals:
    void sendsignal_reshowMain();
private:
    Ui::inClass *ui;
    QNetworkAccessManager* mNetworkManager; //网络
    QNetworkReply* mReply;                  //网络
    QVector<attendanceItem> stuList;        //存储学生考勤列表
    void getCourseMsg();
    void getAttendanceMsg();
    void postServer(QString ,QJsonObject);
    void alertCourseMsgLineStatu(); //修改课程信息栏只读/编辑状态
    void serverSaveCourseMsg(QString ,QString ,QString ,QString ,QString );     //post 令服务器保存课程信息
    void serverSaveAttendanceRemark();//保存编辑的考勤记录备注
    QVector<attendanceItem> beforesList;//存储编辑前表中数据
    QVector<attendanceItem> afterList;  //存储编辑后表中数据  用于判断是否进行了编辑


protected:
    void closeEvent(QCloseEvent *event) override {
        // 在这里执行你的代码
        emit sendsignal_reshowMain();
        // 如果你不想关闭窗口，可以调用 event->ignore();
        // 否则，调用 event->accept(); 以继续关闭过程
        event->accept();
    }
};

struct attendanceItem{
    attendanceItem(){
        stuID="";stuName="";stuAttendanceRecord="";stuRemark="";
    }
    QString stuID;
    QString stuName;
    QString stuAttendanceRecord;
    QString stuRemark;
};

#endif // INCLASS_H
