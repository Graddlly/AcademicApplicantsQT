#include "Views/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    connect(ui -> rbApplicants, SIGNAL(released()), this, SLOT(onRBClicked()));
    connect(ui -> rbMarks, SIGNAL(released()), this, SLOT(onRBClicked()));
    connect(ui -> rbPassports, SIGNAL(released()), this, SLOT(onRBClicked()));
    connect(ui -> rbGroups, SIGNAL(released()), this, SLOT(onRBClicked()));
    connect(ui -> rbFaculties, SIGNAL(released()), this, SLOT(onRBClicked()));
    connect(ui -> rbRegistrations, SIGNAL(released()), this, SLOT(onRBClicked()));
    connect(ui -> rbSpecialities, SIGNAL(released()), this, SLOT(onRBClicked()));

    connect(ui -> btnAdd, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
    connect(ui -> btnDelete, SIGNAL(clicked()), this, SLOT(onBtnDeleteClicked()));

    if (!QSqlDatabase::drivers().contains("QPSQL"))
    {
       QMessageBox::critical(this, "Драйвер", "Драйвера не существует!");
       exit(1);
    };
    conn.createConnection();

    ui -> rbApplicants -> setChecked(true);
    ApplicantsTable();
}

MainWindow::~MainWindow()
{
    delete ui;
    QSqlDatabase::removeDatabase("main_connection");
}

void MainWindow::ApplicantsTable()
{
    QStringList headers;
    headers << "ID абитуриента" << "Полное имя" << "День рождения" << "Серия/номер паспорта" << "Учебное заведение"
            << "Дата окончания учебного заведения" << "Золотая медаль" << "Номер документа об образовании";

    createUI("applicants", headers, 0);
}

void MainWindow::onRBClicked()
{
    if (ui -> rbApplicants -> isChecked()) ApplicantsTable();
    else if (ui -> rbMarks -> isChecked())
    {
        QStringList headers;
        headers << "ID оценок" << "ID абитуриента" << "Экзамен №1" << "Экзамен №2"
                << "Экзамен №3" << "Среднее за экзамены";

        createUI("marks", headers, 1);
    }
    else if (ui -> rbPassports -> isChecked())
    {
        QStringList headers;
        headers << "ID паспорта" << "Кем выдан" << "Когда выдан" << "Серия и номер";

        createUI("passports", headers, 2);
    }
    else if (ui -> rbGroups -> isChecked())
    {
        QStringList headers;
        headers << "ID группы" << "Название группы";

        createUI("groups", headers, 3);
    }
    else if (ui -> rbFaculties -> isChecked())
    {
        QStringList headers;
        headers << "ID факультета" << "Название факультета";

        createUI("faculties", headers, 4);
    }
    else if (ui -> rbRegistrations -> isChecked())
    {
        QStringList headers;
        headers << "ID регистрации" << "ID абитуриента" << "ID группы"
                << "ID факультета" << "ID специальности";

        createUI("registrations", headers, 5);
    }
    else if (ui -> rbSpecialities -> isChecked())
    {
        QStringList headers;
        headers << "ID специальности" << "Название специальности";

        createUI("specialties", headers, 6);
    }
}

void MainWindow::createUI(QString table, QStringList headers, int type)
{
    getTable = new QSqlRelationalTableModel(0, conn.db);
    getTable -> setTable(table);

    for(int i = 0, j = 0; i < getTable -> columnCount(); i++, j++)
    {
        getTable -> setHeaderData(i, Qt::Horizontal, headers[j]);
    }

    getTable -> setSort(0, Qt::AscendingOrder);
    if (type == 3 || type == 4 || type == 6)
    {
        getTable -> setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
        ui -> tableView -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    else
    {
        getTable -> setEditStrategy(QSqlRelationalTableModel::OnRowChange);
        ui -> tableView -> setEditTriggers(QAbstractItemView::DoubleClicked);
    }
    getTable -> select();

    ui -> tableView -> setModel(getTable);
    ui -> tableView -> verticalHeader() -> setVisible(false);
    ui -> tableView -> horizontalHeader() -> setDefaultSectionSize(150);
    ui -> tableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
    ui -> tableView -> setSelectionMode(QAbstractItemView::SingleSelection);
    ui -> tableView -> resizeRowsToContents();
    ui -> tableView -> setItemDelegate(new QSqlRelationalDelegate(ui -> tableView));
    ui -> tableView -> horizontalHeader() -> setStretchLastSection(true);
}

void MainWindow::onBtnAddClicked()
{
    getTable -> insertRow(getTable -> rowCount());
}

void MainWindow::onBtnDeleteClicked()
{
    getTable -> removeRow(ui -> tableView -> selectionModel() -> selectedRows().first().row());
}
