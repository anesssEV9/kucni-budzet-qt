#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fiksniTroskovi.h"
#include "ostalitroskovi.h"
#include "accountsettings.h"
#include "svitroskovi.h"
#include "grafik.h"
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSequentialAnimationGroup>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsDropShadowEffect *neonSjaj = new QGraphicsDropShadowEffect(this);
        neonSjaj->setBlurRadius(30);
        neonSjaj->setColor(QColor("#ff1a1a"));
        neonSjaj->setOffset(0, 0);
        ui->label->setGraphicsEffect(neonSjaj);
        this->centralWidget()->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 #1a1a1a, stop:1 #000000);"
    );
    modelsvitroskovi = new QSqlQueryModel(this);
    refreshsvitroskovi();
    QGraphicsOpacityEffect *efekatProvidnosti = new QGraphicsOpacityEffect(ui->svitroskovi);
    ui->svitroskovi->setGraphicsEffect(efekatProvidnosti);

    QPropertyAnimation *animacijaPojavljivanja = new QPropertyAnimation(efekatProvidnosti, "opacity");
    animacijaPojavljivanja->setDuration(800);
    animacijaPojavljivanja->setStartValue(0.0);
    animacijaPojavljivanja->setEndValue(1.0);

    animacijaPojavljivanja->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::refreshsvitroskovi);
    timer->start(1000);

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

    ikonaAbout       = ui->pushButton_3->icon();
    ikonaSupport     = ui->pushButton_4->icon();
    ikonaTranzakcije = ui->pushButton_5->icon();
    ikonaAccount     = ui->pushButton_6->icon();

    ui->unos->installEventFilter(this);
    ui->unos_2->installEventFilter(this);
    ui->pushButton_3->installEventFilter(this);
    ui->pushButton_4->installEventFilter(this);
    ui->pushButton_5->installEventFilter(this);
    ui->pushButton_6->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_unos_2_clicked()
{
        OstaliTroskovi *prozor = new OstaliTroskovi(this);
        prozor->setWindowFlags(Qt::Window);
        prozor->show();
}

void MainWindow::on_unos_clicked()
{
    prozorFiksni = new fiksniTroskovi(this);
    prozorFiksni->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    prozorFiksni->show();
}

