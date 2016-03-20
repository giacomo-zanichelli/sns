#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

    QString getNome() const;
    QString getCognome() const;
    QString getComune() const;
    QString getCap() const;
    QString getProvincia() const;
    QString getProfessione() const;
    QString getBrevetto();
    bool    getAllLicenses() const;
    bool    getIsValid() const;
    bool    getIsExpired() const;
    bool    getShowAll() const;

signals:
    void ShowPeople_SIGNAL(bool expired);
    void ShowPeopleAboutToExpire_SIGNAL(bool expired);

private slots:
    void on_Visualizza_button_clicked();

    void on_Visualizza_in_scadenza_button_clicked();

    void on_Cancella_button_2_clicked();

    void on_Chiudi_button_clicked();

private:
    Ui::SearchDialog *ui;

    void clearSearchPersonForm();
};

#endif // SEARCHDIALOG_H
