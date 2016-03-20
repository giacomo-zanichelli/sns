#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemModel>
#include "dbpathdialog.h"
#include "validator.h"
#include "excelpathdialog.h"
#include "modelmanager.h"
#include "person.h"
#include "confirmationdialog.h"
#include "xlsxdocument.h"
#include "searchdialog.h"
#include "smtp.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

private slots:
    //register new person tab
    void on_Inserisci_button_clicked();
    void on_Cancella_button_clicked();

    //search person tab
    void setSearchResultsMode(bool expiring);
    void on_Spedisci_avviso_button_clicked();
    void on_Elimina_corsista_button_clicked();
    void on_Elimina_brevetto_button_clicked();
    void on_add_license_button_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_change_is_valid_button_clicked();
    void on_show_search_dialog_button_clicked();
    void on_notified_before_button_clicked();
    void on_never_notified_button_clicked();
    void on_deselect_all_button_clicked();
    void on_select_all_button_clicked();
    void on_refresh_button_clicked();

    //database import tab
    void on_dir_View_clicked(const QModelIndex &index);
    void on_dir_View_2_clicked(const QModelIndex &index);
    void on_file_View_2_doubleClicked(const QModelIndex &index);
    void on_file_View_doubleClicked(const QModelIndex &index);

    //settings tab
    void on_database_path_button_clicked();
    void saveDbPath(QString db_path);
    void on_Conferma_button_clicked();

    void on_Excel_path_button_clicked();
    void saveExcelPath(QString excel_path);

    void on_subject_line_editingFinished();
    void on_body_text_textChanged();

    void on_notification_month_combobox_currentIndexChanged(const QString &month);

    void on_max_auto_email_spinbox_valueChanged(int max_email);

private:
    Ui::MainWindow *ui;

    //settings tab
    DbPathDialog            *database_path_dialog;
    ExcelPathDialog         *excel_path_dialog;
    Smtp                    *smtp;


    //search person tab
    SearchDialog            *search_dialog;
    QFileSystemModel        *search_file_directory_model;
    QFileSystemModel        *search_file_file_model;
    QSortFilterProxyModel   *search_person_results_proxy_model;

    //confirmation dialogs
    ConfirmationDialog      *confirmation_dialog;

    //utility
    ModelManager            *model;
    Validator               validator;
    QSettings               *settings;

    void sendAutomaticNotification() const;

    //constructor's helpers
    void initializeRegisterPersonTab();
    void initializeSearchFilesTab();
    void initializeSettingsTab();
    void initializeSearchPersonTab();


    //helper methods
    Person *    createPersonfromInputForm();
    int         registerNewPerson(Person *person, bool showConfirmation = true) const;
    void        clearNewPersonUI();
    int         ImportDatabaseHelper(int &row, const QString& filepath) const;
    bool        prepareSmtpForSending(Smtp *smtp) const;
    int         getMonthNumber(const QString month) const;

};

#endif // MAINWINDOW_H
