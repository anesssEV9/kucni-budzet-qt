#ifndef SVITROSKOVI_H
#define SVITROSKOVI_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class SviTroskovi;
}

class SviTroskovi : public QDialog
{
    Q_OBJECT

public:
    explicit SviTroskovi(QWidget *parent = nullptr);
    ~SviTroskovi();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SviTroskovi *ui;
    QSqlQueryModel *model;
    void ucitajSveTroskove();
};

#endif
