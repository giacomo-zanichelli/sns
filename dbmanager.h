#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>
#include <QString>
#include "person.h"
#include "sqlquerymodel.h"
#include <QStandardItemModel>
#include "xlsxdocument.h"


class DbManager : public QObject
{
    Q_OBJECT

public:
    explicit DbManager(QObject *parent = 0);
    ~DbManager();

    bool isOpen() const;

    QString getPath() const { return path_; }
    void    setPath(const QString& path) { path_ = path; }
    bool    openConnection();
    bool    createNewDatabase();

    //bool isRegistered(const int id_person) const;
    int     hasLicense(const int id_person, const int id_license) const;     //return people_license.id or -1 (query failed) or -2 (more than one row affected)

    int     getPersonId(const Person *person) const;
    int     getIdLicense(const QString& license) const;
    int     getLicenseValidity(const int id_license) const;
    QDate   getLicenseDateOfIssue(const int id_person, const int id_license) const;
    Person * buildPerson(const int id_person) const;

    bool    addPerson(const Person *person_to_register);
    bool    addAddress(const int id_person, Address* address_to_register);
    void    fillPersonField(const int id_person, const Person *person) const;

    int     findFirstEmptyRow() const;
    bool    isCellEmpty(const int row, const int column) const;
    QVector<int> getPersonRows(const int id_person) const;
    int     getPersonWithLicenseRow(const int id_person, const QString& license) const;
    bool    isPersonRegisteredExcel(Person *person) const;

    void    addPersonExcel(const int id_person, Person *person) const;
    void    addAddressExcel(const QVector<int>& rows, Address *address) const;
    void    updatePersonExcel(const QVector<int>& rows, Person *person) const;
    void    updateValidityExcel(const int& row, const QString& validity) const;
    void    updateLastNotificationExcel(const int row, const QDate& last_notification) const;
    void    deleteLicenseExcel(const int row) const;
    void    deletePersonExcel(const QVector<int>& rows) const;

    bool    changeIsValid(const int id_person, const int id_license, const bool is_valid) const;
    bool    deletePerson(const int id_person) const;
    bool    deleteLicense(const int id_person, const int id_license) const;
    //bool printPersonWithLicenses(const QString& first_name, const QString& last_name);
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

    void    createNewExcelDatabase(const QString &path);

    void setExcelFile(QXlsx::Document* document);
    QXlsx::Document * getExcelFile() const { return Excel_file; }

private slots:
    void updateLastNameExcel();

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
                                                  const QString& license,
                                                  bool           is_valid,
                                                  bool           is_expired);

    bool helperAllLicensesYear(const QString& first_name,
                                                  const QString& last_name,
                                                  const QString& municipality,
                                                  const QString& zip,
                                                  const QString& province,
                                                  const QString& job,
                                                  bool           is_valid,
                                                  bool           is_expired);

};

#endif // DBMANAGER_H
