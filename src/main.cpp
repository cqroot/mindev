#include "constants.h"
#include "mainwindow.h"
#include "settingsmanager.h"
#include "stylemanager.h"

#include <QApplication>
#include <QPixmap>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName(Constants::AppName);
    a.setOrganizationName(Constants::AppName);

    SettingsManager::instance().load();

    QFont font(SettingsManager::instance().fontFamily());
    a.setFont(font);

    StyleManager::applyStyle(a);

    QPixmap pixmap;
    if (pixmap.load("icons/app.png")) {
        a.setWindowIcon(QIcon(pixmap));
    }

    MainWindow w;
    w.show();
    return a.exec();
}
