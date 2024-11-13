#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>


template <typename SectionHeader, typename SymbolHeader>
class SymbolTableSection : public IElfSection<SectionHeader>
{
public:
    SymbolTableSection(std::shared_ptr<SectionHeader> p_sectionHeader, std::vector<SymbolHeader> p_symbolHeaders)
        : m_sectionHeader { p_sectionHeader }
        , m_symbolHeaders { p_symbolHeaders }
    {}

    std::shared_ptr<SectionHeader> getSectionHeader() const
    {
        return m_sectionHeader;
    }

    std::vector<SymbolHeader> getSymbolHeaders() const
    {
        return m_symbolHeaders;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    std::vector<SymbolHeader> m_symbolHeaders;
};
