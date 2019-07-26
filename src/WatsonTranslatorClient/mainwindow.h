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
    QAction *appNewAct, *appSettingsAct, *appExitAct, *appAboutAct;

    void createActions();
    void createMenus();

    void fileAppNew();
    void fileAppSettings();
    void fileAppAbout();
    void fileAppExit();    
    void insertRow(const QString &value, const QString &translation);

    void translate(const QString &value);

    QVector<QPair<QString,QString>> m_languageList;
    void readLanguageList();
    QString buildAuthorizationItem();

public slots:
    void onRequestCompleted(QNetworkReply *rep);
};

#endif // MAINWINDOW_H
