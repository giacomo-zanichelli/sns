#include "confirmationdialog.h"
#include "ui_confirmationdialog.h"

ConfirmationDialog::ConfirmationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmationDialog)
{
    ui->setupUi(this);
}

ConfirmationDialog::~ConfirmationDialog()
{
    delete ui;
}

void ConfirmationDialog::success()
{
    ui->confirmation_label->setText("Nuovo Corsista inserito!");
}

void ConfirmationDialog::failed()
{
    ui->confirmation_label->setText("Inserimento corsista fallito!");
}

void ConfirmationDialog::updated()
{
    ui->confirmation_label->setText("Dati corsista aggiornati!");
}

void ConfirmationDialog::peopleSuccessfullyImported(const int row)
{
    QString str = QVariant(row).toString() + " corsisti importati con successo!";
    ui->confirmation_label->setText(str);
}


void ConfirmationDialog::on_OK_button_clicked()
{
    close();
}

void ConfirmationDialog::emailParamsSaved()
{
    ui->confirmation_label->setText("Parametri salvati con successo!");
}

void ConfirmationDialog::emailParamsNotSaved()
{
    ui->confirmation_label->setText("Salvataggio parametri fallito!");
}

void ConfirmationDialog::excelPathSaved()
{
    ui->confirmation_label->setText("Percorso salvato!");
}

void ConfirmationDialog::excelPathNotSaved()
{
    ui->confirmation_label->setText("Non hai selezionato un file .xlsx. Seleziona un altro file!");
}

void ConfirmationDialog::databasePathSaved()
{
    ui->confirmation_label->setText("Percorso salvato!");
}

void ConfirmationDialog::databaseNotReady()
{
    ui->confirmation_label->setText("Non sono riuscito  connettermi al database. Controlla il percorso!");
}

void ConfirmationDialog::emailNotReady()
{
    ui->confirmation_label->setText("Email non pronta. Controlla i parametri di connessione Email!");
}

void ConfirmationDialog::excelNotReady()
{
    ui->confirmation_label->setText("File Excel non trovato. Controlla il percorso!");
}

void ConfirmationDialog::mailSent()
{
    ui->confirmation_label->setText("Email di notifica spedita con successo!");
}

void ConfirmationDialog::emailInvalid()
{
    ui->confirmation_label->setText("Il soggetto o il corpo della Email non sono validi. Email non spedita!");
}

void ConfirmationDialog::emailContentInvalid()
{
    ui->confirmation_label->setText("Il soggetto o il corpo delle Email di notifica sono vuoti!");
}

void ConfirmationDialog::automaticNotificationsSent(const int sent)
{
    QString str = QVariant(sent).toString() + " Email di notifica spedite con successo!";
    ui->confirmation_label->setText(str);
}


