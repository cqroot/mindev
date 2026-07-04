#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QApplication>
#include <QString>

class StyleManager {
  public:
    static QString findStyleFile();
    static QString getXdgConfigDir();
    static void applyStyle(QApplication &app);
};

#endif
