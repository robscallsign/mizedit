#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QCoreApplication::setOrganizationName("HypeMan");
    QCoreApplication::setOrganizationDomain("HypeMan.com");
    QCoreApplication::setApplicationName("MizEdit");

    w.show();
    return a.exec();
}
