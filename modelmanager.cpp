#include "modelmanager.h"
#include <QVariant>
#include <QRegExp>
#include <QDebug>

#include "xlsxdocument.h"

ModelManager::ModelManager(QObject *parent) : QObject(parent)
{
    db_link                     = new QSqlDatabase();
    search_results_table_model  = new SqlQueryModel();
    Excel_file                  = nullptr;
}

ModelManager::~ModelManager() {
    if (db_link->isOpen()) {
        db_link->close();
    }
    delete search_results_table_model;
}

bool ModelManager::createNewDatabase()
{
    bool success_1;
    bool success_2;
    bool success_3;
    bool success_4;
    bool success_5;
    bool success_6;
    bool success_7;
    bool success_8;
    bool success_9;
    bool success_10;
    bool success_11;
    bool success_12;
    bool success_13;

    *db_link = QSqlDatabase::addDatabase("QSQLITE");
    path_ += "/new_database.db";
    db_link->setDatabaseName(path_);
    db_link->open();
    //Table: license
    QSqlQuery query;
    QString query_string = "CREATE TABLE license (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, license_name UNIQUE NOT NULL, validity INTEGER)";
    query.prepare(query_string);
    success_1 = query.exec();

    //Table: address
    query_string = "CREATE TABLE address (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, person INTEGER REFERENCES people (id) ON DELETE CASCADE ON UPDATE CASCADE NOT NULL, street_name VARCHAR (100), municipality VARCHAR (50), zip VARCHAR (5), province VARCHAR (2), state VARCHAR (2))";
    query.prepare(query_string);
    success_2 = query.exec();

    //Table: people
    query_string = "CREATE TABLE people (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, first_name VARCHAR (50) NOT NULL, last_name VARCHAR (50) NOT NULL, personal_code VARCHAR (16) UNIQUE NOT NULL, job VARCHAR (50), email VARCHAR (100), phone VARCHAR)";
    query.prepare(query_string);
    success_3 = query.exec();

    //Table: people_license
    query_string = "CREATE TABLE people_license (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, id_person REFERENCES people (id) ON DELETE CASCADE ON UPDATE CASCADE NOT NULL, id_license INTEGER REFERENCES license (id) ON DELETE CASCADE ON UPDATE CASCADE NOT NULL, date_of_issue INTEGER, is_valid BOOLEAN DEFAULT false, last_notification INTEGER DEFAULT (- 1))";
    query.prepare(query_string);
    success_4 = query.exec();

    //license population
    query_string = "INSERT INTO license (id, license_name, validity) VALUES (1, 'Bagnino di Salvataggio per Piscine', 3)";
    query.prepare(query_string);
    success_5 = query.exec();

    query_string = "INSERT INTO license (id, license_name, validity) VALUES (2, 'Bagnino di Salvataggio per Piscine e Acque Libere', 3)";
    query.prepare(query_string);
    success_6 = query.exec();

    query_string = "INSERT INTO license (id, license_name, validity) VALUES (3, 'Istruttore di Nuoto', 3)";
    query.prepare(query_string);
    success_7 = query.exec();

    query_string = "INSERT INTO license (id, license_name, validity) VALUES (4, 'Istruttore di Acquafitness e Hydrobike', 3)";
    query.prepare(query_string);
    success_8 = query.exec();

    query_string = "INSERT INTO license (id, license_name, validity) VALUES (5, 'Istruttore di Nuoto per Disabili', 3)";
    query.prepare(query_string);
    success_9 = query.exec();

    query_string = "INSERT INTO license (id, license_name, validity) VALUES (6, 'BLSD', 2)";
    query.prepare(query_string);
    success_10 = query.exec();

    query_string = "INSERT INTO license (id, license_name, validity) VALUES (7, 'Istruttore di Arti Marinaresche', 3)";
    query.prepare(query_string);
    success_11 = query.exec();

    query_string = "INSERT INTO license (id, license_name, validity) VALUES (8, 'Docente Formatore per Istruttori di Nuoto', 3)";
    query.prepare(query_string);
    success_12 = query.exec();

    query_string = "INSERT INTO license (id, license_name, validity) VALUES (9, 'Scienze Motorie', 0)";
    query.prepare(query_string);
    success_13 = query.exec();

    if(success_1 && success_2 && success_3 && success_4 && success_5 && success_6 && success_7 && success_8 && success_9 && success_10 && success_11 && success_12 && success_13) {
        return true;
    } else {
        return false;
    }
}

bool ModelManager::openConnection()
{
    if(!isOpen()) {
        *db_link = QSqlDatabase::addDatabase("QSQLITE");
        db_link->setDatabaseName(path_);

        if (!db_link->open()) {
           qDebug() << "Error: connection with database fail";
           return false;
        } else {
            QSqlQuery query;
            QString query_string = "SELECT * FROM license";
            query.prepare(query_string);
            if(query.exec()) {
                return true;
               qDebug() << "Database: connection ok";
            } else {
               qDebug() << "Error: database not suitable";
               return false;
            }
        }
    } else {
        qDebug() << "Database: already open";
        db_link->close();
        if(openConnection()) {
            return true;
        } else {
            return false;
        }
    }
}

bool ModelManager::isOpen() const {
    if(db_link != 0) {
        return db_link->isOpen();
    } else {
        return false;
    }
}

bool ModelManager::addPerson(const Person *person) {
    if(person && person->isSufficient()) {
        QSqlQuery query;
        QString query_string = "INSERT INTO people (first_name, last_name, personal_code, job, email, phone) "
                                "VALUES (:first_name, :last_name, :personal_code, :job, :email, :phone)";
        query.prepare(query_string);
        query.bindValue(":first_name",       person->getFirst_name());
        query.bindValue(":last_name",        person->getLast_name());
        query.bindValue(":personal_code",    person->getPersonal_code());
        query.bindValue(":job",              person->getJob());
        query.bindValue(":email",            person->getEmail());
        query.bindValue(":phone",            person->getPhone());

        if(query.exec()) {
            int id_person = query.lastInsertId().toInt();
            addAddress(id_person, person->getAddress());
            int id_license = getIdLicense(person->getLicense());
            if(id_license > 0) {
                addLicenseToPerson(id_person, id_license, person->getDateOfIssue());
            }

            return true;
        }
    }
    return false;
}

