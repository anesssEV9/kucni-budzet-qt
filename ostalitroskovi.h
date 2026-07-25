#ifndef OSTALITROSKOVI_H
#define OSTALITROSKOVI_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMessageBox>

namespace Ui {
class OstaliTroskovi;
}

class OstaliTroskovi : public QWidget
{
    Q_OBJECT

public:
    explicit OstaliTroskovi(QWidget *parent = 0);
    ~OstaliTroskovi();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void mjesecPromjenjen(int index);
    void on_tableView_clicked(const QModelIndex &index);
    void prikaziKontekstniMeni(const QPoint &pos);

private:
    Ui::OstaliTroskovi *ui;
    QSqlTableModel *model;

    void osvjeziPrikaz();
    double ucitajPrihode(int mjesec, int godina);
    void spremiPrihode(double iznos, int mjesec, int godina);
    void azurirajPrikazPrihoda(int mjesec, int godina);
    double kumulativnoRaspolozivo(int mjesec, int godina);
    void osvjeziLabelu(int mjesec, int godina);
};

#endif // OSTALITROSKOVI_H
