#include "toolbox.h"
#include "ui_toolbox.h"

toolBox::toolBox(QWidget *parent,const QVector<QString> &qv1,const QVector<QString> &qv2) :
    QWidget(parent),
    ui(new Ui::toolBox)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/imgPool/logo.png"));
    namelist = qv1;
    idlist = qv2;
}

toolBox::~toolBox()
{
    delete ui;
}

void toolBox::on_pushButton_clicked()
{
    qint64 seed = QDateTime::currentMSecsSinceEpoch();
    QRandomGenerator generator(seed);
    int randomNumber = generator.bounded(namelist.size());
    if(namelist.size()>(randomNumber-1)&&idlist.size()>(randomNumber-1)){
        QString name = namelist[randomNumber];
        QString id = idlist[randomNumber];
        QString showtext = name+" "+id;
        ui->textBrowser->setText(showtext);
    }else{
        ui->textBrowser->setText("no student found");
    }
}

