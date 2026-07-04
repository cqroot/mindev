#include "constants.h"
#include "mainwindow.h"

#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>

#include "tools/toolfactory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_toolList(nullptr)
    , m_contentStack(nullptr)
{
    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    resize(700, 500);
    setWindowTitle(Constants::AppTitle);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    QHBoxLayout *mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->addWidget(splitter);

    QWidget *sidebarWidget = new QWidget();
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    m_toolList = new QListWidget();
    m_toolList->setFrameShape(QFrame::NoFrame);
    m_toolList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sidebarLayout->addWidget(m_toolList);

    splitter->addWidget(sidebarWidget);

    m_contentStack = new QStackedWidget();
    splitter->addWidget(m_contentStack);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
    splitter->setHandleWidth(6);

    auto tools = ToolFactory::instance().allTools();
    for (auto *tool : tools) {
        m_toolList->addItem(tool->name());
        m_contentStack->addWidget(tool->widget());
    }

    connect(m_toolList, &QListWidget::currentRowChanged, this, &MainWindow::onToolListClicked);

    m_toolList->setCurrentRow(0);
}

void MainWindow::onToolListClicked(int row)
{
    if (row >= 0) {
        m_contentStack->setCurrentIndex(row);
    }
}
