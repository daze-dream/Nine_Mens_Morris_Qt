#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "mainwindow.h"
#include <QApplication>
#include "boardwidget.h"
#include "manualtests.h"


int main(int argc, char *argv[])
{
    //unit tests with Catch
    int result = Catch::Session().run(argc,argv);

    //Manual Tests by Alec
    manualTests t;
    //Really only valid for the command-line version
    //t.doAllTests();
    QApplication a(argc, argv);
    BoardWidget b;
    b.show();
    return a.exec();
}
