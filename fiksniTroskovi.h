#ifndef fiksniTroskovi_H
#define fiksniTroskovi_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class fiksniTroskovi;
}

class fiksniTroskovi : public QDialog
{
    Q_OBJECT

public:
    explicit fiksniTroskovi(QWidget *parent = 0);
    ~fiksniTroskovi();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_trenutnoStanje_clicked(const QModelIndex &index);
    void prikaziKontekstniMeni(const QPoint &pos);

private:
    Ui::fiksniTroskovi *ui;
    QSqlTableModel *model;

    void kreirajTabeluAkoNePostoji();
    void osvjeziPrikaz();
    double ucitajPrihode(int mjesec, int godina);
    void spremiPrihode(double iznos, int mjesec, int godina);
    void azurirajPrikazPrihoda(int mjesec, int godina);
    double kumulativnoRaspolozivo(int mjesec, int godina);
    void osvjeziLabelu(int mjesec, int godina);
};

#endif // fiksniTroskovi_H
