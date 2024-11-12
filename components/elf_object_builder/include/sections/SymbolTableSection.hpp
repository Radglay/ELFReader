#pragma once

#include "IElfSection.hpp"
#include <vector>


template <typename SectionHeader, typename Symbol>
class SymbolTableSection : public IElfSection<SectionHeader>
{
public:
    SymbolTableSection(SectionHeader* p_sectionHeader, std::vector<Symbol> p_symbols)
        : m_sectionHeader { p_sectionHeader }
        , m_symbols { p_symbols }
    {}

private:
    SectionHeader* m_sectionHeader;
public:
    std::vector<Symbol> m_symbols;
};
