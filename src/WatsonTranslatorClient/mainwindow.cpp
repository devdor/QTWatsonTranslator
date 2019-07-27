#include <QDebug>
#include <QMessageBox>
#include <QHttpMultiPart>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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

    QStringList colHeaderList;
    colHeaderList << "Created" << "Words count" << "Characters count";
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(colHeaderList);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    this->connect(this->ui->btnTranslate, SIGNAL(clicked()), this, SLOT(onTranslate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    this->appSettingsAct = new QAction(tr("&Settings"), this);
    this->appSettingsAct->setStatusTip(tr("Application settings"));
    this->appSettingsAct->setIcon(QIcon::fromTheme("configure_project"));
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
    this->fileMenu->addAction(this->appSettingsAct);
    this->fileMenu->addAction(this->appExitAct);

    this->helpMenu = this->menuBar()->addMenu(tr("Help"));
    this->helpMenu->addAction(this->appAboutAct);

    ui->mainToolBar->addAction(this->appSettingsAct);
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

            this->readLanguageList();
            qDebug() << "appSettingsDlg accepted";
        }
    }
    catch (const std::exception& exc)
    {
        qDebug() << "EXCEPTION in fileAppSettings: " << exc.what() << endl;
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
        QMessageBox *dlg = new QMessageBox();
        dlg->setWindowTitle("About " + QCoreApplication::applicationName() + "...");
        dlg->setText(QCoreApplication::applicationName() + " version " + QCoreApplication::applicationVersion());
        dlg->exec();
    }
    catch (const std::exception& exc)
    {
        qDebug() << "EXCEPTION in fileAppAbout: " << exc.what() << endl;
    }
}

void MainWindow::onTranslate()
{
    try
    {
        this->translate(this->ui->txtLangIn->toPlainText(),
                        this->ui->cmbLangIn->currentData().toString(),
                        this->ui->cmbLangOut->currentData().toString());
    }
    catch (const std::exception& exc)
    {
        qDebug() << "EXCEPTION in fileAppNew: " << exc.what() << endl;
    }
}

void MainWindow::translate(const QString &value, const QString &fromLang, const QString &dstLang)
{
    QString jsonString = "{\"text\":[\"" + value + "\"],\"model_id\":\"" + fromLang +"-" + dstLang +"\"}";
    QByteArray postData = QByteArray::number(jsonString.size());

    QNetworkAccessManager * manager = new QNetworkAccessManager(this);

    QSettings settings;

    QUrlQuery params;
    params.addQueryItem("version", settings.value("IBMSettings/VersionDate").toString());

    QUrl url(settings.value("IBMSettings/ServiceUrl").toString() + "/v3/translate?" + params.query());
    QNetworkRequest request(url);

    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", postData);
    request.setRawHeader("x-watson-learning-opt-out", "true");
    request.setRawHeader("Authorization", this->buildAuthorizationItem().toLocal8Bit());

    //connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onRequestCompleted(QNetworkReply *)));
    QNetworkReply *reply = manager->post(request, jsonString.toUtf8());

    connect(reply, &QNetworkReply::finished, [=]() {
        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray bts = reply->readAll();
            if(bts.length() > 0)
            {
                QJsonDocument jsonResponse = QJsonDocument::fromJson(bts);
                QJsonObject jsonObject = jsonResponse.object();
                this->insertRow(jsonObject.value("word_count").toInt(), jsonObject.value("character_count").toInt());

                QJsonArray jsonArray = jsonObject["translations"].toArray();
                foreach (const QJsonValue & value, jsonArray)
                {
                    QJsonObject obj = value.toObject();
                    this->ui->txtLangOut->setPlainText(
                                obj.value("translation").toString());
                }
            }
        }
        else // handle error
        {
          qDebug() << reply->errorString();
        }
    });
}

void MainWindow::readLanguageList()
{
    QSettings settings;

    QUrlQuery params;
    params.addQueryItem("version", settings.value("IBMSettings/VersionDate").toString());

    QUrl url(settings.value("IBMSettings/ServiceUrl").toString() + "/v3/identifiable_languages?" + params.query());
    QNetworkRequest request(url);

    request.setRawHeader("Authorization", this->buildAuthorizationItem().toLocal8Bit());

    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {

        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray bts = reply->readAll();
            if(bts.length() > 0)
            {
                QJsonDocument jsonResponse = QJsonDocument::fromJson(bts);
                QJsonObject jsonObject = jsonResponse.object();
                QJsonArray jsonArray = jsonObject["languages"].toArray();

                QVector<QPair<QString,QString>> languageList;
                foreach (const QJsonValue & value, jsonArray)
                {
                    QJsonObject obj = value.toObject();
                    languageList.append(
                                QPair<QString, QString>(
                                    obj.value("language").toString(),
                                    obj.value("name").toString()));
                }

                this->ui->cmbLangIn->clear();
                this->ui->cmbLangOut->clear();

                QPair<QString,QString> pair;
                foreach(pair, languageList)
                {
                    QString displayText = pair.second + " (" + pair.first + ")";
                    this->ui->cmbLangIn->addItem(displayText, pair.first);
                    this->ui->cmbLangOut->addItem(displayText, pair.first);
                }

                this->ui->cmbLangIn->setCurrentIndex(this->ui->cmbLangIn->findData("en"));
                this->ui->cmbLangOut->setCurrentIndex(this->ui->cmbLangOut->findData("de"));
            }
        }
        else // handle error
        {
          qDebug() << reply->errorString();
        }
    });
}

QString MainWindow::buildAuthorizationItem()
{
    QSettings settings;
    QString concatenated = "apikey:"+settings.value("IBMSettings/AccessToken").toString();
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    return ("Basic " + data);
}
void MainWindow::insertRow(int numWordsCount, int numCharactersCount)
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int nRow = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(nRow, 0, new QTableWidgetItem(QDateTime::currentDateTime().time().toString()));
    ui->tableWidget->setItem(nRow, 1, new QTableWidgetItem(QString::number(numWordsCount)));
    ui->tableWidget->setItem(nRow, 2, new QTableWidgetItem(QString::number(numCharactersCount)));
}
