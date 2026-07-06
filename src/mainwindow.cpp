#include "constants.h"
#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QFile>
#include <QFont>
#include <QListWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QPixmap>
#include <QStackedWidget>
#include <QSplitter>
#include <QSystemTrayIcon>
#include <QVBoxLayout>

#include "settingsdialog.h"
#include "settingsmanager.h"
#include "stylemanager.h"
#include "tools/toolfactory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_sidebarWidget(nullptr)
    , m_toolList(nullptr)
    , m_contentStack(nullptr)
    , m_toggleSidebarAction(nullptr)
    , m_trayIcon(nullptr) {
    SettingsManager::instance().load();
    setupUi();
    applySettings();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUi() {
    resize(700, 500);
    setWindowTitle(Constants::AppTitle);

    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("File");

    QAction *quitAction = new QAction("Quit", this);
    fileMenu->addAction(quitAction);
    connect(quitAction, &QAction::triggered, this, &MainWindow::onQuit);

    QMenu *viewMenu = menuBar->addMenu("View");

    m_toggleSidebarAction = new QAction("Sidebar", this);
    m_toggleSidebarAction->setCheckable(true);
    m_toggleSidebarAction->setChecked(true);
    viewMenu->addAction(m_toggleSidebarAction);
    connect(m_toggleSidebarAction, &QAction::triggered, this, &MainWindow::onToggleSidebar);

    QMenu *settingsMenu = menuBar->addMenu("Settings");

    QAction *preferencesAction = new QAction("Preferences", this);
    settingsMenu->addAction(preferencesAction);
    connect(preferencesAction, &QAction::triggered, this, &MainWindow::onOpenSettings);

    QMenu *helpMenu = menuBar->addMenu("Help");

    QAction *aboutQtAction = new QAction("About Qt", this);
    helpMenu->addAction(aboutQtAction);
    connect(aboutQtAction, &QAction::triggered, this, [this] { QMessageBox::aboutQt(this); });

    helpMenu->addSeparator();

    QAction *aboutAction = new QAction("About", this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    QHBoxLayout *mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->addWidget(splitter);

    m_sidebarWidget = new QWidget();
    m_sidebarWidget->setObjectName("sidebarWidget");
    QVBoxLayout *sidebarLayout = new QVBoxLayout(m_sidebarWidget);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    m_toolList = new QListWidget();
    m_toolList->setFrameShape(QFrame::NoFrame);
    m_toolList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sidebarLayout->addWidget(m_toolList);

    splitter->addWidget(m_sidebarWidget);

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

    m_trayIcon = new QSystemTrayIcon(this);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);

    QMenu *trayMenu = new QMenu(this);
    QAction *showAction = new QAction("Show", this);
    connect(showAction, &QAction::triggered, this, &QWidget::showNormal);
    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
    m_trayIcon->setContextMenu(trayMenu);
}

void MainWindow::onToolListClicked(int row) {
    if (row >= 0) {
        m_contentStack->setCurrentIndex(row);
    }
}

void MainWindow::onAbout() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(QString("About %1").arg(Constants::AppTitle));
    msgBox.setText(QString("<h3>%1</h3>"
                           "<p>A developer toolbox with useful tools.</p>"
                           "<p>Version " APP_VERSION "</p>")
                       .arg(Constants::AppTitle));
    msgBox.setStyleSheet(qApp->styleSheet());
    msgBox.exec();
}

void MainWindow::onQuit() {
    qApp->quit();
}

void MainWindow::onToggleSidebar() {
    bool visible = !m_sidebarWidget->isVisible();
    m_sidebarWidget->setVisible(visible);
    m_toggleSidebarAction->setChecked(visible);
}

void MainWindow::onOpenSettings() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        applySettings();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        qApp->quit();
    }
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
        showNormal();
        activateWindow();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (SettingsManager::instance().trayIconEnabled() && m_trayIcon->isVisible()) {
        hide();
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::applySettings() {
    auto &settings = SettingsManager::instance();

    QFont font(settings.fontFamily());
    qApp->setFont(font);

    QString themeFile = StyleManager::findStyleFile(settings.theme());
    if (!themeFile.isEmpty()) {
        QFile file(themeFile);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            qApp->setStyleSheet(QString::fromUtf8(file.readAll()));
        }
    }

    m_sidebarWidget->setVisible(settings.sidebarVisible());
    m_toggleSidebarAction->setChecked(settings.sidebarVisible());

    if (settings.trayIconEnabled()) {
        QPixmap pixmap;
        QString iconPath = QCoreApplication::applicationDirPath() + "/icons/app.png";
        if (pixmap.load(iconPath)) {
            m_trayIcon->setIcon(QIcon(pixmap));
        }
        m_trayIcon->setToolTip(Constants::AppTitle);
        m_trayIcon->show();
    } else {
        m_trayIcon->hide();
    }
}
