#ifndef TOOLFACTORY_H
#define TOOLFACTORY_H

#include "toolinterface.h"

#include <QVector>
#include <QStringList>

class ToolFactory {
  public:
    static ToolFactory &instance();

    template<typename T>
    T *registerTool();

    QVector<ToolInterface *> allTools();
    QStringList toolNames() const;

  private:
    ToolFactory() = default;
    void ensureInitialized();

    QVector<ToolInterface *> m_tools;
    bool m_initialized = false;
};

template<typename T>
T *ToolFactory::registerTool() {
    ensureInitialized();
    T *tool = new T();
    m_tools.append(tool);
    return tool;
}

#endif
