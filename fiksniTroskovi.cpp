#include "fiksniTroskovi.h"
#include "ui_fiksniTroskovi.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSequentialAnimationGroup>
#include <QMenu>

fiksniTroskovi::fiksniTroskovi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fiksniTroskovi)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Fiksni troškovi - walletWatch");
    ui->trenutnoStanje->setToolTip("Kliknite desnim klikom na bilo koji trošak da ga obrišete.");
    ui->trenutnoStanje->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->trenutnoStanje, &QTableView::customContextMenuRequested, this, &fiksniTroskovi::prikaziKontekstniMeni);

    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Greska", "Baza podataka nije otvorena!");
        return;
    }

    kreirajTabeluAkoNePostoji();

    model = new QSqlTableModel(this);
    model->setTable("rashodi");
    model->setFilter("kategorija = 'Fiksni'");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Naziv");
    model->setHeaderData(2, Qt::Horizontal, "Iznos (KM)");
    model->setHeaderData(3, Qt::Horizontal, "Kategorija");
    model->setHeaderData(4, Qt::Horizontal, "Mjesec");
    model->setHeaderData(5, Qt::Horizontal, "Godina");
    model->select();

    ui->trenutnoStanje->setModel(model);
    ui->trenutnoStanje->verticalHeader()->setVisible(false);
    ui->trenutnoStanje->hideColumn(0);
    ui->trenutnoStanje->horizontalHeader()->setStretchLastSection(true);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->trenutnoStanje->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->trenutnoStanje->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    int mjesec = ui->comboBox->currentIndex() + 1;
    azurirajPrikazPrihoda(mjesec, 2026);
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
            animacijaSjaja->setDuration(400); // 0.4 sekunde
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
        ui->trenutnoStanje->setStyleSheet(ui->trenutnoStanje->styleSheet() +
            "QMenu { background-color: #1e1e1e; color: #ffffff; border: 1px solid #5a1a1a; }"
            "QMenu::item:selected { background-color: #b33a3a; color: #ffffff; }"
        );
}

fiksniTroskovi::~fiksniTroskovi()
{
    delete ui;
}

void fiksniTroskovi::kreirajTabeluAkoNePostoji()
{
    QSqlQuery upit;
    upit.exec(
        "CREATE TABLE IF NOT EXISTS rashodi ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "naziv TEXT NOT NULL, iznos REAL NOT NULL, "
        "kategorija TEXT NOT NULL, "
        "mjesec INTEGER NOT NULL, godina INTEGER NOT NULL)"
    );
    upit.exec(
        "CREATE TABLE IF NOT EXISTS prihodi ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "iznos REAL NOT NULL, "
        "mjesec INTEGER NOT NULL, godina INTEGER NOT NULL)"
    );
}

void fiksniTroskovi::osvjeziPrikaz()
{
    int mjesec = ui->comboBox->currentIndex() + 1;
    model->setFilter(
        QString("kategorija = 'Fiksni' AND mjesec = %1 AND godina = 2026").arg(mjesec)
    );
    model->select();
}

double fiksniTroskovi::ucitajPrihode(int mjesec, int godina)
{
    QSqlQuery upit;
    upit.prepare("SELECT iznos FROM prihodi WHERE mjesec = :m AND godina = :g");
    upit.bindValue(":m", mjesec);
    upit.bindValue(":g", godina);
    upit.exec();
    if (upit.next()) return upit.value(0).toDouble();
    return 0.0;
}

void fiksniTroskovi::spremiPrihode(double iznos, int mjesec, int godina)
{
    QSqlQuery provjera;
    provjera.prepare("SELECT id FROM prihodi WHERE mjesec = :m AND godina = :g");
    provjera.bindValue(":m", mjesec);
    provjera.bindValue(":g", godina);
    provjera.exec();

    QSqlQuery upit;
    if (provjera.next())
        upit.prepare("UPDATE prihodi SET iznos = :iznos WHERE mjesec = :m AND godina = :g");
    else
        upit.prepare("INSERT INTO prihodi (iznos, mjesec, godina) VALUES (:iznos, :m, :g)");

    upit.bindValue(":iznos", iznos);
    upit.bindValue(":m",     mjesec);
    upit.bindValue(":g",     godina);
    upit.exec();
}

