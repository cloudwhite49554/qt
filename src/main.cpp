#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Letter World");
    w.show();
    return QCoreApplication::exec();
}
