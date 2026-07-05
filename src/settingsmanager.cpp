#include "settingsmanager.h"
#include "constants.h"

#include <QDir>
#include <QStandardPaths>

SettingsManager &SettingsManager::instance()
{
    static SettingsManager instance;
    return instance;
}

SettingsManager::SettingsManager()
    : m_settings(nullptr)
{
    QDir dir(configDirPath());
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    m_settings = new QSettings(configFilePath(), QSettings::IniFormat);
}

QString SettingsManager::configDirPath()
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    return configPath + "/mindev";
}

QString SettingsManager::configFilePath()
{
    return configDirPath() + "/config.ini";
}

void SettingsManager::load()
{
    Q_UNUSED(m_settings);
}

void SettingsManager::save()
{
    m_settings->sync();
}

QString SettingsManager::fontFamily() const
{
#ifdef Q_OS_WIN
    return m_settings->value("font/family", "Consolas").toString();
#else
    return m_settings->value("font/family", "Segoe UI").toString();
#endif
}

void SettingsManager::setFontFamily(const QString &family)
{
    m_settings->setValue("font/family", family);
}

bool SettingsManager::sidebarVisible() const
{
    return m_settings->value("ui/sidebarVisible", true).toBool();
}

void SettingsManager::setSidebarVisible(bool visible)
{
    m_settings->setValue("ui/sidebarVisible", visible);
}

QString SettingsManager::theme() const
{
    return m_settings->value("ui/theme", "dark").toString();
}

void SettingsManager::setTheme(const QString &theme)
{
    m_settings->setValue("ui/theme", theme);
}
