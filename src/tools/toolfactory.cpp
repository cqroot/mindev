#include "toolfactory.h"
#include "baseconvertertool.h"
#include "storageconvertertool.h"
#include "subnetcalculatortool.h"

ToolFactory &ToolFactory::instance()
{
    static ToolFactory instance;
    return instance;
}

void ToolFactory::ensureInitialized()
{
    if (m_initialized) {
        return;
    }
    m_initialized = true;

    registerTool<BaseConverterTool>();
    registerTool<StorageConverterTool>();
    registerTool<SubnetCalculatorTool>();
}

QVector<ToolInterface *> ToolFactory::allTools()
{
    ensureInitialized();
    return m_tools;
}

QStringList ToolFactory::toolNames() const
{
    QStringList names;
    for (const auto *tool : m_tools) {
        names << tool->name();
    }
    return names;
}
