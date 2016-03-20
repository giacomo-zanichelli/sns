#include "dbpathdialog.h"
#include "ui_dbpathdialog.h"
#include <QStandardPaths>

DbPathDialog::DbPathDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbPathDialog)
{
    ui->setupUi(this);

    dir_model = new QFileSystemModel(this);
    qDebug() << dir_model;
    dir_model->setRootPath("/");
    dir_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

    QModelIndex index_to_show = dir_model->index(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory));

    ui->dir_tree->setModel(dir_model);
    ui->dir_tree->setRootIndex(index_to_show);
    ui->dir_tree->resizeColumnToContents(0);
    ui->dir_tree->setColumnHidden(1, true);
    ui->dir_tree->setColumnHidden(2, true);
    ui->dir_tree->setColumnHidden(3, true);
    ui->dir_tree->setHeaderHidden(true);

    file_model = new QFileSystemModel(this);
    file_model->setRootPath("/");
    file_model->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    ui->file_tree->setModel(file_model);
    ui->file_tree->setRootIndex(index_to_show);
    ui->file_tree->resizeColumnToContents(0);
    ui->file_tree->setColumnHidden(1, true);
    ui->file_tree->setColumnHidden(2, true);
    ui->file_tree->setColumnHidden(3, true);
    ui->file_tree->setHeaderHidden(true);

    file_model->setReadOnly(false);

}

DbPathDialog::~DbPathDialog()
{
    delete ui;
}

void DbPathDialog::on_dir_tree_clicked(const QModelIndex &index)
{
    current_dir_model_item_path = dir_model->fileInfo(index).absoluteFilePath();
    ui->file_tree->setRootIndex(file_model->setRootPath(current_dir_model_item_path));
}


void DbPathDialog::on_Cancella_button_clicked()
{
    close();
}

void DbPathDialog::on_Conferma_button_clicked()
{
    QString filepath = file_model->fileInfo(ui->file_tree->currentIndex()).absoluteFilePath();
    SetDbPath_SIGNAL(filepath);
    close();
}


void DbPathDialog::on_Crea_db_button_clicked()
{
    //create a new database in the current folder and let the user edit the title.
    ModelManager * model = new ModelManager();

    model->setPath(current_dir_model_item_path);
    if(model->createNewDatabase()) {
        qDebug() << "new database created";
    }

    delete model;
}
