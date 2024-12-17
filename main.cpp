#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Stage Player");
    MainWindow w;
    w.showMaximized();
    w.itemsResize();
    return a.exec();
}
