#include "ostalitroskovi.h"
#include "ui_ostalitroskovi.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSequentialAnimationGroup>
#include <QMenu>

OstaliTroskovi::OstaliTroskovi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OstaliTroskovi)
{
    ui->setupUi(this);
    ui->groupBox->setGeometry(ui->groupBox->x() - 2, ui->groupBox->y() + 4, ui->groupBox->width() + 54, 62);
    ui->tableView->setToolTip("Kliknite desnim klikom na bilo koji trošak da ga obrišete.");
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &OstaliTroskovi::prikaziKontekstniMeni);
    ui->label->setStyleSheet("color: #b33a3a; font-family: 'Segoe UI', Calibri; font-size: 20pt; font-weight: bold; background: transparent;");
    this->setWindowTitle("Ostali troškovi - walletWatch");
    ui->pushButton->setText("Unos");

    this->setStyleSheet(
        "QWidget {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #1a1a1a, stop:1 #000000);"
        "}"
        "QLabel { color: #cccccc; font-family: Calibri; font-size: 11pt; background: transparent; }"
        "QGroupBox { background: transparent; border: 1px solid #3a1010; border-radius: 6px; }"
        "QLineEdit {"
        "  background-color: #1e1e1e; color: #ffffff;"
        "  border: 1px solid #5a1a1a; border-radius: 6px;"
        "  padding: 4px 10px; font-family: Calibri; font-size: 10pt;"
        "}"
        "QLineEdit:focus { border: 1px solid #b33a3a; }"
        "QDoubleSpinBox {"
        "  background-color: #1e1e1e; color: #ffffff;"
        "  border: 1px solid #5a1a1a; border-radius: 6px;"
        "  padding: 4px 8px; font-family: Calibri; font-size: 10pt;"
        "}"
        "QDoubleSpinBox:focus { border: 1px solid #b33a3a; }"
        "QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {"
        "  background-color: #3a1010; border: none; width: 18px; border-radius: 3px;"
        "}"
        "QDoubleSpinBox::up-button:hover, QDoubleSpinBox::down-button:hover {"
        "  background-color: #b33a3a;"
        "}"
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 #7a1f1f, stop:1 #b33a3a);"
        "  color: white; border: none; border-radius: 8px;"
        "  padding: 6px 14px; font-family: Calibri; font-size: 11pt; font-weight: bold;"
        "}"
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8a2323, stop:1 #c14444); }"
        "QPushButton:pressed { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5c1515, stop:1 #8a2323); }"
        "QTableView {"
        "  background-color: #111111; alternate-background-color: #1a1a1a;"
        "  color: #e0e0e0; gridline-color: #2a2a2a;"
        "  border: 1px solid #3a1010; border-radius: 6px;"
        "  font-family: Calibri; font-size: 10pt;"
        "  selection-background-color: #b33a3a; selection-color: white;"
        "}"
        "QHeaderView::section {"
        "  background-color: #2a0a0a; color: #ffffff;"
        "  font-family: Calibri; font-size: 10pt; font-weight: bold;"
        "  padding: 6px; border: none;"
        "  border-bottom: 2px solid #b33a3a; border-right: 1px solid #1a1a1a;"
        "}"
        "QScrollBar:vertical { background: #111111; width: 10px; border-radius: 5px; }"
        "QScrollBar::handle:vertical { background: #b33a3a; border-radius: 5px; min-height: 20px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
        "QMenu {"
        "  background-color: #1e1e1e;"
        "  color: #ffffff;"
        "  border: 1px solid #5a1a1a;"
        "}"
        "QMenu::item:selected {"
        "  background-color: #b33a3a;"
        "  color: #ffffff;"
        "}"
        "QComboBox {"
        "  background-color: #1e1e1e; color: #ffffff;"
        "  border: 1px solid #5a1a1a; border-radius: 6px;"
        "  padding: 4px 10px; font-family: Calibri; font-size: 10pt;"
        "}"
        "QComboBox:focus { border: 1px solid #b33a3a; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox QAbstractItemView {"
        "  background-color: #1e1e1e; color: #ffffff;"
        "  border: 1px solid #5a1a1a; selection-background-color: #b33a3a; selection-color: white;"
        "}"
    );

    ui->columnView->hide();

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this,         SLOT(mjesecPromjenjen(int)));

    model = new QSqlTableModel(this);
    model->setTable("rashodi");
    model->setFilter("kategorija = 'Ostalo'");
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Naziv");
    model->setHeaderData(2, Qt::Horizontal, "Iznos (KM)");
    model->setHeaderData(3, Qt::Horizontal, "Kategorija");
    model->setHeaderData(4, Qt::Horizontal, "Mjesec");
    model->setHeaderData(5, Qt::Horizontal, "Godina");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->hideColumn(0);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->doubleSpinBox->setMaximum(1000000.00);

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

    ui->label->setText("Ostali troškovi");
    ui->label->setGeometry(20, 10, 380, 45);
    ui->label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->label->setStyleSheet(
        "QLabel {"
        "  color: #b33a3a;"
        "  font-family: 'Segoe UI', Calibri;"
        "  font-size: 22pt;"
        "  font-weight: bold;"
        "  background: transparent;"
        "  border: none;"
        "  padding: 0px;"
        "}"
    );
    ui->label->raise();
}

