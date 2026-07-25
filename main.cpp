#include "mainwindow.h"
#include <login.h>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(
    "QInputDialog { background-color: #1a1a1a; }"
    "QInputDialog QLabel { color: #cccccc; font-family: Calibri; font-size: 11pt; }"
    "QInputDialog QDoubleSpinBox { background-color: #1e1e1e; color: #ffffff; border: 1px solid #5a1a1a; border-radius: 6px; padding: 4px; }"
    "QInputDialog QPushButton { background: #7a1f1f; color: white; border-radius: 4px; padding: 5px 12px; font-weight: bold; }"
    "QInputDialog QPushButton:hover { background: #b33a3a; }"
    );
    a.setWindowIcon(QIcon(":/ikona/letter-w.png"));
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("BazaKucniBudzet.db");

    if (!db.open()) {
        QMessageBox::critical(0, "Greska baze",
            "Ne mogu otvoriti bazu podataka!\n" + db.lastError().text());
        return 1;
    }

    QSqlQuery upit;

    upit.exec(
        "CREATE TABLE IF NOT EXISTS rashodi ("
        "id         INTEGER PRIMARY KEY AUTOINCREMENT, "
        "naziv      TEXT    NOT NULL, "
        "iznos      REAL    NOT NULL, "
        "kategorija TEXT    NOT NULL, "
        "mjesec     INTEGER NOT NULL, "
        "godina     INTEGER NOT NULL)"
    );

    upit.exec(
        "CREATE TABLE IF NOT EXISTS prihodi ("
        "id     INTEGER PRIMARY KEY AUTOINCREMENT, "
        "iznos  REAL    NOT NULL, "
        "mjesec INTEGER NOT NULL, "
        "godina INTEGER NOT NULL)"
    );
    Login l;
    l.show();
    return a.exec();
}
