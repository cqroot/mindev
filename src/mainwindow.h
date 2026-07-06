#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class QListWidget;
class QStackedWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void onToolListClicked(int row);
    void onAbout();
    void onQuit();
    void onToggleSidebar();
    void onOpenSettings();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

  private:
    void setupUi();
    void applySettings();
    void closeEvent(QCloseEvent *event) override;

    QWidget *m_centralWidget;
    QWidget *m_sidebarWidget;
    QListWidget *m_toolList;
    QStackedWidget *m_contentStack;
    QAction *m_toggleSidebarAction;
    QSystemTrayIcon *m_trayIcon;
};

#endif
