#ifndef PERSON_H
#define PERSON_H
#include <QString>
#include "address.h"
#include <QMap>

class Person
{
public:
    Person(     const QString&      first_name,
                const QString&      last_name,
                      Address      *address,
                const QString&      personal_code,
                const QString&      job,
                const QString&      license,
                const QDate &       date_of_issue,
                const QString&      email = "",
                const QString&      phone = ""  );

    Person(     const QString&      first_name,
                const QString&      last_name,
                      Address      *address,
                const QString&      personal_code,
                const QString&      job,
                const QString&      email = "",
                const QString&      phone = ""  );

    Person() {}

    ~Person();

    QString         getFirst_name() const {         return first_name_; }
    QString         getLast_name() const {          return last_name_; }
    Address *       getAddress() const {            return address_; }
    QString         getEmail() const {              return email_; }
    QString         getPhone()  const {             return phone_; }
    QString         getPersonal_code() const {      return personal_code_; }
    QString         getJob() const {                return job_; }
    QString         getLicense() const {            return license_; }
    QDate           getDateOfIssue() const {        return date_of_issue_; }
    bool            getLicenseValidity() const {    return license_validity_; }
    QDate           getLastNotification() const {   return last_notification_; }

    void            setLicenseValidity(const bool& license_validity) { license_validity_ = license_validity; }
    void            setLastNotification(const QDate& last_notification) { last_notification_ = last_notification; }

    void            setLicense(const QString& license, const QDate& date_of_issue, const bool& license_validity = true, const QDate& last_notification = QDate());


    bool            isSufficient() const;
private:
    QString         first_name_;
    QString         last_name_;

    Address*        address_;

    QString         email_;
    QString         phone_;

    QString         personal_code_;

    QString         job_;
    QString         license_;
    QDate           date_of_issue_;
    bool            license_validity_;
    QDate           last_notification_;
};



#endif // PERSON_H
