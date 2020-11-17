#include "mainwindow.h"

#include <QApplication>
#include "boardwidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BoardWidget b;
    b.show();
    return a.exec();
}
