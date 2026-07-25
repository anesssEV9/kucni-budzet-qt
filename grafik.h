#ifndef GRAFIK_H
#define GRAFIK_H
#include <QDialog>
#include "qcustomplot.h"
namespace Ui { class Grafik; }
class Grafik : public QDialog
{
    Q_OBJECT
public:
    explicit Grafik(QWidget *parent = 0);
    ~Grafik();
private slots:
    void on_comboMjesec_currentIndexChanged(int index);
    void on_btnNazad_clicked();
private:
    Ui::Grafik *ui;
    QCPBars *barPrihodi;
    QCPBars *barRashodi;
    void nacrtajGrafik(int mjesecBroj);
};
#endif // GRAFIK_H
