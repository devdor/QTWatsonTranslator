#include "mainwindow.h"
#include <QStyle>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(VERSION_STRING);

    QScreen *pScreen = a.primaryScreen();

    MainWindow w;
    w.setWindowTitle("QTWatsonWidgetClient");
    w.setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    w.size(),
                    pScreen->geometry()));

    w.show();
    return a.exec();
}
