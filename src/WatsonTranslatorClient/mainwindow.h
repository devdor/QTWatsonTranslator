#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPair>

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
    QAction *appSettingsAct, *appExitAct, *appAboutAct;

    void createActions();
    void createMenus();
    void fileAppSettings();
    void fileAppAbout();
    void fileAppExit();    
    void insertRow(int numWordsCount, int numCharactersCount);

    void readLanguageList();
    QString buildAuthorizationItem();
    void translate(const QString &value, const QString &fromLang, const QString &dstLang);

private slots:
    void onTranslate();
};

#endif // MAINWINDOW_H
