#include "constants.h"
#include "mainwindow.h"
#include "stylemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(Constants::AppName);
    a.setOrganizationName(Constants::AppName);

    StyleManager::applyStyle(a);

    MainWindow w;
    w.show();
    return a.exec();
}
