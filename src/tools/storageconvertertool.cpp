#include "../constants.h"
#include "storageconvertertool.h"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QRegularExpressionValidator>
#include <QtMath>

static QString formatNumber(double value) {
    if (value == 0) {
        return "0";
    }
    if (qAbs(value) >= 1e20) {
        return QString::number(value, 'e', 4);
    }
    int decimals = 4;
    if (qAbs(value) >= 1) {
        decimals = qMax(0, 15 - static_cast<int>(log10(qAbs(value))) - 1);
    }
    QString result = QString::number(value, 'f', decimals);
    result.remove(QRegularExpression("0+$"));
    result.remove(QRegularExpression("\\.$"));
    return result;
}

StorageConverterTool::StorageConverterTool(QWidget *parent)
    : QWidget(parent)
    , m_bytesInput(nullptr)
    , m_kbInput(nullptr)
    , m_mbInput(nullptr)
    , m_gbInput(nullptr)
    , m_tbInput(nullptr)
    , m_pbInput(nullptr)
    , m_kibInput(nullptr)
    , m_mibInput(nullptr)
    , m_gibInput(nullptr)
    , m_tibInput(nullptr)
    , m_updating(false) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QVBoxLayout *wrapperLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel(Constants::Tools::StorageConverterName);
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    wrapperLayout->addWidget(title);

    QGroupBox *group = new QGroupBox();
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(16);
    formLayout->setLabelAlignment(Qt::AlignRight);

    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("[0-9]*\\.?[0-9]*"), this);

    m_bytesInput = new QLineEdit();
    m_bytesInput->setPlaceholderText("Bytes");
    m_bytesInput->setValidator(validator);
    formLayout->addRow("Bytes (B):", m_bytesInput);

    m_kbInput = new QLineEdit();
    m_kbInput->setPlaceholderText("Kilobytes");
    m_kbInput->setValidator(validator);
    formLayout->addRow("Kilobytes (KB):", m_kbInput);

    m_mbInput = new QLineEdit();
    m_mbInput->setPlaceholderText("Megabytes");
    m_mbInput->setValidator(validator);
    formLayout->addRow("Megabytes (MB):", m_mbInput);

    m_gbInput = new QLineEdit();
    m_gbInput->setPlaceholderText("Gigabytes");
    m_gbInput->setValidator(validator);
    formLayout->addRow("Gigabytes (GB):", m_gbInput);

    m_tbInput = new QLineEdit();
    m_tbInput->setPlaceholderText("Terabytes");
    m_tbInput->setValidator(validator);
    formLayout->addRow("Terabytes (TB):", m_tbInput);

    m_pbInput = new QLineEdit();
    m_pbInput->setPlaceholderText("Petabytes");
    m_pbInput->setValidator(validator);
    formLayout->addRow("Petabytes (PB):", m_pbInput);

    m_kibInput = new QLineEdit();
    m_kibInput->setPlaceholderText("Kibibytes");
    m_kibInput->setValidator(validator);
    formLayout->addRow("Kibibytes (KiB):", m_kibInput);

    m_mibInput = new QLineEdit();
    m_mibInput->setPlaceholderText("Mebibytes");
    m_mibInput->setValidator(validator);
    formLayout->addRow("Mebibytes (MiB):", m_mibInput);

    m_gibInput = new QLineEdit();
    m_gibInput->setPlaceholderText("Gibibytes");
    m_gibInput->setValidator(validator);
    formLayout->addRow("Gibibytes (GiB):", m_gibInput);

    m_tibInput = new QLineEdit();
    m_tibInput->setPlaceholderText("Tebibytes");
    m_tibInput->setValidator(validator);
    formLayout->addRow("Tebibytes (TiB):", m_tibInput);

    group->setLayout(formLayout);
    wrapperLayout->addWidget(group);

    wrapperLayout->addStretch();

    connect(m_bytesInput, &QLineEdit::textChanged, this, &StorageConverterTool::onBytesChanged);
    connect(m_kbInput, &QLineEdit::textChanged, this, &StorageConverterTool::onKBChanged);
    connect(m_mbInput, &QLineEdit::textChanged, this, &StorageConverterTool::onMBChanged);
    connect(m_gbInput, &QLineEdit::textChanged, this, &StorageConverterTool::onGBChanged);
    connect(m_tbInput, &QLineEdit::textChanged, this, &StorageConverterTool::onTBChanged);
    connect(m_pbInput, &QLineEdit::textChanged, this, &StorageConverterTool::onPBChanged);
    connect(m_kibInput, &QLineEdit::textChanged, this, &StorageConverterTool::onKiBChanged);
    connect(m_mibInput, &QLineEdit::textChanged, this, &StorageConverterTool::onMiBChanged);
    connect(m_gibInput, &QLineEdit::textChanged, this, &StorageConverterTool::onGiBChanged);
    connect(m_tibInput, &QLineEdit::textChanged, this, &StorageConverterTool::onTiBChanged);
}

