#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>

#include <QtSql>
#include <QString>
#include "person.h"
#include "sqlquerymodel.h"
#include <QStandardItemModel>
#include "xlsxdocument.h"


class ModelManager : public QObject
{
    Q_OBJECT
public:
    explicit ModelManager(QObject *parent = 0);

    ~ModelManager();

    bool isOpen() const;

    QString getPath() const { return path_; }
    void    setPath(const QString& path) { path_ = path; }
    bool    openConnection();
    bool    createNewDatabase();

    int     hasLicense(const int id_person, const int id_license) const;     //return people_license.id or -1 (query failed) or -2 (more than one row affected)

    QVector<int> getAllPersonId() const;
    QVector<int> getAllLicensesId(const int id_person) const;
    int     getPersonId(const Person *person) const;
    int     getIdLicense(const QString& license) const;
    QString getLicenseNameFromId(const int id_license) const;
    int     getLicenseValidity(const int id_license) const;
    QDate   getLicenseDateOfIssue(const int id_person, const int id_license) const;
    QDate   getLastNotification(const int id_person, const int id_license) const;
    Person * buildPerson(const int id_person) const;

    bool    addPerson(const Person *person_to_register);
    bool    addAddress(const int id_person, Address* address_to_register);
    void    fillPersonField(const int id_person, const Person *person) const;
    bool    updateLastNotificationDate(const int id_person, const int id_license, const QDate& date) const;
    //bool    updateLicenseValidity(const int id_person, const int id_license) const;

    int     findFirstEmptyRow() const;
    bool    isCellEmpty(const int row, const int column) const;
    QVector<int> getPersonRows(const int id_person) const;
    int     getPersonWithLicenseRow(const int id_person, const QString& license) const;
    bool    isPersonRegisteredExcel(Person *person) const;

    void    processBodyEmail(QString& body, const QMap<QString, QString>& replacements) const;
    bool    isSuitableForAutomaticNotification(const int &id_person, const int &id_license, const int& month) const;

    void    addPersonExcel(const int id_person, Person *person) const;
    void    addAddressExcel(const QVector<int>& rows, Address *address) const;
    void    updatePersonExcel(const QVector<int>& rows, Person *person) const;
    void    updateValidityExcel(const int& row, const QString& validity) const;
    void    updateLastNotificationExcel(const int row, const QDate& last_notification) const;
    void    deleteLicenseExcel(const int row) const;
    void    deletePersonExcel(const QVector<int>& rows) const;
    void    getAllPerson(const QString& filepath);

    bool    changeIsValid(const int id_person, const int id_license, const bool is_valid) const;
    bool    deletePerson(const int id_person) const;
    bool    deleteLicense(const int id_person, const int id_license) const;
    bool    addLicenseToPerson(const int id_person, const int id_license, const QDate &date_of_issue = QDate(0, 0, 0)) const;
    bool    setSearch_results_table_model( const QString& first_name,
                                           const QString& last_name,
                                           const QString& municipality,
                                           const QString& zip,
                                           const QString& province,
                                           const QString& job,
                                           const QString& license,
                                           bool           is_valid,
                                           bool           is_expired,
                                           bool           all_licenses  = false,
                                           bool           expiring      = false     );

    bool    runLastShowQuery();

    SqlQueryModel *getSearch_results_table_model();

    void setExcelFile(QXlsx::Document* document);
    QXlsx::Document * getExcelFile() const { return Excel_file; }

signals:

public slots:

    void changeFirstNameExcel(int id_person, QString first_name);
    void changeLastNameExcel(int id_person, QString last_name);
    void changeStreetExcel(int id_person, QString street);
    void changeZipExcel(int id_person, QString zip);
    void changeMunicipalityExcel(int id_person, QString municipality);
    void changeProvinceExcel(int id_person, QString province);
    void changePersonalCodeExcel(int id_person, QString personal_code);
    void changeJobExcel(int id_person, QString job);
    void changeEmailExcel(int id_person, QString email);
    void changePhoneExcel(int id_person, QString phone);

private:
    QSqlDatabase    *db_link;
    SqlQueryModel   *search_results_table_model;
    QSqlQuery       last_show_query;
    QXlsx::Document *Excel_file;
    QString         path_;

    bool helperOneLicense(const QString& first_name,
                                                  const QString& last_name,
                                                  const QString& municipality,
                                                  const QString& zip,
                                                  const QString& province,
                                                  const QString& job,
                                                  const QString& license,
                                                  bool           is_valid,
                                                  bool           is_expired);

    bool helperAllLicenses(                       const QString& first_name,
                                                  const QString& last_name,
                                                  const QString& municipality,
                                                  const QString& zip,
                                                  const QString& province,
                                                  const QString& job,
                                                  bool           is_valid,
                                                  bool           is_expired);

    bool helperOneLicenseYear(                    const QString& first_name,
                                                  const QString& last_name,
                                                  const QString& municipality,
                                                  const QString& zip,
                                                  const QString& province,
                                                  const QString& job,
                                                  const QString& license);

    bool helperAllLicensesYear(                   const QString& first_name,
                                                  const QString& last_name,
                                                  const QString& municipality,
                                                  const QString& zip,
                                                  const QString& province,
                                                  const QString& job);

};

#endif // MODELMANAGER_H
