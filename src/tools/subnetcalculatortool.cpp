#include "subnetcalculatortool.h"

#include "../constants.h"

#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

SubnetCalculatorTool::SubnetCalculatorTool(QWidget *parent)
    : QWidget(parent)
    , m_input(nullptr)
    , m_networkLabel(nullptr)
    , m_broadcastLabel(nullptr)
    , m_subnetMaskLabel(nullptr)
    , m_wildcardMaskLabel(nullptr)
    , m_firstUsableLabel(nullptr)
    , m_lastUsableLabel(nullptr)
    , m_totalHostsLabel(nullptr)
    , m_usableHostsLabel(nullptr)
    , m_ipClassLabel(nullptr)
    , m_cidrLabel(nullptr)
    , m_binaryLabel(nullptr)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    QVBoxLayout *wrapperLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel(Constants::Tools::SubnetCalculatorName);
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    wrapperLayout->addWidget(title);

    QGroupBox *inputGroup = new QGroupBox();
    QFormLayout *inputLayout = new QFormLayout();

    m_input = new QLineEdit();
    m_input->setPlaceholderText("e.g. 192.168.1.0/24");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(
        QRegularExpression("^([0-9]{1,3}\\.){3}[0-9]{1,3}/([0-9]|[1-2][0-9]|3[0-2])$"), this);
    m_input->setValidator(validator);
    inputLayout->addRow("CIDR Notation:", m_input);

    inputGroup->setLayout(inputLayout);
    wrapperLayout->addWidget(inputGroup);

    QGroupBox *resultGroup = new QGroupBox("");
    QFormLayout *resultLayout = new QFormLayout();
    resultLayout->setSpacing(12);

    m_cidrLabel = new QLabel("-");
    m_cidrLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("CIDR:", m_cidrLabel);

    m_networkLabel = new QLabel("-");
    m_networkLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("Network Address:", m_networkLabel);

    m_broadcastLabel = new QLabel("-");
    m_broadcastLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("Broadcast Address:", m_broadcastLabel);

    m_subnetMaskLabel = new QLabel("-");
    m_subnetMaskLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("Subnet Mask:", m_subnetMaskLabel);

    m_wildcardMaskLabel = new QLabel("-");
    m_wildcardMaskLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("Wildcard Mask:", m_wildcardMaskLabel);

    m_firstUsableLabel = new QLabel("-");
    m_firstUsableLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("First Usable IP:", m_firstUsableLabel);

    m_lastUsableLabel = new QLabel("-");
    m_lastUsableLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("Last Usable IP:", m_lastUsableLabel);

    m_totalHostsLabel = new QLabel("-");
    m_totalHostsLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("Total Hosts:", m_totalHostsLabel);

    m_usableHostsLabel = new QLabel("-");
    m_usableHostsLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("Usable Hosts:", m_usableHostsLabel);

    m_ipClassLabel = new QLabel("-");
    m_ipClassLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("IP Class:", m_ipClassLabel);

    m_binaryLabel = new QLabel("-");
    m_binaryLabel->setWordWrap(true);
    m_binaryLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resultLayout->addRow("Binary:", m_binaryLabel);

    resultGroup->setLayout(resultLayout);
    wrapperLayout->addWidget(resultGroup);

    wrapperLayout->addStretch();

    connect(m_input, &QLineEdit::textChanged, this, &SubnetCalculatorTool::onInputChanged);
}

SubnetCalculatorTool::~SubnetCalculatorTool()
{
}

QString SubnetCalculatorTool::name() const
{
    return Constants::Tools::SubnetCalculatorName;
}

QWidget *SubnetCalculatorTool::widget()
{
    return this;
}

void SubnetCalculatorTool::onInputChanged(const QString &text)
{
    if (text.isEmpty()) {
        m_cidrLabel->setText("-");
        m_networkLabel->setText("-");
        m_broadcastLabel->setText("-");
        m_subnetMaskLabel->setText("-");
        m_wildcardMaskLabel->setText("-");
        m_firstUsableLabel->setText("-");
        m_lastUsableLabel->setText("-");
        m_totalHostsLabel->setText("-");
        m_usableHostsLabel->setText("-");
        m_ipClassLabel->setText("-");
        m_binaryLabel->setText("-");
        return;
    }
    calculate(text);
}

void SubnetCalculatorTool::calculate(const QString &input)
{
    QStringList parts = input.split("/");
    if (parts.size() != 2) {
        return;
    }

    QString ipStr = parts[0];
    int prefix = parts[1].toInt();

    quint32 ip = ipToUint(ipStr);
    if (ip == 0) {
        return;
    }

    quint32 mask = (prefix == 0) ? 0 : (~0u << (32 - prefix));
    quint32 network = ip & mask;
    quint32 broadcast = network | ~mask;
    quint32 wildcard = ~mask;

    quint32 firstUsable = (prefix >= 31) ? network : network + 1;
    quint32 lastUsable = (prefix >= 31) ? broadcast : broadcast - 1;

    quint32 totalHosts = 1u << (32 - prefix);
    quint32 usableHosts = (prefix >= 31) ? totalHosts : totalHosts - 2;

    QString ipClass;
    quint32 firstOctet = (ip >> 24) & 0xFF;
    if (firstOctet < 128) {
        ipClass = "A";
    } else if (firstOctet < 192) {
        ipClass = "B";
    } else if (firstOctet < 224) {
        ipClass = "C";
    } else if (firstOctet < 240) {
        ipClass = "D (Multicast)";
    } else {
        ipClass = "E (Reserved)";
    }

    QString binary = QString("%1/%2").arg(ipToString(ip)).arg(prefix);

    m_cidrLabel->setText(QString("/%1").arg(prefix));
    m_networkLabel->setText(ipToString(network));
    m_broadcastLabel->setText(ipToString(broadcast));
    m_subnetMaskLabel->setText(ipToString(mask));
    m_wildcardMaskLabel->setText(ipToString(wildcard));
    m_firstUsableLabel->setText(ipToString(firstUsable));
    m_lastUsableLabel->setText(ipToString(lastUsable));
    m_totalHostsLabel->setText(QString::number(totalHosts));
    m_usableHostsLabel->setText(QString::number(usableHosts));
    m_ipClassLabel->setText(ipClass);
    m_binaryLabel->setText(binary);
}

QString SubnetCalculatorTool::ipToString(quint32 ip)
{
    return QString("%1.%2.%3.%4")
        .arg((ip >> 24) & 0xFF)
        .arg((ip >> 16) & 0xFF)
        .arg((ip >> 8) & 0xFF)
        .arg(ip & 0xFF);
}

quint32 SubnetCalculatorTool::ipToUint(const QString &ip)
{
    QStringList octets = ip.split(".");
    if (octets.size() != 4) {
        return 0;
    }

    quint32 result = 0;
    for (int i = 0; i < 4; ++i) {
        bool ok = false;
        quint32 val = octets[i].toUInt(&ok);
        if (!ok || val > 255) {
            return 0;
        }
        result = (result << 8) | val;
    }
    return result;
}
