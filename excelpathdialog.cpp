#include "excelpathdialog.h"
#include "ui_excelpathdialog.h"
#include <QStandardPaths>
#include <QtDebug>

ExcelPathDialog::ExcelPathDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExcelPathDialog)
{
    ui->setupUi(this);

    dir_model = new QFileSystemModel(this);
    dir_model->setRootPath("/");
    dir_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

    QModelIndex index_to_show = dir_model->index(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory));

    ui->dir_tree_view->setModel(dir_model);
    ui->dir_tree_view->setRootIndex(index_to_show);
    ui->dir_tree_view->resizeColumnToContents(0);
    ui->dir_tree_view->setColumnHidden(1, true);
    ui->dir_tree_view->setColumnHidden(2, true);
    ui->dir_tree_view->setColumnHidden(3, true);
    ui->dir_tree_view->setHeaderHidden(true);

    file_model = new QFileSystemModel(this);
    file_model->setRootPath("/");
    file_model->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    ui->file_tree_view->setModel(file_model);
    ui->file_tree_view->setRootIndex(index_to_show);
    ui->file_tree_view->resizeColumnToContents(0);
    ui->file_tree_view->setColumnHidden(1, true);
    ui->file_tree_view->setColumnHidden(2, true);
    ui->file_tree_view->setColumnHidden(3, true);
    ui->file_tree_view->setHeaderHidden(true);

    file_model->setReadOnly(false);
}

ExcelPathDialog::~ExcelPathDialog()
{
    delete ui;
}

void ExcelPathDialog::on_Conferma_button_clicked()
{
    QString filepath = file_model->fileInfo(ui->file_tree_view->currentIndex()).absoluteFilePath();
    SetExcelPath_SIGNAL(filepath);
    close();
}

void ExcelPathDialog::on_Cancella_button_clicked()
{
    close();
}

void ExcelPathDialog::on_dir_tree_view_clicked(const QModelIndex &index)
{
    current_dir_model_item_path = dir_model->fileInfo(index).absoluteFilePath();
    QString path = dir_model->fileInfo(index).absoluteFilePath();
    ui->file_tree_view->setRootIndex(file_model->setRootPath(path));
}


void ExcelPathDialog::on_create_excel_file_button_clicked()
{
    QString filename = current_dir_model_item_path + "/excel_db.xlsx";
    QXlsx::Document * excel_file = new QXlsx::Document(filename);

    excel_file->write(1, 1 , "ID");
    excel_file->write(1, 2 , "COGNOME");
    excel_file->write(1, 3 , "NOME");
    excel_file->write(1, 4 , "VIA E CIVICO");
    excel_file->write(1, 5 , "CAP");
    excel_file->write(1, 6 , "COMUNE");
    excel_file->write(1, 7 , "PROVINCIA");
    excel_file->write(1, 8 , "CODICE FISCALE");
    excel_file->write(1, 9 , "PROFESSIONE");
    excel_file->write(1, 10 , "EMAIL");
    excel_file->write(1, 11 , "TELEFONO");
    excel_file->write(1, 12 , "BREVETTO");
    excel_file->write(1, 13 , "VALIDITA'");
    excel_file->write(1, 14 , "DATA DI RILASCIO");
    excel_file->write(1, 15 , "ULTIMA NOTIFICA");

    ModelManager * model = new ModelManager();
    model->setExcelFile(excel_file);

    QVector<int> id_people = model->getAllPersonId();

    foreach(int id_person, id_people) {
        Person * person = model->buildPerson(id_person);

        QVector<int> id_licenses = model->getAllLicensesId(id_person);
        foreach(int id_license, id_licenses) {
            person->setLicense(model->getLicenseNameFromId(id_license), model->getLicenseDateOfIssue(id_person, id_license), model->getLicenseValidity(id_license), model->getLastNotification(id_person, id_license));
            model->addPersonExcel(id_person, person);
        }
        model->addAddressExcel(model->getPersonRows(id_person), person->getAddress());
        delete person;
    }
    delete model;

    excel_file->save();
}
