#include "settingsdialog.h"
#include "settingsmanager.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFontDatabase>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");
    setMinimumWidth(400);
    setupUi();

    auto &settings = SettingsManager::instance();
    m_fontFamilyCombo->setCurrentText(settings.fontFamily());
    m_fontSizeSpin->setValue(settings.fontSize());
    m_sidebarCheckBox->setChecked(settings.sidebarVisible());
}

void SettingsDialog::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *appearanceGroup = new QGroupBox("Appearance");
    QFormLayout *appearanceLayout = new QFormLayout();

    m_fontFamilyCombo = new QComboBox();
    const QStringList families = QFontDatabase::families();
    for (const QString &family : families) {
        m_fontFamilyCombo->addItem(family);
    }
    appearanceLayout->addRow("Font:", m_fontFamilyCombo);

    m_fontSizeSpin = new QSpinBox();
    m_fontSizeSpin->setRange(8, 24);
    appearanceLayout->addRow("Font Size:", m_fontSizeSpin);

    appearanceGroup->setLayout(appearanceLayout);
    mainLayout->addWidget(appearanceGroup);

    QGroupBox *uiGroup = new QGroupBox("Interface");
    QFormLayout *uiLayout = new QFormLayout();

    m_sidebarCheckBox = new QCheckBox("Show sidebar by default");
    uiLayout->addRow(m_sidebarCheckBox);

    uiGroup->setLayout(uiLayout);
    mainLayout->addWidget(uiGroup);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

void SettingsDialog::onAccept()
{
    auto &settings = SettingsManager::instance();
    settings.setFontFamily(m_fontFamilyCombo->currentText());
    settings.setFontSize(m_fontSizeSpin->value());
    settings.setSidebarVisible(m_sidebarCheckBox->isChecked());
    settings.save();
    accept();
}
