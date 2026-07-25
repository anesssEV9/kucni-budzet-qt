#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QIcon>
#include "fiksniTroskovi.h"
#include <QSqlQueryModel>
#include <QEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_unos_2_clicked();
    void on_unos_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    fiksniTroskovi *prozorFiksni;
    QSqlQueryModel *modelsvitroskovi;
    void refreshsvitroskovi();
    double dohvatiStanje();

    QIcon ikonaAbout;
    QIcon ikonaSupport;
    QIcon ikonaTranzakcije;
    QIcon ikonaAccount;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
