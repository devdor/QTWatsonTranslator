#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>

namespace Ui {
class AppSettingsDialog;
}

class AppSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppSettingsDialog(QWidget *parent = nullptr);
    ~AppSettingsDialog();
    void Init(const QString &serviceUrl, const QString &accessToken, const QString &versionDate);

    QString GetServiceUrl();
    QString GetAccessToken();
    QString GetVersionDate();
private:
    Ui::AppSettingsDialog *ui;
     void setUpGUI();
};

#endif // APP_SETTINGS_H
