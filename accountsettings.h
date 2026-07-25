#ifndef ACCOUNTSETTINGS_H
#define ACCOUNTSETTINGS_H

#include <QDialog>

namespace Ui {
class AccountSettings;
}

class AccountSettings : public QDialog
{
    Q_OBJECT

public:
    explicit AccountSettings(QWidget *parent = nullptr);
    ~AccountSettings();

private slots:
    void on_btnSpasi_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AccountSettings *ui;
};

#endif
