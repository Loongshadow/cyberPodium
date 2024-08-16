#include"logwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogWidget w;
    w.show();
    return a.exec();
}
