#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

  private:
    void setupUi();
    void applySettings();

    QWidget *m_centralWidget;
    QWidget *m_sidebarWidget;
    QListWidget *m_toolList;
    QStackedWidget *m_contentStack;
    QAction *m_toggleSidebarAction;
};

#endif