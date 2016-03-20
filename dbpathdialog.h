#ifndef DBPATHDIALOG_H
#define DBPATHDIALOG_H

#include <QDialog>
#include <QFileSystemModel>

#include "modelmanager.h"

namespace Ui {
class DbPathDialog;
}

class DbPathDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DbPathDialog(QWidget *parent = 0);
    ~DbPathDialog();

signals:
    void SetDbPath_SIGNAL(QString filepath);

private slots:
    void on_dir_tree_clicked(const QModelIndex &index);

    void on_Cancella_button_clicked();

    void on_Conferma_button_clicked();

    void on_Crea_db_button_clicked();

private:
    Ui::DbPathDialog    *ui;

    QFileSystemModel    *dir_model;
    QFileSystemModel    *file_model;

    QString             current_dir_model_item_path;
};

#endif // DBPATHDIALOG_H
