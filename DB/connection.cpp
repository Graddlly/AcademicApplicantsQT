#include <QSettings>
#include <QFile>
#include <QDir>
#include "DB/connection.h"

connection::connection()
{

}

void connection::createConnection()
{
    if (!QFile::exists("./db.ini"))
    {
        QFile file("./db.ini");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << "[db]\nhost=\ndb=\nusername=\npassword=";
        file.close();

        QMessageBox::warning(0, "Создание файла",
                             "Файл с настройками БД не был найден!\nСоздан новый конфигурационный файл!\nПожалуйста, заполните его и повторите попытку.");
        exit(1);
    }

    QSettings settings("./db.ini", QSettings::IniFormat);
    QVariant host = settings.value("db/host");
    QVariant database = settings.value("db/db");
    QVariant username = settings.value("db/username");
    QVariant password = settings.value("db/password");

    if (host == "" || database == "" || username == "" || password == "")
    {
        QDir dir(QCoreApplication::applicationDirPath());
        QString inifile = dir.absoluteFilePath("db.ini");

        QMessageBox::critical(0, "Пустой файл конфигурации",
                              "Пожалуйста, заполните файл конфигурации и повторите попытку.\n\nФайл конфигарации лежит по данному пути: " +
                              inifile);
        exit(1);
    }

    db = QSqlDatabase::addDatabase("QPSQL", "main_connection");
    db.setHostName(host.toString());
    db.setDatabaseName(database.toString());
    db.setUserName(username.toString());
    db.setPassword(password.toString());

    if (!db.open())
    {
        QMessageBox::warning(0, "Ошибка БД", db.lastError().text());
        exit(1);
    }
    else
    {
       QMessageBox::information(0, "Успешно", "Соединение с БД успешно");
    }
}
