#include "constants.h"
#include "mainwindow.h"
#include "settingsmanager.h"
#include "stylemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(Constants::AppName);
    a.setOrganizationName(Constants::AppName);

    SettingsManager::instance().load();

    QFont font(SettingsManager::instance().fontFamily());
    a.setFont(font);

    StyleManager::applyStyle(a);

    MainWindow w;
    w.show();
    return a.exec();
}
