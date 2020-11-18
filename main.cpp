#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "mainwindow.h"
#include <QApplication>
#include "boardwidget.h"


int main(int argc, char *argv[])
{

    int result = Catch::Session().run(argc,argv);
    QApplication a(argc, argv);
    BoardWidget b;
    b.show();
    return a.exec();
}
