#include "mainwindow.h"
#include <QStyle>
#include <QDesktopWidget>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("QTWatsonWidgetClient");
    w.setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    w.size(),
                    a.desktop()->availableGeometry()));

    w.show();
    return a.exec();
}
