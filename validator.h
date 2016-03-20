#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <QString>
#include <QRegExp>
#include <QValidator>
#include <QDate>
#include <QDebug>

class Validator
{
public:
    Validator();

    bool registerNewPersonInputValidator(const QString& firstname,
                                                        const QString& lastname,
                                                        const QString& street,
                                                        const QString& municipality,
                                                        const QString& zip,
                                                        const QString& province,
                                                        const QString& personal_code,
                                                        const QString& job,
                                                        const QString& license,
                                                        const QDate&   date_of_issue,
                                                        const QString& email,
                                                        const QString& phone    );

    bool FirstNameValidator(const QString& Nome, QString &&firstname = "");
    bool LastNameValidator(const QString& Cognome, QString &&lastname = "");
    bool NomeValidator(const QString& Nome, const QString& Cognome, QString&& firstname = "", QString&& lastname = "");
    bool ViaValidator(const QString& Via, QString&& street = "");
    bool ComuneValidator(const QString& Comune, QString&& municipality = "");
    bool CapValidator(const QString& Cap, QString &&zip = 0);
    bool ProvinciaValidator(const QString& Provincia, QString&& province = "");
    bool ProfessioneValidator(const QString& Professione, QString&& job = "");
    bool CodiceFiscaleValidator(const QString& CodiceFiscale, QString&& personal_code = "");
    bool TelefonoValidator(const QString& Telefono, QString&& phone = "");
    bool EmailValidator(const QString& Email, QString&& email = "");
    bool AnnoValidator(const QDate& Anno, int&& year = 0);
    bool ValiditaValidator(const QString& is_valid, bool &validity);


    bool EmailTelefonoValidator(const QString& Email, const QString& Telefono, QString&& email = "", QString&& phone = "");



private:
};

#endif // VALIDATOR_H