double fiksniTroskovi::kumulativnoRaspolozivo(int mjesec, int godina)
{
    QSqlQuery upitPrihodi;
    upitPrihodi.prepare(
        "SELECT COALESCE(SUM(iznos), 0) FROM prihodi "
        "WHERE mjesec <= :m AND godina = :g"
    );
    upitPrihodi.bindValue(":m", mjesec);
    upitPrihodi.bindValue(":g", godina);
    upitPrihodi.exec();
    double ukupniPrihodi = upitPrihodi.next() ? upitPrihodi.value(0).toDouble() : 0.0;

    QSqlQuery upitTroskovi;
    upitTroskovi.prepare(
        "SELECT COALESCE(SUM(iznos), 0) FROM rashodi "
        "WHERE mjesec <= :m AND godina = :g"
    );
    upitTroskovi.bindValue(":m", mjesec);
    upitTroskovi.bindValue(":g", godina);
    upitTroskovi.exec();
    double ukupniTroskovi = upitTroskovi.next() ? upitTroskovi.value(0).toDouble() : 0.0;

    return ukupniPrihodi - ukupniTroskovi;
}

void fiksniTroskovi::osvjeziLabelu(int mjesec, int godina)
{
    double raspolozivo = kumulativnoRaspolozivo(mjesec, godina);

    QSqlQuery q;
    q.prepare("SELECT COALESCE(SUM(iznos), 0) FROM prihodi WHERE mjesec <= :m AND godina = :g");
    q.bindValue(":m", mjesec);
    q.bindValue(":g", godina);
    q.exec();
    double ukupniPrihodi = q.next() ? q.value(0).toDouble() : 0.0;

    int procenatPreostalo = 0;
    if (ukupniPrihodi > 0)
        procenatPreostalo = (int)((raspolozivo / ukupniPrihodi) * 100.0);

    QString boja;
    QString prefiks;

    if (raspolozivo < 0) {
        boja    = "#cc0000";   // tamno crvena - u minusu
        prefiks = "! ";
    } else if (procenatPreostalo < 20) {
        boja    = "#dd3300";   // crvena - kritično malo
        prefiks = "";
    } else if (procenatPreostalo < 50) {
        boja    = "#cc8800";   // narandzasta - paznja
        prefiks = "";
    } else {
        boja    = "#00cc55";   // zelena - sve OK
        prefiks = "";
    }

    ui->lblPrihodi->setText(
        QString("%1Stanje: %2 KM")
            .arg(prefiks)
            .arg(raspolozivo, 0, 'f', 2)
    );

    ui->lblPrihodi->setStyleSheet(
        QString(
            "QLabel {"
            "  font-family: Calibri; font-size: 14pt; font-weight: bold;"
            "  color: %1; background: transparent;"
            "  border: 2px solid %1; border-radius: 6px; padding: 4px 12px;"
            "}"
        ).arg(boja)
    );
}

void fiksniTroskovi::azurirajPrikazPrihoda(int mjesec, int godina)
{
    double prihodi = ucitajPrihode(mjesec, godina);

    if (prihodi == 0.0) {
        bool ok;
        double unos = QInputDialog::getDouble(
            this,
            "Unos prihoda",
            QString("Unesite prihode za %1. mjesec %2:\n"
                    "(Ostatak iz prethodnih mjeseci se automatski prenosi)")
                .arg(mjesec).arg(godina),
            0.0, 0.0, 999999.0, 2, &ok
        );
        if (ok)
            spremiPrihode(unos, mjesec, godina);
    }

    osvjeziLabelu(mjesec, godina);
}

void fiksniTroskovi::on_comboBox_currentIndexChanged(int index)
{
    int mjesec = index + 1;
    azurirajPrikazPrihoda(mjesec, 2026);
    osvjeziPrikaz();
}

