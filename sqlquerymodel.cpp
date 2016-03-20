/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtSql>
#include "sqlquerymodel.h"

SqlQueryModel::SqlQueryModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

Qt::ItemFlags SqlQueryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() > 0 && index.column() < 11)
        flags |= Qt::ItemIsEditable;
    return flags;
}

QVariant SqlQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if (value.isValid() && role == Qt::DisplayRole) {
        //is_valid
        if (index.column() == 12) {
            if(value.toBool()) {
                return QVariant("VALIDO");
            } else {
                return QVariant("SCADUTO");
            }
        }

        //date
        if (index.column() == 13) {
            if(value.toInt() != 0) {
                return QVariant(QDate::fromJulianDay(value.toInt()).toString("d/M/yyyy"));
            } else {
                return QVariant("");
            }
        }

        //last_notification
        if (index.column() == 14) {
            if(value.toInt() != -1) {
                return QVariant(QDate::fromJulianDay(value.toInt()));
            } else {
                return QVariant("MAI");
            }
        }
    }
    return value;
}

bool SqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() < 1 || index.column() > 10) {
        return false;
    }
    
    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);

    int id = data(primaryKeyIndex, role).toInt();

    bool ok;
    //COGNOME
    if (index.column() == 1) {
        ok = setLastName(id, value.toString().trimmed().toUpper());
    }
    
    //NOME
    if (index.column() == 2) {
        ok = setFirstName(id, value.toString().trimmed().toUpper());
    }
    
    //INDIRIZZO
    if (index.column() == 3) {
        ok = setStreet(id, value.toString().trimmed().toUpper());
    }
    
    //CAP
    if (index.column() == 4) {
        ok = setZip(id, value.toString().trimmed().toUpper());
    }
    
    //COMUNE
    if (index.column() == 5) {
        ok = setMunicipality(id, value.toString().trimmed().toUpper());
    }
    
    //PROVINCIA
    if (index.column() == 6) {
        ok = setProvince(id, value.toString().trimmed().toUpper());
    }
    
    //CODICE FISCALE
    if (index.column() == 7) {
        ok = setPersonalCode(id, value.toString().trimmed().toUpper());
    }
    
    //PROFESSIONE
    if (index.column() == 8) {
        ok = setJob(id, value.toString().trimmed().toUpper());
    }
    
    //EMAIL
    if (index.column() == 9) {
        ok = setEmail(id, value.toString().trimmed().toLower());
    }
    
    //TELEFONO
    if (index.column() == 10) {
        ok = setPhone(id, value.toString().trimmed());
    }
    return ok;
}

bool SqlQueryModel::setLastName(int personId, const QString &lastName)
{
    Validator v;

    if(v.LastNameValidator(lastName)) {
        QSqlQuery query;
        query.prepare("UPDATE people SET last_name = :last_name WHERE id = :id");
        query.bindValue(":last_name", QVariant(lastName));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit lastNameChanged(personId, lastName);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool SqlQueryModel::setFirstName(int personId, const QString &firstName)
{
    Validator v;

    if(v.FirstNameValidator(firstName)) {
        QSqlQuery query;
        query.prepare("UPDATE people SET first_name = :first_name WHERE id = :id");
        query.bindValue(":first_name", QVariant(firstName));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit firstNameChanged(personId, firstName);
            return true;
        } else {
            return false;
        }    }
    return false;
}

bool SqlQueryModel::setStreet(int personId, const QString &street)
{
    Validator v;

    if(v.ViaValidator(street)) {
        QSqlQuery query;
        query.prepare("UPDATE address SET street_name = :street WHERE person = :id");
        query.bindValue(":street", QVariant(street));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit streetChanged(personId, street);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool SqlQueryModel::setZip(int personId, const QString &zip)
{
    Validator v;

    if(v.CapValidator(zip)) {
        QSqlQuery query;
        query.prepare("UPDATE address SET zip = :zip WHERE person = :id");
        query.bindValue(":zip", QVariant(zip));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit zipChanged(personId, zip);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool SqlQueryModel::setMunicipality(int personId, const QString &municipality)
{
    Validator v;

    if(v.ComuneValidator(municipality)) {
        QSqlQuery query;
        query.prepare("UPDATE address SET municipality = :municipality WHERE person = :id");
        query.bindValue(":municipality", QVariant(municipality));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit municipalityChanged(personId, municipality);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool SqlQueryModel::setProvince(int personId, const QString &province)
{
    Validator v;

    if(v.ProvinciaValidator(province)) {
        QSqlQuery query;
        query.prepare("UPDATE address SET province = :province WHERE person = :id");
        query.bindValue(":province", QVariant(province));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit provinceChanged(personId, province);
            return true;
        } else {
            return false;
        }
    }
    return false;
}
bool SqlQueryModel::setPersonalCode(int personId, const QString &personal_code)
{
    Validator v;

    if(v.CodiceFiscaleValidator(personal_code)) {
        QSqlQuery query;
        query.prepare("UPDATE people SET personal_code = :personal_code WHERE id = :id");
        query.bindValue(":personal_code", QVariant(personal_code));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit personalCodeChanged(personId, personal_code);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool SqlQueryModel::setJob(int personId, const QString &job)
{
    Validator v;

    if(v.ProfessioneValidator(job)) {
        QSqlQuery query;
        query.prepare("UPDATE people SET job = :job WHERE id = :id");
        query.bindValue(":job", QVariant(job));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit jobChanged(personId, job);
            return true;
        } else {
            return false;
        }
    }
    return false;
}
bool SqlQueryModel::setEmail(int personId, const QString &email)
{
    Validator v;

    if(v.EmailValidator(email)) {
        QSqlQuery query;
        query.prepare("UPDATE people SET email = :email WHERE id = :id");
        query.bindValue(":email", QVariant(email));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit emailChanged(personId, email);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool SqlQueryModel::setPhone(int personId, const QString &phone)
{
    Validator v;

    if(v.TelefonoValidator(phone)) {
        QSqlQuery query;
        query.prepare("UPDATE people SET phone = :phone WHERE id = :id");
        query.bindValue(":phone", QVariant(phone));
        query.bindValue(":id", QVariant(personId));
        if(query.exec()) {
            emit phoneChanged(personId, phone);
            return true;
        } else {
            return false;
        }
    }
    return false;
}


