#include "constants.h"
#include "stylemanager.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>

QString StyleManager::getXdgConfigDir() {
    QStringList configDirs = QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation);
    for (const QString &dir : configDirs) {
        QDir d(dir);
        if (d.exists()) {
            return dir;
        }
    }
    return QString();
}

QStringList StyleManager::findAvailableStyles() {
    QStringList styles;
    QStringList searchPaths;
    QString appPath = QCoreApplication::applicationDirPath();

    searchPaths << appPath + QDir::separator() + Constants::Styles::StylesDir;
    searchPaths << getXdgConfigDir() + QDir::separator() + Constants::Styles::XdgConfigSubdir + QDir::separator() + Constants::Styles::StylesDir;

    for (const QString &path : searchPaths) {
        QDir dir(path);
        if (dir.exists()) {
            QStringList filters = QStringList() << "*.qss" << "*.QSS";
            QStringList files = dir.entryList(filters, QDir::Files);
            for (const QString &file : files) {
                QString baseName = QFileInfo(file).baseName();
                if (!styles.contains(baseName)) {
                    styles.append(baseName);
                }
            }
        }
    }

    return styles;
}

QString StyleManager::findStyleFile(const QString &theme) {
    if (theme.isEmpty()) {
        return QString();
    }

    QStringList searchPaths;
    QString appPath = QCoreApplication::applicationDirPath();

    searchPaths << appPath + QDir::separator() + Constants::Styles::StylesDir;
    searchPaths << getXdgConfigDir() + QDir::separator() + Constants::Styles::XdgConfigSubdir + QDir::separator() + Constants::Styles::StylesDir;

    for (const QString &path : searchPaths) {
        QString styleFile = path + QDir::separator() + theme + ".qss";
        if (QFile::exists(styleFile)) {
            return styleFile;
        }
        styleFile = path + QDir::separator() + theme + ".QSS";
        if (QFile::exists(styleFile)) {
            return styleFile;
        }
    }

    return QString();
}

void StyleManager::applyStyle(QApplication &app) {
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