void MainWindow::refreshsvitroskovi(){
    QSqlQuery query;
    query.prepare("SELECT naziv,iznos, kategorija FROM rashodi ORDER BY id DESC");
    if(query.exec()){
        modelsvitroskovi->setQuery(query);
        modelsvitroskovi->setHeaderData(0,Qt::Horizontal,"Naziv troska");
        modelsvitroskovi->setHeaderData(1,Qt::Horizontal,"Iznos (KM)");
        modelsvitroskovi->setHeaderData(2,Qt::Horizontal,"Kategorija");
        ui->svitroskovi->setModel(modelsvitroskovi);
        ui->svitroskovi->verticalHeader()->setVisible(false);
        ui->svitroskovi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    AccountSettings *prozor = new AccountSettings(this);
    prozor->exec();
}

void MainWindow::on_pushButton_5_clicked()
{
    SviTroskovi *prozor = new SviTroskovi(this);
    prozor->exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox msgBox(this);
        msgBox.setWindowTitle("O aplikaciji - walletWatch");
        msgBox.setText(
            "<div style='color: #ffffff;'>"
            "<h3><b>WalletWatch v1.0</b></h3>"
            "<p>Aplikacija za pouzdano i jednostavno upravljanje kućnim budžetom.</p>"
            "<hr style='border: 0; border-top: 1px solid #444444;'>"
            "<p><b>Razvojni tim:</b></p>"
            "<ul>"
            "  <li>Adi Delić</li>"
            "  <li>Anes Meškić</li>"
            "  <li>Mevludin Hasanbašić</li>"
            "</ul>"
            "<p>© 2026 WalletWatch d.o.o. Sva prava zadržana.</p>"
            "</div>"
        );
        msgBox.setStyleSheet(
                "QMessageBox { background-color: #2b2b2b; }"
                "QLabel { color: #ffffff; font-size: 13px; }"
                "QPushButton { background-color: #3b3b3b; color: #ffffff; border: 1px solid #555555; padding: 5px 15px; border-radius: 3px; }"
                "QPushButton:hover { background-color: #4b4b4b; }"
            );
        msgBox.exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    QMessageBox msgBox(this);
        msgBox.setWindowTitle("Tehnička podrška - walletWatch");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(
            "Trebate pomoć ili ste uočili grešku u radu aplikacije?\n\n"
            "Naš tim za podršku vam stoji na raspolaganju:\n"
            "📧 Email: support@walletwatch.com\n"
            "📞 Telefon: +387 33 123-456\n"
            "🕒 Radno vrijeme: Pon - Pet (08:00 - 16:00h)\n\n"
            "Prilikom prijave greške, molimo vas da navedete kratak opis problema."
        );
        msgBox.setStyleSheet(
                "QMessageBox { background-color: #2b2b2b; }"
                "QLabel { color: #ffffff; font-size: 13px; }"
                "QPushButton { background-color: #3b3b3b; color: #ffffff; border: 1px solid #555555; padding: 5px 15px; border-radius: 3px; }"
                "QPushButton:hover { background-color: #4b4b4b; }"
            );
        msgBox.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    Grafik *prozor = new Grafik(this);
    prozor->exec();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->unos || obj == ui->unos_2) {
        if (event->type() == QEvent::Enter) {
            double stanje = dohvatiStanje();
            QString boja;
            if (stanje < 0)
                boja = "#cc0000";
            else if (stanje < 500)
                boja = "#cc8800";
            else
                boja = "#00cc55";

            QPushButton *btn = (QPushButton*)obj;
            btn->setText(QString("Stanje: %1 KM").arg(stanje, 0, 'f', 2));
            btn->setStyleSheet(QString(
                "QPushButton {"
                "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0a0a0a, stop:1 #1a1a1a);"
                "  color: %1; border: 2px solid %1; border-radius: 14px;"
                "  font-family: Calibri; font-size: 16pt; font-weight: bold;"
                "}").arg(boja));

        } else if (event->type() == QEvent::Leave) {
            QPushButton *btn = (QPushButton*)obj;
            btn->setText(obj == ui->unos ? "Fiksni troškovi" : "Ostali troškovi");
            btn->setStyleSheet(
                "QPushButton {"
                "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7a1f1f, stop:1 #b33a3a);"
                "  color: white; border: none; border-radius: 14px;"
                "  font-family: Calibri; font-size: 14pt; font-weight: bold;"
                "}");
        }
    }

    if (obj == ui->pushButton_3 || obj == ui->pushButton_4 ||
        obj == ui->pushButton_5 || obj == ui->pushButton_6) {

        QPushButton *btn = (QPushButton*)obj;

        if (event->type() == QEvent::Enter) {
            QString tekst;
            QIcon iconaZaVracat;
            if (obj == ui->pushButton_3) {tekst = "About"; iconaZaVracat = ikonaAbout; }
            else if (obj == ui->pushButton_4) {tekst = "Support"; iconaZaVracat = ikonaSupport; }
            else if (obj == ui->pushButton_5) {tekst = "Tranzakcije"; iconaZaVracat = ikonaTranzakcije; }
            else {tekst = "Account"; iconaZaVracat = ikonaAccount; }

            btn->setIcon(QIcon());
            btn->setIconSize(QSize(0, 0));
            btn->setText(tekst);

        } else if (event->type() == QEvent::Leave) {
            btn->setText("");
            if (obj == ui->pushButton_3) btn->setIcon(ikonaAbout);
            else if (obj == ui->pushButton_4) btn->setIcon(ikonaSupport);
            else if (obj == ui->pushButton_5) btn->setIcon(ikonaTranzakcije);
            else btn->setIcon(ikonaAccount);
            btn->setIconSize(QSize(32, 32));
        }
    }

    return false;
}

double MainWindow::dohvatiStanje()
{
    int m = QDate::currentDate().month();
    int g = QDate::currentDate().year();

    QSqlQuery q1;
    q1.prepare("SELECT COALESCE(SUM(iznos), 0) FROM prihodi WHERE mjesec <= :m AND godina = :g");
    q1.bindValue(":m", m); q1.bindValue(":g", g); q1.exec();
    double prihodi = q1.next() ? q1.value(0).toDouble() : 0.0;

    QSqlQuery q2;
    q2.prepare("SELECT COALESCE(SUM(iznos), 0) FROM rashodi WHERE mjesec <= :m AND godina = :g");
    q2.bindValue(":m", m); q2.bindValue(":g", g); q2.exec();
    double troskovi = q2.next() ? q2.value(0).toDouble() : 0.0;

    return prihodi - troskovi;
}
