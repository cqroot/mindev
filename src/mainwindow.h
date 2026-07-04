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

  private:
    void setupUi();

    QWidget *m_centralWidget;
    QListWidget *m_toolList;
    QStackedWidget *m_contentStack;
};

#endif
