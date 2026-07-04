#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QComboBox;
class QSpinBox;
class QCheckBox;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
    void onAccept();

private:
    void setupUi();

    QComboBox *m_fontFamilyCombo;
    QSpinBox *m_fontSizeSpin;
    QCheckBox *m_sidebarCheckBox;
};

#endif
