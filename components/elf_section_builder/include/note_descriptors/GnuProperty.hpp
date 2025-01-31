#pragma once

#include "IGnuProperty.hpp"


template <typename PropertyType>
class GnuProperty : public IGnuProperty
{
public:
    GnuProperty(PropertyType p_property)
        : m_property { p_property }
        {}

    PropertyType getProperty() const
    {
        return m_property;
    }

private:
    PropertyType m_property;
};