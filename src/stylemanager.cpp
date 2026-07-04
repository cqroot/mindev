#include "constants.h"
#include "stylemanager.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>

QString StyleManager::getXdgConfigDir()
{
    QStringList configDirs = QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation);
    for (const QString &dir : configDirs) {
        QDir d(dir);
        if (d.exists()) {
            return dir;
        }
    }
    return QString();
}

QString StyleManager::findStyleFile()
{
    QStringList searchPaths;
    QString appPath = QCoreApplication::applicationDirPath();

    searchPaths << appPath + QDir::separator() + Constants::Styles::StylesDir;
    searchPaths << getXdgConfigDir() + QDir::separator() + Constants::Styles::XdgConfigSubdir + QDir::separator() + Constants::Styles::StylesDir;

    for (const QString &path : searchPaths) {
        QDir dir(path);
        if (dir.exists()) {
            QStringList filters = QStringList() << "*.qss" << "*.QSS";
            QStringList files = dir.entryList(filters, QDir::Files);
            if (!files.isEmpty()) {
                return path + QDir::separator() + files.first();
            }
        }
    }

    return QString();
}

void StyleManager::applyStyle(QApplication &app)
{
    QString styleFile = findStyleFile();
    if (styleFile.isEmpty()) {
        return;
    }

    QFile file(styleFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QString styleSheet = QString::fromUtf8(file.readAll());
    app.setStyleSheet(styleSheet);
}
