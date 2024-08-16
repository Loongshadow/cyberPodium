#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QWidget>

#include <QVector>
#include <QRandomGenerator>
#include <QDateTime>

namespace Ui {
class toolBox;
}

class toolBox : public QWidget
{
    Q_OBJECT

public:
    explicit toolBox(QWidget *parent = nullptr,const QVector<QString> &param1 = QVector<QString>(),const QVector<QString> &param2 = QVector<QString>());
    ~toolBox();
    void startRandomCall();

    QVector<QString> namelist;
    QVector<QString> idlist;
private slots:
    void on_pushButton_clicked();

private:
    Ui::toolBox *ui;
};

#endif // TOOLBOX_H
