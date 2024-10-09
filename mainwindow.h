#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QMenuBar *m_menubar = nullptr;
    QToolBar *m_toolbar = nullptr;
    QStatusBar *m_statusbar = nullptr;
};
#endif // MAINWINDOW_H
