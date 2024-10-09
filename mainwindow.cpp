#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setObjectName("mainwindow");

//    m_menubar = new QMenuBar(this);
//    m_statusbar = new QStatusBar(this);
//    m_toolbar = new QToolBar(this);

//    m_menubar->setMinimumHeight(20);
//    m_toolbar->setMinimumHeight(20);
//    m_statusbar->setMinimumHeight(20);

//    this->setMenuBar(m_menubar);
//    this->addToolBar(Qt::TopToolBarArea, m_toolbar);
//    this->setStatusBar(m_statusbar);

//    m_menubar->setObjectName("m_menubar");
//    m_toolbar->setObjectName("m_toolbar");
//    m_statusbar->setObjectName("m_statusbar");
//    m_menubar->setStyleSheet(R"(
//        QMenuBar#m_menubar {
//            background-color:red;
//        }
//    )");
//    m_toolbar->setStyleSheet(R"(
//        QToolBar#m_toolbar {
//            background-color:blue;
//        }
//    )");
//    m_statusbar->setStyleSheet(R"(
//        QStatusBar#m_statusbar {
//            background-color:green;
//        }
//    )");

    this->setStyleSheet(R"(
        QWidget#mainwindow {
            background-color:#727272;
        }
    )");
}

MainWindow::~MainWindow()
{
}

