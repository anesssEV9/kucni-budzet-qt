#include "grafik.h"
#include "ui_grafik.h"
#include "qcustomplot.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
Grafik::Grafik(QWidget *parent) : QDialog(parent), ui(new Ui::Grafik)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Grafikon - walletWatch");
    barPrihodi = new QCPBars(ui->Grafik2->xAxis, ui->Grafik2->yAxis);
    barRashodi = new QCPBars(ui->Grafik2->xAxis, ui->Grafik2->yAxis);
    barPrihodi->setWidth(0.4);
    barRashodi->setWidth(0.4);
    barPrihodi->setBaseValue(0);
    barRashodi->setBaseValue(0);
    barPrihodi->setPen(QPen(QColor(231, 76, 60)));
    barPrihodi->setBrush(QColor(231, 76, 60));
    barRashodi->setPen(QPen(QColor(255, 255, 255)));
    barRashodi->setBrush(QColor(255, 255, 255));
    ui->comboMjesec->blockSignals(true);
    ui->comboMjesec->setCurrentIndex(0);
    ui->comboMjesec->blockSignals(false);
    nacrtajGrafik(1);
}
Grafik::~Grafik()
{
    delete ui;
}
void Grafik::on_comboMjesec_currentIndexChanged(int index)
{
    nacrtajGrafik(index + 1);
}
void Grafik::nacrtajGrafik(int mjesecBroj)
{
    double ukupnoRashodi = 0;
    double ukupnoPrihodi = 0;
    QSqlQuery q1;
    q1.prepare("SELECT COALESCE(SUM(iznos), 0) FROM rashodi WHERE mjesec = :mjesec AND godina = 2026");
    q1.bindValue(":mjesec", mjesecBroj);
    if (q1.exec() && q1.next()) {
        ukupnoRashodi = q1.value(0).toDouble();
    }
    QSqlQuery q2;
    q2.prepare("SELECT COALESCE(SUM(iznos), 0) FROM prihodi WHERE mjesec = :mjesec AND godina = 2026");
    q2.bindValue(":mjesec", mjesecBroj);
    if (q2.exec() && q2.next()) {
        ukupnoPrihodi = q2.value(0).toDouble();
    }
    if (ukupnoPrihodi <= 0.01) ukupnoPrihodi = 0;
    if (ukupnoRashodi <= 0.01) ukupnoRashodi = 0;
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2;
    labels << "Prihodi" << "Rashodi";
    #if QCUSTOMPLOT_VERSION >= 0x020000
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    textTicker->setSubTickCount(0);
    ui->Grafik2->xAxis->setTicker(textTicker);
    #else
    ui->Grafik2->xAxis->setAutoTicks(false);
    ui->Grafik2->xAxis->setAutoTickLabels(false);
    ui->Grafik2->xAxis->setTickVector(ticks);
    ui->Grafik2->xAxis->setTickVectorLabels(labels);
    ui->Grafik2->xAxis->setSubTickCount(0);
    #endif
    ui->Grafik2->xAxis->setRange(0, 3);
    ui->Grafik2->setBackground(QBrush(QColor(43, 43, 43)));
    ui->Grafik2->xAxis->setBasePen(QPen(Qt::white));
    ui->Grafik2->xAxis->setTickLabelColor(Qt::white);
    ui->Grafik2->yAxis->setBasePen(QPen(Qt::white));
    ui->Grafik2->yAxis->setTickLabelColor(Qt::white);
    ui->Grafik2->yAxis->grid()->setPen(QPen(QColor(100, 100, 100), 1, Qt::DotLine));
    double maxIznos = (ukupnoPrihodi > ukupnoRashodi) ? ukupnoPrihodi : ukupnoRashodi;
    if (maxIznos > 0) {
        ui->Grafik2->yAxis->setRange(0, maxIznos * 1.15);
        barPrihodi->clearData();
        barRashodi->clearData();
        barPrihodi->addData(1.0, ukupnoPrihodi);
        barRashodi->addData(2.0, ukupnoRashodi);
        barPrihodi->setVisible(true);
        barRashodi->setVisible(true);
    } else {
        ui->Grafik2->yAxis->setRange(0, 100);
        barPrihodi->clearData();
        barRashodi->clearData();
        barPrihodi->setVisible(false);
        barRashodi->setVisible(false);
    }
    ui->Grafik2->setPlottingHints(QCP::phForceRepaint);
    ui->Grafik2->replot();
}
void Grafik::on_btnNazad_clicked()
{
    this->close();
}

