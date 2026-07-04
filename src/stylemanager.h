#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QApplication>
#include <QString>

class StyleManager {
  public:
    static QString findStyleFile(const QString &theme = QString());
    static QStringList findAvailableStyles();
    static QString getXdgConfigDir();
    static void applyStyle(QApplication &app);
};

#endif
