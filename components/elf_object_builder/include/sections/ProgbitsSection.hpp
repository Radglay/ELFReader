#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>
#include <vector>


template <typename SectionHeader>
class ProgbitsSection : public IElfSection<SectionHeader>
{
public:
    ProgbitsSection(std::shared_ptr<SectionHeader> p_sectionHeader, std::vector<unsigned char>& p_bytes)
        : m_sectionHeader { p_sectionHeader }
        , m_bytes { p_bytes }
    {}

    std::shared_ptr<SectionHeader> getSectionHeader() const
    {
        return m_sectionHeader;
    }

    std::vector<unsigned char> getBytes() const
    {
        return m_bytes;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    std::vector<unsigned char> m_bytes;
};