bool ModelManager::addAddress(const int id_person, Address* address_to_register)
{
    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO address (person, street_name, municipality, zip, province, state)"
                     "VALUES (:person, :street_name, :municipality, :zip, :province, :state)");

    queryAdd.bindValue(":person",           id_person);
    queryAdd.bindValue(":street_name",      address_to_register->getStreet());
    queryAdd.bindValue(":municipality",     address_to_register->getMunicipality());
    queryAdd.bindValue(":zip",              address_to_register->getZip());
    queryAdd.bindValue(":province",         address_to_register->getProvince());
    queryAdd.bindValue(":state",            address_to_register->getState());


    if(queryAdd.exec()) {
        return true;
    } else {
        return false;
    }
}

bool ModelManager::addLicenseToPerson(const int id_person, const int id_license, const QDate& date_of_issue) const
{
    if(id_person > 0 && id_license > 0) {
        QSqlQuery query_select;
        QString query_select_string = "SELECT id FROM people_license WHERE id_person = :id_person AND id_license = :id_license";
        query_select.prepare(query_select_string);
        query_select.bindValue(":id_person", id_person);
        query_select.bindValue(":id_license", id_license);
        if(query_select.exec()) {
            if(query_select.isActive()) {
                if(!query_select.first()) {
                    QSqlQuery query;
                    QString query_string =     "INSERT INTO people_license (id_person, id_license, date_of_issue, is_valid, last_notification)"
                                               "VALUES (:id_person, :id_license, :date, 'true', '-1')";
                    query.prepare(query_string);
                    query.bindValue(":id_person", id_person);
                    query.bindValue(":id_license", id_license);
                    if(date_of_issue != QDate(0, 0, 0)) {
                        query.bindValue(":date", date_of_issue.toJulianDay());
                    } else {
                        query.bindValue(":date", 0);
                    }
                    if(query.exec()) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void ModelManager::fillPersonField(const int id_person, const Person *person) const
{
    if(id_person > 0 && person) {
        QSqlQuery query;
        QSqlQuery update_query;
        QString query_string;
        QString update_query_string;
        query_string =     "SELECT   people.job, "
                                    "people.email, "
                                    "people.phone, "
                                    "address.street_name, "
                                    "address.municipality, "
                                    "address.zip, "
                                    "address.province, "
                                    "address.state "
                           "FROM people "
                           "INNER JOIN address ON   people.id = :id_person AND "
                                                    "people.id = address.person";
        query.prepare(query_string);
        query.bindValue(":id_person", id_person);
        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    if(query.value(0) == "" && person->getJob() != "") {
                        update_query_string = "UPDATE people SET job = :job WHERE id = :id_person";
                        update_query.prepare(update_query_string);
                        update_query.bindValue(":job", person->getJob());
                        update_query.bindValue(":id_person", id_person);
                        update_query.exec();
                    }
                    if(query.value(1) == "" && person->getEmail() != "") {
                        update_query_string = "UPDATE people SET email = :email WHERE id = :id_person";
                        update_query.prepare(update_query_string);
                        update_query.bindValue(":email", person->getEmail());
                        update_query.bindValue(":id_person", id_person);
                        update_query.exec();
                    }
                    if(query.value(2) == "" && person->getPhone() != "") {
                        update_query_string = "UPDATE people SET phone = :phone WHERE id = :id_person";
                        update_query.prepare(update_query_string);
                        update_query.bindValue(":phone", person->getPhone());
                        update_query.bindValue(":id_person", id_person);
                        update_query.exec();
                    }
                    if(query.value(3) == "" && person->getAddress()->getStreet() != "") {
                        update_query_string = "UPDATE address SET street_name = :street WHERE person = :id_person";
                        update_query.prepare(update_query_string);
                        update_query.bindValue(":street", person->getAddress()->getStreet());
                        update_query.bindValue(":id_person", id_person);
                        update_query.exec();
                    }
                    if(query.value(4) == "" && person->getAddress()->getMunicipality() != "") {
                        update_query_string = "UPDATE address SET municipality = :municipality WHERE person = :id_person";
                        update_query.prepare(update_query_string);
                        update_query.bindValue(":municipality", person->getAddress()->getMunicipality());
                        update_query.bindValue(":id_person", id_person);
                        update_query.exec();
                    }
                    if(query.value(5) == "" && person->getAddress()->getZip() != "") {
                        update_query_string = "UPDATE address SET zip = :zip WHERE person = :id_person";
                        update_query.prepare(update_query_string);
                        update_query.bindValue(":zip", person->getAddress()->getZip());
                        update_query.bindValue(":id_person", id_person);
                        update_query.exec();
                    }
                    if(query.value(6) == "" && person->getAddress()->getProvince() != "") {
                        update_query_string = "UPDATE address SET province = :province WHERE person = :id_person";
                        update_query.prepare(update_query_string);
                        update_query.bindValue(":province", person->getAddress()->getProvince());
                        update_query.bindValue(":id_person", id_person);
                        update_query.exec();
                    }
                    if(query.value(7) == "" && person->getAddress()->getState() != "") {
                        update_query_string = "UPDATE address SET state = :state WHERE person = :id_person";
                        update_query.prepare(update_query_string);
                        update_query.bindValue(":state", person->getAddress()->getState());
                        update_query.bindValue(":id_person", id_person);
                        update_query.exec();
                    }
                }
            }
        }
    }
}

bool ModelManager::updateLastNotificationDate(const int id_person, const int id_license, const QDate& date) const
{
    if(id_person > 0 && id_license > 0 && date.isValid()) {
        QSqlQuery query;
        QString query_string = "UPDATE people_license SET last_notification = :last_notification WHERE id_person = :id_person AND id_license = :id_license";
        query.prepare(query_string);
        query.bindValue(":id_person", QVariant(id_person));
        query.bindValue(":id_license", QVariant(id_license));
        query.bindValue(":last_notification", QVariant(date.toJulianDay()));
        return query.exec();
    }
    return false;
}


int ModelManager::hasLicense(const int id_person, const int id_license) const
{
    if(id_person > 0 && id_license > 0) {
        QSqlQuery query;
        QString query_string;
        query_string =     "SELECT      people_license.id "
                           "FROM        people_license "
                           "WHERE       people_license.id_person = :id_person AND "
                           "            people_license.id_license = :id_license";
        query.prepare(query_string);
        query.bindValue(":id_person", id_person);
        query.bindValue(":id_license", id_license);
        if(query.exec()) {
            int size = 0;
            if(query.isActive()) {
                if(query.first()) {
                    ++size;
                    while(query.next()) {
                        ++size;
                    }
                    if(size == 1) {
                        return 1;
                    } else {
                        return -1;
                    }
                } else {
                    return -2;
                }
            }
        }
    }
    return -3;
}

int ModelManager::getPersonId(const Person *person) const
{
    if(person) {
        QSqlQuery query;
        QString query_string = "SELECT id FROM people WHERE people.personal_code = :code";

        query.prepare(query_string);
        query.bindValue(":code", person->getPersonal_code());
        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    return query.value(0).toInt();
                }
            }
        }
    }
    return -1;
}

