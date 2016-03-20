#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model                                   = new ModelManager();
    confirmation_dialog                     = new ConfirmationDialog();
    search_file_directory_model             = new QFileSystemModel(this);
    search_file_file_model                  = new QFileSystemModel(this);
    search_dialog                           = new SearchDialog(this);
    smtp                                    = new Smtp();
    settings                                = new QSettings("GiacomoZanichelli", "SNS", this);
    database_path_dialog                    = new DbPathDialog();
    excel_path_dialog                       = new ExcelPathDialog();

    initializeRegisterPersonTab();
    initializeSettingsTab();
    initializeSearchFilesTab();
    initializeSearchPersonTab();

    sendAutomaticNotification();

}

//constructor's helpers
void MainWindow::initializeRegisterPersonTab()
{
    ui->test_date_form->setDate(QDate::currentDate());
}

//constructor's helpers
void MainWindow::initializeSearchPersonTab()
{
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(QDate::currentDate().addYears(-3));

    connect(search_dialog, SIGNAL(ShowPeople_SIGNAL(bool)), this, SLOT(setSearchResultsMode(bool)));
    connect(search_dialog, SIGNAL(ShowPeopleAboutToExpire_SIGNAL(bool)), this, SLOT(setSearchResultsMode(bool)));

    setSearchResultsMode(false);

}

//constructor's helpers
void MainWindow::initializeSearchFilesTab()
{

    QModelIndex index_to_show = search_file_directory_model->index(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory));

    search_file_directory_model->setRootPath("/");
    search_file_directory_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

    ui->dir_View->setModel(search_file_directory_model);
    ui->dir_View_2->setModel(search_file_directory_model);

    ui->dir_View->setRootIndex(index_to_show);
    ui->dir_View_2->setRootIndex(index_to_show);

    ui->dir_View->resizeColumnToContents(0);
    ui->dir_View->setColumnHidden(1, true);
    ui->dir_View->setColumnHidden(2, true);
    ui->dir_View->setColumnHidden(3, true);
    ui->dir_View->setHeaderHidden(true);

    ui->dir_View_2->resizeColumnToContents(0);
    ui->dir_View_2->setColumnHidden(1, true);
    ui->dir_View_2->setColumnHidden(2, true);
    ui->dir_View_2->setColumnHidden(3, true);
    ui->dir_View_2->setHeaderHidden(true);

    search_file_file_model->setRootPath("/");
    search_file_file_model->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    ui->file_View->setModel(search_file_file_model);
    ui->file_View_2->setModel(search_file_file_model);

    ui->file_View->setRootIndex(index_to_show);
    ui->file_View_2->setRootIndex(index_to_show);
}

