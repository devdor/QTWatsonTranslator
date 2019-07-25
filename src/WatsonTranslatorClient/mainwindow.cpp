#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "app_settings_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("Acme");
    QCoreApplication::setOrganizationDomain("local");
    QCoreApplication::setApplicationName("WatsonTranslatorClient");

    ui->setupUi(this);
    this->createActions();
    this->createMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    this->appNewAct = new QAction(tr("&New"), this);
    this->appNewAct->setShortcuts(QKeySequence::New);
    this->appNewAct->setStatusTip(tr("Create a new file"));
    connect(this->appNewAct, &QAction::triggered, this, &MainWindow::fileAppNew);

    this->appSettingsAct = new QAction(tr("&Settings"), this);
    this->appSettingsAct->setStatusTip(tr("Application settings"));
    connect(this->appSettingsAct, &QAction::triggered, this, &MainWindow::fileAppSettings);

    this->appExitAct = new QAction(tr("&Exit"), this);
    this->appExitAct->setShortcut(QKeySequence::Close);
    this->appExitAct->setStatusTip(tr("Exit application"));
    connect(this->appExitAct, &QAction::triggered, this, &MainWindow::fileAppExit);

    this->appAboutAct = new QAction(tr("&About..."), this);
    this->appAboutAct->setStatusTip(tr("About this application"));
    connect(this->appAboutAct, &QAction::triggered, this, &MainWindow::fileAppAbout);
}

void MainWindow::createMenus()
{
    this->fileMenu = this->menuBar()->addMenu(tr("&File"));
    this->fileMenu->addAction(this->appNewAct);
    this->fileMenu->addAction(this->appSettingsAct);
    this->fileMenu->addAction(this->appExitAct);

    this->helpMenu = this->menuBar()->addMenu(tr("Help"));
    this->helpMenu->addAction(this->appAboutAct);
}

void MainWindow::fileAppSettings()
{
    try
    {
        QSettings settings;

        AppSettingsDialog *appSettingsDlg = new AppSettingsDialog(this);
        appSettingsDlg->setWindowFlag(Qt::WindowType::Tool);
        appSettingsDlg->setWindowTitle(tr("Login"));
        appSettingsDlg->Init(
                    settings.value("IBMSettings/ServiceUrl").toString(),
                    settings.value("IBMSettings/VersionDate").toString(),
                    settings.value("IBMSettings/AccessToken").toString());

        int dlgResult = appSettingsDlg->exec();
        if(dlgResult == QDialog::Accepted)
        {
            settings.setValue("IBMSettings/ServiceUrl", appSettingsDlg->GetServiceUrl());
            settings.setValue("IBMSettings/VersionDate", appSettingsDlg->GetVersionDate());
            settings.setValue("IBMSettings/AccessToken", appSettingsDlg->GetAccessToken());
            qDebug() << "appSettingsDlg accepted";
        }
    }
    catch (const std::exception& exc)
    {
        qDebug() << "EXCEPTION in fileAppSettings: " << exc.what() << endl;
    }
}

void MainWindow::fileAppNew()
{
    try
    {
    }
    catch (const std::exception& exc)
    {
        qDebug() << "EXCEPTION in fileAppNew: " << exc.what() << endl;
    }
}

void MainWindow::fileAppExit()
{
    try
    {
        QApplication::exit();
    }
    catch (const std::exception& exc)
    {
        qDebug() << "EXCEPTION in fileAppExit: " << exc.what() << endl;
    }
}

void MainWindow::fileAppAbout()
{
    try
    {

    }
    catch (const std::exception& exc)
    {
        qDebug() << "EXCEPTION in fileAppAbout: " << exc.what() << endl;
    }
}
