#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

QString SearchDialog::getNome() const
{
    return ui->Nome_search_line->text().toUpper().trimmed();
}

QString SearchDialog::getCognome() const
{
    return ui->Cognome_search_line->text().toUpper().trimmed();
}

QString SearchDialog::getComune() const
{
    return ui->Comune_search_line->text().toUpper().trimmed();
}

QString SearchDialog::getCap() const
{
    return ui->Cap_search_line->text().toUpper().trimmed();
}

QString SearchDialog::getProvincia() const
{
    return ui->Provincia_search_line->text().toUpper().trimmed();
}

QString SearchDialog::getProfessione() const
{
    return ui->Professione_search_line->text().toUpper().trimmed();
}

QString SearchDialog::getBrevetto()
{
    return ui->Brevetto_search_box->currentText();
}

bool SearchDialog::getAllLicenses() const
{
    return ui->all_licenses_radio->isChecked();
}

bool SearchDialog::getIsValid() const
{
    return ui->is_valid_radio->isChecked();
}

bool SearchDialog::getIsExpired() const
{
    return ui->is_expired_radio->isChecked();
}

bool SearchDialog::getShowAll() const
{
    return ui->show_all_radio->isChecked();
}

void SearchDialog::on_Visualizza_button_clicked()
{
    emit ShowPeople_SIGNAL(false);
}

void SearchDialog::on_Visualizza_in_scadenza_button_clicked()
{
    emit ShowPeopleAboutToExpire_SIGNAL(true);
}

void SearchDialog::on_Cancella_button_2_clicked()
{
    clearSearchPersonForm();
}

void SearchDialog::clearSearchPersonForm()
{
    ui->Nome_search_line->clear();
    ui->Cognome_search_line->clear();
    ui->Comune_search_line->clear();
    ui->Cap_search_line->clear();
    ui->Provincia_search_line->clear();
    ui->Professione_search_line->clear();
}

void SearchDialog::on_Chiudi_button_clicked()
{
    close();
}