StorageConverterTool::~StorageConverterTool() {
}

QString StorageConverterTool::name() const {
    return Constants::Tools::StorageConverterName;
}

QWidget *StorageConverterTool::widget() {
    return this;
}

void StorageConverterTool::onBytesChanged(const QString &text) {
    if (m_updating) {
        return;
    }
    if (text.isEmpty()) {
        updateResults(0);
        return;
    }
    bool ok = false;
    double value = text.toDouble(&ok);
    if (ok) {
        updateResults(value);
    }
}

void StorageConverterTool::onKBChanged(const QString &text) {
    convertFromUnit(text, 1000.0);
}

void StorageConverterTool::onMBChanged(const QString &text) {
    convertFromUnit(text, 1000.0 * 1000.0);
}

void StorageConverterTool::onGBChanged(const QString &text) {
    convertFromUnit(text, 1000.0 * 1000.0 * 1000.0);
}

void StorageConverterTool::onTBChanged(const QString &text) {
    convertFromUnit(text, 1000.0 * 1000.0 * 1000.0 * 1000.0);
}

void StorageConverterTool::onPBChanged(const QString &text) {
    convertFromUnit(text, 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0);
}

void StorageConverterTool::onKiBChanged(const QString &text) {
    convertFromUnit(text, 1024.0);
}

void StorageConverterTool::onMiBChanged(const QString &text) {
    convertFromUnit(text, 1024.0 * 1024.0);
}

void StorageConverterTool::onGiBChanged(const QString &text) {
    convertFromUnit(text, 1024.0 * 1024.0 * 1024.0);
}

void StorageConverterTool::onTiBChanged(const QString &text) {
    convertFromUnit(text, 1024.0 * 1024.0 * 1024.0 * 1024.0);
}

void StorageConverterTool::convertFromUnit(const QString &text, double multiplier) {
    if (m_updating) {
        return;
    }
    if (text.isEmpty()) {
        updateResults(0);
        return;
    }
    bool ok = false;
    double value = text.toDouble(&ok);
    if (ok) {
        updateResults(value * multiplier);
    }
}

void StorageConverterTool::updateResults(double bytes) {
    m_updating = true;

    m_bytesInput->blockSignals(true);
    m_bytesInput->setText(formatNumber(bytes));
    m_bytesInput->blockSignals(false);

    const double kb = 1000.0;
    const double mb = kb * 1000.0;
    const double gb = mb * 1000.0;
    const double tb = gb * 1000.0;
    const double pb = tb * 1000.0;

    const double kib = 1024.0;
    const double mib = kib * 1024.0;
    const double gib = mib * 1024.0;
    const double tib = gib * 1024.0;

    m_kbInput->blockSignals(true);
    m_kbInput->setText(formatNumber(bytes / kb));
    m_kbInput->blockSignals(false);

    m_mbInput->blockSignals(true);
    m_mbInput->setText(formatNumber(bytes / mb));
    m_mbInput->blockSignals(false);

    m_gbInput->blockSignals(true);
    m_gbInput->setText(formatNumber(bytes / gb));
    m_gbInput->blockSignals(false);

    m_tbInput->blockSignals(true);
    m_tbInput->setText(formatNumber(bytes / tb));
    m_tbInput->blockSignals(false);

    m_pbInput->blockSignals(true);
    m_pbInput->setText(formatNumber(bytes / pb));
    m_pbInput->blockSignals(false);

    m_kibInput->blockSignals(true);
    m_kibInput->setText(formatNumber(bytes / kib));
    m_kibInput->blockSignals(false);

    m_mibInput->blockSignals(true);
    m_mibInput->setText(formatNumber(bytes / mib));
    m_mibInput->blockSignals(false);

    m_gibInput->blockSignals(true);
    m_gibInput->setText(formatNumber(bytes / gib));
    m_gibInput->blockSignals(false);

    m_tibInput->blockSignals(true);
    m_tibInput->setText(formatNumber(bytes / tib));
    m_tibInput->blockSignals(false);

    m_updating = false;
}
