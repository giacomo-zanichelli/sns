#include "validator.h"

Validator::Validator()
{

}

bool Validator::FirstNameValidator(const QString& Nome, QString &&firstname)
{
    QRegExp rx("^[A-Za-z ]+$");
    if(rx.indexIn(Nome) != -1) {
        firstname = Nome;
        firstname.trimmed();
        return true;
    } else {
        return false;
    }
}

bool Validator::LastNameValidator(const QString& Cognome, QString &&lastname)
{
    QRegExp rx("^[A-Za-z ]+$");
    if(rx.indexIn(Cognome) != -1) {
        lastname = Cognome;
        lastname.trimmed();
        return true;
    } else {
        return false;
    }
}

bool Validator::NomeValidator(const QString& Nome, const QString& Cognome, QString &&firstname, QString &&lastname)
{
    QRegExp rx("^[A-Za-z ]+$");
    if(rx.indexIn(Nome) != -1 && rx.indexIn(Cognome) != -1) {
        firstname = Nome;
        lastname = Cognome;
        firstname.trimmed();
        lastname.trimmed();
        return true;
    } else {
        return false;
    }
}

bool Validator::ViaValidator(const QString &Via, QString&& street)
{
    QRegExp rx("^[A-Za-z0-9/.°\\ ]*$");
    if(rx.indexIn(Via) != -1) {
        street = Via;
        street.trimmed();
        return true;
    } else {
        return false;
    }
}

bool Validator::ComuneValidator(const QString &Comune, QString&& municipality)
{
    QRegExp rx("^[A-Za-z ]*$");

    if(rx.indexIn(Comune) != -1) {
        municipality = Comune;
        municipality.trimmed();
        return true;
    } else {
        return false;
    }
}

bool Validator::CapValidator(const QString& Cap, QString&& zip)
{
    QRegExp rx("^[0-9]{5}$|^$");
    if(rx.indexIn(Cap) != -1) {
        zip = Cap.trimmed();
        return true;
    } else {
        return false;
    }
}

bool Validator::ProvinciaValidator(const QString& Provincia, QString &&province)
{
    QRegExp rx("^[A-Za-z ]{2}$|^$");
    if(rx.indexIn(Provincia) != -1) {
        province = Provincia;
        return true;
    } else {
        return false;
    }
}

bool Validator::ProfessioneValidator(const QString& Professione, QString&& job)
{
    QRegExp rx("^[A-Za-z ]*$");
    if(rx.indexIn(Professione) != -1) {
        job = Professione;
        job.trimmed();
        return true;
    } else {
        return false;
    }
}

bool Validator::CodiceFiscaleValidator(const QString& CodiceFiscale, QString&& personal_code)
{
    QRegExp rx("^[a-zA-Z]{6}[0-9]{2}[abcdehlmprstABCDEHLMPRST]{1}[0-9]{2}([a-zA-Z]{1}[0-9]{3})[a-zA-Z]{1}$");
    if(rx.indexIn(CodiceFiscale) != -1) {
        personal_code = CodiceFiscale;
        return true;
    } else {
        return false;
    }
}

bool Validator::TelefonoValidator(const QString& Telefono, QString&& phone)
{
    QRegExp rx("^[0-9]+$");
    if(rx.indexIn(Telefono) != -1) {
        phone = Telefono;
        return true;
    } else {
        return false;
    }
}

bool Validator::EmailValidator(const QString& Email, QString&& email)
{
     QRegExp rx("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$");
     if(rx.indexIn(Email) != -1) {
         email = Email;
         return true;
     } else {
         return false;
     }
}

bool Validator::EmailTelefonoValidator(const QString& Email, const QString& Telefono, QString&& email, QString&& phone)
{
    if(EmailValidator(Email) || TelefonoValidator(Telefono)) {
        return true;
    } else {
        return false;
    }
}

bool Validator::AnnoValidator(const QDate& Anno, int&& year)
{
//    QRegExp rx("^[0-9]{4}$");
//    if(rx.indexIn(Anno.year()) != -1) {
        year = Anno.year();
        return true;
//    } else {
//        return false;
//    }
}

bool Validator::ValiditaValidator(const QString& is_valid, bool& validity) {
    if(is_valid.trimmed().toUpper() == "VALIDO") {
        validity = true;
        return true;
    } else if (is_valid.trimmed().toUpper() == "SCADUTO") {
        validity = false;
        return true;
    } else {
        return false;
    }
}



bool Validator::registerNewPersonInputValidator(    const QString& firstname,
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
                                                    const QString& phone        )
{
    if(!NomeValidator(firstname, lastname)) {
        return false;
    }

    if(!ViaValidator(street)) {
        return false;
    }

    if(!ComuneValidator(municipality)) {
        return false;
    }

    if(!CapValidator(zip)) {
        return false;
    }

    if(!ProvinciaValidator(province)) {
        return false;
    }

    if(!CodiceFiscaleValidator(personal_code)) {
        return false;
    }

    if(!ProfessioneValidator(job)) {
        return false;
    }

    if(!(EmailValidator(email) || TelefonoValidator(phone))) {
        return false;
    }

    return true;
}
