#include "person.h"

Person::Person(const QString&      first_name,
                    const QString&      last_name,
                          Address*      address,
                    const QString&      personal_code,
                    const QString&      job,
                    const QString&      license,
                    const QDate&        date_of_issue,
                    const QString&      email,
                    const QString&      phone)
{
    first_name_         = first_name;
    last_name_          = last_name;
    address_            = address;
    personal_code_      = personal_code;
    job_                = job;
    license_            = license;
    date_of_issue_      = date_of_issue;
    email_              = email;
    phone_              = phone;
}

Person::Person(     const QString&      first_name,
                    const QString&      last_name,
                          Address*      address,
                    const QString&      personal_code,
                    const QString&      job,
                    const QString&      email,
                    const QString&      phone)
{
    first_name_         = first_name;
    last_name_          = last_name;
    address_            = address;
    personal_code_      = personal_code;
    job_                = job;
    email_              = email;
    phone_              = phone;
}


Person::~Person() {
    delete address_;
}


bool Person::isSufficient() const
{
    return !first_name_.isEmpty() &&
           !last_name_.isEmpty() &&
           !personal_code_.isEmpty() &&
          (!email_.isEmpty() || phone_ != 0 );
}

void Person::setLicense(const QString& license, const QDate& date_of_issue, const bool& license_validity, const QDate& last_notification)
{
    license_            = license;
    date_of_issue_      = date_of_issue;
    license_validity_   = license_validity;
    last_notification_  = last_notification;
}
