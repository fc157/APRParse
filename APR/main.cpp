#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    bool ok = false;
//    QString str1 = "-10.555";
//    float num1 = str1.toFloat();
//    qDebug() << num1;
    return a.exec();
}
