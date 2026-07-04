#ifndef TOOLINTERFACE_H
#define TOOLINTERFACE_H

#include <QString>
#include <QWidget>

class ToolInterface {
  public:
    virtual ~ToolInterface() = default;
    virtual QString name() const = 0;
    virtual QWidget *widget() = 0;
};

#endif
