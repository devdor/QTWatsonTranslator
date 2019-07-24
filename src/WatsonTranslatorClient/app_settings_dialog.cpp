#include "app_settings_dialog.h"
#include "ui_app_settings_dialog.h"

AppSettingsDialog::AppSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsDialog)
{
    ui->setupUi(this);
}

AppSettingsDialog::~AppSettingsDialog()
{
    delete ui;
}

void AppSettingsDialog::Init(const QString &serviceUrl, const QString &accessToken, const QString &versionDate)
{
    this->ui->lineEditServiceUrl->setText(serviceUrl);
    this->ui->lineEditAccessToken->setText(accessToken);
    this->ui->lineEditVersionDate->setText(versionDate);
}

QString AppSettingsDialog::GetServiceUrl()
{
    return  this->ui->lineEditServiceUrl->text();
}

QString AppSettingsDialog::GetAccessToken()
{
    return  this->ui->lineEditAccessToken->text();
}

QString AppSettingsDialog::GetVersionDate()
{
    return  this->ui->lineEditVersionDate->text();
}
