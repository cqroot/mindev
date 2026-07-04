#ifndef SUBNETCALCULATORTOOL_H
#define SUBNETCALCULATORTOOL_H

#include "toolinterface.h"

class QLineEdit;
class QLabel;
class QGroupBox;

class SubnetCalculatorTool : public QWidget, public ToolInterface {
    Q_OBJECT

  public:
    explicit SubnetCalculatorTool(QWidget *parent = nullptr);
    ~SubnetCalculatorTool();

    QString name() const override;
    QWidget *widget() override;

  private slots:
    void onInputChanged(const QString &text);

  private:
    void calculate(const QString &input);
    QString ipToString(quint32 ip);
    quint32 ipToUint(const QString &ip);

    QLineEdit *m_input;
    QLabel *m_networkLabel;
    QLabel *m_broadcastLabel;
    QLabel *m_subnetMaskLabel;
    QLabel *m_wildcardMaskLabel;
    QLabel *m_firstUsableLabel;
    QLabel *m_lastUsableLabel;
    QLabel *m_totalHostsLabel;
    QLabel *m_usableHostsLabel;
    QLabel *m_ipClassLabel;
    QLabel *m_cidrLabel;
    QLabel *m_binaryLabel;
};
#endif
