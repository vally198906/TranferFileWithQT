#include "mainwindow.h"
#include "iplineedit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setFixedSize(920, 800);

    //IPLineEdit *pIpLineEdit = new IPLineEdit(&w);
    //pIpLineEdit->setGeometry(150,150,360,50);

    w.show();

    return a.exec();
}
