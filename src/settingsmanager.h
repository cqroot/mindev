#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QFont>
#include <QSettings>

class SettingsManager {
  public:
    static SettingsManager &instance();

    void load();
    void save();

    QString fontFamily() const;
    void setFontFamily(const QString &family);

    bool sidebarVisible() const;
    void setSidebarVisible(bool visible);

    bool trayIconEnabled() const;
    void setTrayIconEnabled(bool enabled);

    QString theme() const;
    void setTheme(const QString &theme);

  private:
    SettingsManager();

    static QString configDirPath();
    static QString configFilePath();

    QSettings *m_settings;
};

#endif
