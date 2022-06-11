#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>

class connection
{
public:
    connection();
    QSqlDatabase db;
    void createConnection();
};

#endif // CONNECTION_H
