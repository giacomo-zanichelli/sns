#ifndef ADDRESS_H
#define ADDRESS_H

#include <QString>
#include <QDate>

class Address
{
public:
    Address(    const QString&          street,
                const QString&          municipality,
                const QString&          province,
                const QString&          zip = "",
                const QString&          state = "IT"       );

    bool isEmpty() const;

    const QString& getStreet() const { return street_; }
    const QString& getMunicipality() const { return municipality_; }
    const QString& getZip() const { return zip_; }
    const QString& getProvince() const { return province_; }
    const QString& getState() const { return state_; }


    friend class Person;

private:
    QString         street_;
    QString         municipality_;
    QString         zip_;
    QString         province_;
    QString         state_;

    Address() {}
};

#endif // ADDRESS_H
