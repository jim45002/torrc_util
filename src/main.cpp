#include "mainwindow.h"
#include <QMap>
#include <QStringList>
#include <QMetaType>
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<QMap<QString, QString> >("QMap<QString, QString>");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
