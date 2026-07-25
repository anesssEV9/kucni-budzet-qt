#include "svitroskovi.h"
#include "ui_svitroskovi.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSequentialAnimationGroup>

SviTroskovi::SviTroskovi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SviTroskovi)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Svi troškovi - walletWatch");
    ui->tableView->setCornerButtonEnabled(false);
    ui->tableView->verticalHeader()->setVisible(false);
    model = new QSqlQueryModel(this);
    ucitajSveTroskove();
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QList<QPushButton*> svaDugmad = this->findChildren<QPushButton*>();
        for(QPushButton* dugme : svaDugmad) {
            if(!dugme) continue;
            dugme->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

            QGraphicsDropShadowEffect *sjaj = new QGraphicsDropShadowEffect(this);
            sjaj->setBlurRadius(0);
            sjaj->setColor(QColor("#ff3333"));
            sjaj->setOffset(0, 0);
            dugme->setGraphicsEffect(sjaj);

            QPropertyAnimation *animacijaSjaja = new QPropertyAnimation(sjaj, "blurRadius");
            animacijaSjaja->setDuration(400);
            animacijaSjaja->setStartValue(0);
            animacijaSjaja->setEndValue(25);

            QRect pocetnaGeometrija = dugme->geometry();
            QRect vecaGeometrija = pocetnaGeometrija;
            vecaGeometrija.setX(pocetnaGeometrija.x() - 3);
            vecaGeometrija.setY(pocetnaGeometrija.y() - 3);
            vecaGeometrija.setWidth(pocetnaGeometrija.width() + 6);
            vecaGeometrija.setHeight(pocetnaGeometrija.height() + 6);

            QPropertyAnimation *animacijaSirenja = new QPropertyAnimation(dugme, "geometry");
            animacijaSirenja->setDuration(350);
            animacijaSirenja->setStartValue(pocetnaGeometrija);
            animacijaSirenja->setEndValue(vecaGeometrija);
            animacijaSirenja->setEasingCurve(QEasingCurve::OutBack);

            animacijaSjaja->start(QAbstractAnimation::DeleteWhenStopped);
            animacijaSirenja->start(QAbstractAnimation::DeleteWhenStopped);
        }
}

SviTroskovi::~SviTroskovi()
{
    delete ui;
}

void SviTroskovi::ucitajSveTroskove()
{
    QSqlQuery query;
    query.prepare(
        "SELECT naziv, iznos || ' KM' AS Iznos, kategorija, "
        "printf('%02d/%d', mjesec, godina) AS Datum "
        "FROM rashodi "
        "ORDER BY id DESC"
    );

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, "Naziv troška");
        model->setHeaderData(1, Qt::Horizontal, "Iznos");
        model->setHeaderData(2, Qt::Horizontal, "Kategorija");
        model->setHeaderData(3, Qt::Horizontal, "Period");
    } else {
        QMessageBox::critical(this, "Greška", "Nemoguće učitati troškove: " + query.lastError().text());
    }
}
void SviTroskovi::on_pushButton_clicked()
{
    this->close();
}