OstaliTroskovi::~OstaliTroskovi()
{
    delete ui;
}

void OstaliTroskovi::osvjeziPrikaz()
{
    int mjesec = ui->comboBox->currentIndex() + 1;
    model->setFilter(
        QString("kategorija = 'Ostalo' AND mjesec = %1 AND godina = 2026").arg(mjesec)
    );
    model->select();
}

double OstaliTroskovi::ucitajPrihode(int mjesec, int godina)
{
    QSqlQuery upit;
    upit.prepare("SELECT iznos FROM prihodi WHERE mjesec = :m AND godina = :g");
    upit.bindValue(":m", mjesec);
    upit.bindValue(":g", godina);
    upit.exec();
    if (upit.next()) return upit.value(0).toDouble();
    return 0.0;
}

void OstaliTroskovi::spremiPrihode(double iznos, int mjesec, int godina)
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

double OstaliTroskovi::kumulativnoRaspolozivo(int mjesec, int godina)
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

void OstaliTroskovi::osvjeziLabelu(int mjesec, int godina)
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
        boja    = "#cc0000";
        prefiks = "! ";
    } else if (procenatPreostalo < 20) {
        boja    = "#dd3300";
        prefiks = "";
    } else if (procenatPreostalo < 50) {
        boja    = "#cc8800";
        prefiks = "";
    } else {
        boja    = "#00cc55";
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

void OstaliTroskovi::azurirajPrikazPrihoda(int mjesec, int godina)
{
    double prihodi = ucitajPrihode(mjesec, godina);

    if (prihodi == 0.0) {
        bool ok;
        double unos = QInputDialog::getDouble(
            this, "Unos prihoda",
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

void OstaliTroskovi::mjesecPromjenjen(int index)
{
    int mjesec = index + 1;
    azurirajPrikazPrihoda(mjesec, 2026);
    osvjeziPrikaz();
}

void OstaliTroskovi::on_pushButton_clicked()
{
    QString naziv  = ui->lineEdit->text().trimmed();
    double  iznos  = ui->doubleSpinBox->value();
    int     mjesec = ui->comboBox->currentIndex() + 1;
    int     godina = 2026;

    if (naziv.isEmpty()) {
        QMessageBox::warning(this, "Upozorenje", "Unesite naziv troska!");
        return;
    }
    if (iznos <= 0) {
        QMessageBox::warning(this, "Upozorenje", "Iznos mora biti veci od 0!");
        return;
    }

    double raspolozivo = kumulativnoRaspolozivo(mjesec, godina);

    if (iznos > raspolozivo) {
        QMessageBox::warning(this, "Prekoracenje budzeta!",
            QString("Ne mozete unijeti ovaj trosak!\n\n"
                    "Raspolozivo stanje: %1 KM\n"
                    "Novi trosak:        %2 KM\n"
                    "Nedostaje:          %3 KM")
                .arg(raspolozivo, 0, 'f', 2)
                .arg(iznos, 0, 'f', 2)
                .arg(iznos - raspolozivo, 0, 'f', 2)
        );
        return;
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO rashodi (naziv, iznos, kategorija, mjesec, godina) "
        "VALUES (:naziv, :iznos, :kat, :mjesec, :godina)"
    );
    query.bindValue(":naziv",  naziv);
    query.bindValue(":iznos",  iznos);
    query.bindValue(":kat",    "Ostalo");
    query.bindValue(":mjesec", mjesec);
    query.bindValue(":godina", godina);

    if (query.exec()) {
        ui->lineEdit->clear();
        ui->doubleSpinBox->setValue(0.0);
        osvjeziPrikaz();
        osvjeziLabelu(mjesec, godina);
    } else {
        QMessageBox::critical(this, "Greska", "Greska pri upisivanju:\n" + query.lastError().text());
    }
}

void OstaliTroskovi::on_pushButton_2_clicked()
{
    this->close();
}

void OstaliTroskovi::on_tableView_clicked(const QModelIndex &index)
{
    if (index.column() == 1 || index.column() == 2){
        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    } else {
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void OstaliTroskovi::prikaziKontekstniMeni(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if (!index.isValid()) return;

    QMenu meni(this);
    QAction *akcijaObrisi = meni.addAction("Obrisi ovaj trosak");

    QAction *odabranaAkcija = meni.exec(ui->tableView->viewport()->mapToGlobal(pos));

    if (odabranaAkcija == akcijaObrisi) {
        int red = index.row();
        QString id = ui->tableView->model()->data(ui->tableView->model()->index(red, 0)).toString();

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
