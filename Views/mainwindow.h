#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "DB/connection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlRelationalTableModel *getTable;

private slots:
    void onRBClicked();
    void onBtnAddClicked();

private:
    Ui::MainWindow      *ui;
    connection          conn;

private:
    void ApplicantsTable();
    void createUI(QString table, QStringList headers, int type);
};

#endif // MAINWINDOW_H
