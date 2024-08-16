#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <QWidget>

#include <QVector>
#include <QString>
#include <QDate>
#include <QCloseEvent>
//network
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


namespace Ui {
class attendance;
}

class attendance : public QWidget
{
    Q_OBJECT

public:
    explicit attendance(QWidget *parent = nullptr,QString uID="",QString cID="");
    ~attendance();
    //
    QString userID;
    QString courseID;
    //获取考勤信息
    void initData();
    void drawTable();
    //更新考勤信息
    void updateData();
    //
    QNetworkAccessManager* mNetworkManager;
    QNetworkReply* mReply;
    void postServer(QString,QJsonObject);
    QVector<QString> stuNameList;
    QVector<QString> stuIDList;
    QVector<QString> stuAttList;
signals:
    void sendsignal_reshowMain();
private:
    Ui::attendance *ui;
    //存储表格数据

protected:
    void closeEvent(QCloseEvent *event) override {
        // 在这里执行你的代码
        //emit sendsignal_reshowMain();
        // 如果你不想关闭窗口，可以调用 event->ignore();
        hide();
        event->ignore();
        // 否则，调用 event->accept(); 以继续关闭过程
        //event->accept();
    }
};

#endif // ATTENDANCE_H
