#ifndef EXCELPATHDIALOG_H
#define EXCELPATHDIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include "xlsxdocument.h"
#include "modelmanager.h"

namespace Ui {
class ExcelPathDialog;
}

class ExcelPathDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExcelPathDialog(QWidget *parent = 0);
    ~ExcelPathDialog();

signals:
    void SetExcelPath_SIGNAL(QString filepath);
    void ExcelFileCreated(QString filepath);

private slots:
    void on_Conferma_button_clicked();

    void on_Cancella_button_clicked();

    void on_dir_tree_view_clicked(const QModelIndex &index);

    void on_create_excel_file_button_clicked();

private:
    Ui::ExcelPathDialog *ui;

    QFileSystemModel    *dir_model;
    QFileSystemModel    *file_model;

    QString             current_dir_model_item_path;
};

#endif // EXCELPATHDIALOG_H
