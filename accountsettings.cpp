#include "accountsettings.h"
#include "ui_accountsettings.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSequentialAnimationGroup>
#include <QSettings>
#include <QMessageBox>

AccountSettings::AccountSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountSettings)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Promjena podataka - walletWatch");
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

AccountSettings::~AccountSettings()
{
    delete ui;
}

void AccountSettings::on_btnSpasi_clicked()
{
    QString noviUser = ui->txtNoviUser->text().trimmed();
    QString novaSifra = ui->txtNovaSifra->text();

    if(noviUser.isEmpty() || novaSifra.isEmpty()) {
        QMessageBox::warning(this, "Greska", "Polja ne smiju biti prazna!");
        return;
    }

    QSettings settings("MojKucniBudzet", "LoginPodaci");
    settings.setValue("username", noviUser);
    settings.setValue("password", novaSifra);

    QMessageBox::information(this, "Uspjeh", "Podaci za login su promijenjeni!");
}

void AccountSettings::on_pushButton_2_clicked()
{
    this->close();
}