//constructor's helpers
void MainWindow::initializeSettingsTab()
{
    if(settings->value("database_path") != "") {
        ui->database_path_line->setText(settings->value("database_path").toString());
        model->setPath(settings->value("database_path").toString());
        if(!model->openConnection()) {
            confirmation_dialog->databaseNotReady();
            confirmation_dialog->setModal(true);
            confirmation_dialog->exec();
        }
    } else {
        confirmation_dialog->databaseNotReady();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }

    if(settings->value("server").toString() != "" && settings->value("port").toInt() != 0 && settings->value("email").toString() != "" && settings->value("password").toString() != "") {
        QString host        = settings->value("server").toString();
        int port            = settings->value("port").toInt();
        QString user        = settings->value("email").toString();
        QString password    = settings->value("password").toString();

        ui->server_line->setText(host);
        ui->port_line->setText(QVariant(port).toString());
        ui->email_line->setText(user);
        ui->password_line->setText(password);

        smtp->setParams(user, password, host, port);
    } else {
        confirmation_dialog->emailNotReady();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }

    if(settings->value("excel_path") != "") {
        ui->Excel_path_line->setText(settings->value("excel_path").toString());
        model->setExcelFile(new QXlsx::Document(settings->value("excel_path").toString()));
    } else {
        confirmation_dialog->excelNotReady();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }


    bool email_invalid = false;
    if(settings->value("email_subject").toString() != "") {
        ui->subject_line->setText(settings->value("email_subject").toString());

    } else {
        email_invalid = true;
    }

    if(settings->value("email_body").toString() != "") {
        ui->body_text->setPlainText(settings->value("email_body").toString());
    } else {
        email_invalid = true;
    }

    if(email_invalid) {
        confirmation_dialog->emailContentInvalid();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }

    if(settings->value("notification_month").toString() != "") {
        ui->notification_month_combobox->setCurrentText(settings->value("notification_month").toString());
    }

    if(settings->value("max_email").toString() != "") {
        ui->max_auto_email_spinbox->setValue(settings->value("max_email").toInt());
    }

    connect(database_path_dialog, SIGNAL(SetDbPath_SIGNAL(QString)), this, SLOT(saveDbPath(QString)));
    connect(excel_path_dialog, SIGNAL(SetExcelPath_SIGNAL(QString)), this, SLOT(saveExcelPath(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete confirmation_dialog;
    delete model;
    delete settings;
    delete database_path_dialog;
    delete excel_path_dialog;
    delete smtp;
}

//register new person tab
void MainWindow::on_Inserisci_button_clicked()
{
    registerNewPerson(createPersonfromInputForm());
    clearNewPersonUI();
}

//register new person tab
void MainWindow::on_Cancella_button_clicked()
{
    clearNewPersonUI();
}

//search person tab
void MainWindow::setSearchResultsMode(bool expiring)
{
    //change all the base object from ui to search_dialog->ui and implement all the getters from search_dialog for the ui items
    if(model->setSearch_results_table_model(      search_dialog->getNome(),
                                                  search_dialog->getCognome(),
                                                  search_dialog->getComune(),
                                                  search_dialog->getCap(),
                                                  search_dialog->getProvincia(),
                                                  search_dialog->getProfessione(),
                                                  search_dialog->getBrevetto(),
                                                  search_dialog->getIsValid(),
                                                  search_dialog->getIsExpired(),
                                                  search_dialog->getAllLicenses(),
                                                  expiring)) {
        ui->search_results_table->setModel(model->getSearch_results_table_model());
        model->getSearch_results_table_model()->setHeaderData(0, Qt::Horizontal, tr("ID"));
        model->getSearch_results_table_model()->setHeaderData(1, Qt::Horizontal, tr("COGNOME"));
        model->getSearch_results_table_model()->setHeaderData(2, Qt::Horizontal, tr("NOME"));
        model->getSearch_results_table_model()->setHeaderData(3, Qt::Horizontal, tr("INDIRIZZO"));
        model->getSearch_results_table_model()->setHeaderData(4, Qt::Horizontal, tr("CAP"));
        model->getSearch_results_table_model()->setHeaderData(5, Qt::Horizontal, tr("COMUNE"));
        model->getSearch_results_table_model()->setHeaderData(6, Qt::Horizontal, tr("PROVINCIA"));
        model->getSearch_results_table_model()->setHeaderData(7, Qt::Horizontal, tr("CODICE FISCALE"));
        model->getSearch_results_table_model()->setHeaderData(8, Qt::Horizontal, tr("PROFESSIONE"));
        model->getSearch_results_table_model()->setHeaderData(9, Qt::Horizontal, tr("EMAIL"));
        model->getSearch_results_table_model()->setHeaderData(10, Qt::Horizontal, tr("TELEFONO"));
        model->getSearch_results_table_model()->setHeaderData(11, Qt::Horizontal, tr("BREVETTO"));
        model->getSearch_results_table_model()->setHeaderData(12, Qt::Horizontal, tr("VALIDITA'"));
        model->getSearch_results_table_model()->setHeaderData(13, Qt::Horizontal, tr("DATA DI CONSEGUIMENTO"));
        model->getSearch_results_table_model()->setHeaderData(14, Qt::Horizontal, tr("ULTIMA NOTIFICA"));


        search_dialog->close();
        search_person_results_proxy_model = new QSortFilterProxyModel(this);
        search_person_results_proxy_model->sort(1);
        search_person_results_proxy_model->setSourceModel(model->getSearch_results_table_model());
        ui->search_results_table->setModel(search_person_results_proxy_model);

        ui->search_results_table->resizeColumnsToContents();
        ui->search_results_table->horizontalHeader()->setStretchLastSection(true);
    } else {
        search_dialog->close();
        model->getSearch_results_table_model()->clear();
    }
}


//search person tab
void MainWindow::on_Elimina_corsista_button_clicked()
{
    int row;
    QModelIndex id_person_index;
    int id_person;

    QItemSelectionModel * selection_model = ui->search_results_table->selectionModel();
    if(selection_model != nullptr) {
        QModelIndexList rows_index_list = selection_model->selectedIndexes();
        foreach (QModelIndex row_index, rows_index_list) {
            row = row_index.row();
            id_person_index = row_index.sibling(row, 0);
            id_person = id_person_index.data().toInt();
            model->deletePerson(id_person);

            model->deletePersonExcel(model->getPersonRows(id_person));
        }
    }
    model->runLastShowQuery();
}

//search person tab
void MainWindow::on_Elimina_brevetto_button_clicked()
{
    int row;
    QModelIndex id_person_index;
    QModelIndex id_license_index;
    int id_person;
    int id_license;
    QString license;

    QItemSelectionModel * selection_model = ui->search_results_table->selectionModel();
    if(selection_model != nullptr) {
        QModelIndexList rows_index_list = selection_model->selectedIndexes();
        foreach (QModelIndex row_index, rows_index_list) {
            row = row_index.row();
            id_person_index = row_index.sibling(row, 0);
            id_person = id_person_index.data().toInt();
            id_license_index = row_index.sibling(row, 11);
            license = id_license_index.data().toString();
            id_license = model->getIdLicense(license);
            model->deleteLicense(id_person, id_license);

            model->deleteLicenseExcel(model->getPersonWithLicenseRow(id_person, license));
        }
    }
    model->runLastShowQuery();
}

//search person tab
void MainWindow::on_change_is_valid_button_clicked()
{
    int row;
    QModelIndex id_person_index;
    QModelIndex id_license_index;
    QString is_valid_string;
    int id_person;
    int id_license;
    QString license;
    bool is_valid;

    Validator v;
    QItemSelectionModel * selection_model = ui->search_results_table->selectionModel();
    if(selection_model != nullptr) {
        QModelIndexList rows_index_list = selection_model->selectedIndexes();
        foreach (QModelIndex row_index, rows_index_list) {
            row = row_index.row();
            id_person_index = row_index.sibling(row, 0);
            id_person = id_person_index.data().toInt();
            id_license_index = row_index.sibling(row, 11);
            license = id_license_index.data().toString();
            id_license = model->getIdLicense(license);
            is_valid_string = row_index.sibling(row, 12).data().toString();

            if(v.ValiditaValidator(is_valid_string, is_valid)) {
                model->changeIsValid(id_person, id_license, is_valid);
                model->updateValidityExcel(model->getPersonWithLicenseRow(id_person, license), is_valid_string);
            }
        }
    }
    model->runLastShowQuery();
}

//search person tab
void MainWindow::on_add_license_button_clicked()
{
    int row = ui->search_results_table->currentIndex().row();
    QModelIndex id_person_index = ui->search_results_table->currentIndex().sibling(row, 0);
    int id_person = id_person_index.data().toInt();
    QString license = ui->Brevetto_search_box_2->currentText();
    QDate date_of_issue = ui->dateEdit->date();
    int id_license = model->getIdLicense(license);
    Person *person = model->buildPerson(id_person);

    if(person != nullptr) {
        person->setLicense(license, date_of_issue);

        if(model->hasLicense(id_person, id_license) == -2) {
            model->addLicenseToPerson(id_person, id_license, ui->dateEdit->date());
            model->addPersonExcel(id_person, person);
            model->addAddressExcel(model->getPersonRows(id_person), person->getAddress());
        }
        model->runLastShowQuery();
    }
}

//search person tab
void MainWindow::on_notified_before_button_clicked()
{
    QItemSelectionModel * selection_model = ui->search_results_table->selectionModel();
    if(selection_model != nullptr) {
        selection_model->clear();
        ui->search_results_table->setSelectionMode(QAbstractItemView::MultiSelection);

        for(int i = 0; i < model->getSearch_results_table_model()->rowCount(); ++i) {
            QDate date = model->getSearch_results_table_model()->data(model->getSearch_results_table_model()->index(i, 14), Qt::DisplayRole).toDate();
            if(date.isValid()) {
                if(date.toJulianDay() < ui->dateEdit_2->date().toJulianDay()) {
                    ui->search_results_table->selectRow(i);
                }
            }
        }
    }
}

//search person tab
void MainWindow::on_never_notified_button_clicked()
{
    QItemSelectionModel * selection_model = ui->search_results_table->selectionModel();
    if(selection_model != nullptr) {
        selection_model->clear();
        ui->search_results_table->setSelectionMode(QAbstractItemView::MultiSelection);

        for(int i = 0; i < model->getSearch_results_table_model()->rowCount(); ++i) {
            if(model->getSearch_results_table_model()->data(model->getSearch_results_table_model()->index(i, 14), Qt::DisplayRole).toString() == "MAI") {
                ui->search_results_table->selectRow(i);
            }
        }
    }
}

//search person tab
void MainWindow::on_deselect_all_button_clicked()
{
    QItemSelectionModel * selection_model = ui->search_results_table->selectionModel();
    if(selection_model != nullptr) {
        selection_model->clear();
    }
}

//search person tab
void MainWindow::on_select_all_button_clicked()
{
    ui->search_results_table->selectAll();
}

//search person tab
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index != 1) {
        search_dialog->close();
    }
    on_refresh_button_clicked();
}

//search person tab
void MainWindow::on_show_search_dialog_button_clicked()
{
    search_dialog->show();
}

//search person tab
void MainWindow::on_refresh_button_clicked()
{
    setSearchResultsMode(false);
}


//search person tab
void MainWindow::on_Spedisci_avviso_button_clicked()
{
    QString subject = settings->value("email_subject").toString();
    QString body = settings->value("email_body").toString();
    int sent = 0;

    if(subject != "" && body != "") {
        int row;
        QString license;
        QString email;
        QDate date_of_issue;
        int id_license;
        int id_person;
        QString first_name;
        QString last_name;

        QModelIndexList rows_index_list = ui->search_results_table->selectionModel()->selectedIndexes();
        QVector<QString> recipients;

        foreach (QModelIndex row_index, rows_index_list) {
            row = row_index.row();
            license = row_index.sibling(row, 11).data().toString();
            id_license = model->getIdLicense(license);
            id_person = row_index.sibling(row, 0).data().toInt();
            email = row_index.sibling(row, 9).data().toString();
            date_of_issue = row_index.sibling(row, 13).data().toDate();
            first_name = row_index.sibling(row, 2).data().toString().toLower();
            first_name[0] = first_name[0].toUpper();
            last_name = row_index.sibling(row, 1).data().toString().toLower();
            last_name[0] = last_name[0].toUpper();
            license = license.toLower();
            license[0] =  license[0].toUpper();
            for(int i = 0; i < license.size(); ++i) {
                if(license[i] == ' ') {
                    license[i + 1] =  license[i + 1].toUpper();
                }
            }

            QMap<QString, QString> replacements;
            replacements.insert("nome_cognome", first_name + " " + last_name);
            replacements.insert("brevetto", license);

            model->processBodyEmail(subject, replacements);
            model->processBodyEmail(body, replacements);

            recipients.append(email);
            if(smtp->sendMessage(recipients, subject, body)) {
                model->updateLastNotificationDate(id_person, id_license, QDate::currentDate());
                model->updateLastNotificationExcel(model->getPersonWithLicenseRow(id_person, license.toUpper()), QDate::currentDate());
                ++sent;
            }

        }

        confirmation_dialog->automaticNotificationsSent(sent);
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();

        model->runLastShowQuery();
    } else {
        confirmation_dialog->emailInvalid();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }
}

//Excel import tab
void MainWindow::on_dir_View_clicked(const QModelIndex &index)
{
    QString path = search_file_directory_model->fileInfo(index).absoluteFilePath();
    ui->file_View->setRootIndex(search_file_file_model->setRootPath(path));
}

//Excel import tab: TO DO
void MainWindow::on_file_View_doubleClicked(const QModelIndex &index)
{
   //import from file
}

//database import tab
void MainWindow::on_dir_View_2_clicked(const QModelIndex &index)
{
    QString path = search_file_directory_model->fileInfo(index).absoluteFilePath();
    ui->file_View_2->setRootIndex(search_file_file_model->setRootPath(path));

}

//database import tab: Excel database update missing
void MainWindow::on_file_View_2_doubleClicked(const QModelIndex &index)
{
    QString filepath = search_file_directory_model->fileInfo(index).absoluteFilePath();

    QXlsx::Document xlsx_database(filepath);
    int number_of_row = xlsx_database.dimension().lastRow();

    int row = 2;
    int successfully_imported = 0;
    while (row <= number_of_row) {
        successfully_imported += ImportDatabaseHelper(row, filepath);
    }
    confirmation_dialog->peopleSuccessfullyImported(successfully_imported);
    confirmation_dialog->setModal(true);
    confirmation_dialog->exec();
}

//settings tab
void MainWindow::on_database_path_button_clicked()
{
    //show dialog
    database_path_dialog->setModal(true);
    database_path_dialog->exec();
}


void MainWindow::saveDbPath(QString db_path)
{
    QStringList chunks = db_path.split("/");
    QString ext = chunks.last().split(".").last();
    if(ext == "db") {
        model->setPath(db_path);
        if(model->openConnection()) {
            settings->setValue("database_path", QVariant(db_path));
            ui->database_path_line->setText(db_path);
            confirmation_dialog->databasePathSaved();
            confirmation_dialog->setModal(true);
            confirmation_dialog->exec();
        } else {
            confirmation_dialog->databaseNotReady();
            confirmation_dialog->setModal(true);
            confirmation_dialog->exec();
        }
    } else {
        confirmation_dialog->databaseNotReady();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }
}

//settings tab
void MainWindow::on_Excel_path_button_clicked()
{
    //show dialog
    excel_path_dialog->setModal(true);
    excel_path_dialog->exec();
}

void MainWindow::saveExcelPath(QString excel_path)
{
    QStringList chunks = excel_path.split("/");
    QString ext = chunks.last().split(".").last();
    if(ext == "xlsx") {
        settings->setValue("excel_path", QVariant(excel_path));
        model->setExcelFile(new QXlsx::Document(excel_path));

        ui->Excel_path_line->setText(excel_path);
        confirmation_dialog->excelPathSaved();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    } else {
        confirmation_dialog->excelPathNotSaved();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }
}

//settings tab
void MainWindow::on_Conferma_button_clicked()
{
    QString server      = ui->server_line->text();
    int port            = ui->port_line->text().toInt();
    QString email       = ui->email_line->text();
    QString password    = ui->password_line->text();

    if(server != "" && port != 0 && validator.EmailValidator(email) && password != "") {
        settings->setValue("email", QVariant(email));
        settings->setValue("password", QVariant(password));
        settings->setValue("server", QVariant(server));
        settings->setValue("port", QVariant(port));

        smtp->setParams(email, password, server, port);

        confirmation_dialog->emailParamsSaved();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    } else {
        confirmation_dialog->emailParamsNotSaved();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }

}

void MainWindow::on_subject_line_editingFinished()
{
    settings->setValue("email_subject", QVariant(ui->subject_line->text()));
}

void MainWindow::on_body_text_textChanged()
{
    settings->setValue("email_body", QVariant(ui->body_text->toPlainText()));
}

void MainWindow::on_notification_month_combobox_currentIndexChanged(const QString &month)
{
    settings->setValue("notification_month", QVariant(month));
}

void MainWindow::on_max_auto_email_spinbox_valueChanged(int max_email)
{
    settings->setValue("max_email", QVariant(max_email));
}


/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

//helper methods
Person * MainWindow::createPersonfromInputForm()
{
    qDebug() << ui->Email_line->text();
    if(validator.registerNewPersonInputValidator(       ui->Nome_line->text(),
                                                        ui->Cognome_line->text(),
                                                        ui->Via_line->text(),
                                                        ui->Comune_line->text(),
                                                        ui->Cap_line->text(),
                                                        ui->Provincia_line->text(),
                                                        ui->Codice_Fiscale_line->text(),
                                                        ui->Professione_line->text(),
                                                        ui->Brevetto_box->currentText(),
                                                        ui->test_date_form->date(),
                                                        ui->Email_line->text(),
                                                        ui->Telefono_line->text()       )) {

         Person *tmp = new Person(  ui->Nome_line->text().trimmed().toUpper(),
                                    ui->Cognome_line->text().trimmed().toUpper(),
                                    new Address(    ui->Via_line->text().trimmed().toUpper(),
                                                    ui->Comune_line->text().trimmed().toUpper(),
                                                    ui->Provincia_line->text().trimmed().toUpper(),
                                                    ui->Cap_line->text().trimmed().toUpper(),
                                                    "IT"),
                                    ui->Codice_Fiscale_line->text().trimmed().toUpper(),
                                    ui->Professione_line->text().trimmed().toUpper(),
                                    ui->Brevetto_box->currentText(),
                                    ui->test_date_form->date(),
                                    ui->Email_line->text().trimmed().toLower(),
                                    ui->Telefono_line->text()   );
         return tmp;
    } else {
        return nullptr;
    }
}

int MainWindow::registerNewPerson(Person *person, bool showConfirmation) const
{
    /*
     * check if the person is registered using personal code.
     *      If he/she is, check all the empty field and try to fill them.
     *      Check the licenses and add the license if it's missing.
     * if it's not, register the person.
     * */

    if(person) {
        int id_person = model->getPersonId(person);
        if(id_person == -1) {
            if(model->addPerson(person)) {

                id_person = model->getPersonId(person);
                if(id_person != -1) {
                    model->addPersonExcel(id_person, person);
                    model->addAddressExcel(model->getPersonRows(id_person), person->getAddress());
                }

                if(showConfirmation) {
                    confirmation_dialog->success();
                    confirmation_dialog->setModal(true);
                    confirmation_dialog->exec();
                }
                return 0;
            } else {
                qDebug() << "-1";
                if(showConfirmation) {
                    confirmation_dialog->failed();
                    confirmation_dialog->setModal(true);
                    confirmation_dialog->exec();
                }
                return -1;
            }
        } else {
            model->fillPersonField(id_person, person);
            int id_license = model->getIdLicense(person->getLicense());
            if(id_license > 0) {
                model->addLicenseToPerson(id_person, id_license, person->getDateOfIssue());
                if(!model->hasLicense(id_person, id_license)) {
                    model->addPersonExcel(id_person, person);
                }
            }
            model->addAddressExcel(model->getPersonRows(id_person), person->getAddress());
            if(showConfirmation) {
                qDebug() << "1";
                confirmation_dialog->updated();
                confirmation_dialog->setModal(true);
                confirmation_dialog->exec();
            }
            return 1;
        }
    }
    if(showConfirmation) {
        qDebug() << "-2";
        confirmation_dialog->failed();
        confirmation_dialog->setModal(true);
        confirmation_dialog->exec();
    }
    return -2;
}

void MainWindow::clearNewPersonUI()
{
    ui->Nome_line->clear();
    ui->Cognome_line->clear();
    ui->Via_line->clear();
    ui->Comune_line->clear();
    ui->Provincia_line->clear();
    ui->Cap_line->clear();
    ui->Codice_Fiscale_line->clear();
    ui->Professione_line->clear();
    ui->Email_line->clear();
    ui->Telefono_line->clear();
}

int MainWindow::ImportDatabaseHelper(int& row, const QString& filepath) const
{
    int result = 0;

    QString     first_name;
    QString     last_name;
    QString     street;
    QString     municipality;
    QString     province;
    QString     zip;
    QString     personal_code;
    QString     job;
    QStringList licenses;
    QString     email;
    QString     phone;
    //QDate       date;

    QStringList person_fields;
    QStringList chunks;

    QXlsx::Document xlsx_database(filepath);

    int number_of_row = xlsx_database.dimension().lastRow();
    int number_of_column = xlsx_database.dimension().lastColumn();

    int count = 0;
    for(int i = row; (i <= number_of_row) && (count < 100); ++i) {
        for(int column = 1; column <= number_of_column ; ++column) {
            person_fields.append(xlsx_database.read(i, column).toString());
        }

        bool success = true;

        chunks = person_fields.at(0).split(" ");

        //lastname
        bool long_last_name = 0;
        if(chunks.at(0).trimmed().size() <= 2) {
            last_name = chunks.at(0).trimmed().toUpper() + " " + chunks.at(1).trimmed().toUpper();
            long_last_name = 1;
        } else {
            last_name = chunks.at(0).trimmed().toUpper();
        }

        //firstname
        for(int i = 1 + long_last_name; i < chunks.size(); i++) {
            first_name += chunks.at(i).trimmed().toUpper() + " ";
        }
        first_name = first_name.trimmed();

        if(person_fields.at(1) != "NULL") {
            street          = person_fields.at(1).trimmed().toUpper();
        } else {
            success = false;
        }

        if(person_fields.at(3) != "NULL") {
            municipality    = person_fields.at(3).trimmed().toUpper();
        } else {
            success = false;
        }

        if(person_fields.at(4) != "NULL") {
            province        = person_fields.at(4).trimmed().toUpper();
        } else {
            success = false;
        }

        if(person_fields.at(2) != "NULL") {
            QString temp = person_fields.at(2);
            zip             = temp.remove(" ").trimmed();
        } else {
            success = false;
        }

        if(person_fields.at(5) != "NULL") {
            personal_code   = person_fields.at(5).trimmed();
        } else {
            success = false;
        }

        if(person_fields.at(6) != "NULL") {
            job   = person_fields.at(6).trimmed().toUpper();
        } else {
            success = false;
        }

        if(person_fields.at(10) != "NULL") {
            email           = person_fields.at(10);
        } else {
            email           = "";
        }

        if(person_fields.at(7) != "NULL") {
            QString temp = person_fields.at(7);
            phone = temp.trimmed().remove(" ");
        } else {
            phone = "";
        }

//        if(person_fields.at(9) == "") {
//            date = QDate(0,0,0);
//        } else {
//            QString temp = person_fields.at(9);
//            date.setDate(temp.trimmed().remove(" ").toInt(), 12, 31);
//        }


        QStringList licenses_codes = person_fields.at(8).split(" ");
        foreach(QString code, licenses_codes) {
            if(code.remove(" ") == "BP") {
                licenses.append("Bagnino di Salvataggio per Piscine");
            }

            if(code.remove(" ") == "BM") {
                licenses.append("Bagnino di Salvataggio per Piscine e Acque Libere");
            }

            if(code.remove(" ") == "IN") {
                licenses.append("Istruttore di Nuoto");
            }

            if(code.remove(" ") == "IA") {
                licenses.append("Istruttore di Acquafitness e Hydrobike");
            }
            ////////////////
//            if(code.remove(" ") == "BP") {
//                licenses.append("Istruttore di Nuoto per Disabili");
//            }
            ///////////////
            if(code.remove(" ") == "DF") {
                licenses.append("BLSD");
            }

            if(code.remove(" ") == "IAMS") {
                licenses.append("Istruttore di Arti Marinaresche");
            }

            if(code.remove(" ") == "SM") {
                licenses.append("Scienze Motorie");
            }
        }

        Person* tmp = NULL;
        if(success) {
            tmp = new Person(       first_name,
                                    last_name,
                                    new Address(            street,
                                                            municipality,
                                                            province,
                                                            zip         ),
                                    personal_code,
                                    job,
                                    email,
                                    phone   );
            int result_code = registerNewPerson(tmp, false);

            if(result_code == 0) {
                //person added
                ++result;
                int id_person = model->getPersonId(tmp);
                int id_license;
                foreach(QString license, licenses) {
                    id_license = model->getIdLicense(license);
                    model->addLicenseToPerson(id_person, id_license);
                }
            } else if (result_code == 1) {
                //person already exist, license checked
                //++result;
                int id_person = model->getPersonId(tmp);
                int id_license;
                foreach(QString license, licenses) {
                    id_license = model->getIdLicense(license);
                    model->addLicenseToPerson(id_person, id_license);
                }
            } else if (result_code == -1) {
                //add person failed
            } else if (result_code == -2) {
                //person pointer not valid
            }
            delete tmp;
        }

        ++count;
        person_fields.clear();
        first_name = "";
        last_name = "";
        licenses.clear();
    }
    row += count;
    return result;
}

bool MainWindow::prepareSmtpForSending(Smtp *smtp) const
{
    if(settings->value("server").toString() != "" && settings->value("port").toInt() != 0 && settings->value("email").toString() != "" && settings->value("password").toString() != "") {
        smtp->setParams(        settings->value("email").toString(),
                                settings->value("password").toString(),
                                settings->value("server").toString(),
                                settings->value("port").toInt()      );
        return true;
    } else {
        return false;
    }
}

int MainWindow::getMonthNumber(const QString month) const
{
    if(month == "Gennaio") {
        return 1;
    }
    if(month == "Febbraio") {
        return 2;
    }
    if(month == "Marzo") {
        return 3;
    }
    if(month == "Aprile") {
        return 4;
    }
    if(month == "Maggio") {
        return 5;
    }
    if(month == "Giugno") {
        return 6;
    }
    if(month == "Luglio") {
        return 7;
    }
    if(month == "Agosto") {
        return 8;
    }
    if(month == "Settembre") {
        return 9;
    }
    if(month == "Ottobre") {
        return 10;
    }
    if(month == "Novembre") {
        return 11;
    }
    if(month == "Dicembre") {
        return 12;
    }
    return -1;
}


void MainWindow::sendAutomaticNotification() const
{
    int max_email = settings->value("max_email").toInt();
    if(max_email > 0) {
        QVector<int> id_people = model->getAllPersonId();
        int sent = 0;
        QString month = settings->value("notification_month").toString();

        int month_number = getMonthNumber(month);

        if(month_number > 0 && month_number < 13) {
            foreach(int id_person, id_people) {
                QVector<int> id_licenses = model->getAllLicensesId(id_person);
                foreach(int id_license, id_licenses) {
                    if(model->isSuitableForAutomaticNotification(id_person, id_license, month_number)) {

                        QString subject = settings->value("email_subject").toString();
                        QString body = settings->value("email_body").toString();

                        if(subject != "" && body != "") {
                            Person *person = model->buildPerson(id_person);
                            QString email = person->getEmail();
                            QString first_name = person->getFirst_name().toLower();
                            first_name[0] = first_name[0].toUpper();
                            QString last_name = person->getLast_name().toLower();
                            last_name[0] = last_name[0].toUpper();
                            QString license = model->getLicenseNameFromId(id_license);
                            QVector<QString> recipients;

                            QMap<QString, QString> replacements;
                            replacements.insert("nome_cognome", first_name + " " + last_name);
                            replacements.insert("brevetto", license);

                            model->processBodyEmail(subject, replacements);
                            model->processBodyEmail(body, replacements);

                            recipients.append(email);
                            if(smtp->sendMessage(recipients, subject, body)) {
                                model->updateLastNotificationDate(id_person, id_license, QDate::currentDate());
                                model->updateLastNotificationExcel(model->getPersonWithLicenseRow(id_person, model->getLicenseNameFromId(id_license)), QDate::currentDate());
                                ++sent;
                            }

                            if(sent == max_email) {
                                return;
                            }
                        }
                    }
                }
            }
        }
        if(sent != 0) {
            confirmation_dialog->automaticNotificationsSent(sent);
            confirmation_dialog->setModal(true);
            confirmation_dialog->exec();
        }
    }
}