QVector<int> ModelManager::getAllPersonId() const
{
    QVector<int> result;
    QSqlQuery query;
    QString query_string = "SELECT id FROM people";
    query.prepare(query_string);
    if(query.exec()) {
        if(query.isActive()) {
            if(query.first()) {
                result.append(query.value(0).toInt());
                while(query.next()) {
                     result.append(query.value(0).toInt());
                }
            }
        }
    }
    return result;
}

QVector<int> ModelManager::getAllLicensesId(const int id_person) const
{
    QVector<int> id_licenses;
    if(id_person > 0) {
        QSqlQuery query;
        QString query_string = "SELECT id_license FROM people_license WHERE id_person = :id_person";
        query.prepare(query_string);
        query.bindValue(":id_person", id_person);

        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    id_licenses.append(query.value(0).toInt());
                    while(query.next()) {
                        id_licenses.append(query.value(0).toInt());
                    }
                }
            }
        }
    }
    return id_licenses;
}


int ModelManager::getIdLicense(const QString& license) const
{
    if(license != "") {
        QSqlQuery query;

        QString query_string = "SELECT id FROM license WHERE license_name = :name";
        query.prepare(query_string);
        query.bindValue(":name", license);

        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    return query.value(0).toInt();
                }
            }
        }
    }
    return -1;
}

QString ModelManager::getLicenseNameFromId(const int id_license) const
{
    if(id_license > 0) {
        QSqlQuery query;
        QString query_string = "SELECT license_name FROM license WHERE id = :id_license";
        query.prepare(query_string);
        query.bindValue(":id_license", id_license);
        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    return query.value(0).toString();
                }
            }
        }
    }
    return "";
}


int ModelManager::getLicenseValidity(const int id_license) const
{
    if(id_license > 0) {
        QSqlQuery query;
        QString query_string = "SELECT license.validity FROM license WHERE license.id = :id_license";
        query.prepare(query_string);
        query.bindValue(":id_license", id_license);
        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    return query.value(0).toInt();
                }
            }
        }
    }
    return -1;
}

QDate ModelManager::getLicenseDateOfIssue(const int id_person, const int id_license) const
{
    if(id_person > 0 && id_license > 0) {
        qDebug() << id_person;
        qDebug() << id_license;
        QSqlQuery query;
        QString query_string = "SELECT              people_license.date_of_issue "
                               "FROM                people_license "
                               "WHERE               people_license.id_person = :id_person AND "
                                                   "people_license.id_license = :id_license";
        query.prepare(query_string);
        query.bindValue(":id_person", id_person);
        query.bindValue(":id_license", id_license);
        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    if(query.value(0).toInt() != 0) {
                        qDebug() << QDate::fromJulianDay(query.value(0).toInt());
                        return QDate::fromJulianDay(query.value(0).toInt());
                    } else {
                        return QDate();
                    }
                }
            }
        }
    }
    return QDate();
}

QDate ModelManager::getLastNotification(const int id_person, const int id_license) const
{
    if(id_person > 0 && id_license > 0) {
        QSqlQuery query;
        QString query_string = "SELECT last_notification FROM people_license WHERE id_person = :id_person AND id_license = :id_license";
        query.prepare(query_string);
        query.bindValue(":id_person", id_person);
        query.bindValue(":id_license", id_license);
        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    return query.value(0).toDate();
                }
            }
        }
    }
    return QDate();
}


Person * ModelManager::buildPerson(const int id_person) const
{
    Person *person = nullptr;
    if(id_person > 0) {
        QSqlQuery query;
        QString query_string = "SELECT  "
                                                    "people.last_name, "
                                                    "people.first_name, "
                                                    "address.street_name, "
                                                    "address.zip, "
                                                    "address.municipality, "
                                                    "address.province, "
                                                    "address.state, "
                                                    "people.personal_code, "
                                                    "people.job, "
                                                    "people.email, "
                                                    "people.phone "
                                "FROM                people "
                                "INNER JOIN          address ON people.id = address.person";

        query_string += " AND people.id = :id_person";

        query.prepare(query_string);
        query.bindValue(":id_person", QVariant(id_person));

        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    person = new Person(        query.value(1).toString(),
                                                query.value(0).toString(),
                                                new Address (       query.value(2).toString(),
                                                                    query.value(4).toString(),
                                                                    query.value(5).toString(),
                                                                    query.value(3).toString(),
                                                                    query.value(6).toString()       ),
                                                query.value(7).toString(),
                                                query.value(8).toString(),
                                                query.value(9).toString(),
                                                query.value(10).toString()      );
                }
            }
        }
    }
    return person;
}


