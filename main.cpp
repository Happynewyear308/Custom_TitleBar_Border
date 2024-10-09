#include "mainwindow.h"
#include "customtitlebarwdg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow;
//    w->show();
    CustomTitlebarWdg win;
    win.setTitleIcon(":/image/icon_custom.png");
    win.setTitleText("Test CustomTitleBar");
    win.setContent(w);
    win.resize(700, 600);
    win.show();
    return a.exec();
}
