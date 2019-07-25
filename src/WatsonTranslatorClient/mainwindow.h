#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMenu *fileMenu, *helpMenu;
    QAction *appNewAct, *appSettingsAct, *appExitAct, *appAboutAct;

    void createActions();
    void createMenus();

    void fileAppNew();
    void fileAppSettings();
    void fileAppAbout();
    void fileAppExit();
};

#endif // MAINWINDOW_H
