#include "login.h"
#include "ui_login.h"
#include <mainwindow.h>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSequentialAnimationGroup>
#include <QSettings>
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Login - walletWatch");
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

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    QString user = ui->user->text();
    QString sifra = ui->sifra->text();

    QSettings settings("MojKucniBudzet", "LoginPodaci");
    QString tacanUser = settings.value("username", "adianesmelo").toString();
    QString tacnaSifra = settings.value("password", "1234").toString();

    if(user == tacanUser && sifra == tacnaSifra){
        MainWindow *glavniProzor = new MainWindow();
        glavniProzor->show();
        this->close();
    } else {
        QMessageBox::warning(this,"Greska","Pogresan user ili sifra!");
        ui->sifra->clear();
        ui->user->clear();
    }
}