void fiksniTroskovi::on_pushButton_clicked()
{
    int    mjesec = ui->comboBox->currentIndex() + 1;
    int    godina = 2026;

    double noviTroskovi = 0;
    noviTroskovi += ui->spinBox->value();
    noviTroskovi += ui->spinBox_2->value();
    noviTroskovi += ui->spinBox_3->value();
    noviTroskovi += ui->spinBox_4->value();
    noviTroskovi += ui->spinBox_5->value();
    noviTroskovi += ui->spinBox_6->value();
    noviTroskovi += ui->spinBox_7->value();

    if (noviTroskovi == 0) {
        QMessageBox::warning(this, "Upozorenje", "Niste unijeli nijedan iznos!");
        return;
    }

    double raspolozivo = kumulativnoRaspolozivo(mjesec, godina);

    if (noviTroskovi > raspolozivo) {
        QMessageBox::warning(this, "Prekoracenje budzeta!",
            QString("Ne mozete unijeti ove troskove!\n\n"
                    "Raspolozivo stanje: %1 KM\n"
                    "Novi troskovi:      %2 KM\n"
                    "Nedostaje:          %3 KM")
                .arg(raspolozivo, 0, 'f', 2)
                .arg(noviTroskovi, 0, 'f', 2)
                .arg(noviTroskovi - raspolozivo, 0, 'f', 2)
        );
        return;
    }

    struct Trosak { QString naziv; int iznos; };
    Trosak lista[7];
    lista[0].naziv = "Rezije";      lista[0].iznos = ui->spinBox->value();
    lista[1].naziv = "Komunalije";  lista[1].iznos = ui->spinBox_2->value();
    lista[2].naziv = "Vozilo";      lista[2].iznos = ui->spinBox_3->value();
    lista[3].naziv = "Investicije"; lista[3].iznos = ui->spinBox_4->value();
    lista[4].naziv = "Stednja";     lista[4].iznos = ui->spinBox_5->value();
    lista[5].naziv = "Kredit";      lista[5].iznos = ui->spinBox_6->value();
    lista[6].naziv = "Zabava";      lista[6].iznos = ui->spinBox_7->value();

    bool sveOk = true;
    for (int i = 0; i < 7; i++) {
        if (lista[i].iznos > 0) {
            QSqlQuery upit;
            upit.prepare(
                "INSERT INTO rashodi (naziv, iznos, kategorija, mjesec, godina) "
                "VALUES (:naziv, :iznos, :kategorija, :mjesec, :godina)"
            );
            upit.bindValue(":naziv",      lista[i].naziv);
            upit.bindValue(":iznos",      lista[i].iznos);
            upit.bindValue(":kategorija", "Fiksni");
            upit.bindValue(":mjesec",     mjesec);
            upit.bindValue(":godina",     godina);
            if (!upit.exec()) sveOk = false;
        }
    }

    if (sveOk) {
        QMessageBox::information(this, "Uspjeh", "Troskovi su uspjesno upisani!");
        ui->spinBox->setValue(0);   ui->spinBox_2->setValue(0);
        ui->spinBox_3->setValue(0); ui->spinBox_4->setValue(0);
        ui->spinBox_5->setValue(0); ui->spinBox_6->setValue(0);
        ui->spinBox_7->setValue(0);
        osvjeziPrikaz();
        osvjeziLabelu(mjesec, godina); // azuriraj odmah nakon unosa
    } else {
        QMessageBox::critical(this, "Greska", "Doslo je do greske pri upisivanju!");
    }
}

void fiksniTroskovi::on_pushButton_2_clicked()
{
    this->close();
}

void fiksniTroskovi::on_trenutnoStanje_clicked(const QModelIndex &index)
{
    if (index.column() != 2){
        ui->trenutnoStanje->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }else{
        ui->trenutnoStanje->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    }
}
void fiksniTroskovi::prikaziKontekstniMeni(const QPoint &pos)
{
    QModelIndex index = ui->trenutnoStanje->indexAt(pos);
    if (!index.isValid()) return;
    QMenu meni(this);
    meni.setStyleSheet("QMenu { background-color: #1e1e1e; color: white; border: 1px solid #5a1a1a; } QMenu::item { color: white; } QMenu::item:selected { background-color: #b33a3a; }");
    QAction *akcijaObrisi = meni.addAction("Obrisi ovaj trosak");
    QAction *odabranaAkcija = meni.exec(ui->trenutnoStanje->viewport()->mapToGlobal(pos));
    if (odabranaAkcija == akcijaObrisi) {
        int red = index.row();
        QString id = ui->trenutnoStanje->model()->data(ui->trenutnoStanje->model()->index(red, 0)).toString();
        QMessageBox::StandardButton potvrda;
        potvrda = QMessageBox::question(this, "Brisanje troska",
                                        "Da li ste sigurni da zelite obrisati ovaj trosak?",
                                        QMessageBox::Yes | QMessageBox::No);

        if (potvrda == QMessageBox::Yes) {
            QSqlQuery query;
            query.prepare("DELETE FROM rashodi WHERE id = :id");
            query.bindValue(":id", id);
            if(query.exec()) {
                osvjeziPrikaz();
                osvjeziLabelu(ui->comboBox->currentIndex() + 1, 2026);
            } else {
                QMessageBox::critical(this, "Greska", query.lastError().text());
            }
        }
    }
}
