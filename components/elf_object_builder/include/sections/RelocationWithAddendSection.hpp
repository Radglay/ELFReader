#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>


template <typename SectionHeader, typename RelocationWithAddend>
class RelocationWithAddendSection : public IElfSection<SectionHeader>
{
public:
    RelocationWithAddendSection(std::shared_ptr<SectionHeader> p_sectionHeader, std::vector<RelocationWithAddend> p_relocationWithAddendHeaders)
        : m_sectionHeader { p_sectionHeader }
        , m_relocationWithAddendHeaders { p_relocationWithAddendHeaders }
    {}

    std::shared_ptr<SectionHeader> getSectionHeader() const
    {
        return m_sectionHeader;
    }

    std::vector<RelocationWithAddend> getRelocationWithAddendHeaders() const
    {
        return m_relocationWithAddendHeaders;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    std::vector<RelocationWithAddend> m_relocationWithAddendHeaders;
};
