#ifndef STORAGECONVERTERTOOL_H
#define STORAGECONVERTERTOOL_H

#include "toolinterface.h"

class QLineEdit;

class StorageConverterTool : public QWidget, public ToolInterface {
    Q_OBJECT

  public:
    explicit StorageConverterTool(QWidget *parent = nullptr);
    ~StorageConverterTool();

    QString name() const override;
    QWidget *widget() override;

  private slots:
    void onBytesChanged(const QString &text);
    void onKBChanged(const QString &text);
    void onMBChanged(const QString &text);
    void onGBChanged(const QString &text);
    void onTBChanged(const QString &text);
    void onPBChanged(const QString &text);
    void onKiBChanged(const QString &text);
    void onMiBChanged(const QString &text);
    void onGiBChanged(const QString &text);
    void onTiBChanged(const QString &text);

  private:
    void updateResults(double bytes);
    void convertFromUnit(const QString &text, double multiplier);

    QLineEdit *m_bytesInput;
    QLineEdit *m_kbInput;
    QLineEdit *m_mbInput;
    QLineEdit *m_gbInput;
    QLineEdit *m_tbInput;
    QLineEdit *m_pbInput;
    QLineEdit *m_kibInput;
    QLineEdit *m_mibInput;
    QLineEdit *m_gibInput;
    QLineEdit *m_tibInput;

    bool m_updating;
};

#endif