bool ModelManager::setSearch_results_table_model(const QString& first_name,
                                              const QString& last_name,
                                              const QString& municipality,
                                              const QString& zip,
                                              const QString& province,
                                              const QString& job,
                                              const QString& license,
                                              bool           is_valid,
                                              bool           is_expired,
                                              bool           all_licenses,
                                              bool           expiring   )
{
    if(!all_licenses && !expiring) {
        return helperOneLicense        (first_name, last_name, municipality, zip, province, job, license, is_valid, is_expired);
    }

    if(all_licenses && !expiring) {
        return helperAllLicenses       (first_name, last_name, municipality, zip, province, job, is_valid, is_expired);
    }

    if(!all_licenses && expiring) {
        return helperOneLicenseYear    (first_name, last_name, municipality, zip, province, job, license);
    }

    if(all_licenses && expiring) {
        return helperAllLicensesYear   (first_name, last_name, municipality, zip, province, job);
    }
    return false;
}

bool ModelManager::helperOneLicense(             const QString& first_name,
                                              const QString& last_name,
                                              const QString& municipality,
                                              const QString& zip,
                                              const QString& province,
                                              const QString& job,
                                              const QString& license,
                                              bool           is_valid,
                                              bool           is_expired)
{

    QSqlQuery query;
    QString query_string = "SELECT  "
                                                "people.id, "
                                                "people.last_name, "
                                                "people.first_name, "
                                                "address.street_name, "
                                                "address.zip, "
                                                "address.municipality, "
                                                "address.province, "
                                                "people.personal_code, "
                                                "people.job, "
                                                "people.email, "
                                                "people.phone, "
                                                "license.license_name, "
                                                "people_license.is_valid, "
                                                "people_license.date_of_issue,"
                                                "people_license.last_notification "
                            "FROM                people "
                            "INNER JOIN          address             ON people.id = address.person "
                            "INNER JOIN          people_license      ON people_license.id_person = people.id "
                            "INNER JOIN          license             ON license.id = people_license.id_license ";

    query_string += " AND people.last_name LIKE :last_name";
    query_string += " AND people.first_name LIKE :first_name";
    query_string += " AND address.municipality LIKE :municipality";
    query_string += " AND address.zip LIKE :zip";
    query_string += " AND address.province LIKE :province";
    query_string += " AND people.job LIKE :job";
    query_string += " AND license.license_name = :license ";
    if(is_valid) {
        query_string += " AND people_license.is_valid = 'true'";
    } else if(is_expired) {
        query_string += " AND people_license.is_valid = 'false'";
    }

    query.prepare(query_string);

    query.bindValue(":last_name",       QVariant('%' + last_name + '%'));
    query.bindValue(":first_name",      QVariant('%' + first_name + '%'));
    query.bindValue(":municipality",    QVariant('%' + municipality + '%'));
    query.bindValue(":zip",             QVariant('%' + zip + '%'));
    query.bindValue(":province",        QVariant('%' + province + '%'));
    query.bindValue(":job",             QVariant('%' + job + '%'));
    query.bindValue(":license",         QVariant(license));

    search_results_table_model = new SqlQueryModel();
    connect(search_results_table_model, SIGNAL(firstNameChanged(int, QString)), this, SLOT(changeFirstNameExcel(int, QString)));
    connect(search_results_table_model, SIGNAL(lastNameChanged(int,QString)), this, SLOT(changeLastNameExcel(int, QString)));
    connect(search_results_table_model, SIGNAL(streetChanged(int,QString)), this, SLOT(changeStreetExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(zipChanged(int,QString)), this, SLOT(changeZipExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(municipalityChanged(int,QString)), this, SLOT(changeMunicipalityExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(provinceChanged(int,QString)), this, SLOT(changeProvinceExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(personalCodeChanged(int,QString)), this, SLOT(changePersonalCodeExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(jobChanged(int,QString)), this, SLOT(changeJobExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(emailChanged(int,QString)), this, SLOT(changeEmailExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(phoneChanged(int,QString)), this, SLOT(changePhoneExcel(int,QString)));
    if(query.exec()) {
        search_results_table_model->setQuery(query);
        last_show_query = query;
        return true;
    } else {
        search_results_table_model->setQuery(query);
        qDebug() << "query failed: " << query.lastError();
        return false;
    }
}

bool ModelManager::helperAllLicenses(const QString& first_name,
                                              const QString& last_name,
                                              const QString& municipality,
                                              const QString& zip,
                                              const QString& province,
                                              const QString& job,
                                              bool           is_valid,
                                              bool           is_expired)
{
    QSqlQuery query;
    QString query_string = "SELECT  "
                                                "people.id, "
                                                "people.last_name, "
                                                "people.first_name, "
                                                "address.street_name, "
                                                "address.zip, "
                                                "address.municipality, "
                                                "address.province, "
                                                "people.personal_code, "
                                                "people.job, "
                                                "people.email, "
                                                "people.phone, "
                                                "license.license_name, "
                                                "people_license.is_valid, "
                                                "people_license.date_of_issue,"
                                                "people_license.last_notification "
                            "FROM                people "
                            "INNER JOIN          address             ON people.id = address.person "
                            "INNER JOIN          people_license      ON people_license.id_person = people.id "
                            "INNER JOIN          license             ON license.id = people_license.id_license ";


    query_string += " AND people.last_name LIKE :last_name";
    query_string += " AND people.first_name LIKE :first_name";
    query_string += " AND address.municipality LIKE :municipality";
    query_string += " AND address.zip LIKE :zip";
    query_string += " AND address.province LIKE :province";
    query_string += " AND people.job LIKE :job" ;
    if(is_valid) {
        query_string += " AND people_license.is_valid = 'true'";
    } else if(is_expired) {
        query_string += " AND people_license.is_valid = 'false'";
    }

    query.prepare(query_string);

    query.bindValue(":last_name",       QVariant('%' + last_name + '%'));
    query.bindValue(":first_name",      QVariant('%' + first_name + '%'));
    query.bindValue(":municipality",    QVariant('%' + municipality + '%'));
    query.bindValue(":zip",             QVariant('%' + zip + '%'));
    query.bindValue(":province",        QVariant('%' + province + '%'));
    query.bindValue(":job",             QVariant('%' + job + '%'));

    search_results_table_model = new SqlQueryModel();
    connect(search_results_table_model, SIGNAL(firstNameChanged(int, QString)), this, SLOT(changeFirstNameExcel(int, QString)));
    connect(search_results_table_model, SIGNAL(lastNameChanged(int,QString)), this, SLOT(changeLastNameExcel(int, QString)));
    connect(search_results_table_model, SIGNAL(streetChanged(int,QString)), this, SLOT(changeStreetExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(zipChanged(int,QString)), this, SLOT(changeZipExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(municipalityChanged(int,QString)), this, SLOT(changeMunicipalityExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(provinceChanged(int,QString)), this, SLOT(changeProvinceExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(personalCodeChanged(int,QString)), this, SLOT(changePersonalCodeExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(jobChanged(int,QString)), this, SLOT(changeJobExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(emailChanged(int,QString)), this, SLOT(changeEmailExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(phoneChanged(int,QString)), this, SLOT(changePhoneExcel(int,QString)));
    if(query.exec()) {
        search_results_table_model->setQuery(query);
        last_show_query = query;
        return true;
    } else {
        search_results_table_model->setQuery(query);
        qDebug() << "query failed: " << query.lastError();
        return false;
    }
}

bool ModelManager::helperOneLicenseYear(const QString& first_name,
                                              const QString& last_name,
                                              const QString& municipality,
                                              const QString& zip,
                                              const QString& province,
                                              const QString& job,
                                              const QString& license)
{
    int license_validity = getLicenseValidity(getIdLicense(license));
    if(license_validity != -1) {
        QDate date_of_issue;
        date_of_issue = QDate::currentDate().addYears(-license_validity + 1);

        date_of_issue = QDate(date_of_issue.year(), 12, 31);

        QSqlQuery query;
        QString query_string;
        query_string =          "SELECT  "
                                                    "people.id, "
                                                    "people.last_name, "
                                                    "people.first_name, "
                                                    "address.street_name, "
                                                    "address.zip, "
                                                    "address.municipality, "
                                                    "address.province, "
                                                    "people.personal_code, "
                                                    "people.job, "
                                                    "people.email, "
                                                    "people.phone, "
                                                    "license.license_name, "
                                                    "people_license.is_valid, "
                                                    "people_license.date_of_issue,"
                                                    "people_license.last_notification "
                                "FROM                people "
                                "INNER JOIN          address             ON people.id = address.person "
                                "INNER JOIN          people_license      ON people_license.id_person = people.id "
                                "INNER JOIN          license             ON license.id = people_license.id_license ";

        query_string += " AND people.last_name LIKE :last_name";
        query_string += " AND people.first_name LIKE :first_name";
        query_string += " AND address.municipality LIKE :municipality";
        query_string += " AND address.zip LIKE :zip";
        query_string += " AND address.province LIKE :province";
        query_string += " AND people.job LIKE :job";
        query_string += " AND license.license_name = :license";
        query_string += " AND people_license.is_valid = 'true'";
        query_string += " AND people_license.date_of_issue <= :date_of_issue";


        query.prepare(query_string);

        query.bindValue(":last_name",       QVariant('%' + last_name + '%'));
        query.bindValue(":first_name",      QVariant('%' + first_name + '%'));
        query.bindValue(":municipality",    QVariant('%' + municipality + '%'));
        query.bindValue(":zip",             QVariant('%' + zip + '%'));
        query.bindValue(":province",        QVariant('%' + province + '%'));
        query.bindValue(":job",             QVariant('%' + job + '%'));
        query.bindValue(":license",         QVariant(license));
        query.bindValue(":date_of_issue",   QVariant(date_of_issue.toJulianDay()));

        search_results_table_model = new SqlQueryModel();
        connect(search_results_table_model, SIGNAL(firstNameChanged(int, QString)), this, SLOT(changeFirstNameExcel(int, QString)));
        connect(search_results_table_model, SIGNAL(lastNameChanged(int,QString)), this, SLOT(changeLastNameExcel(int, QString)));
        connect(search_results_table_model, SIGNAL(streetChanged(int,QString)), this, SLOT(changeStreetExcel(int,QString)));
        connect(search_results_table_model, SIGNAL(zipChanged(int,QString)), this, SLOT(changeZipExcel(int,QString)));
        connect(search_results_table_model, SIGNAL(municipalityChanged(int,QString)), this, SLOT(changeMunicipalityExcel(int,QString)));
        connect(search_results_table_model, SIGNAL(provinceChanged(int,QString)), this, SLOT(changeProvinceExcel(int,QString)));
        connect(search_results_table_model, SIGNAL(personalCodeChanged(int,QString)), this, SLOT(changePersonalCodeExcel(int,QString)));
        connect(search_results_table_model, SIGNAL(jobChanged(int,QString)), this, SLOT(changeJobExcel(int,QString)));
        connect(search_results_table_model, SIGNAL(emailChanged(int,QString)), this, SLOT(changeEmailExcel(int,QString)));
        connect(search_results_table_model, SIGNAL(phoneChanged(int,QString)), this, SLOT(changePhoneExcel(int,QString)));

        if(query.exec()) {
            search_results_table_model->setQuery(query);
            last_show_query = query;
            return true;
        } else {
            search_results_table_model->setQuery(query);
            qDebug() << "query failed: " << query.lastError();
            return false;
        }
    } else {
        return false;
    }
}

//missing the year restriction
bool ModelManager::helperAllLicensesYear(        const QString& first_name,
                                              const QString& last_name,
                                              const QString& municipality,
                                              const QString& zip,
                                              const QString& province,
                                              const QString& job)
{
    //gettting all the licenses validity
    QMap<QString, int> licenses_validity;
    QSqlQuery query;
    QString query_string = "SELECT * FROM license";
    query.prepare(query_string);
    if(query.exec()) {
        if(query.isActive()) {
            if(query.first()) {
                //qDebug() << query.value(1);
                licenses_validity.insert(query.value(1).toString(), query.value(2).toInt());
                while(query.next()) {
                    licenses_validity.insert(query.value(1).toString(), query.value(2).toInt());
                }
            }
        }
    }

    QMap<QString, QDate> dates_of_issue;
    QDate temp_date;
    QMapIterator<QString, int> iterator(licenses_validity);
    while (iterator.hasNext()) {
        iterator.next();
        if(iterator.value() != -1) {
            temp_date = QDate::currentDate().addYears(-iterator.value() + 1);
            temp_date = QDate(temp_date.year(), 12, 31);

            dates_of_issue.insert(iterator.key(), temp_date);
        }
    }

    qDebug() << licenses_validity.size();

    query_string =          "SELECT  "
                                                "people.id, "
                                                "people.last_name, "
                                                "people.first_name, "
                                                "address.street_name, "
                                                "address.zip, "
                                                "address.municipality, "
                                                "address.province, "
                                                "people.personal_code, "
                                                "people.job, "
                                                "people.email, "
                                                "people.phone, "
                                                "license.license_name, "
                                                "people_license.is_valid, "
                                                "people_license.date_of_issue,"
                                                "people_license.last_notification "
                            "FROM                people "
                            "INNER JOIN          address             ON people.id = address.person "
                            "INNER JOIN          people_license      ON people_license.id_person = people.id "
                            "INNER JOIN          license             ON license.id = people_license.id_license ";

    query_string += " AND people.last_name LIKE :last_name";
    query_string += " AND people.first_name LIKE :first_name";
    query_string += " AND address.municipality LIKE :municipality";
    query_string += " AND address.zip LIKE :zip";
    query_string += " AND address.province LIKE :province";
    query_string += " AND people.job LIKE :job";
    query_string += " AND people_license.is_valid = 'true'";

    query_string += " AND (";

    query_string += " license.license_name = :license1";
    query_string += " AND people_license.date_of_issue <= :date_of_issue1";

    query_string += " OR license.license_name = :license2";
    query_string += " AND people_license.date_of_issue <= :date_of_issue2";

    query_string += " OR license.license_name = :license3";
    query_string += " AND people_license.date_of_issue <= :date_of_issue3";

    query_string += " OR license.license_name = :license4";
    query_string += " AND people_license.date_of_issue <= :date_of_issue4";

    query_string += " OR license.license_name = :license5";
    query_string += " AND people_license.date_of_issue <= :date_of_issue5";

    query_string += " OR license.license_name = :license6";
    query_string += " AND people_license.date_of_issue <= :date_of_issue6";

    query_string += " OR license.license_name = :license7";
    query_string += " AND people_license.date_of_issue <= :date_of_issue7";

    query_string += " OR license.license_name = :license8";
    query_string += " AND people_license.date_of_issue <= :date_of_issue8";

    query_string += " OR license.license_name = :license9";
    query_string += " AND people_license.date_of_issue <= :date_of_issue9";

    query_string += " )";


    query.prepare(query_string);

    query.bindValue(":last_name",       QVariant('%' + last_name + '%'));
    query.bindValue(":first_name",      QVariant('%' + first_name + '%'));
    query.bindValue(":municipality",    QVariant('%' + municipality + '%'));
    query.bindValue(":zip",             QVariant('%' + zip + '%'));
    query.bindValue(":province",        QVariant('%' + province + '%'));
    query.bindValue(":job",             QVariant('%' + job + '%'));

    int count = 1;
    QString license_bind;
    QString date_bind;

    QMapIterator<QString, QDate> dates_of_issue_it(dates_of_issue);
    while (dates_of_issue_it.hasNext()) {
        dates_of_issue_it.next();
        license_bind = QString(":license%1").arg(count);
        date_bind = QString(":date_of_issue%1").arg(count);
        query.bindValue(license_bind,         QVariant(dates_of_issue_it.key()));
        query.bindValue(date_bind,            QVariant(dates_of_issue_it.value().toJulianDay()));
        ++count;
    }

    search_results_table_model = new SqlQueryModel();
    connect(search_results_table_model, SIGNAL(firstNameChanged(int, QString)), this, SLOT(changeFirstNameExcel(int, QString)));
    connect(search_results_table_model, SIGNAL(lastNameChanged(int,QString)), this, SLOT(changeLastNameExcel(int, QString)));
    connect(search_results_table_model, SIGNAL(streetChanged(int,QString)), this, SLOT(changeStreetExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(zipChanged(int,QString)), this, SLOT(changeZipExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(municipalityChanged(int,QString)), this, SLOT(changeMunicipalityExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(provinceChanged(int,QString)), this, SLOT(changeProvinceExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(personalCodeChanged(int,QString)), this, SLOT(changePersonalCodeExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(jobChanged(int,QString)), this, SLOT(changeJobExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(emailChanged(int,QString)), this, SLOT(changeEmailExcel(int,QString)));
    connect(search_results_table_model, SIGNAL(phoneChanged(int,QString)), this, SLOT(changePhoneExcel(int,QString)));

    if(query.exec()) {
        search_results_table_model->setQuery(query);
        last_show_query = query;
        return true;
    } else {
        search_results_table_model->setQuery(query);
        qDebug() << "query failed: " << query.lastError();
        return false;
    }
}


bool ModelManager::runLastShowQuery()
{
    return last_show_query.exec();
}

SqlQueryModel* ModelManager::getSearch_results_table_model()
{
    return search_results_table_model;
}

//WRONG!! people_license.validity field doesn't exist!
void ModelManager::getAllPerson(const QString &filepath)
{
    QXlsx::Document sns_new(filepath);

    QSqlQuery query;
    QString query_string =
            "SELECT                 people.id, "
                                   "people.last_name, "
                                   "people.first_name, "
                                   "address.street_name, "
                                   "address.zip, "
                                   "address.municipality, "
                                   "address.province, "
                                   "people.personal_code, "
                                   "people.job, "
                                   "people.email, "
                                   "license.license_name, "
                                   "people_license.validity, "
                                   "license.license_name "
            "FROM people "
            "INNER JOIN address ON people.id = address.person "
            "INNER JOIN people_license ON people_license.id_person = people.id "
            "INNER JOIN license ON   license.id = people_license.id_license ";

    query.prepare(query_string);
    if(query.exec()) {
        if(query.isActive()) {
            qDebug() << "FIRST_EXCEL";
            if(query.first()) {
                int row = 3;
                while(query.next()) {
                    for(int i = 1; i < 14; i++) {
                        sns_new.write(row, i , query.value(i));
                    }
                    ++row;
                }
            }
        }
    }
    sns_new.save();
}

bool ModelManager::deletePerson(const int id_person) const
{
    bool success = false;

    if(id_person > 0) {
        QSqlQuery query;
        QString query_string = "DELETE FROM people WHERE id = :id_person";
        query.prepare(query_string);
        query.bindValue(":id_person", QVariant(id_person));
        if(query.exec()) {
            QSqlQuery address_query;
            QString address_query_string = "DELETE FROM address WHERE person = :id_person";
            address_query.prepare(address_query_string);
            address_query.bindValue(":id_person", QVariant(id_person));
            if(address_query.exec()) {
                success = true;
            } else {
                success = false;
            }
            QSqlQuery license_query;
            QString license_query_string = "DELETE FROM people_license WHERE id_person = :id_person";
            license_query.prepare(license_query_string);
            license_query.bindValue(":id_person", QVariant(id_person));
            if(license_query.exec()) {
                success = true;
            } else {
                success = false;
            }
        }
    }
    return success;
}

bool ModelManager::deleteLicense(const int id_person, const int id_license) const
{
    if(id_person > 0 && id_license > 0) {
        QSqlQuery query;
        QString query_string = "DELETE FROM people_license WHERE id_person = :id_person AND id_license = :id_license";
        query.prepare(query_string);
        query.bindValue(":id_person", QVariant(id_person));
        query.bindValue(":id_license", QVariant(id_license));
        if(query.exec()) {
            return true;
        }
    }
    return false;
}

bool ModelManager::changeIsValid(const int id_person, const int id_license, const bool is_valid) const
{
    if(id_person > 0 && id_license > 0) {
        QSqlQuery query;
        query.prepare("UPDATE people_license SET is_valid = :is_valid WHERE id_person = :id_person AND id_license = :id_license");
        query.bindValue(":id_person", QVariant(id_person));
        query.bindValue(":id_license", QVariant(id_license));
        if(is_valid) {
            query.bindValue(":is_valid", QVariant("false"));
        } else {
            query.bindValue(":is_valid", QVariant("true"));
        }
        return query.exec();
    }
    return false;
}

int ModelManager::findFirstEmptyRow() const
{
    if(Excel_file != nullptr) {
        int last_row = Excel_file->dimension().lastRow();

        for(int row = 2; row <= last_row; ++row) {
            if(Excel_file->read(row, 1).toString() == "\"\"") {
                Excel_file->save();
                return row;
            }
        }
        Excel_file->save();
        return last_row + 1;
    }
    return -1;
}


bool ModelManager::isCellEmpty(const int row, const int column) const
{
    if(Excel_file != nullptr) {
        int last_row = Excel_file->dimension().lastRow();
        int last_column = 15;

        if(row > last_row && column > last_column) {
            Excel_file->save();
            return false;
        } else {
            if(Excel_file->read(row, column).toString() == "\"NULL\"") {
                Excel_file->save();
                return true;
            } else {
                Excel_file->save();
                return false;
            }
        }
    }
    return false;
}

QVector<int> ModelManager::getPersonRows(const int id_person) const
{
    QVector<int> result;
    if(Excel_file != nullptr) {
        int last_row = Excel_file->dimension().lastRow();
        int id_column = 1;

        for(int row = 2; row <= last_row; ++row) {
            if(Excel_file->read(row, id_column).toInt() == id_person) {
                result.append(row);
            }
        }
        Excel_file->save();
    }
    return result;
}

int ModelManager::getPersonWithLicenseRow(const int id_person, const QString& license) const
{
    if(Excel_file != nullptr) {
        QVector<int> person_rows = getPersonRows(id_person);
        foreach (int row, person_rows) {
            if(Excel_file->read(row, 12).toString() == license) {
                Excel_file->save();
                return row;
            }
        }
        Excel_file->save();
    }
    return -1;
}

bool ModelManager::isPersonRegisteredExcel(Person *person) const
{
    if(Excel_file != nullptr) {
        int last_row = Excel_file->dimension().lastRow();

        for(int row = 2; row <= last_row; ++row) {
            if(Excel_file->read(row, 8).toString() == "\"" + person->getPersonal_code() +  "\"" && Excel_file->read(row, 12).toString() == "\"" + person->getLicense() +  "\"") {
                Excel_file->save();
                return true;
            }
        }
        Excel_file->save();
    }
    return false;
}

void ModelManager::processBodyEmail(QString& body, const QMap<QString, QString>& replacements) const
{
    QString sub_str;
    int start = body.indexOf("@");
    int end = body.indexOf("@", start + 1);

    while(start != -1 && end != -1) {
        sub_str = body.section("@", 1, 1);
        body.replace(start, end - start + 1, replacements.value(sub_str));
        qDebug() << sub_str;
        qDebug() << replacements.value(sub_str);
        start = body.indexOf("@");
        end = body.indexOf("@", start + 1);
    }
}

bool ModelManager::isSuitableForAutomaticNotification(const int& id_person, const int& id_license, const int& month) const
{
    if(id_person > 0 && id_license > 0 && month > 0 && month < 13) {
        int license_validity = getLicenseValidity(id_license);


        QSqlQuery query;
        QString query_string = "SELECT           date_of_issue, "
                                                "is_valid, "
                                                "last_notification "
                                "FROM people_license WHERE people_license.id_person = :id_person AND people_license.id_license = :id_license ";

        query.prepare(query_string);
        query.bindValue(":id_person", id_person);
        query.bindValue(":id_license", id_license);


        if(query.exec()) {
            if(query.isActive()) {
                if(query.first()) {
                    if(     query.value(0).toInt() != 0 &&
                            query.value(1).toBool() == true &&
                            (query.value(2).toInt() == -1 || (QDate::currentDate().year() - QDate::fromJulianDay(query.value(2).toInt()).year() >= license_validity - 1)) &&
                            (QDate::currentDate().month() == month)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


void ModelManager::addPersonExcel(const int id_person, Person *person) const
{
    if(Excel_file != nullptr) {
        int right_row = findFirstEmptyRow();

        if(!isPersonRegisteredExcel(person)) {
            Excel_file->write(right_row, 1, id_person);
            Excel_file->write(right_row, 2, person->getLast_name());
            Excel_file->write(right_row, 3, person->getFirst_name());
            Excel_file->write(right_row, 4, "NULL");
            Excel_file->write(right_row, 5, "NULL");
            Excel_file->write(right_row, 6, "NULL");
            Excel_file->write(right_row, 7, "NULL");
            Excel_file->write(right_row, 8, person->getPersonal_code());
            if(person->getJob() != "") {
                Excel_file->write(right_row, 9, person->getJob());
            } else {
                Excel_file->write(right_row, 9, "NULL");
            }
            if(person->getEmail() != "") {
                Excel_file->write(right_row, 10, person->getEmail());
            } else {
                Excel_file->write(right_row, 10, "NULL");
            }
            if(person->getPhone() != "") {
                Excel_file->write(right_row, 11, person->getPhone());
            } else {
                Excel_file->write(right_row, 11, "NULL");
            }

            Excel_file->write(right_row, 12, person->getLicense());
            Excel_file->write(right_row, 13, "VALIDO");
            Excel_file->write(right_row, 14, person->getDateOfIssue().toString("d/M/yyyy"));
            Excel_file->write(right_row, 15, "MAI");
        }

        Excel_file->save();
    }
}

void ModelManager::addAddressExcel(const QVector<int> &rows, Address *address) const
{
    if(Excel_file != nullptr) {
        foreach (int row, rows) {
            if(address->getStreet() != "") {
                Excel_file->write(row, 4, QVariant(address->getStreet()));
            }
            if(address->getZip() != "") {
                Excel_file->write(row, 5, QVariant(address->getZip()));
            }
            if(address->getMunicipality() != "") {
                Excel_file->write(row, 6, QVariant(address->getMunicipality()));
            }
            if(address->getProvince() != "") {
                Excel_file->write(row, 7, QVariant(address->getProvince()));
            }
        }
        Excel_file->save();
    }
}

void ModelManager::updatePersonExcel(const QVector<int>& rows, Person *person) const
{
    if(Excel_file != nullptr) {
        foreach (int row, rows) {
            if(person->getLast_name() != "") {
                Excel_file->write(row, 2, QVariant(person->getLast_name()));
            }
            if(person->getFirst_name() != "") {
                Excel_file->write(row, 3, QVariant(person->getFirst_name()));
            }
            if(person->getPersonal_code() != "") {
                Excel_file->write(row, 8, QVariant(person->getPersonal_code()));
            }
            if(person->getJob() != "") {
                Excel_file->write(row, 9, QVariant(person->getJob()));
            }
            if(person->getEmail() != "") {
                Excel_file->write(row, 10, QVariant(person->getEmail()));
            }
            if(person->getPhone() != "") {
                Excel_file->write(row, 11, QVariant(person->getPhone()));
            }
        }
        Excel_file->save();
    }
}

void ModelManager::updateValidityExcel(const int& row, const QString& validity) const
{
    if(Excel_file != nullptr) {
        if(validity == "VALIDO") {
            Excel_file->write(row, 13, "SCADUTO");
        } else if (validity == "SCADUTO") {
            Excel_file->write(row, 13, "VALIDO");
        }
        Excel_file->save();
    }
}


void ModelManager::updateLastNotificationExcel(const int row, const QDate& last_notification) const
{
    if(Excel_file != nullptr) {
        int last_row = Excel_file->dimension().lastRow();
        if(row <= last_row) {
            Excel_file->write(row, 15, last_notification.toString("d/M/yyyy"));
        }
        Excel_file->save();
    }
}

void ModelManager::deleteLicenseExcel(const int row) const
{
    if(Excel_file != nullptr) {
        for(int column = 1; column <= 15; ++column) {
            Excel_file->write(row, column, "");
        }
        Excel_file->save();
    }
}

void ModelManager::deletePersonExcel(const QVector<int>& rows) const
{
    if(Excel_file != nullptr) {
        foreach(int row, rows) {
            for(int column = 1; column <= 15; ++column) {
                Excel_file->write(row, column, "");
            }
        }
        Excel_file->save();
    }
}


void ModelManager::setExcelFile(QXlsx::Document* document)
{
    if(Excel_file != nullptr) {
        delete Excel_file;
    }
    Excel_file = document;
}

void ModelManager::changeFirstNameExcel(int id_person, QString first_name)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 3, first_name);
        }
        Excel_file->save();
    }
}

void ModelManager::changeLastNameExcel(int id_person, QString last_name)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 2, last_name);
        }
        Excel_file->save();
    }
}

void ModelManager::changeStreetExcel(int id_person, QString street)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 4, street);
        }
        Excel_file->save();
    }
}

void ModelManager::changeZipExcel(int id_person, QString zip)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 5, zip);
        }
        Excel_file->save();
    }
}

void ModelManager::changeMunicipalityExcel(int id_person, QString municipality)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 6, municipality);
        }
        Excel_file->save();
    }
}

void ModelManager::changeProvinceExcel(int id_person, QString province)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 7, province);
        }
        Excel_file->save();
    }
}

void ModelManager::changePersonalCodeExcel(int id_person, QString personal_code)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 8, personal_code);
        }
        Excel_file->save();
    }
}

void ModelManager::changeJobExcel(int id_person, QString job)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 9, job);
        }
        Excel_file->save();
    }
}

void ModelManager::changeEmailExcel(int id_person, QString email)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 10, email);
        }
        Excel_file->save();
    }
}

void ModelManager::changePhoneExcel(int id_person, QString phone)
{
    if(Excel_file != nullptr) {
        QVector<int> rows = getPersonRows(id_person);
        foreach(int row, rows) {
            Excel_file->write(row, 11, phone);
        }
        Excel_file->save();
    }
}
