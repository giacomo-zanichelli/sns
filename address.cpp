#include "address.h"

Address::Address(       const QString&          street,
                        const QString&          municipality,
                        const QString&          province,
                        const QString&          zip,
                        const QString&          state        )
{
    street_         = street;
    municipality_   = municipality;
    zip_            = zip;
    province_       = province;
    state_          = state;
}

bool Address::isEmpty() const
{
    return  street_.isEmpty() ||
            municipality_.isEmpty() ||
            zip_.isEmpty() ||
            province_.isEmpty();
}
