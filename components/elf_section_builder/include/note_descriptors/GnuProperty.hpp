#pragma once

#include "IGnuProperty.hpp"
#include "ElfPart.hpp"
#include "IGnuPropertyVisitor.hpp"


template <typename PropertyType>
class GnuProperty : public IGnuProperty
{
public:
    GnuProperty(PropertyType p_property)
        : m_property { p_property }
        {}

    ElfField acceptGnuPropertyVisitor(IGnuPropertyVisitor& p_visitor) override
    {
        return p_visitor.assembleElfFieldFromGnuProperty(m_property);
    }

    PropertyType getProperty() const
    {
        return m_property;
    }

private:
    PropertyType m_property;
};